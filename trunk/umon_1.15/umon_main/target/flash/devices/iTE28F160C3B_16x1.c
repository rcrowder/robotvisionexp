/* iTE28F160C3B_16x1.c:
 * Support for INTEL TE28F160C3B.
 * A 16-bit device in a 1-device-wide bank.
 */
#include "config.h"
#if INCLUDE_FLASH
#define FLASHCFG_16x1	1
#include "stddefs.h"
#include "genlib.h"
#include "cpu.h"
#include "flash.h"
#include "iTE28F160C3B_16x1.h"

#include "strata.h"

/* Manufacturer and device id... */
//#define INTEL_DT28F640J5		0x00890015
//#define INTEL_28F640			0x00890017
//#define INTEL_DT28F128J5		0x00890018
#define INTEL_iTE28F160C3B		0x008988C3

/* iTE28F160C3B_16x1_erase():
 * Based on the 'snum' value, erase the appropriate sector(s).
 * Return 0 if success, else -1.
 */
int
iTE28F160C3B_16x1_erase(struct flashinfo *fdev,int snum)
{
	/* Issue the setup/confirm sequence:
	 */
	STRATACMD_BLOCKERASE(fdev->sectors[snum].begin);
	STRATACMD_CONFIRM(fdev->sectors[snum].begin);

	/* Wait for completion:
	 */
	WAIT_FOR_WSMS_STATUS_READY();

	/* Cleanup:
	 */
	STRATACMD_CLEARSTATUS();
	STRATACMD_READARRAY();
	WAIT_FOR_FF(fdev->sectors[snum].begin);
	return(0);
}

/* EndiTE28F160C3B_16x1_erase():
 * Function place holder to determine the end of the above function.
 */
void
EndiTE28F160C3B_16x1_erase(void)
{}


/* iTE28F160C3B_16x1_write():
 * Copy specified number of bytes from source to destination.  The destination
 * address is assumed to be flash space.
 */
int
iTE28F160C3B_16x1_write(struct flashinfo *fdev,uchar *dest,uchar *src,
	long bytecnt)
{
	int				i;

	for(i = 0; i < bytecnt; i += sizeof(ftype)) {

		/* Flash program setup command */
		STRATACMD_PROGRAM();
		
		/* Write the value */
		FLASH_WRITEPTR(dest,src);

		/* Wait for completion */
		WAIT_FOR_WSMS_READY();

		/* Cleanup */
		STRATACMD_CLEARSTATUS();
		STRATACMD_READARRAY();

		/* Verify data */
		WAIT_FOR_DATA(dest,src);		

		dest+=sizeof(ftype);
		src+=sizeof(ftype);
	}
	return(0);
}

/* EndiTE28F160C3B_16x1_write():
 * Function place holder to determine the end of the above function.
 */
void
EndiTE28F160C3B_16x1_write(void)
{}

/* iTE28F160C3B_16x1_ewrite():
 * Erase all sectors that are part of the address space to be written,
 * then write the data to that address space.  This is basically a
 * concatenation of the above erase & write done in one step.  This is
 * necessary primarily for re-writing the bootcode; because after the boot
 * code is erased, there is nowhere to return so the re-write must be done
 * while executing out of ram also.  It is only needed in systems that are
 * executing the monitor out of the same device that is being updated.
 */
int
iTE28F160C3B_16x1_ewrite(struct flashinfo *fdev,uchar *destA,uchar *srcA,
	long bytecnt)
{
	int	    sector, i;
	void	(*reset)();
	uchar   *src, *dest;

	src = srcA;
	dest = destA;

	STRATACMD_CLEARSTATUS();
	STRATACMD_READARRAY();

	FLASHOP_PRINT(("ewrite erase...\n"));

	/* For each sector, if it overlaps any of the destination space */
	/* then erase that sector. */
	for (sector = 0; sector < fdev->sectorcnt; sector++) {
		if ((((uchar *)dest) > (fdev->sectors[sector].end)) ||
		    (((uchar *)dest+bytecnt-1) < (fdev->sectors[sector].begin))) {
			continue;
		}

		FLASHOP_PRINT(("erase %d...\n",sector));

		/* Issue the ERASE setup/confirm sequence: */
		STRATACMD_BLOCKERASE(fdev->sectors[sector].begin);
		STRATACMD_CONFIRM(fdev->sectors[sector].begin);
		WAIT_FOR_WSMS_STATUS_READY();
		STRATACMD_CLEARSTATUS();
		STRATACMD_READARRAY();
		WAIT_FOR_FF(fdev->sectors[sector].begin);
	}

	FLASHOP_PRINT(("ewrite write...\n"));

	for(i = 0; i < bytecnt; i += sizeof(ftype)) {

		/* Flash program setup command */
		STRATACMD_PROGRAM();
		
		/* Write the value */
		FLASH_WRITEPTR(dest,src);

		/* Wait for completion */
		WAIT_FOR_WSMS_READY();

		/* Cleanup */
		STRATACMD_CLEARSTATUS();
		STRATACMD_READARRAY();

		/* Verify data */
		WAIT_FOR_DATA(dest,src);		

		dest+=sizeof(ftype);
		src+=sizeof(ftype);
	}
	
	FLASHOP_PRINT(("ewrite done...\n"));
	FLASHOP_FLUSH();

	/* Now that the re-programming of flash is complete, reset: */
	reset = RESETFUNC();
	reset();

	return(0);	/* won't get here */
}

