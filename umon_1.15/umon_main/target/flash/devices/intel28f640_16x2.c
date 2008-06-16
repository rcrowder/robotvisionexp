/* intel28f640_16x2.c:
 * Support for INTEL 28f640.
 * Bank configuration is as a set of 2 16-bit devices in parallel.
 */
#include "config.h"
#if INCLUDE_FLASH
#include "endian.h"
#include "stddefs.h"
#include "genlib.h"
#include "cpu.h"
#include "flash.h"
#include "intel28f640_16x2.h"

#define FLASHCFG_16x2	1
#include "strata.h"

/* Manufacturer and device id... */
#define INTEL_DT28F128J5		0x00890018
#define INTEL_28F640			0x00890017
#define INTEL_DT28F640J5		0x00890015
#define ST_M58LW064D			0x00200017

/* Intel28f640_16x2_erase():
 * Erase sector 'snum'.
 * Return 0 if success, else negative.
 */
int
Intel28f640_16x2_erase(struct flashinfo *fdev,int snum)
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

/* EndIntel28f640_16x2_erase():
 * Function place holder to determine the end of the above function.
 */
void
EndIntel28f640_16x2_erase(void)
{}

int
Intel28f640_16x2_write(struct flashinfo *fdev,uchar *dest,uchar *src,
	long bytecnt)
{
	volatile uchar	buf[BUFFER_SIZE], *aligndest, *block, *destend;
	volatile int	tot, ret, i, giveup, aligntot, size, bcount;

	destend = dest+bytecnt;

	/* The write buffer can only be used on 64-byte blocks; hence, the
	 * low 6 bits of the destination must be zero at the start of a 
	 * buffer write.  This means that we must align the destination
	 * address on this boundary.  To do this, we decrement the destination
	 * address until the alignment is reached, then load that space with
	 * the same data that is already there.
	 */ 
	aligntot = 0;
	aligndest = dest;
	while(((ulong)aligndest & BUFFER_ALIGN) != 0) {
		aligndest--;
		aligntot++;
		bytecnt++;
	}

	ret = tot = 0;
	while((tot < bytecnt) && (ret == 0)) {
		size = bytecnt - tot;
		if (size > BUFFER_SIZE)
			size = BUFFER_SIZE;

		block = aligndest;

		/* Copy buffer's worth of data into local buffer just in
		 * case the source is this flash device.
		 */
		for(i=0;i<size;i++) {
			if (aligndest < dest)
				buf[i] = *aligndest++;
			else
				buf[i] = *src++;
		}

		/* If size is less than BUFFER_SIZE, then we must be on the last
		 * pass through this loop, so we need to fill buf with the same
		 * data that is already in memory at the destination.
		 */
		while(size < BUFFER_SIZE) {
			buf[i++] = *destend++;
			size++;
		}
		aligndest = block;
    
		/* Issue request to write to the buffer, then poll extended
		 * status register to wait for availability.
		 */
		giveup = FLASH_LOOP_TIMEOUT;
		do {
			STRATACMD_WRITETOBUFFER(aligndest);	
			giveup--;
		} while (((FLASH_READ(aligndest) & WBS) == 0) && (giveup > 0));
		if (giveup == 0) {
			STRATACMD_READARRAY();	
			ret = -2;
			break;
		}
                
		/* Write the byte count into each of the 2 parallel devices:
		 */
		bcount = (size-4)/4;
		FLASH_WRITEVAL(block,((bcount << 16) | bcount));

		/* Write the buffer data...
		 */
		for(i=0;i<size;i+=sizeof(ftype)) {
			FLASH_WRITEPTR(aligndest,(&buf[i]));
			aligndest+=sizeof(ftype);
		}
		
		STRATACMD_CONFIRM(block);
		tot += size;

		STRATACMD_READSTATUS();
		WAIT_FOR_WSMS_READY();
		STRATACMD_READARRAY();	
	}
	return(ret);
}

/* EndIntel28f640_16x2_write():
 * Function place holder to determine the end of the above function.
 */
void
EndIntel28f640_16x2_write(void)
{}

/* Intel28f640_16x2_ewrite():
 * Erase all sectors that are part of the address space to be written,
 * then write the data to that address space.  This is basically a
 * concatenation of the above erase & write done in one step.  This is
 * necessary primarily for re-writing the bootcode; because after the boot
 * code is erased, there is nowhere to return so the re-write must be done
 * while executing out of ram also.  It is only needed in systems that are
 * executing the monitor out of the same device that is being updated.
 */
