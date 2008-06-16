/* intel28f128_8x1.c:
 * Support for INTEL 28f128 configured as an 8-bit wide device.
 */
#include "config.h"
#if INCLUDE_FLASH
#include "stddefs.h"
#include "genlib.h"
#include "cpu.h"
#include "flash.h"
#include "intel28f128_8x1.h"

extern struct flashinfo	Fdev;
extern int FlashProtectWindow;

#define	ftype		    volatile unsigned char

/* Strata flash buffer info:
 */
#define BUFFER_SIZE		32
#define BUFFER_ALIGN	0x1f

/* Status register definitions:
 */
#define WSMS	0x80	/* Wait state machine status 1=ready */
#define ESS		0x40	/* Erase suspend status 1=suspended */
#define ECLBS	0x20	/* Erase/clear lock bit status 1=error */
#define PSLBS	0x10	/* Program/set lock bit status 1=error */
#define VPENS	0x08	/* Programming voltage status 1=error */
#define PSS		0x04	/* Program suspend status 1=suspended */
#define DPS		0x02	/* Device protect status 1=lock detected */
#define WBS		0x80	/* Write buffer status 1=available */

/* INTEL_STRATA-Specific Macros...
 */
#define STRATACMD_READARRAY()				(*(ftype *)(fdev->base) = 0xff)
#define STRATACMD_PROTPROGRAM()				(*(ftype *)(fdev->base) = 0xc0)
#define STRATACMD_READQUERY()				(*(ftype *)(fdev->base) = 0x98)
#define STRATACMD_READID()					(*(ftype *)(fdev->base) = 0x90)
#define STRATACMD_READSTATUS()				(*(ftype *)(fdev->base) = 0x70)
#define STRATACMD_LOCKBIT()					(*(ftype *)(fdev->base) = 0x60)
#define STRATACMD_CLEARSTATUS()				(*(ftype *)(fdev->base) = 0x50)
#define STRATACMD_PROGRAM()					(*(ftype *)(fdev->base) = 0x40)
#define STRATACMD_WRITETOBUFFER(addr)		(*(ftype *)(addr) = 0xe8)
#define STRATACMD_CONFIRM(addr)				(*(ftype *)(addr) = 0xd0)
#define STRATACMD_BLOCKERASE(addr)			(*(ftype *)(addr) = 0x20)
#define STRATACMD_SETLOCKCONFIRM(addr)		(*(ftype *)(addr) = 0x01)
#define STRATA_BUFFERBYTECOUNT(addr,val)	(*(ftype *)(addr) = val)

/* General Macros...
 */
#define FLASH_WRITE(to,frm)				(*(ftype *)(to) = *(ftype *)(frm))
#define FLASH_READ(addr)				(*(ftype *)(addr))
#define FLASH_READBASE()				(*(ftype *)(fdev->base))
#define NOT_EQUAL(p1,p2)				(*(ftype *)(p1) != *(ftype *)(p2))
#define WAIT_FOR_WRITE(add,data)		while(*(ftype *)add != *(ftype *)data)
#define NOTERASED_32(a)					(a != 0xffffffff)

/* Intel28f128_8x1_erase():
 * Based on the 'snum' value, erase the appropriate sector(s).
 * Return 0 if success, else -1.
 */
int
Intel28f128_8x1_erase(struct flashinfo *fdev,int snum)
{
	struct	sectorinfo *sip;
	int		ret, sector;

	ret = 0;

	/* Erase the request sector(s):
	 */
	for (sector=0; sector < fdev->sectorcnt; sector++) {
		sip = &fdev->sectors[sector];
		if ((!FlashProtectWindow) && (sip->protected)) {
			continue;
		}
		if ((snum == ALL_SECTORS) || (snum == sector)) {
			register ulong *lp, *lp1;
			int	noterased;

			/* Avoid issuing the erase command if the sector is already
			 * erased...
			 */
			noterased = 0;
			lp = (ulong *)fdev->sectors[sector].begin; 
			lp1 = (ulong *)((char *)lp + sip->size - 1); 
			while(lp <= lp1) {
				if (NOTERASED_32(*lp++)) {
					noterased = 1;
					break;
				}
			}
			if (noterased) {
				/* Issue the ERASE setup/confirm sequence...
				 */
				STRATACMD_BLOCKERASE(sip->begin);
				STRATACMD_CONFIRM(sip->begin);

				/* Device automatically outputs status after the
				 * above sequence, so now poll status waiting for
				 * completion of the erase...
				 */
				do {
					STRATACMD_READSTATUS();
				} while((FLASH_READBASE() & WSMS) == 0);

				/* Clear status register and go to read-array mode.
				 */
				STRATACMD_CLEARSTATUS();
				STRATACMD_READARRAY();	
				while(NOTERASED_32(*(ulong *)(sip->begin)));
			}
			if (ret == -1)
				break;
		}
	}
	return(ret);
}

