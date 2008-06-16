/* s29al032d_16x1.c:
 * Device interface for the Spansion S29AL032D flash device configured
 * for x16 mode with 1 device in parallel.  
 */
#include "config.h"

#if INCLUDE_FLASH

#include "stddefs.h"
#include "genlib.h"	
#include "cpu.h"
#include "flash.h"			/* Part of monitor common code */
#include "s29al032d_16x1.h"

#define ftype 					volatile unsigned short
#define Is_ff(add)			   	(*(ftype *)add == 0xffff)
#define Is_not_ff(add)	    	(*(ftype *)add != 0xffff)
#define Is_Equal(p1,p2) 	    (*(ftype *)p1 == *(ftype *)p2)
#define Is_Not_Equal(p1,p2)		(*(ftype *)p1 != *(ftype *)p2)
#define Fwrite(to,frm)  	    (*(ftype *)to = *(ftype *)frm)
#define D5_Timeout(add)	   		((*(ftype *)add & 0x0020) == 0x0020)


#define SECTOR_ERASE(add)		{ \
		*(ftype *)(fdev->base+(0x555<<1)) = 0x00aa; \
		*(ftype *)(fdev->base+(0x2aa<<1)) = 0x0055; \
		*(ftype *)(fdev->base+(0x555<<1)) = 0x0080; \
		*(ftype *)(fdev->base+(0x555<<1)) = 0x00aa; \
		*(ftype *)(fdev->base+(0x2aa<<1)) = 0x0055; \
		*(ftype *)add = 0x0030; }

#define FLASH_WRITE(dest,src)	{ 	\
		*(ftype *)(fdev->base+(0x555<<1)) = 0x00aa; \
		*(ftype *)(fdev->base+(0x2aa<<1)) = 0x0055; \
		*(ftype *)(fdev->base+(0x555<<1)) = 0x00a0; \
		Fwrite(dest,src); }

#define AUTO_SELECT()			{	\
		*(ftype *)(fdev->base+(0x555<<1)) = 0x00aa; \
		*(ftype *)(fdev->base+(0x2aa<<1)) = 0x0055; \
		*(ftype *)(fdev->base+(0x555<<1)) = 0x0090; }

#define READ_RESET() 			{	\
		ftype val;	\
		*(ftype *)(fdev->base) = 0x00f0; \
		val = *(ftype *)(fdev->base); }


/* S29al032d_16x1_erase():
 * Based on the 'snum' value, erase the appropriate sector(s).
 * Return 0 if success, else -1.
 */
