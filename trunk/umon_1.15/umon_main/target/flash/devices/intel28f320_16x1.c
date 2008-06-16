/* intel28f320_16x1.c:
 * Support for INTEL 28f320 (32 0x20000-byte sectors) and 28f128 
 * (128 0x20000-byte sectors).
 * Bank configuration is as a single 16-bit device.
 */
#include "config.h"
#if INCLUDE_FLASH
#include "stddefs.h"
#include "genlib.h"	
#include "cpu.h"
#include "flash.h"
#include "intel28f320_16x1.h"

#define FLASHCFG_16x1	1
#define SECTOR_TOTAL	(32*4)

#include "strata.h"

/* Manufacturer and device id... */
#define INTEL_28F320J3		0x00890016
#define INTEL_28F128J3		0x00890018

/* Intel28f320_16x1_erase():
 * Erase sector 'snum'.
 * Return 0 if success, else negative.
 */
int
Intel28f320_16x1_erase(struct flashinfo *fdev,int snum)
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

/* EndIntel28f320_16x1_erase():
 * Function place holder to determine the end of the above function.
 */
void
EndIntel28f320_16x1_erase(void)
{}

int
Intel28f320_16x1_write(struct flashinfo *fdev,uchar *dest,uchar *src,
	long bytecnt)
{
	volatile int	tot, ret;
	int				i, giveup, aligntot, size;
	volatile uchar	buf[BUFFER_SIZE], *aligndest, *block;

	/* The write buffer can only be used on 32-byte blocks; hence, the
	 * low 5 bits of the destination must be zero at the start of a 
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
		if (size & 1) {
			size++;
			buf[i] = dest[bytecnt];
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
                
		/* Write the byte count...  Notice that the bytecount fed to the
		 * device is one less than the actual count. 
		 */
		FLASH_WRITEVAL(block,(size-1)/2);

		/* Write the buffer data...
		 */
		for(i=0;i<size;i+=2) {
			FLASH_WRITEPTR(aligndest,(&buf[i]));
			aligndest+=2;
		}
		
		STRATACMD_CONFIRM(block);
		tot += size;

		WAIT_FOR_WSMS_READY();
		STRATACMD_READARRAY();	
	}
	return(ret);
}

/* EndIntel28f320_16x1_write():
 * Function place holder to determine the end of the above function.
 */
void
EndIntel28f320_16x1_write(void)
{}

/* Intel28f320_16x1_ewrite():
 * Erase all sectors that are part of the address space to be written,
 * then write the data to that address space.  This is basically a
 * concatenation of the above erase & write done in one step.  This is
 * necessary primarily for re-writing the bootcode; because after the boot
 * code is erased, there is nowhere to return so the re-write must be done
 * while executing out of ram also.  It is only needed in systems that are
 * executing the monitor out of the same device that is being updated.
 */
int
Intel28f320_16x1_ewrite(struct flashinfo *fdev,uchar *destA,uchar *srcA,
	long bytecnt)
{
	int	    sector, i;
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

	/* Now that the re-programming of flash is complete, reset: */
	RESETFUNC();

	return(0);	/* won't get here */
}

/* EndIntel28f320_16x1_ewrite():
 * Function place holder to determine the end of the above function.
 */
void
EndIntel28f320_16x1_ewrite(void)
{}

/* Intel28f320_16x1_lock():
 */

int
Intel28f320_16x1_lock(struct flashinfo *fdev,int snum,int operation)
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
		WAIT_FOR_WSMS_READY();
		bstat = FLASH_READ_BLOCKSTATUS(fdev->sectors[snum].begin);
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

/* EndIntel28f320_16x1_lock():
 * Function place holder to determine the end of the above function.
 */
void
EndIntel28f320_16x1_lock(void)
{
}

/* Intel28f320_16x1_type():
 * Run the AUTOSELECT algorithm to retrieve the manufacturer and
 * device id of the flash.
 */
int
Intel28f320_16x1_type(struct flashinfo *fdev)
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

/* EndIntel28f320_16x1_type():
 * Function place holder to determine the end of the above function.
 */
void
EndIntel28f320_16x1_type(void)
{}

/**************************************************************************
 **************************************************************************
 *
 * The remainder of the code in this file can be included if the
 * target configuration is such that this 28F320 device is the only
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


struct sectorinfo sinfotbl[SECTOR_TOTAL];

/* FlashNamId[]:
 * Used to correlate between the ID and a string representing the name
 * of the flash device.
 */