int
Intel28f640_16x2_ewrite(struct flashinfo *fdev,uchar *destA,uchar *srcA,
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

/* EndIntel28f640_16x2_ewrite():
 * Function place holder to determine the end of the above function.
 */
void
EndIntel28f640_16x2_ewrite(void)
{}

/* Intel28f640_16x2_lock():
 */

int
Intel28f640_16x2_lock(struct flashinfo *fdev,int snum,int operation)
{
	ftype sample, bstat;

	sample = FLASH_READBASE();

	if (operation == FLASH_LOCKABLE) {
		return(1);
	}
	else if (operation == FLASH_UNLOCK) {
		STRATACMD_LOCKBIT();
		STRATACMD_CONFIRM(fdev->base);
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
		if (fdev->id != ST_M58LW064D)
			WAIT_FOR_WSMS_READY();
		bstat = FLASH_READ_BLOCKSTATUS(fdev->sectors[snum].begin);
		STRATACMD_READARRAY();
		FLASHOP_PRINT(("bstat[snum %d] = 0x%lx\n",snum,(long)bstat));
		if ((bstat & 0x0001) == 0x0001)
			return(1);
		else
			return(0);
	}
	else
		return(-1);
}

/* EndIntel28f640_16x2_lock():
 * Function place holder to determine the end of the above function.
 */
void
EndIntel28f640_16x2_lock(void)
{
}

/* Intel28f640_16x2_type():
 * Run the AUTOSELECT algorithm to retrieve the manufacturer and
 * device id of the flash.
 */
int
Intel28f640_16x2_type(struct flashinfo *fdev)
{
	ulong	id;
	ftype	sample;
	ushort	man, dev;

	sample = FLASH_READBASE();

	/* Issue the read configuration command: */
	STRATACMD_READID();

	man = (ushort)FLASH_READBASE();			/* Manufacturer ID */
	dev = (ushort)FLASH_READ_DEVICEID(); 			/* device ID */
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

/* EndIntel28f640_16x2_type():
 * Function place holder to determine the end of the above function.
 */
void
EndIntel28f640_16x2_type(void)
{}

/**************************************************************************
 **************************************************************************
 *
 * The remainder of the code in this file can be included if the
 * target configuration is such that this 28F640 device is the only
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
ulong	FlashLockFbuf[400];
ulong	FlashTypeFbuf[400];
ulong	FlashEraseFbuf[400];
ulong	FlashWriteFbuf[400];
ulong	FlashEwriteFbuf[400];
#endif

/* FlashNamId[]:
 * Used to correlate between the ID and a string representing the name
 * of the flash device.
 * Note that this table (and the case statement in FlashBankInit())
 * allow a 28F128 flash ID to sneak by... This is to allow a 28F128
 * device to be put in the footprint of a 28F640, but with the upper
 * half of the device inaccessible (some CSB360 boards).
 */
struct flashdesc FlashNamId[] = {
	{ INTEL_28F640,			"INTEL-28F640" },
	{ ST_M58LW064D,			"SGS_THOMPSON-M58LW064D" },
	{ INTEL_DT28F640J5,		"INTEL-DT28F640J5" },
	{ INTEL_DT28F128J5,		"INTEL-28F128" },
	{ 0, 0 },
};

int
FlashBankInit(struct flashinfo *fbnk,int snum)
{
	uchar	*saddr;
	int		i, msize;
	struct	sectorinfo *sinfotbl;

	/* Based on the flash bank ID returned, load a sector count and a
	 * sector size-information table...
	 */
	flashtype(fbnk);
	switch(fbnk->id) {
		case ST_M58LW064D:
		case INTEL_28F640:
		case INTEL_DT28F640J5:
			fbnk->sectorcnt = 64;
			break;
		case INTEL_DT28F128J5:	
			fbnk->sectorcnt = 128;
			break;
		default:
			printf("Unrecognized flashid: 0x%08lx\n",fbnk->id);
			return(-1);
			break;
	}

	/* Create the per-sector information table.  The size of the table
	 * depends on the number of sectors in the device...
	 */
	if (fbnk->sectors)
		free((char *)fbnk->sectors);
	msize = fbnk->sectorcnt * (sizeof(struct sectorinfo));
	sinfotbl = (struct sectorinfo *)malloc(msize);
	if (!sinfotbl) {
		printf("Can't allocate space for flash sector information\n");
		return(-1);
	}
	fbnk->sectors = sinfotbl;

	/* Using the above-determined sector count, build the sector
	 * information table as part of the flash-bank structure.  For
	 * this set of devices, all sectors are the same size (0x20000).
	 */
	saddr = fbnk->base;
	for(i=0;i<fbnk->sectorcnt;i++) {
		fbnk->sectors[i].snum = snum+i;
		fbnk->sectors[i].size = 0x20000;
		fbnk->sectors[i].begin = saddr;
		fbnk->sectors[i].end =
		    fbnk->sectors[i].begin + fbnk->sectors[i].size - 1;
		fbnk->sectors[i].protected = 0;
		saddr += 0x20000;
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

	if (flashopload((ulong *)Intel28f640_16x2_lock,
		(ulong *)EndIntel28f640_16x2_lock,
		FlashLockFbuf,sizeof(FlashLockFbuf)) < 0)
		return(-1);
	if (flashopload((ulong *)Intel28f640_16x2_type,
		(ulong *)EndIntel28f640_16x2_type,
		FlashTypeFbuf,sizeof(FlashTypeFbuf)) < 0)
		return(-1);
	if (flashopload((ulong *)Intel28f640_16x2_erase,
		(ulong *)EndIntel28f640_16x2_erase,
		FlashEraseFbuf,sizeof(FlashEraseFbuf)) < 0)
		return(-1);
	if (flashopload((ulong *)Intel28f640_16x2_ewrite,
		(ulong *)EndIntel28f640_16x2_ewrite,
		FlashEwriteFbuf,sizeof(FlashEwriteFbuf)) < 0)
		return(-1);
	if (flashopload((ulong *)Intel28f640_16x2_write,
		(ulong *)EndIntel28f640_16x2_write,
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
	fbnk->fltype = Intel28f640_16x2_type;
	fbnk->flerase = Intel28f640_16x2_erase;
	fbnk->flwrite = Intel28f640_16x2_write;
	fbnk->flewrite = Intel28f640_16x2_ewrite;
	fbnk->fllock = Intel28f640_16x2_lock;
#endif

	snum += FlashBankInit(fbnk,snum);

	sectorProtect(FLASH_PROTECT_RANGE,1);

#ifdef FLASHRAM_BASE
#ifdef FLASHRAM_SECTORSIZE
#define ramSectors 0
#endif
	FlashRamInit(snum, FLASHRAM_SECTORCOUNT,
		&FlashBank[FLASHRAM_BANKNUM], sinfoRAM, ramSectors);
#endif
	return(0);
}

#endif	/* SINGLE_FLASH_DEVICE */

#endif	/* INCLUDE_FLASH */