int
S29al032d_16x1_erase(struct flashinfo *fdev,int snum)
{
	ulong	add;
	int		ret;

	ret = 0;
	add = (ulong)(fdev->sectors[snum].begin);

				SECTOR_ERASE(add);

	/* Wait for sector erase to complete or timeout..
	 * DQ7 polling: wait for D7 to be 1.
	 * DQ6 toggling: wait for D6 to not toggle.
	 * DQ5 timeout: if DQ7 is 0, and DQ5 = 1, timeout.
	 */
				while(1) {
					WATCHDOG_MACRO;
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

	/* If the erase failed for some reason, then issue the read/reset
	 * command sequence prior to returning...
	 */
	if (ret == -1)  
		READ_RESET();

	return(ret);
}

/* EndS29al032d_16x1_erase():
 * Function place holder to determine the end of the above function.
 */
void
EndS29al032d_16x1_erase(void)
{
}

/* S29al032d_16x1_write():
 * Copy specified number of bytes from source to destination.  The destination
 * address is assumed to be flash space.
 */
int
S29al032d_16x1_write(struct flashinfo *fdev,
	uchar *dest,uchar *src,long bytecnt)
{
	ftype	val;
	long	cnt;
	int		i, ret;
	uchar 	*src1;

	ret = 0;
	src1 = (uchar *)&val;

	/* Since we are working on a 2-byte wide device, every write to the
	 * device must be aligned on a 2-byte boundary.  If our incoming
	 * destination address is odd, then decrement the destination by one
	 * and build a fake source using *dest-1 and src[0]...
	 */
	if (NotAligned16(dest)) {
		dest--;
		
		src1[0] = *dest;
		src1[1] = *src;

		FLASH_WRITE(dest,src1);

		/* Wait for write to complete or timeout. */
		while(1) {
			WATCHDOG_MACRO;

			if (Is_Equal(dest,src1)) {
				if (Is_Equal(dest,src1))
					break;
			}
			/* Check D5 for timeout... */
			if (D5_Timeout(dest)) {
				if (Is_Not_Equal(dest,src1)) {
					ret = -1;
					goto done;
				}
				break;
			}
		}

		dest += 2;
		src++;
		bytecnt--;
	}

	cnt = bytecnt & ~1;

	/* Each pass through this loop writes 'fdev->width' bytes...
	 */

	for (i=0;i<cnt;i+=fdev->width) {

		/* Just in case src is not aligned... */
		src1[0] = src[0];
		src1[1] = src[1];

		FLASH_WRITE(dest,src1);
		
		/* Wait for write to complete or timeout. */
		while(1) {
			WATCHDOG_MACRO;

			if (Is_Equal(dest,src1)) {
				if (Is_Equal(dest,src1))
					break;
			}
			/* Check D5 for timeout... */
			if (D5_Timeout(dest)) {
				if (Is_Not_Equal(dest,src1)) {
					ret = -1;
					goto done;
				}
				break;
			}
		}
		dest += fdev->width; 
		src += fdev->width;
	}

	/* Similar to the front end of this function, if the byte count is not
	 * even, then we have one byte left to write, so we need to write a 
	 * 16-bit value by writing the last byte, plus whatever is already in
	 * the next flash location.
	 */
	if (cnt < bytecnt) {
		src1[0] = *src;
		src1[1] = dest[1];

		FLASH_WRITE(dest,src1);

		/* Wait for write to complete or timeout. */
		while(1) {
			WATCHDOG_MACRO;

			if (Is_Equal(dest,src1)) {
				if (Is_Equal(dest,src1))
					break;
			}
			/* Check D5 for timeout... */
			if (D5_Timeout(dest)) {
				if (Is_Not_Equal(dest,src1)) {
					ret = -1;
					goto done;
				}
				break;
			}
		}
	}

done:
	READ_RESET();
	return(ret);
}

/* EndS29al032d_16x1_write():
 * Function place holder to determine the end of the above function.
 */
void
EndS29al032d_16x1_write(void)
{}

/* S29al032d_16x1_ewrite():
 * Erase all sectors that are part of the address space to be written,
 * then write the data to that address space.  This is basically a
 * concatenation of the above erase & write done in one step.  This is
 * necessary primarily for re-writing the bootcode; because after the boot
 * code is erased, there is nowhere to return so the re-write must be done
 * while executing out of ram also.  It is only needed in systems that are
 * executing the monitor out of the same device that is being updated.
 */
int
S29al032d_16x1_ewrite(struct flashinfo *fdev,
	uchar *dest,uchar *src,long bytecnt)
{
	volatile int	i;
	ulong	add;
	void	(*reset)();
	ftype	*src1, *dest1;

	add = (ulong)(fdev->base);
	src1 = (ftype *)src;
	dest1 = (ftype *)dest;

	/* For each sector, if it overlaps any of the destination space
	 * then erase that sector.
	 */
	for (i=0;i<fdev->sectorcnt;i++) {
		if ((((uchar *)dest) > (fdev->sectors[i].end)) ||
		    (((uchar *)dest+bytecnt-1) < (fdev->sectors[i].begin))) {
			add += fdev->sectors[i].size;
			continue;
		}

		SECTOR_ERASE(add);

		/* Wait for sector erase to complete or timeout..
		 * DQ7 polling: wait for D7 to be 1.
		 * DQ6 toggling: wait for D6 to not toggle.
		 * DQ5 timeout: if DQ7 is 0, and DQ5 = 1, timeout.
		 */
		while(1) {
			WATCHDOG_MACRO;

			if (Is_ff(add)) {
				if (Is_ff(add))
					break;
			}
		}
		add += fdev->sectors[i].size;
	}

	READ_RESET();

	for(i=0;i<bytecnt;i+=fdev->width) {
		FLASH_WRITE(dest,src);

		while(1) {
			WATCHDOG_MACRO;

			if (Is_Equal(dest,src)) {
				if (Is_Equal(dest,src))
					break;
			}
		}

		dest+=2; 
		src+=2;
	}

	/* Issue the read/reset command sequence: */
	READ_RESET();

	/* Wait till flash is readable, or timeout: */
	for(i=0;i<FLASH_TIMEOUT;i++) {
		WATCHDOG_MACRO;

		if (Is_Equal(dest1,src1))
			break;
	}

	/* Now that the re-programming of flash is complete, reset: */
	reset = RESETFUNC();
	reset();

	return(0);	/* won't get here */
}

/* EndS29al032d_16x1_ewrite():
 * Function place holder to determine the end of the above function.
 */
void
EndS29al032d_16x1_ewrite(void)
{}


/* S29al032d_16x1_type():
 * Run the AUTOSELECT algorithm to retrieve the manufacturer and
 * device id of the flash.
 */
int
S29al032d_16x1_type(struct flashinfo *fdev)
{
	ushort	man, dev;
	ulong	id;

	/* Enter "auto-select" mode and retrieve manufacturer and device ID:
	 */
	AUTO_SELECT();

	man = *(ftype *)(fdev->base);		/* manufacturer ID */
	dev = *(ftype *)(fdev->base + 2);	/* device ID */

	READ_RESET();

	id = man;
	id <<= 16;
	id |= dev;

	fdev->id = id;


	return((int)(fdev->id));
}

/* EndS29al032d_16x1_type():
 * Function place holder to determine the end of the above function.
 */
void
EndS29al032d_16x1_type(void)
{}

/**************************************************************************
 **************************************************************************
 *
 * The remainder of the code in this file should only included if the
 * target configuration is such that this AM29F040 device is the only
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
	{ S29AL032DT,	"Sp-S29al032dT" },	/* Spansion part */
	{ S29AL032DB,	"Sp-S29al032dB" },
	{ 0, (char *)0 },
};

