/* flashpic_28f640.c:
    This file contains the portion of the flash driver code that can be
    relocated (pic =  position independent code) to RAM space so that it
    is not executing out of the same flash device that it is operating on.

    This code is specific to a 16-bit configuration of the 28f640, but
    could be made width independent if the need arises.
*/
#include "config.h"
#if INCLUDE_FLASH
typedef unsigned char uchar;
typedef unsigned short ushort;
typedef unsigned long ulong;
typedef volatile unsigned char vuchar;
typedef volatile unsigned short vushort;
typedef volatile unsigned long vulong;
typedef volatile unsigned int vuint;
typedef volatile int vint;

#include "cpu.h"
#include "flashdev.h"
#include "flash.h"
#include "flashpic.h"

extern struct flashinfo Fdev;
extern int FlashProtectWindow;

/* Flasherase():
   Based on the 'snum' value, erase the appropriate sector(s).
   Write the "Erase Setup" and "Erase Confirm" commands...
   Return 0 if success, else -1.
*/
int
Flasherase28F640_16(struct flashinfo *fdev,int snum)
{
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
            register ulong *lp, *lp1;
            int noterased;

            /* See if the sector is already erased: */
            noterased = 0;
            lp = (ulong *)fdev->sectors[sector].begin; 
            lp1 = (ulong *)((char *)lp + fdev->sectors[sector].size); 
            while(lp < lp1) {
                if (*lp++ != 0xffffffff) {
                    noterased = 1;
                    break;
                }
            }
            if (noterased) {
                Write_20_to_base();             /* setup */
                Write_d0_to_(add);              /* confirm */
                while(!(Read_0000() & WSMS));   /* Wait for SMS ready */
                if (Read_0000() & ECLBS)        /* Check for error */
                    ret = -1;
                Write_50_to_base();             /* Clear status register */
                Write_ff_to_base();             /* Go to read-array mode */
            }
            if (ret == -1)
                break;
        }
        add += fdev->sectors[sector].size;
    }
    return(ret);
}

/* EndFlasherase():
   Function place holder to determine the "end" of the
   sectorerase() function.
*/
void
EndFlasherase28F640_16()
{
}


/* Flashwrite():
    Return 0 if successful, else -1.
    There are two versions of this function.  The currently removed version
    supports the one-word/byte-at-a-time programming mode and it works but
    it is slow.  The currently active version uses the 28F640's write buffer
    mode of programming.  This is much faster, but it requires a bit more
    complexity.  See comments in code.
*/
int
Flashwrite28F640_16(struct flashinfo *fdev,uchar *dest,uchar *src,long bytecnt)
{
    int     tot, ret;
    long    cnt;
    uchar   *src1;
    ftype   val;

    /* If destination address is not properly aligned, then build a fake   */
    /* source buffer based on the current value in dest[-1] and src[0].    */
    /* Then call this function to do that 2-byte operation.  Once that     */
    /* completes, simply increment dest and src by 1 and continue in this  */
    /* context. */
    if (NotAligned(dest)) {
        uchar buf[2];
        buf[0] = *(dest-1);
        buf[1] = *src;
        Flashwrite28F640_16(fdev,dest-1,buf,2);
        dest++; src++; bytecnt--;
    }

    /* This device supports a write-buffer mode.  The buffer is 32 bytes    */
    /* (16 words) and greatly speeds up the programming process.  This      */
    /* This function starts up with the word-at-a-time mode and uses it     */
    /* until the destination address is mod32.  Then for each successive    */
    /* 32-byte block, the write buffer is used. When there is less than 32  */
    /* bytes left, the algorithm falls back to the one-word-at-a-time mode  */
    /* to finish up. */
    ret = 0;
    cnt = bytecnt & ~1;
    src1 = (uchar *)&val;

    for (tot=0;tot<cnt;tot+=2) {
        if (((ulong)dest & 0x1f) == 0)  /* Break out when 5 LSBs are zero */
            break;

        src1[0] = src[0];               /* Just in case src is not aligned... */
        src1[1] = src[1];               

        Write_40_to_(dest);             /* Flash program setup command */
        Fwrite(dest,src1);              /* Write the value */
        while(!(Read_0000() & WSMS));   /* Wait for SMS ready */
        if (Read_0000() & PSLBS)        /* Check for error */
            ret = -1;
        Write_50_to_base();             /* Clear status register */
        Write_ff_to_base();             /* Go to read-array mode */
        dest += 2; 
        src += 2;
    }

    /* If tot is less than cnt, then the upper loop reached a point where   */
    /* the destination address had the 5 LSBs low.  This means that we can  */
    /* use the write-buffer for all remaining blocks of 32 bytes...         */
    if (tot < cnt) {
        while((cnt-tot) >= 32) {
            int i;
            uchar *src2, buf32[32], *base;
    
            src2 = buf32;               /* Copy next buffer's worth of data */
            for(i=0;i<32;i++)           /* into local buffer just in case */
                buf32[i] = *src++;      /* the source is this flash device. */
    
            do { 
                Write_e8_to_(dest);         /* Write-to-buffer command */
            } while (!(Read_0000() & WBS)); /* Write buffer available? */
                
            base = dest;
            Write_0f_to_(dest);         /* Word count is 16 (load 16-1) */
            for(i=0;i<16;i++) {
                src1[0] = src2[0];      /* Just in case src is not aligned... */
                src1[1] = src2[1];              
                Fwrite(dest,src1);      /* Write the value */
                dest += 2; 
                src2 += 2; 
            }
            Write_d0_to_(base);             /* Write-confirm command */
            while(!(Read_0000() & WSMS));   /* Wait for SMS ready */
            Write_ff_to_base();             /* Go to read-array mode */
            tot += 32;
        }
    }

onemore:
    for (;tot<cnt;tot+=2) {
        src1[0] = src[0];               /* Just in case src is not aligned... */
        src1[1] = src[1];

        Write_40_to_(dest);             /* Flash program setup command */
        Fwrite(dest,src1);              /* Write the value */
        while(!(Read_0000() & WSMS));   /* Wait for SMS ready */
        if (Read_0000() & PSLBS)        /* Check for error */
            ret = -1;
        Write_50_to_base();             /* Clear status register */
        Write_ff_to_base();             /* Go to read-array mode */
        dest += 2; 
        src += 2;
    }

    /* If bytecount was odd... */
    /* If cnt != bytecnt then bytecnt is odd, so one more byte must be */
    /* written to flash.  To do this, the one byte must be combined with */
    /* the next byte that is already stored in flash; then re-written... */
    if (cnt != bytecnt) {
        val = (ftype)*dest | ((ftype)(*src) << 8);
        src = (uchar *)&val;
        tot = 0;
        bytecnt = cnt = 1;
        goto onemore;
    }

    return(ret);
}

