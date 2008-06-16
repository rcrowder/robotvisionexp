/* flashpic.c:
   This file contains the flash-support code that is relocated to
   RAM prior to execution.  This file supports all bus widths, dependent
   on the definition of WIDTH8, WIDTH16, or WIDTH32 on the command line.
   Refer to flash.h for definition of the width-dependent macros.
*/

#include "cpu.h"
#include "flashdev.h"
#include "flash.h"
#include "config.h"
#include "stddefs.h"

#if INCLUDE_FLASH

/* Flasherase():
   Based on the 'snum' value, erase the appropriate sector(s).
   Return 0 if success, else -1.
*/
int
Flasherase(struct flashinfo *fdev,int snum)
{
    ftype   val;
    ulong   add;
    int ret, sector;

    ret = 0;
    add = (ulong)(fdev->base);

    /* Erase the request sector(s): */
    for (sector=0;sector<fdev->sectorcnt;sector++) {
        if ((!FlashProtectWindow) &&
            (fdev->sectors[sector].protected)) {
            add += fdev->sectors[sector].size;
            continue;
        }
        if ((snum == ALL_SECTORS) || (snum == sector)) {
            /* Issue the sector erase command sequence: */
            Write_aa_to_555();
            Write_55_to_2aa();
            Write_80_to_555();
            Write_aa_to_555();
            Write_55_to_2aa();
            Write_30_to_(add);
    
            /* Wait for sector erase to complete or timeout.. */
            /* DQ7 polling: wait for D7 to be 1. */
            /* DQ6 toggling: wait for D6 to not toggle. */
            /* DQ5 timeout: if DQ7 is 0, and DQ5 = 1, timeout. */
            while(1) {
                if (Is_ff(add)) {
                    if (Is_ff(add))
                        break;
                }
                if (D5_Timeout(add)) {
                    if (Is_not_ff(add))
                        ret = -1;
                    break;
                }
            }
        }
        add += fdev->sectors[sector].size;
    }

    /* If the erase failed for some reason, then issue the read/reset */
    /* command sequence prior to returning... */
    if (ret == -1) {
        Write_f0_to_base();
        val = Read_555();
    }
    return(ret);
}

/* EndFlasherase():
   Function place holder to determine the "end" of the
   sectorerase() function.
*/
void
EndFlasherase()
{}


/* Flashwrite():
   Return 0 if successful, else -1.
   Note: this assumes that source & destination properly aligned
   based on the width of the flash bank.
*/
int
Flashwrite(struct flashinfo *fdev,ftype *dest,ftype *src,long bytecnt)
{
    int i, ret;
    ftype   val;

    /* Each pass through this loop writes 'fdev->width' bytes... */
    ret = 0;
    for (i=0;i<bytecnt;i+=fdev->width) {

        /* Flash write command */
        Write_aa_to_555();
        Write_55_to_2aa();
        Write_a0_to_555();

        /* Write the value */
        Fwrite(dest,src);

        /* Wait for write to complete or timeout. */
        while(1) {
            if (Is_Equal(dest,src)) {
                if (Is_Equal(dest,src))
                    break;
            }
            /* Check D5 for timeout... */
            if (D5_Timeout(dest)) {
                if (Is_Not_Equal(dest,src))
                    ret = -1;
                goto done;
            }
        }
        dest++; src++;
    }

done:
    /* Read/reset command: */
    Write_f0_to_base();
    val = Read_555();
    return(ret);
}

/* EndFlashwrite():
    Function place holder to determine the "end" of the
    Flashwrite() function.
*/
void
EndFlashwrite()
{}

/* Ewrite():
   Erase all sectors that are part of the address space to be written,
   then write the data to that address space.  This is basically a
   concatenation of flasherase and flashwrite done in one step.  This is
   necessary primarily for re-writing the bootcode; because after the boot
   code is erased, there is nowhere to return so the re-write must be done
   while executing out of ram also.
*/

int
Flashewrite(struct flashinfo *fdev,ftype *dest,ftype *src,int bytecnt)
{
    int i;
    ulong   add;
    void    (*reset)();
    ftype   val, *src1, *dest1;

    add = (ulong)(fdev->base);
    src1 = src;
    dest1 = dest;

    /* For each sector, if it overlaps any of the destination space */
    /* then erase that sector. */
    for (i=0;i<fdev->sectorcnt;i++) {
        if ((((uchar *)dest) > (fdev->sectors[i].end)) ||
            (((uchar *)dest+bytecnt-1) < (fdev->sectors[i].begin))) {
            add += fdev->sectors[i].size;
            continue;
        }

        /* Sector erase command: */
        Write_aa_to_555();
        Write_55_to_2aa();
        Write_80_to_555();
        Write_aa_to_555();
        Write_55_to_2aa();
        Write_30_to_(add);
    
        /* Wait for sector erase to complete or timeout.. */
        /* DQ7 polling: wait for D7 to be 1. */
        /* DQ6 toggling: wait for D6 to not toggle. */
        /* DQ5 timeout: if DQ7 is 0, and DQ5 = 1, timeout. */
        while(1) {
            if (Is_ff(add)) {
                if (Is_ff(add))
                    break;
            }
            /* Check D5 for timeout... */
            /* In this case, there is nothing to return to */
            /* because the flash was just erased, so just break.*/
            if (D5_Timeout(add)) {
                goto quit;
            }
        }
        add += fdev->sectors[i].size;
    }

    /* Read/reset command: */
    Write_f0_to_base();
    val = Read_555();

    for(i=0;i<bytecnt;i+=fdev->width) {
        /* Write command: */
        Write_aa_to_555();
        Write_55_to_2aa();
        Write_a0_to_555();
        Fwrite(dest,src);

        while(1) {
            if (Is_Equal(dest,src)) {
                if (Is_Equal(dest,src))
                    break;
            }
            /* Check D5 for timeout... */
            if (D5_Timeout(dest)) {
                if (Is_Not_Equal(dest,src))
                    goto quit;
                continue;
            }
        }

        dest++; src++;
    }

quit:
    /* Issue the read/reset command sequence: */
    Write_f0_to_base();
    val = Read_555();

    /* Wait till flash is readable, or timeout: */
    for(i=0;i<FLASH_TIMEOUT;i++) {
        if (Is_Equal(dest1,src1))
            break;
    }

    /* Now that the re-programming of flash is complete, reset: */
    reset = RESETFUNC();
    reset();

    return(0);  /* won't get here */
}

/* EndFlashewrite():
    Function place holder to determine the "end" of the
    FlashEraseAndWrite() function.
*/
void
EndFlashewrite()
{}


/* Flashtype():
   Use the AUTOSELECT command sequence to determine the type of device.
*/

int
Flashtype(struct flashinfo *fdev)
{
    ushort  man, dev;
    ulong   id;

    /* Issue the id command sequence: */
    Write_aa_to_555();
    Write_55_to_2aa();
    Write_90_to_555();
    
    man = (ushort)Read_0000();  /* manufacturer ID */
    dev = (ushort)Read_0002();  /* device ID */
    id = man;
    id <<= 16;
    id |= dev;

    fdev->id = id;

    /* Issue the read/reset command sequence: */
    Write_f0_to_base();

    return((int)(fdev->id));
}

/* EndFlashtype():
    Function place holder to determine the "end" of the
    Flashtype() function.
*/
void
EndFlashtype()
{}
#endif
