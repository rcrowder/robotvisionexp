/* am29f040_8x1.c:
 * Support for AMD 29f040.
 * An 8-bit device in a 1-device-wide bank.
 */
#include "config.h"
#if INCLUDE_FLASH
#include "stddefs.h"
#include "genlib.h"
#include "cpu.h"
#include "flash.h"
#include "am29f040_8x1.h"

#define	ftype		    	volatile unsigned char
#define Read_0000()	    	(*(ftype *)(fdev->base))
#define Read_0001()	    	(*(ftype *)(fdev->base+1))
#define Is_ff(add)	    	(*(ftype *)add == 0xff)
#define Is_not_ff(add)	    (*(ftype *)add != 0xff)
#define Is_Equal(p1,p2)     (*(ftype *)p1 == *(ftype *)p2)
#define Is_Not_Equal(p1,p2) (*(ftype *)p1 != *(ftype *)p2)
#define D5_Timeout(add)	    ((*(ftype *)add & 0x20) == 0x20)

#define SECTOR_ERASE(add) { \
		(*(ftype *)(fdev->base+0x5555) = 0xaa); \
		(*(ftype *)(fdev->base+0x2aaa) = 0x55); \
		(*(ftype *)(fdev->base+0x5555) = 0x80); \
		(*(ftype *)(fdev->base+0x5555) = 0xaa); \
		(*(ftype *)(fdev->base+0x2aaa) = 0x55); \
		(*(ftype *)add = 0x30); }

#define FLASH_WRITE(dest,src) { \
		(*(ftype *)(fdev->base+0x5555) = 0xaa); \
		(*(ftype *)(fdev->base+0x2aaa) = 0x55); \
		(*(ftype *)(fdev->base+0x5555) = 0xa0); \
		(*(ftype *)dest = *(ftype *)src); }

#define AUTO_SELECT() { \
		(*(ftype *)(fdev->base+0x5555) = 0xaa); \
		(*(ftype *)(fdev->base+0x2aaa) = 0x55); \
		(*(ftype *)(fdev->base+0x5555) = 0x90); }

#define READ_RESET() { \
		(*(ftype *)(fdev->base) = 0xf0); }

/* Am29f040_8x1_erase():
 * Based on the 'snum' value, erase the appropriate sector(s).
 * Return 0 if success, else -1.
 */
int
Am29f040_8x1_erase(struct flashinfo *fdev,int snum)
{
	ulong	add;
	int	ret, sector;

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
			SECTOR_ERASE(add);
	
			/* Wait for sector erase to complete or timeout..
			 * DQ7 polling: wait for D7 to be 1.
			 * DQ6 toggling: wait for D6 to not toggle.
			 * DQ5 timeout: if DQ7 is 0, and DQ5 = 1, timeout.
			 */
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

	/* If the erase failed for some reason, then issue the read/reset
	 * command sequence prior to returning...
	 */
	if (ret == -1) {
		READ_RESET();
	}
	return(ret);
}

/* EndAm29f040_8x1_erase():
 * Function place holder to determine the end of the above function.
 */
void
EndAm29f040_8x1_erase(void)
{}


/* Am29f040_8x1_write():
 * Copy specified number of bytes from source to destination.  The destination
 * address is assumed to be flash space.
 */
int
Am29f040_8x1_write(struct flashinfo *fdev,unsigned char *dest,unsigned char *src,long bytecnt)
{
	int	i, ret;

	/* Each pass through this loop writes 'fdev->width' bytes...
	 */
	ret = 0;
	for (i=0;i<bytecnt;i+=fdev->width) {

		FLASH_WRITE(dest,src);

		/* Wait for write to complete or timeout.
		 */
		while(1) {
			if (Is_Equal(dest,src)) {
				if (Is_Equal(dest,src))
					break;
			}
			/* Check D5 for timeout...
			 */
			if (D5_Timeout(dest)) {
				if (Is_Not_Equal(dest,src))
					ret = -1;
				goto done;
			}
		}
		dest++; src++;
	}

done:
	READ_RESET();
	return(ret);
}

/* EndAm29f040_8x1_write():
 * Function place holder to determine the end of the above function.
 */
void
EndAm29f040_8x1_write(void)
{}

/* Am29f040_8x1_ewrite():
 * Erase all sectors that are part of the address space to be written,
 * then write the data to that address space.  This is basically a
 * concatenation of the above erase & write done in one step.  This is
 * necessary primarily for re-writing the bootcode; because after the boot
 * code is erased, there is nowhere to return so the re-write must be done
 * while executing out of ram also.  It is only needed in systems that are
 * executing the monitor out of the same device that is being updated.
 */