/* EndIntel28f128_8x1_erase():
 * Function place holder to determine the end of the above function.
 */
void
EndIntel28f128_8x1_erase(void)
{}

int
Intel28f128_8x1_write(struct flashinfo *fdev,uchar *dest,uchar *src,
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
		aligndest = block;
    
		/* Issue request to write to the buffer, then poll extended
		 * status register to wait for availability.
		 */
		giveup = 10000000;
		do {
			STRATACMD_WRITETOBUFFER(aligndest);	
			giveup--;
		} while (((FLASH_READ(aligndest) & WBS) == 0) && (giveup > 0));
		if (giveup == 0) {
			ret = -2;
			break;
		}
                
		/* Write the byte count...  Notice that the bytecount fed to the
		 * device is one less than the actual count. 
		 */
		STRATA_BUFFERBYTECOUNT(block,size-1);

		/* Write the buffer data...
		 */
		for(i=0;i<size;i++) {
			FLASH_WRITE(aligndest,&buf[i]);
			aligndest++;
		}
		
		STRATACMD_CONFIRM(block);
		tot += size;

		do {
			STRATACMD_READSTATUS();
		} while((FLASH_READBASE() & WSMS) == 0);

		STRATACMD_READARRAY();	
	}
	return(ret);
}

/* EndIntel28f128_8x1_write():
 * Function place holder to determine the end of the above function.
 */
void
EndIntel28f128_8x1_write(void)
{}

/* Intel28f128_8x1_ewrite():
 * Erase all sectors that are part of the address space to be written,
 * then write the data to that address space.  This is basically a
 * concatenation of the above erase & write done in one step.  This is
 * necessary primarily for re-writing the bootcode; because after the boot
 * code is erased, there is nowhere to return so the re-write must be done
 * while executing out of ram also.  It is only needed in systems that are
 * executing the monitor out of the same device that is being updated.
 */

int
Intel28f128_8x1_ewrite(struct flashinfo *fdev,uchar *destA,uchar *srcA,
	long bytecnt)
{
	int	    sector, i;
	void	(*reset)();
	uchar   *src, *dest;
	struct	sectorinfo *sip;

	src = srcA;
	dest = destA;
	STRATACMD_CLEARSTATUS();
	STRATACMD_READARRAY();

	/* For each sector, if it overlaps any of the destination space
	 * then erase that sector.
	 */
	for (sector = 0; sector < fdev->sectorcnt; sector++) {
		sip = &fdev->sectors[sector];

		if ((((uchar *)dest) > (sip->end)) ||
		    (((uchar *)dest+bytecnt-1) < (sip->begin))) {
			continue;
		}

		/* Issue the ERASE setup/confirm sequence...
		 */
		STRATACMD_BLOCKERASE(sip->begin);
		STRATACMD_CONFIRM(sip->begin);

		/* Device automatically outputs status after the
		 * above sequence, so now poll status waiting for
		 * completion of the erase...
		 */
		do {
			STRATACMD_READSTATUS();
		} while((FLASH_READBASE() & WSMS) == 0);

		/* Clear status register and go to read-array mode.
		 */
		STRATACMD_CLEARSTATUS();
		STRATACMD_READARRAY();	
		while(NOTERASED_32(*(ulong *)(sip->begin)));
	}

	for(i = 0; i < bytecnt; i++) {
		
		/* Program sequence...
		 */
		STRATACMD_PROGRAM();
		FLASH_WRITE(dest,src);

		/* Wait for write to complete by polling RSR...
		 */
		do {
			STRATACMD_READSTATUS();	
		} while((FLASH_READBASE() & WSMS) == 0);

		STRATACMD_CLEARSTATUS();	

		do {
			STRATACMD_READARRAY();	
		} WAIT_FOR_WRITE(dest,src);		

		dest++;
		src++;
	}
	
	/* Now that the re-programming of flash is complete, reset: */
	reset = RESETFUNC();
	reset();

	return(0);	/* won't get here */
}


/* EndIntel28f128_8x1_ewrite():
 * Function place holder to determine the end of the above function.
 */
void
EndIntel28f128_8x1_ewrite(void)
{}

/* Intel28f128_8x1_lock():
 */
