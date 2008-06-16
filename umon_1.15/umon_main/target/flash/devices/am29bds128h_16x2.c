/* am29bds128h_16x2.c:
 * Device interface for the AM29BDS128H flash device configured for
 * x16 mode with 2 devices in parallel.  
 */
#include "config.h"

#if INCLUDE_FLASH

#include "stddefs.h"
#include "cpu.h"
#include "flash.h"			/* Part of monitor common code */
#include "am29bds128h_16x2.h"

#define ftype               volatile unsigned long
#define Write_aa_to_555()  	(*(ftype *)(fdev->base + (0x555 << 2)) = 0x00aa00aa)
#define Write_55_to_2aa()  	(*(ftype *)(fdev->base + (0x2aa << 2)) = 0x00550055)
#define Write_80_to_555()  	(*(ftype *)(fdev->base + (0x555 << 2)) = 0x00800080)
#define Write_90_to_555()  	(*(ftype *)(fdev->base + (0x555 << 2)) = 0x00900090)
#define Write_a0_to_555()  	(*(ftype *)(fdev->base + (0x555 << 2)) = 0x00a000a0)
#define Write_f0_to_base()  (*(ftype *)(fdev->base) = 0x00f000f0)
#define Write_30_to_(add)   (*(ftype *)add = 0x00300030)
#define Read_0000()         (*(ftype *)(fdev->base))
#define Read_0002()         (*(ftype *)(fdev->base + 4))
#define Read_555()          (*(ftype *)(fdev->base + (0x555 << 2)))
#define Is_ff(add)          (*(ftype *)add == 0xffffffff)
#define Is_not_ff(add)      (*(ftype *)add != 0xffffffff)
#define D5_Timeout(add)     ((*(ftype *)add & 0x00200020) == 0x00200020)
#define Fwrite(to,frm)      (*(ftype *)to = *(ftype *)frm)
#define Is_Equal(p1,p2)     (*(ftype *)p1 == *(ftype *)p2)
#define Is_Not_Equal(p1,p2) (*(ftype *)p1 != *(ftype *)p2)

/* Am29bds128h_16x2_erase():
 * Based on the 'snum' value, erase the appropriate sector(s).
 * Return 0 if success, else -1.
 */