int
Am29f040_8x1_ewrite(struct flashinfo *fdev,unsigned char *dest,unsigned char *src,long bytecnt)
{
	volatile ulong	add;
	void	(*reset)();
	ftype	*src1, *dest1;
	volatile int	i;

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

		SECTOR_ERASE(add);
	
		/* Wait for sector erase to complete or timeout..
		 * DQ7 polling: wait for D7 to be 1.
		 * DQ6 toggling: wait for D6 to not toggle.
		 * DQ5 timeout: if DQ7 is 0, and DQ5 = 1, timeout.
		 */
		while(1) {
			if (Is_ff(add)) {
				if (Is_ff(add))
					break;
			}
			/* Check D5 for timeout...
			 * In this case, there is nothing to return to
			 * because the flash was just erased, so just break.
			 */
			if (D5_Timeout(add)) {
				goto quit;
			}
		}
		add += fdev->sectors[i].size;
	}

	/* Read/reset command:
	 */
	READ_RESET();

	for(i=0;i<bytecnt;i+=fdev->width) {
		FLASH_WRITE(dest,src);

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
	/* Issue the read/reset command sequence:
	 */
	READ_RESET();

	/* Wait till flash is readable, or timeout: */
	for(i=0;i<FLASH_LOOP_TIMEOUT;i++) {
		if (Is_Equal(dest1,src1))
			break;
	}
	for(i=0;i<FLASH_LOOP_TIMEOUT;i++);

	/* Now that the re-programming of flash is complete, reset:
	 */
	reset = RESETFUNC();
	reset();

	return(0);	/* won't get here */
}

/* EndAm29f040_8x1_ewrite():
 * Function place holder to determine the end of the above function.
 */
void
EndAm29f040_8x1_ewrite(void)
{}

/* Am29f040_8x1_type():
 * Run the AUTOSELECT algorithm to retrieve the manufacturer and
 * device id of the flash.
 *
 * Note: there is one additional step that I found necessary to keep
 * SGS29040 device happy... For some reason after issuing the read/reset
 * command and returning (to code that actually executes out of the FLASH
 * device) I was consistently getting an illegal opcode exception at
 * the return location in the flash.  It appears that the SGS part needs
 * a bit of time after the read/reset to be able to fetch an instruction.
 * Reading a value in the flash (stored in val) prior to issuing the
 * command sequence, then waiting for that read to be the same after 
 * issuing the read/reset, assures the algorithm of not returning unless
 * the flash device is readable.  Note that I found this ONLY to be necessary
 * for the signature read command of SGS flash.
 */
int
Am29f040_8x1_type(struct flashinfo *fdev)
{
	volatile int	i;
	ftype	val;
	ushort	man, dev;

	val = Read_0000();

	AUTO_SELECT();
	
	for(i=0;i<FLASH_LOOP_TIMEOUT;i++) {
		if (Read_0000() != val)
			break;
	}

	man = (ushort)Read_0000();	/* manufacturer ID */
	dev = (ushort)Read_0001();	/* device ID */
	man &= 0xff;
	dev &= 0xff;
	man <<=8;
	dev |= man;

	fdev->id = (ushort)dev;

	READ_RESET();

	/* The SGS29040 seems to need this, and it doesn't hurt anything
	 * when using the AMD parts...
	 */
	for(i=0;i<FLASH_LOOP_TIMEOUT;i++) {
		if (val == Read_0000())
			break;
	}
	return((int)(fdev->id));
}

/* EndAm29f040_8x1_type():
 * Function place holder to determine the end of the above function.
 */
void
EndAm29f040_8x1_type(void)
{}

/* Am29f040 doesn't support lock so...
 */
int
Am29f040_8x1_lock(struct flashinfo *fdev,int snum,int operation)
{
	vulong	add;

	add = (ulong)(fdev->base);

	if (operation == FLASH_LOCKABLE) {
		return(0);		/* Return 1 if device is lockable; else 0. */
	}
	else if (operation == FLASH_UNLOCK) {
		return(-1);		/* Return negative for failure else 1 */
	}
	else if (operation == FLASH_LOCK) {
		return(-1);		/* Return negative for failure else 1 */
	}
	else if (operation == FLASH_LOCKQRY) {
		return(-1);		/* Return 1 if device is locked else 0. */
	}
	else
		return(-1);
}

/* EndFlashtemplate_lock():
 * Function place holder to determine the end of the above function.
 */
void
EndAm29f040_8x1_lock(void)
{
}


/**************************************************************************
 **************************************************************************
 *
 * The remainder of the code in this file can be included if the
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
ulong	FlashTypeFbuf[400];
ulong	FlashEraseFbuf[400];
ulong	FlashWriteFbuf[400];
ulong	FlashEwriteFbuf[400];
ulong	FlashLockFbuf[400];
#endif

/* FlashNamId[]:
 * Used to correlate between the ID and a string representing the name
 * of the flash device.
 */