int
Intel28f128_8x1_lock(struct flashinfo *fdev,int snum,int operation)
{
	ulong	add;
	int		sector;

	add = (ulong)(fdev->base);

	if (operation == FLASH_LOCKABLE)
		return(1);

	if (operation == FLASH_LOCKQRY) {
//		TODO: Write this 
		return(-1);
	}

	/* Not applicable for this device.
	 */
	if (operation == FLASH_LOCKDWN)
		return(-1);

	/* For this device the unlock is applied to all
	 * sectors, so no need to enter the loop below.
	 */
	if (operation == FLASH_UNLOCK) {
		STRATACMD_LOCKBIT();
		STRATACMD_CONFIRM(add);

		/* Wait for unlock to complete by polling status
		 * register, then go to read-array mode.
		 */
		while((FLASH_READBASE() & WSMS) == 0);
		STRATACMD_READARRAY();	
		return(0);
	}

	/* Lock the requested sector(s):
	 */
	for (sector=0;sector<fdev->sectorcnt;sector++) {
		if ((snum == ALL_SECTORS) || (snum == sector)) {
			/* Issue the setup/lock command sequence
			 */
			STRATACMD_LOCKBIT();
			STRATACMD_SETLOCKCONFIRM(add);

			/* Wait for lock/unlock to complete by polling
			 * status register.
			 */
			while((*(ftype *)add & WSMS) == 0);
			STRATACMD_READARRAY();	
		}
		add += fdev->sectors[sector].size;
	}
	return(0);
}

/* EndIntel28f128_8x1_lock():
 * Function place holder to determine the end of the above function.
 */
void
EndIntel28f128_8x1_lock(void)
{
}

/* Intel28f128_8x1_type():
 * Run the AUTOSELECT algorithm to retrieve the manufacturer and
 * device id of the flash.
 */
int
Intel28f128_8x1_type(struct flashinfo *fdev)
{
	ushort	man, dev, id;

	STRATACMD_READID();

	man = *(uchar *)fdev->base;		/* manufacturer ID */
	dev = *(uchar *)(fdev->base+2);	/* device ID */
	id = (ushort)man;
	id <<= 8;
	id |= dev;

	fdev->id = id;

	STRATACMD_READARRAY();	
	
	return((int)(fdev->id));
}

/* EndIntel28f128_8x1_type():
 * Function place holder to determine the end of the above function.
 */
void
EndIntel28f128_8x1_type(void)
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
	{ INTEL_DT28F128J5,		"INTEL-DT28F128J5" },
	{ INTEL_DT28F640J5,		"INTEL-DT28F640J5" },
	{ INTEL_28F640,			"INTEL-28F640" },
	{ 0, 0 },
};

/* This configuration is a single 28F128 device.
 * Each device has 128 128Kbyte sectors...
 */
int	SectorSizes28F128_8[] = {
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
};


struct sectorinfo sinfo128[sizeof(SectorSizes28F128_8)/sizeof(int)];

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
		case INTEL_DT28F128J5:
			fbnk->sectorcnt = (sizeof(SectorSizes28F128_8)/sizeof(int));
			sizetable = SectorSizes28F128_8;
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

	if (flashopload((ulong *)Intel28f128_8x1_lock,
		(ulong *)EndIntel28f128_8x1_lock,
		FlashLockFbuf,sizeof(FlashLockFbuf)) < 0)
		return(-1);
	if (flashopload((ulong *)Intel28f128_8x1_type,
		(ulong *)EndIntel28f128_8x1_type,
		FlashTypeFbuf,sizeof(FlashTypeFbuf)) < 0)
		return(-1);
	if (flashopload((ulong *)Intel28f128_8x1_erase,
		(ulong *)EndIntel28f128_8x1_erase,
		FlashEraseFbuf,sizeof(FlashEraseFbuf)) < 0)
		return(-1);
	if (flashopload((ulong *)Intel28f128_8x1_ewrite,
		(ulong *)EndIntel28f128_8x1_ewrite,
		FlashEwriteFbuf,sizeof(FlashEwriteFbuf)) < 0)
		return(-1);
	if (flashopload((ulong *)Intel28f128_8x1_write,
		(ulong *)EndIntel28f128_8x1_write,
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
	fbnk->fltype = Intel28f128_8x1_type;
	fbnk->flerase = Intel28f128_8x1_erase;
	fbnk->flwrite = Intel28f128_8x1_write;
	fbnk->flewrite = Intel28f128_8x1_ewrite;
	fbnk->fllock = Intel28f128_8x1_lock;
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