int	SectorSizes032DT[] = {	/* Top boot */
	0x10000, 0x10000, 0x10000, 0x10000, 0x10000, 0x10000, 0x10000, 0x10000,
	0x10000, 0x10000, 0x10000, 0x10000, 0x10000, 0x10000, 0x10000, 0x10000,
	0x10000, 0x10000, 0x10000, 0x10000, 0x10000, 0x10000, 0x10000, 0x10000,
	0x10000, 0x10000, 0x10000, 0x10000, 0x10000, 0x10000, 0x10000, 0x10000,
	0x10000, 0x10000, 0x10000, 0x10000, 0x10000, 0x10000, 0x10000, 0x10000,
	0x10000, 0x10000, 0x10000, 0x10000, 0x10000, 0x10000, 0x10000, 0x10000,
	0x10000, 0x10000, 0x10000, 0x10000, 0x10000, 0x10000, 0x10000, 0x10000,
	0x10000, 0x10000, 0x10000, 0x10000, 0x10000, 0x10000, 0x10000,
	0x02000, 0x02000, 0x02000, 0x02000, 0x02000, 0x02000, 0x02000, 0x02000,
};

int	SectorSizes032DB[] = {	/* Bottom boot */
	0x02000, 0x02000, 0x02000, 0x02000, 0x02000, 0x02000, 0x02000, 0x02000,
	0x10000, 0x10000, 0x10000, 0x10000, 0x10000, 0x10000, 0x10000, 0x10000,
	0x10000, 0x10000, 0x10000, 0x10000, 0x10000, 0x10000, 0x10000, 0x10000,
	0x10000, 0x10000, 0x10000, 0x10000, 0x10000, 0x10000, 0x10000, 0x10000,
	0x10000, 0x10000, 0x10000, 0x10000, 0x10000, 0x10000, 0x10000, 0x10000,
	0x10000, 0x10000, 0x10000, 0x10000, 0x10000, 0x10000, 0x10000, 0x10000,
	0x10000, 0x10000, 0x10000, 0x10000, 0x10000, 0x10000, 0x10000, 0x10000,
	0x10000, 0x10000, 0x10000, 0x10000, 0x10000, 0x10000, 0x10000, 0x10000,
	0x10000, 0x10000, 0x10000, 0x10000, 0x10000, 0x10000, 0x10000,
};

