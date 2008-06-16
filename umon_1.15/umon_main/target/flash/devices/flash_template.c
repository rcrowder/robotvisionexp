/* flash_template.c:
 * Template file for building a flash driver for use with MicroMonitor 1.0
 * Refer to the README for naming conventions and other details regarding
 * uMon1.0's flash device driver interface.
 */
#include "config.h"
#if INCLUDE_FLASH
#include "stddefs.h"
#include "cpu.h"
#include "flash.h"
#include "flash_template.h"

/* Add all device specific macros here.  
 *
 * Note that the flash_template.h file is ONLY used to declare the
 * function prototypes in this file.  Put all other device specific
 * macros here.
 *
 * Define 'ftype' to be vuchar, vushort or vulong depending on the
 * width of the flash bank in this driver.
 */

/*************************************************************************
 *
 * Flashtemplate_erase():
 * Erase the specified sector on the device specified by the flash info
 * poniter.  Return 0 if success, else negative to indicate device-specific
 * error or reason for failure.
 */
int
Flashtemplate_erase(struct flashinfo *fdev,int snum)
{
	return(0);
}

/* EndFlashtemplate_erase():
 * Function place holder to determine the end of the above function.
 */
void
EndFlashtemplate_erase(void)
{}

/*************************************************************************
 *
 * Flashtemplate_write():
 * Write the specified block of data.  Return 0 if success, else negative
 * to indicate device-specific error or reason for failure.
 */
int
Flashtemplate_write(struct flashinfo *fdev,uchar *dest,uchar *src,long bytecnt)
{
	return(0);
}

/* EndFlashtemplate_write():
 * Function place holder to determine the end of the above function.
 */
void
EndFlashtemplate_write(void)
{}

/*************************************************************************
 *
 * Flashtemplate_ewrite():
 * Erase all sectors that are part of the address space to be written,
 * then write the data to that address space.  This is essentially a
 * concatenation of the above erase & write done in one step.  This is used
 * primarily by uMon's ability to update itself; because after the boot
 * code is erased, there is nowhere to return so the re-write must be done
 * while executing out of ram also.  Note that this function is only needed
 * in systems that are executing the monitor out of the same device that
 * is being updated.
 */
int
Flashtemplate_ewrite(struct flashinfo *fdev,uchar *dest,uchar *src,long bytecnt)
{
	/* Upon completion of the 'ewrite' operation, jump to the reset point
	 * to restart the monitor.  Note that you can't return because the
	 * code that called this function has been replaced.
	 */
	reset = RESETFUNC();
	reset();

	/* Should not get here.
	 */
	return(0);
}

/* EndFlashtemplate_ewrite():
 * Function place holder to determine the end of the above function.
 */
void
EndFlashtemplate_ewrite(void)
{}

/*************************************************************************
 *
 * Flashtemplate_lock():
 * Deal with the flash devices's ability to do a per-sector lock/unlock/query.
 * Note that if the device does  not support lock, then the FLASH_LOCKABLE
 * operation should return 0 and all others should return -1.
 */
int
Flashtemplate_lock(struct flashinfo *fdev,int snum,int operation)
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
EndFlashtemplate_lock(void)
{
}

/* Flashtemplate_type():
 * Run the AUTOSELECT algorithm to retrieve the manufacturer and
 * device id of the flash.
 */
int
Flashtemplate_type(struct flashinfo *fdev)
{
	ftype	preval;

	/* Sample the data.
	 */
	preval = FLASH_READBASE();

	/* Add code here to read the device's id. */
	
	/* Wait for the original data to be readable...
	 */
	for(i=0;i<SR_WAIT;i++) {
		if (FLASH_READBASE() == preval)
			break;
	}
	return((int)(fdev->id));
}

/* EndFlashtemplate_type():
 * Function place holder to determine the end of the above function.
 */
void
EndFlashtemplate_type(void)
{}

/**************************************************************************
 **************************************************************************
 *
 * The remainder of the code in this file can be included if the
 * target configuration is such that this flash device is the only
 * real flash device in the system that is to be visible to the monitor.
 * If more than one flash bank is in the system (SINGLE_FLASH_DEVIC is not
 * defined), then the FlashInit() function must be specifically written
 * for multiple banks.
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

#ifndef FLASH_FBUF_SIZE
#define FLASH_FBUF_SIZE 400
#endif

ulong	FlashTypeFbuf[FLASH_FBUF_SIZE];
ulong	FlashEraseFbuf[FLASH_FBUF_SIZE];
ulong	FlashWriteFbuf[FLASH_FBUF_SIZE];
ulong	FlashEwriteFbuf[FLASH_FBUF_SIZE];
ulong	FlashLockFbuf[FLASH_FBUF_SIZE];
#endif

/* FlashNamId[]:
 * Used to correlate between the ID and a string representing the name
 * of the flash device.  Usually, there's only one string; however, some
 * flash device footprints may support compatible devices from different
 * manufacturers; hence, the need for possibly more than one name...
 */
struct flashdesc FlashNamId[] = {
	{ DEVICE_ID1,	"DEVICE1_NAME" },
	{ DEVICE_ID2,	"DEVICE2_NAME" },
	{ DEVICE_ID3,	"DEVICE3_NAME" },
	{ 0, 0 },
};