/* EndiTE28F160C3B_16x1_ewrite():
 * Function place holder to determine the end of the above function.
 */
void
EndiTE28F160C3B_16x1_ewrite(void)
{}

/* iTE28F160C3B_16x1_lock():
 */
int
iTE28F160C3B_16x1_lock(struct flashinfo *fdev,int snum,int operation)
{
	ftype sample, bstat;

	sample = FLASH_READBASE();

	if (operation == FLASH_LOCKABLE) {
		return(1);
	}
	else if (operation == FLASH_UNLOCK) {
		STRATACMD_LOCKBIT();
		STRATACMD_CONFIRM(fdev->sectors[snum].begin);
		WAIT_FOR_WSMS_READY();
		STRATACMD_READARRAY();
		WAIT_FOR_DATA(fdev->base,&sample);		
		return(0);
	}
	else if (operation == FLASH_LOCK) {
		STRATACMD_LOCKBIT();
		STRATACMD_SETLOCKCONFIRM(fdev->sectors[snum].begin);
		WAIT_FOR_WSMS_READY();
		STRATACMD_READARRAY();
		WAIT_FOR_DATA(fdev->base,&sample);		
		return(0);
	}
	else if (operation == FLASH_LOCKQRY) {
		STRATACMD_READID();
		WAIT_FOR_WSMS_READY();
		bstat = FLASH_READ_BLOCKSTATUS(fdev->sectors[snum].begin + 0x0002);
		STRATACMD_READARRAY();
		FLASHOP_PRINT(("bstat = 0x%lx\n",(long)bstat));
		if ((bstat & 0x0001) == 0x0001)
			return(1);
		else
			return(0);
	}
	else
		return(-1);
}

/* EndiTE28F160C3B_16x1_lock():
 * Function place holder to determine the end of the above function.
 */
void
EndiTE28F160C3B_16x1_lock(void)
{
}

/* iTE28F160C3B_16x1_type():
 * Run the AUTOSELECT algorithm to retrieve the manufacturer and
 * device id of the flash.
 */
int
iTE28F160C3B_16x1_type(struct flashinfo *fdev)
{
	ulong	id;
	ftype	sample;
	ushort	man, dev;

	sample = FLASH_READBASE();

	/* Issue the read configuration command: */
	STRATACMD_READID();

	man = (ushort)FLASH_READBASE();			/* Manufacturer ID */
	dev = (ushort)FLASH_READ_DEVICEID();	/* device ID */
	id = man;
	id <<= 16;
	id |= dev;

	fdev->id = id;

	/* Issue the read array command: */
	STRATACMD_READARRAY();
	
	/* Wait for the original data to be readable... */
	WAIT_FOR_DATA(fdev->base,&sample);

	return((int)(fdev->id));
}

/* EndiTE28F160C3B_16x1_type():
 * Function place holder to determine the end of the above function.
 */
void
EndiTE28F160C3B_16x1_type(void)
{}

/**************************************************************************
 **************************************************************************
 *
 * The remainder of the code in this file can be included if the
 * target configuration is such that this 28F128 device is the only
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
 * of the flash device.  Note that this will support the presence of
 * a '640 device.  The sector count is simply cut in half.
 */
struct flashdesc FlashNamId[] = {
	{ INTEL_iTE28F160C3B,	"INTEL-TE28F160C3B" },
	{ 0, 0 },
};

/* This configuration is a single TE28F160C3B device.
 */
int	SectorSizes[] = {
	 0x2000,  0x2000,  0x2000,  0x2000,  0x2000,  0x2000,  0x2000,  0x2000,	//0-7
	0x10000, 0x10000, 0x10000, 0x10000, 0x10000, 0x10000, 0x10000, 0x10000,	//8-15
	0x10000, 0x10000, 0x10000, 0x10000, 0x10000, 0x10000, 0x10000, 0x10000,	//16-23
	0x10000, 0x10000, 0x10000, 0x10000, 0x10000, 0x10000, 0x10000, 0x10000, //24-31
	0x10000, 0x10000, 0x10000, 0x10000, 0x10000, 0x10000, 0x10000,         	//32-38
};


struct sectorinfo sinfo[sizeof(SectorSizes)/sizeof(int)];