int
Am29bds128h_16x2_erase(struct flashinfo *fdev,int snum)
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
			register ulong *lp, *lp1;
			int	noterased;

			/* See if the sector is already erased: */
			noterased = 0;
			lp = (ulong *)fdev->sectors[sector].begin; 
			lp1 = (ulong *)((char *)lp + fdev->sectors[sector].size-1); 
			while(lp <= lp1) {
				if (*lp++ != 0xffffffff) {
					noterased = 1;
					break;
				}
			}
			if (noterased) {
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

/* EndAm29bds128h_16x2_erase():
 * Function place holder to determine the end of the above function.
 */
void
EndAm29bds128h_16x2_erase(void)
{
}

/* Am29bds128h_16x2_write():
 * Copy specified number of bytes from source to destination.  The destination
 * address is assumed to be flash space.
 */
int
Am29bds128h_16x2_write(struct flashinfo *fdev,
	uchar *dest,uchar *src,long bytecnt)
{
    int i, ret;
    ftype   val, d6_sample1, d6_sample2;

    /* Each pass through this loop writes 'fdev->width' bytes... */
    ret = 0;
    for (i=0;i<bytecnt;i+=fdev->width) {

        /* Flash write command */
        Write_aa_to_555();
        Write_55_to_2aa();
        Write_a0_to_555();

        /* Write the value */
        Fwrite(dest,src);

        /* Wait for D6 to stop toggling...
		 */
        while(1) {
			d6_sample1 = *(ftype *)dest & 0x00400040;
			d6_sample2 = *(ftype *)dest & 0x00400040;
			if (d6_sample1 == d6_sample2)
				break;
        }
        dest += 4;
		src += 4;
    }

    /* Read/reset command: */
    Write_f0_to_base();
    val = Read_555();
    return(ret);
}

/* EndAm29bds128h_16x2_write():
 * Function place holder to determine the end of the above function.
 */
void
EndAm29bds128h_16x2_write(void)
{}

/* Am29bds128h_16x2_ewrite():
 * Erase all sectors that are part of the address space to be written,
 * then write the data to that address space.  This is basically a
 * concatenation of the above erase & write done in one step.  This is
 * necessary primarily for re-writing the bootcode; because after the boot
 * code is erased, there is nowhere to return so the re-write must be done
 * while executing out of ram also.  It is only needed in systems that are
 * executing the monitor out of the same device that is being updated.
 */
int
Am29bds128h_16x2_ewrite(struct flashinfo *fdev,
	uchar *dest,uchar *src,long bytecnt)
{
    int i;
    ulong   add;
    void    (*reset)();
    ftype   val, *src1, *dest1;

    add = (ulong)(fdev->base);
    src1 = (ftype *)src;
    dest1 = (ftype *)dest;

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

        dest += 4;
		src += 4;
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

/* EndAm29bds128h_16x2_ewrite():
 * Function place holder to determine the end of the above function.
 */
void
EndAm29bds128h_16x2_ewrite(void)
{}


/* Am29bds128h_16x2_type():
 * Run the AUTOSELECT algorithm to retrieve the manufacturer and
 * device id of the flash.
 */
int
Am29bds128h_16x2_type(struct flashinfo *fdev)
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

/* EndAm29bds128h_16x2_type():
 * Function place holder to determine the end of the above function.
 */
void
EndAm29bds128h_16x2_type(void)
{}

/**************************************************************************
 **************************************************************************
 *
 * The remainder of the code in this file should only included if the
 * target configuration is such that this device is the only
 * real flash device in the system that is to be visible to the monitor.
 *
 **************************************************************************
 **************************************************************************
 */
#ifdef SINGLE_FLASH_DEVICE

/* FlashXXXFbuf[]:
 * If FLASH_COPY_TO_RAM is defined then these arrays will contain the
 * flash operation functions above.  To operate on most flash devices,
 * you cannot be executing out of it (there are exceptions, but
 * in general, we do not assume the flash supports this).  The flash
 * functions are copied here, then executed through the function
 * pointers established in the flashinfo structure below.
 * One obvious requirement...  The size of each array must be at least
 * as large as the function that it will contain.
 */
#ifdef FLASH_COPY_TO_RAM
ulong    FlashTypeFbuf[400];
ulong    FlashEraseFbuf[400];
ulong    FlashWriteFbuf[400];
ulong    FlashEwriteFbuf[400];
#endif

/* FlashNamId[]:
 * Used to correlate between the ID and a string representing the name
 * of the flash device.
 */
struct flashdesc FlashNamId[] = {
	{ AM29BDS128H,		"Am29BDS128H" },
	{ 0, (char *)0 },
};

int SectorSizes[] = {  
    0x4000, 0x4000, 0x4000, 0x4000, 0x4000, 0x4000, 0x4000, 0x4000,
	0x20000, 0x20000, 0x20000,

    0x20000, 0x20000, 0x20000, 0x20000, 0x20000, 0x20000, 0x20000, 0x20000,
	0x20000, 0x20000, 0x20000, 0x20000, 0x20000, 0x20000, 0x20000, 0x20000,
	0x20000, 0x20000, 0x20000, 0x20000, 0x20000, 0x20000, 0x20000, 0x20000,
	0x20000, 0x20000, 0x20000, 0x20000, 0x20000, 0x20000, 0x20000, 0x20000,
    0x20000, 0x20000, 0x20000, 0x20000, 0x20000, 0x20000, 0x20000, 0x20000,
	0x20000, 0x20000, 0x20000, 0x20000, 0x20000, 0x20000, 0x20000, 0x20000,
	0x20000, 0x20000, 0x20000, 0x20000, 0x20000, 0x20000, 0x20000, 0x20000,
	0x20000, 0x20000, 0x20000, 0x20000, 0x20000, 0x20000, 0x20000, 0x20000,
    0x20000, 0x20000, 0x20000, 0x20000, 0x20000, 0x20000, 0x20000, 0x20000,
	0x20000, 0x20000, 0x20000, 0x20000, 0x20000, 0x20000, 0x20000, 0x20000,
	0x20000, 0x20000, 0x20000, 0x20000, 0x20000, 0x20000, 0x20000, 0x20000,
	0x20000, 0x20000, 0x20000, 0x20000, 0x20000, 0x20000, 0x20000, 0x20000,
    0x20000, 0x20000, 0x20000, 0x20000, 0x20000, 0x20000, 0x20000, 0x20000,
	0x20000, 0x20000, 0x20000, 0x20000, 0x20000, 0x20000, 0x20000, 0x20000,
	0x20000, 0x20000, 0x20000, 0x20000, 0x20000, 0x20000, 0x20000, 0x20000,
	0x20000, 0x20000, 0x20000, 0x20000, 0x20000, 0x20000, 0x20000, 0x20000,
    0x20000, 0x20000, 0x20000, 0x20000, 0x20000, 0x20000, 0x20000, 0x20000,
	0x20000, 0x20000, 0x20000, 0x20000, 0x20000, 0x20000, 0x20000, 0x20000,
	0x20000, 0x20000, 0x20000, 0x20000, 0x20000, 0x20000, 0x20000, 0x20000,
	0x20000, 0x20000, 0x20000, 0x20000, 0x20000, 0x20000, 0x20000, 0x20000,
    0x20000, 0x20000, 0x20000, 0x20000, 0x20000, 0x20000, 0x20000, 0x20000,
	0x20000, 0x20000, 0x20000, 0x20000, 0x20000, 0x20000, 0x20000, 0x20000,
	0x20000, 0x20000, 0x20000, 0x20000, 0x20000, 0x20000, 0x20000, 0x20000,
	0x20000, 0x20000, 0x20000, 0x20000, 0x20000, 0x20000, 0x20000, 0x20000,
    0x20000, 0x20000, 0x20000, 0x20000, 0x20000, 0x20000, 0x20000, 0x20000,
	0x20000, 0x20000, 0x20000, 0x20000, 0x20000, 0x20000, 0x20000, 0x20000,
	0x20000, 0x20000, 0x20000, 0x20000, 0x20000, 0x20000, 0x20000, 0x20000,
	0x20000, 0x20000, 0x20000, 0x20000, 0x20000, 0x20000, 0x20000, 0x20000,
    0x20000, 0x20000, 0x20000, 0x20000, 0x20000, 0x20000, 0x20000, 0x20000,
	0x20000, 0x20000, 0x20000, 0x20000, 0x20000, 0x20000, 0x20000, 0x20000,
	0x20000, 0x20000, 0x20000, 0x20000, 0x20000, 0x20000, 0x20000, 0x20000,

	0x20000, 0x20000, 0x20000,
    0x4000, 0x4000, 0x4000, 0x4000, 0x4000, 0x4000, 0x4000, 0x4000,
};

struct sectorinfo sinfo[sizeof(SectorSizes)/sizeof(int)];

/* FlashInit():
 * Initialize data structures for each bank of flash...
 */
int
FlashInit()
{
	int	i,	snum;
	uchar	*begin;
	struct	flashinfo *fbnk;

	snum = 0;
	FlashCurrentBank = 0;

	/* Copy functions to ram space...
	 * Note that this MUST be done when cache is disabled to assure that
	 * the RAM is occupied by the designated block of code.
	 */

#ifdef FLASH_COPY_TO_RAM
	if (flashopload((ulong *)Am29bds128h_16x2_type,
		(ulong *)EndAm29bds128h_16x2_type,
		FlashTypeFbuf,sizeof(FlashTypeFbuf)) < 0)
		return(-1);

	if (flashopload((ulong *)Am29bds128h_16x2_erase,
		(ulong *)EndAm29bds128h_16x2_erase,
		FlashEraseFbuf,sizeof(FlashEraseFbuf)) < 0)
		return(-1);

	if (flashopload((ulong *)Am29bds128h_16x2_ewrite,
		(ulong *)EndAm29bds128h_16x2_ewrite,
		FlashEwriteFbuf,sizeof(FlashEwriteFbuf)) < 0)
		return(-1);

	if (flashopload((ulong *)Am29bds128h_16x2_write,
		(ulong *)EndAm29bds128h_16x2_write,
		FlashWriteFbuf,sizeof(FlashWriteFbuf)) < 0)
		return(-1);
#endif

	fbnk = &FlashBank[0];
	fbnk->base = (unsigned char *)FLASH_BANK0_BASE_ADDR;
	fbnk->sectorcnt = (sizeof(SectorSizes)/sizeof(int));
	fbnk->width = 4;

#ifdef FLASH_COPY_TO_RAM
	fbnk->fltype = (int(*)())FlashTypeFbuf;			/* flashtype(). */
	fbnk->flerase = (int(*)())FlashEraseFbuf;		/* flasherase(). */
	fbnk->flwrite = (int(*)())FlashWriteFbuf;		/* flashwrite(). */
	fbnk->flewrite = (int(*)())FlashEwriteFbuf;		/* flashewrite(). */
#else
	fbnk->fltype = Am29bds128h_16x2_type;
	fbnk->flerase = Am29bds128h_16x2_erase;
	fbnk->flwrite = Am29bds128h_16x2_write;
	fbnk->flewrite = Am29bds128h_16x2_ewrite;
#endif
	fbnk->fllock = FlashLockNotSupported;

	fbnk->sectors = sinfo;
	fbnk->id = flashtype(fbnk);

	begin = fbnk->base;
	for(i=0;i<fbnk->sectorcnt;i++,snum++) {
		int	ssize;

		ssize = SectorSizes[i];
		fbnk->sectors[i].snum = snum;
		fbnk->sectors[i].size = ssize;
		fbnk->sectors[i].begin = begin;
		fbnk->sectors[i].end = fbnk->sectors[i].begin + ssize - 1;
		fbnk->sectors[i].protected = 0;
		begin += ssize;
	}
	fbnk->end = begin - 1;

	sectorProtect(FLASH_PROTECT_RANGE,1);

#if FLASHRAM_BASE
	FlashRamInit(snum, FLASHRAM_SECTORCOUNT, &FlashBank[FLASHRAM_BANKNUM],
		sinfoRAM, ramSectors);
#endif

	return(0);
}

#endif	/* SINGLE_FLASH_DEVICE */

#endif	/* INCLUDE_FLASH */