/* EndFlashwrite():
    Function place holder to determine the "end" of the
    Flashwrite() function.
*/
void
EndFlashwrite28F640_16()
{
}

/* Ewrite():
   Erase all sectors that are part of the address space to be written,
   then write the data to that address space.  This is basically a
   concatenation of flasherase and flashwrite done in one step.  This is
   necessary primarily for re-writing the bootcode; because after the boot
   code is erased, there is nowhere to return so the re-write must be done
   while executing out of ram also.
*/

int
Flashewrite28F640_16(struct flashinfo *fdev,ftype *dest,ftype *src,int bytecnt)
{
    int i, err;
    ulong   add;
    void    (*reset)();
    ftype   *src1;

    err = 0;
    add = (ulong)(fdev->base);
    src1 = src;

    /* For each sector, if it overlaps any of the destination space */
    /* then erase that sector. */
    for (i=0;i<fdev->sectorcnt;i++) {
        if ((((uchar *)dest) > (fdev->sectors[i].end)) ||
            (((uchar *)dest+bytecnt) < (fdev->sectors[i].begin))) {
            add += fdev->sectors[i].size;
            continue;
        }

        Write_20_to_base();             /* setup */
        Write_d0_to_(add);              /* confirm */
        while(!(Read_0000() & WSMS));   /* Wait for SMS ready */
        if (Read_0000() & ECLBS)        /* Check for error */
            err = 1;
        Write_50_to_base();             /* Clear status register */
        Write_ff_to_base();             /* Go to read-array mode */
        if (err)
            return(-1);
        add += fdev->sectors[i].size;
    }

    for(i=0;i<bytecnt;i+=fdev->width) {
        
        /* Just in case src is not aligned... */
        src1[0] = src[0];
        src1[1] = src[1];

        Write_40_to_(dest);             /* Flash program setup command */
        Fwrite(dest,src1);              /* Write the value */
        while(!(Read_0000() & WSMS));   /* Wait for SMS ready */
        if (Read_0000() & PSLBS)        /* Check for error */
            err = 1;
        Write_50_to_base();             /* Clear status register */
        Write_ff_to_base();             /* Go to read-array mode */
        if (err)
            return(-1);
        dest++; 
        src++;
    }
    
    /* Wait till flash is readable, then reset: */
    while(1) {
        if (Is_Equal((dest-1),src1))
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
EndFlashewrite28F640_16()
{
}

/* Flashtype():
   Use the ReadCOnfiguration command (90H) to read manufacturer code (0000)
   and device id code (0001).
*/

int
Flashtype28F640_16(struct flashinfo *fdev)
{
    ftype   val;
    ushort  man, dev;
    ulong   id;

    val = Read_0000();

    /* Issue the read configuration command: */
    Write_90_to_base();

    man = (ushort)Read_0000();  /* manufacturer ID */
    dev = (ushort)Read_0001();  /* device ID */
    id = man;
    id <<= 16;
    id |= dev;

    fdev->id = id;

    /* Issue the read array command: */
    Write_ff_to_base();
    return((int)(fdev->id));
}

/* EndFlashtype():
    Function place holder to determine the "end" of the
    Flashtype() function.
*/
void
EndFlashtype28F640_16()
{
}
#endif