int
FlashBankInit(struct flashinfo *fbnk,int snum)
{
	uchar	*saddr;
	int		i, *sizetable, msize;
	struct	sectorinfo *sinfotbl;

	/* Based on the flash bank ID returned, load a sector count and a */
	/* sector size-information table... */
	flashtype(fbnk);
	switch(fbnk->id) {
		case INTEL_iTE28F160C3B:
			fbnk->sectorcnt = (sizeof(SectorSizes)/sizeof(int));
			sizetable = SectorSizes;
			break;
		default:
			printf("Unrecognized flashid: 0x%08lx\n",fbnk->id);
			return(-1);
			break;
	}

	/* Create the per-sector information table.  The size of the table */
	/* depends on the number of sectors in the device...  */
	if (fbnk->sectors)
		free((char *)fbnk->sectors);
	msize = fbnk->sectorcnt * (sizeof(struct sectorinfo));
	sinfotbl = (struct sectorinfo *)malloc(msize);
	if (!sinfotbl) {
		printf("Can't allocate space for flash sector information\n");
		return(-1);
	}
	fbnk->sectors = sinfotbl;

	/* Using the above-determined sector count and size table, build */
	/* the sector information table as part of the flash-bank structure: */
	saddr = fbnk->base;
	for(i=0;i<fbnk->sectorcnt;i++) {
		fbnk->sectors[i].snum = snum+i;
		fbnk->sectors[i].size = sizetable[i];
		fbnk->sectors[i].begin = saddr;
		fbnk->sectors[i].end =
		    fbnk->sectors[i].begin + fbnk->sectors[i].size - 1;
		fbnk->sectors[i].protected = 0;
		saddr += sizetable[i];
	}
	fbnk->end = saddr-1;
	return(fbnk->sectorcnt);
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

#ifdef FLASH_COPY_TO_RAM

	/* Copy functions to ram space... */
	/* Note that this MUST be done when cache is disabled to assure that */
	/* the RAM is occupied by the designated block of code. */

	if (flashopload((ulong *)iTE28F160C3B_16x1_lock,
		(ulong *)EndiTE28F160C3B_16x1_lock,
		FlashLockFbuf,sizeof(FlashLockFbuf)) < 0)
		return(-1);
	if (flashopload((ulong *)iTE28F160C3B_16x1_type,
		(ulong *)EndiTE28F160C3B_16x1_type,
		FlashTypeFbuf,sizeof(FlashTypeFbuf)) < 0)
		return(-1);
	if (flashopload((ulong *)iTE28F160C3B_16x1_erase,
		(ulong *)EndiTE28F160C3B_16x1_erase,
		FlashEraseFbuf,sizeof(FlashEraseFbuf)) < 0)
		return(-1);
	if (flashopload((ulong *)iTE28F160C3B_16x1_ewrite,
		(ulong *)EndiTE28F160C3B_16x1_ewrite,
		FlashEwriteFbuf,sizeof(FlashEwriteFbuf)) < 0)
		return(-1);
	if (flashopload((ulong *)iTE28F160C3B_16x1_write,
		(ulong *)EndiTE28F160C3B_16x1_write,
		FlashWriteFbuf,sizeof(FlashWriteFbuf)) < 0)
		return(-1);

#endif

	fbnk = &FlashBank[0];
	fbnk->base = (unsigned char *)FLASH_BANK0_BASE_ADDR;
	fbnk->width = FLASH_BANK0_WIDTH;
#ifdef FLASH_COPY_TO_RAM
	fbnk->fltype = (int(*)())FlashTypeFbuf;		/* flashtype(). */
	fbnk->flerase = (int(*)())FlashEraseFbuf;	/* flasherase(). */
	fbnk->flwrite = (int(*)())FlashWriteFbuf;	/* flashwrite(). */
	fbnk->flewrite = (int(*)())FlashEwriteFbuf;	/* flashewrite(). */
	fbnk->fllock = (int(*)())FlashLockFbuf;		/* flashelock(). */
#else
	fbnk->fltype = iTE28F160C3B_16x1_type;
	fbnk->flerase = iTE28F160C3B_16x1_erase;
	fbnk->flwrite = iTE28F160C3B_16x1_write;
	fbnk->flewrite = iTE28F160C3B_16x1_ewrite;
	fbnk->fllock = iTE28F160C3B_16x1_lock;
#endif

	snum += FlashBankInit(fbnk,snum);

	sectorProtect(FLASH_PROTECT_RANGE,1);

#ifdef FLASHRAM_BASE
	FlashRamInit(snum, FLASHRAM_SECTORCOUNT,
		&FlashBank[FLASHRAM_BANKNUM], sinfoRAM, ramSectors);
#endif
	return(0);
}

#endif	/* SINGLE_FLASH_DEVICE */

#endif	/* INCLUDE_FLASH */