struct sectorinfo sinfo320[sizeof(SectorSizes032DB)/sizeof(int)];

/* FlashInit():
 * Initialize data structures for each bank of flash...
 */
int
FlashInit()
{
	uchar	*begin;
	struct	flashinfo *fbnk;
	int	i,	snum, *sizetbl;

	snum = 0;
	FlashCurrentBank = 0;

	/* If code is in flash, then we must copy the flash ops to RAM.
	 * Note that this MUST be done when cache is disabled to assure
	 * that the RAM is occupied by the designated block of code.
	 */

#ifdef FLASH_COPY_TO_RAM
	if (flashopload((ulong *)S29al032d_16x1_type,
		(ulong *)EndS29al032d_16x1_type,
		FlashTypeFbuf,sizeof(FlashTypeFbuf)) < 0)
		return(-1);
	
	if (flashopload((ulong *)S29al032d_16x1_erase,
		(ulong *)EndS29al032d_16x1_erase,
		FlashEraseFbuf,sizeof(FlashEraseFbuf)) < 0)
		return(-1);
	
	if (flashopload((ulong *)S29al032d_16x1_ewrite,
		(ulong *)EndS29al032d_16x1_ewrite,
		FlashEwriteFbuf,sizeof(FlashEwriteFbuf)) < 0)
		return(-1);
	
	if (flashopload((ulong *)S29al032d_16x1_write,
		(ulong *)EndS29al032d_16x1_write,
		FlashWriteFbuf,sizeof(FlashWriteFbuf)) < 0)
		return(-1);
#endif

	fbnk = &FlashBank[0];
	fbnk->base = (unsigned char *)FLASH_BANK0_BASE_ADDR;
	fbnk->end = fbnk->base + FLASH_BANK0_SIZE - 1;
	fbnk->width = FLASH_BANK0_WIDTH;

#ifdef FLASH_COPY_TO_RAM
	fbnk->fltype = (int(*)())FlashTypeFbuf;
	fbnk->flerase = (int(*)())FlashEraseFbuf;
	fbnk->flwrite = (int(*)())FlashWriteFbuf;
	fbnk->flewrite = (int(*)())FlashEwriteFbuf;
#else
	fbnk->fltype = S29al032d_16x1_type;
	fbnk->flerase = S29al032d_16x1_erase;
	fbnk->flwrite = S29al032d_16x1_write;
	fbnk->flewrite = S29al032d_16x1_ewrite;
#endif

	/* This device requires 12V on reset to support flash lock, do not assume that, so set the pointer
	 * to the default FlashLockNotSupported() function...
	 */
	fbnk->fllock = FlashLockNotSupported;

	fbnk->sectors = sinfo320;
	fbnk->id = flashtype(fbnk);
	if (fbnk->id == S29AL032DT) {
		sizetbl = SectorSizes032DT;
		fbnk->sectorcnt = (sizeof(SectorSizes032DT)/sizeof(int));
	}
	else if (fbnk->id == S29AL032DB) {
		sizetbl = SectorSizes032DB;
		fbnk->sectorcnt = (sizeof(SectorSizes032DB)/sizeof(int));
	}
	else {
		printf("Invalid bank id: 0x%lx\n",fbnk->id);
		return(-1);
	}

	begin = fbnk->base;
	for(i=0;i<fbnk->sectorcnt;i++,snum++) {
		int	ssize;

		ssize = sizetbl[i];
		fbnk->sectors[i].snum = snum;
		fbnk->sectors[i].size = ssize;
		fbnk->sectors[i].begin = begin;
		fbnk->sectors[i].end = fbnk->sectors[i].begin + ssize - 1;
		fbnk->sectors[i].protected = 0;
		begin += ssize;
	}

	sectorProtect(FLASH_PROTECT_RANGE,1);

#if FLASHRAM_BASE
	FlashRamInit(snum, FLASHRAM_SECTORCOUNT, &FlashBank[FLASHRAM_BANKNUM],
		sinfoRAM, 0);
#endif

	return(0);
}

#endif	/* SINGLE_FLASH_DEVICE */

#endif	/* INCLUDE_FLASH */