struct flashdesc FlashNamId[] = {
	{ STM_29F040,	"STM-29F040" },		/* SGS-Thompson Microelectronics */
	{ STM_M29W040B,	"STM-M29W040B" },
	{ AMD_29F040,	"AMD-29F040" },		/* AMD */
	{ AMD_29F010,	"AMD-29F010" },
	{ AMD_29LV040,	"AMD-29LV040" }
};

struct sectorinfo sinfo040[8];

/* FlashBankInit():
 * Initialize flash structures and determine flash device type.
 */
int
FlashBankInit(struct flashinfo *fbnk, int snum)
{
	int	i, ssize;

	flashtype(fbnk);
	switch(fbnk->id) {
		case AMD_29LV040:
		case STM_M29W040B:
		case STM_29F040:
		case AMD_29F040:
			fbnk->sectorcnt = 8;
			ssize = 0x10000 * fbnk->width;
			fbnk->end = fbnk->base + (0x80000 * fbnk->width) - 1;
			break;
		case AMD_29F010:
			fbnk->sectorcnt = 8;
			ssize = 0x4000 * fbnk->width;
			fbnk->end = fbnk->base + (0x20000 * fbnk->width) - 1;
			break;
		default:
			printf("Flash device id 0x%lx unknown\n", fbnk->id);
			return(-1);
	}
	for(i=0;i<fbnk->sectorcnt;i++) {
		fbnk->sectors[i].snum = snum+i;
		fbnk->sectors[i].size = ssize;
		fbnk->sectors[i].begin = fbnk->base + (i*ssize);
		fbnk->sectors[i].end = fbnk->sectors[i].begin + ssize - 1;
		fbnk->sectors[i].protected = 0;
	}
	return(8);
}

/* FlashInit():
 * Initialize data structures for each bank of flash...
 */
int
FlashInit(void)
{
	int		snum;
	struct	flashinfo *fbnk;

	snum = 0;
	FlashCurrentBank = 0;

	/* Copy functions to ram space...
	 * Note that this MUST be done when cache is disabled to assure that
	 * the RAM is occupied by the designated block of code.
	 */

#ifdef FLASH_COPY_TO_RAM
	if (flashopload((ulong *)Am29f040_8x1_type,
		(ulong *)EndAm29f040_8x1_type,
		FlashTypeFbuf,sizeof(FlashTypeFbuf)) < 0)
		return(-1);

	if (flashopload((ulong *)Am29f040_8x1_erase,
		(ulong *)EndAm29f040_8x1_erase,
		FlashEraseFbuf,sizeof(FlashEraseFbuf)) < 0)
		return(-1);

	if (flashopload((ulong *)Am29f040_8x1_ewrite,
		(ulong *)EndAm29f040_8x1_ewrite,
		FlashEwriteFbuf,sizeof(FlashEwriteFbuf)) < 0)
		return(-1);

	if (flashopload((ulong *)Am29f040_8x1_write,
		(ulong *)EndAm29f040_8x1_write,
		FlashWriteFbuf,sizeof(FlashWriteFbuf)) < 0)
		return(-1);

	if (flashopload((ulong *)Am29f040_8x1_lock,
		(ulong *)EndAm29f040_8x1_lock,
		FlashLockFbuf,sizeof(FlashLockFbuf)) < 0)
		return(-1);
#endif

	fbnk = &FlashBank[0];
	fbnk->base = (unsigned char *)FLASH_BANK0_BASE_ADDR;
	fbnk->width = FLASH_BANK0_WIDTH;

#ifdef FLASH_COPY_TO_RAM
	fbnk->fltype = (int(*)())FlashTypeFbuf;
	fbnk->flerase = (int(*)())FlashEraseFbuf;
	fbnk->flwrite = (int(*)())FlashWriteFbuf;
	fbnk->flewrite = (int(*)())FlashEwriteFbuf;
	fbnk->fllock = (int(*)())FlashLockFbuf;
#else
	fbnk->fltype = Am29f040_8x1_type;
	fbnk->flerase = Am29f040_8x1_erase;
	fbnk->flwrite = Am29f040_8x1_write;
	fbnk->flewrite = Am29f040_8x1_ewrite;
	fbnk->fllock = Am29f040_8x1_lock;
#endif
	fbnk->fllock = FlashLockNotSupported;

	fbnk->sectors = sinfo040;
	snum += FlashBankInit(fbnk,snum);

	sectorProtect(FLASH_PROTECT_RANGE,1);

#if FLASHRAM_BASE
	FlashRamInit(snum, FLASHRAM_SECTORCOUNT, &FlashBank[FLASHRAM_BANKNUM],
		sinfoRAM, 0);
#endif

	return(0);
}

#endif	/* SINGLE_FLASH_DEVICE */

#endif	/* INCLUDE_FLASH */