struct flashdesc FlashNamId[] = {
	{ INTEL_28F320J3,		"INTEL-28F320" },
	{ INTEL_28F128J3,		"INTEL-28F128" },
	{ 0, 0 },
};

/* FlashInit():
 * Initialize data structures for each bank of flash...
 */
int
FlashInit(void)
{
	int		i;
	uchar	*saddr;
	struct	flashinfo *fbnk;

	ActiveFlashBanks = 1;
	FlashCurrentBank = 0;

#ifdef FLASH_COPY_TO_RAM

	/* Copy functions to ram space... */
	/* Note that this MUST be done when cache is disabled to assure that */
	/* the RAM is occupied by the designated block of code. */

	if (flashopload((ulong *)Intel28f320_16x1_lock,
		(ulong *)EndIntel28f320_16x1_lock,
		FlashLockFbuf,sizeof(FlashLockFbuf)) < 0)
		return(-1);
	if (flashopload((ulong *)Intel28f320_16x1_type,
		(ulong *)EndIntel28f320_16x1_type,
		FlashTypeFbuf,sizeof(FlashTypeFbuf)) < 0)
		return(-1);
	if (flashopload((ulong *)Intel28f320_16x1_erase,
		(ulong *)EndIntel28f320_16x1_erase,
		FlashEraseFbuf,sizeof(FlashEraseFbuf)) < 0)
		return(-1);
	if (flashopload((ulong *)Intel28f320_16x1_ewrite,
		(ulong *)EndIntel28f320_16x1_ewrite,
		FlashEwriteFbuf,sizeof(FlashEwriteFbuf)) < 0)
		return(-1);
	if (flashopload((ulong *)Intel28f320_16x1_write,
		(ulong *)EndIntel28f320_16x1_write,
		FlashWriteFbuf,sizeof(FlashWriteFbuf)) < 0)
		return(-1);

#endif

	fbnk = &FlashBank[0];
	fbnk->base = (unsigned char *)FLASH_BANK0_BASE_ADDR;
	fbnk->width = 2;

#ifdef FLASH_COPY_TO_RAM
	fbnk->fltype = (int(*)())FlashTypeFbuf;		/* flashtype(). */
	fbnk->flerase = (int(*)())FlashEraseFbuf;	/* flasherase(). */
	fbnk->flwrite = (int(*)())FlashWriteFbuf;	/* flashwrite(). */
	fbnk->flewrite = (int(*)())FlashEwriteFbuf;	/* flashewrite(). */
	fbnk->fllock = (int(*)())FlashLockFbuf;		/* flashelock(). */
#else
	fbnk->fltype = Intel28f320_16x1_type;
	fbnk->flerase = Intel28f320_16x1_erase;
	fbnk->flwrite = Intel28f320_16x1_write;
	fbnk->flewrite = Intel28f320_16x1_ewrite;
	fbnk->fllock = Intel28f320_16x1_lock;
#endif

	flashtype(fbnk);
	switch(fbnk->id) {
		case INTEL_28F128J3:
			fbnk->sectorcnt = SECTOR_TOTAL;
			break;
		default:
		case INTEL_28F320J3:
			fbnk->sectorcnt = SECTOR_TOTAL/4;
			break;
	}
	fbnk->sectors = sinfotbl;

	/* Build the sector information table as part of the flash-bank
	 * structure.  For this device, all sectors are the same size
	 * (0x20000).
	 */
	saddr = fbnk->base;
	for(i=0;i<fbnk->sectorcnt;i++) {
		fbnk->sectors[i].snum = i;
		fbnk->sectors[i].size = 0x20000;
		fbnk->sectors[i].begin = saddr;
		fbnk->sectors[i].end =
		    fbnk->sectors[i].begin + fbnk->sectors[i].size - 1;
		fbnk->sectors[i].protected = 0;
		saddr += 0x20000;
	}
	fbnk->end = saddr-1;
	
#ifdef FLASH_PROTECT_RANGE
	sectorProtect(FLASH_PROTECT_RANGE,1);
#endif

	return(0);
}

#endif	/* SINGLE_FLASH_DEVICE */

#endif	/* INCLUDE_FLASH */