/* Sector size configuration:
 * Each entry in this table represents the size of a sector in the 
 * flash bank, so establish them appropriately...
 */
int	SectorSizesFlashtemplate[] = {
	0x80000, 	0x80000,	0x80000,	0x80000,
	0x80000, 	0x80000,	0x80000,	0x80000,
	0x80000, 	0x80000,	0x80000,	0x80000,
	0x80000, 	0x80000,	0x80000,	0x80000,
	0x80000, 	0x80000,	0x80000,	0x80000,
	0x80000, 	0x80000,	0x80000,	0x80000,
	0x80000, 	0x80000,	0x80000,	0x80000,
	0x80000, 	0x80000,	0x80000,	0x80000,
};

struct sectorinfo sinfoFlash[sizeof(SectorSizesFlashtemplate)/sizeof(int)];

/* FlashBankInit():
 * Initialize the sector info table for each flash bank.
 * For most systems, there's only one flash bank, but that's not a rule.
 */
int
FlashBankInit(struct flashinfo *fbnk,int snum)
{
	uchar	*saddr;
	int		i, msize;
	struct	sectorinfo *sinfotbl;

	/* Create the per-sector information table.  The size of the table
	 * depends on the number of sectors in the device... 
	 */
	if (fbnk->sectors)
		free((char *)fbnk->sectors);

	msize = fbnk->sectorcnt * (sizeof(struct sectorinfo));
	sinfotbl = (struct sectorinfo *)malloc(msize);
	if (!sinfotbl) {
		printf("Can't allocate space (%d bytes) for flash sector info\n",msize);
		return(-1);
	}
	fbnk->sectors = sinfotbl;

	/* Using the above-determined sector count and size table, build
	 * the sector information table as part of the flash-bank structure:
	 */
	saddr = fbnk->base;
	for(i=0;i<fbnk->sectorcnt;i++) {
		fbnk->sectors[i].snum = snum+i;
		fbnk->sectors[i].size = SectorSizesFlashtemplate[i];
		fbnk->sectors[i].begin = saddr;
		fbnk->sectors[i].end =
		    fbnk->sectors[i].begin + fbnk->sectors[i].size - 1;
		fbnk->sectors[i].protected = 0;
		saddr += SectorSizesFlashtemplate[i];
	}
	fbnk->end = saddr-1;
	return(fbnk->sectorcnt);
}

/* FlashInit():
 * Initialize data structures for each bank of flash.  This is called by
 * the MicroMonitor core code prior to enabling cache.
 */
int
FlashInit(void)
{
	int		snum;
	struct	flashinfo *fbnk;

	snum = 0;
	FlashCurrentBank = 0;

#ifdef FLASH_COPY_TO_RAM

	/* If FLASH_COPY_TO_RAM is defined, then we copy the above driver
	 * functions to ram space.  Note that this MUST be done when cache
	 * is disabled to assure that the RAM is occupied by the designated
	 * block of code.
	 */

	if (flashopload((ulong *)Flashtemplate_lock,
		(ulong *)EndFlashtemplate_lock,
		FlashLockFbuf,sizeof(FlashLockFbuf)) < 0)
		return(-1);
	if (flashopload((ulong *)Flashtemplate_type,
		(ulong *)EndFlashtemplate_type,
		FlashTypeFbuf,sizeof(FlashTypeFbuf)) < 0)
		return(-1);
	if (flashopload((ulong *)Flashtemplate_erase,
		(ulong *)EndFlashtemplate_erase,
		FlashEraseFbuf,sizeof(FlashEraseFbuf)) < 0)
		return(-1);
	if (flashopload((ulong *)Flashtemplate_ewrite,
		(ulong *)EndFlashtemplate_ewrite,
		FlashEwriteFbuf,sizeof(FlashEwriteFbuf)) < 0)
		return(-1);
	if (flashopload((ulong *)Flashtemplate_write,
		(ulong *)EndFlashtemplate_write,
		FlashWriteFbuf,sizeof(FlashWriteFbuf)) < 0)
		return(-1);

#endif

	fbnk = &FlashBank[0];
	fbnk->base = (unsigned char *)FLASH_BANK0_BASE_ADDR;
	fbnk->id = DEVICE_28F320J3;
	fbnk->width = FLASH_BANK0_WIDTH;
	fbnk->sectorcnt = (sizeof(SectorSizes28f320_8x4)/sizeof(int));

#ifdef FLASH_COPY_TO_RAM
	fbnk->fltype = (int(*)())FlashTypeFbuf;	
	fbnk->flerase = (int(*)())FlashEraseFbuf;
	fbnk->flwrite = (int(*)())FlashWriteFbuf;
	fbnk->flewrite = (int(*)())FlashEwriteFbuf;
	fbnk->fllock = (int(*)())FlashLockFbuf;	
#else
	fbnk->fltype = Flashtemplate_type;
	fbnk->flerase = Flashtemplate_erase;
	fbnk->flwrite = Flashtemplate_write;
	fbnk->flewrite = Flashtemplate_ewrite;
	fbnk->fllock = Flashtemplate_lock;
#endif

	snum += FlashBankInit(fbnk,snum);

	sectorProtect(FLASH_PROTECT_RANGE,1);

	return(0);
}

#endif	/* SINGLE_FLASH_DEVICE */

#endif	/* INCLUDE_FLASH */
