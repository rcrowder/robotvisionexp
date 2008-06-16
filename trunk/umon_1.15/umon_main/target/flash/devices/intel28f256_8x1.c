/* intel28f256_8x1.c:
 * Support for a single INTEL 28f256 device configured in 8-bit mode.
 */
#include "config.h"
#if INCLUDE_FLASH
#include "stddefs.h"
#include "genlib.h"
#include "cpu.h"
#include "flash.h"
#include "intel28f256_8x1.h"

#define SR_WAIT	50000

/* Strata flash buffer info:
 * Each device can buffer 32 bytes.
 * In this configuration we have 4 devices in parallel;
 * hence, 128 byte buffering...
 */
#define BUFFER_SIZE		32
#define BUFFER_ALIGN	0x1f

#define DEV_WIDTH		1

#define WSMS			0x80
#define ESS				0x40
#define ES				0x20
#define PSS				0x04
#define PS				0x10
#define WBS				0x80

#define	ftype		    volatile unsigned char

/* Manufacturer and device id... */
#define DEVICE_28F320J3		0x00890016		/* 32  Mbit (3 volt strata) */
#define DEVICE_28F640J3		0x00890017		/* 64  Mbit (3 volt strata) */
#define DEVICE_28F128J3		0x00890018		/* 128 Mbit (3 volt strata) */
#define DEVICE_28F256J3		0x0089001d		/* 256 Mbit (3 volt strata) */

/* INTEL_STRATA-Specific Macros...
 */
#define STRATACMD_READARRAY()			(*(ftype *)(fdev->base) = 0xff)
#define STRATACMD_PROTPROGRAM()			(*(ftype *)(fdev->base) = 0xc0)
#define STRATACMD_READID()				(*(ftype *)(fdev->base) = 0x90)
#define STRATACMD_READSTATUS()			(*(ftype *)(fdev->base) = 0x70)
#define STRATACMD_LOCKBIT()				(*(ftype *)(fdev->base) = 0x60)
#define STRATACMD_CLEARSTATUS()			(*(ftype *)(fdev->base) = 0x50)
#define STRATACMD_PROGRAM(addr)			(*(ftype *)(addr) = 0x40)
#define STRATACMD_WRITETOBUFFER(addr)	(*(ftype *)(addr) = 0xe8)
#define STRATACMD_CONFIRM(addr)			(*(ftype *)(addr) = 0xd0)
#define STRATACMD_BLOCKERASE(addr)		(*(ftype *)(addr) = 0x20)
#define STRATACMD_SETLOCKCONFIRM(addr)	(*(ftype *)(addr) = 0x01)

/* General Macros...
 */
#define FLASH_READ(addr)	    		(*(ftype *)(addr))
#define FLASH_READBASE()				(*(ftype *)(fdev->base))
#define FLASH_READ_MANUFACTURER()		(*(ftype *)(fdev->base))
//#define FLASH_READ_DEVICEID()			(*(ftype *)(fdev->base+8))
#define FLASH_READ_BLOCKSTATUS(sbase)	(*(ftype *)(sbase+4))
#define FLASH_WRITE(to,frm)				(*(ftype *)(to) = *(ftype *)(frm))

#define WAIT_FOR_DATA(add,data) \
	{ \
		volatile int timeout = FLASH_LOOP_TIMEOUT; \
		while(*(ftype *)add != *(ftype *)data) { \
			if (--timeout <= 0) { \
				STRATACMD_READARRAY(); \
				return(-2); \
			} \
			WATCHDOG_MACRO; \
		} \
	}

#define WAIT_FOR_FF(add) \
	{ \
		volatile int timeout = FLASH_LOOP_TIMEOUT; \
		while(*(ftype *)add != 0xff) { \
			if (--timeout <= 0) { \
				STRATACMD_READARRAY(); \
				return(-3); \
			} \
			WATCHDOG_MACRO; \
		} \
	}


#define WAIT_FOR_WSMS_READY() \
	{ \
		volatile int timeout = FLASH_LOOP_TIMEOUT; \
		while((*(ftype *)(fdev->base) & WSMS) != WSMS) { \
			if (--timeout <= 0) { \
				STRATACMD_READARRAY(); \
				return(-4); \
			} \
			WATCHDOG_MACRO; \
		} \
	}

#define ERASE_FAILURE()			(*(ftype *)(fdev->base) & (ESS|ES|PS))

/* Intel28f256_8x1_erase():
 * Erase the sector specified by snum.
 * Return 0 if success, else negative to indicate some failure.
 */
int
Intel28f256_8x1_erase(struct flashinfo *fdev,int snum)
{
	STRATACMD_CLEARSTATUS();

	/* Issue the setup/confirm sequence: */
	STRATACMD_BLOCKERASE(fdev->base);
	STRATACMD_CONFIRM(fdev->sectors[snum].begin);

	/* Wait for sector erase to complete by polling RSR... */
	WAIT_FOR_WSMS_READY();
				
	if (ERASE_FAILURE()) {
		STRATACMD_READARRAY();
		return(-1);
	}

	STRATACMD_READARRAY();

	WAIT_FOR_FF(fdev->sectors[snum].begin);
	return(0);
}

/* EndIntel28f256_8x1_erase():
 * Function place holder to determine the end of the above function.
 */
void
EndIntel28f256_8x1_erase(void)
{}

int
Intel28f256_8x1_write(struct flashinfo *fdev,uchar *dest,uchar *src, long bytecnt)
{
	volatile int	tot;
	ulong			bcount;
	int				i, j, giveup, aligntot, size;
	volatile uchar	buf[BUFFER_SIZE], *aligndest, *block, *destend;

	/* The write buffer can only be used on (32)-byte blocks; hence, the
	 * low 5 bits of the destination must be zero at the start of a 
	 * buffer write.  This means that we must align the destination
	 * address on this boundary.  To do this, we decrement the destination
	 * address until the alignment is reached, then load that space with
	 * the same data that is already there.
	 */ 
	destend = dest + bytecnt;
	aligntot = 0;
	aligndest = dest;
	while(((ulong)aligndest & BUFFER_ALIGN) != 0) 
    {
		aligndest--;
		aligntot++;
		bytecnt++;
	}

	tot = 0;
	while(tot < bytecnt) 
    {
		size = bytecnt - tot;
		if (size > BUFFER_SIZE) size = BUFFER_SIZE;

		block = aligndest;

		/* Copy buffer's worth of data into local buffer just in
		 * case the source is this flash device.
		 */
		for(i = 0; i < size; i++) 
        {
			if (aligndest < dest)
				buf[i] = *aligndest++;
			else
				buf[i] = *src++;
		}

		j = 0;
		while (size < BUFFER_SIZE) 
        {
			size++;
			buf[i++] = destend[j++];
		}
		aligndest = block;

		/* Issue request to write to the buffer, then poll extended
		 * status register to wait for availability.
		 */
		giveup = SR_WAIT;
		do {
			STRATACMD_WRITETOBUFFER(aligndest);	
			giveup--;
		} while (((FLASH_READ(aligndest) & WBS) != WBS) && (giveup > 0));

		if (giveup == 0) {
			STRATACMD_READARRAY();	
			return(-1);
		}
                
		/* Write the byte count.  Notice that the bytecount fed to the
		 * device is one less than the actual count.*/
		bcount = size-1;
		*(ftype *)block = bcount; 
        

		/* Write the buffer data...
		 */
		for(i = 0; i < size; i++) 
        {
			FLASH_WRITE(aligndest,(&buf[i]));
			aligndest+=1;
		}
		
		STRATACMD_CONFIRM(block);
		tot += size;

		giveup = SR_WAIT;
		do {
			STRATACMD_READSTATUS();
			giveup--;
		} while(((FLASH_READBASE() & WSMS) != WSMS) && (giveup > 0));

		if (giveup == 0) {
			STRATACMD_READARRAY();	
			return(-2);
		}
		STRATACMD_READARRAY();	

	}
	return(0);
}

/* EndIntel28f256_8x1_write():
 * Function place holder to determine the end of the above function.
 */
void
EndIntel28f256_8x1_write(void)
{}

/* Intel28f256_8x1_ewrite():
 * Erase all sectors that are part of the address space to be written,
 * then write the data to that address space.  This is basically a
 * concatenation of the above erase & write done in one step.  This is
 * necessary primarily for re-writing the bootcode; because after the boot
 * code is erased, there is nowhere to return so the re-write must be done
 * while executing out of ram also.  It is only needed in systems that are
 * executing the monitor out of the same device that is being updated.
 */
int
Intel28f256_8x1_ewrite(struct flashinfo *fdev,uchar *destA,uchar *srcA,
	long bytecnt)
{
	ulong   		addr;
	volatile int	sector, i;
	void			(*reset)();
	volatile uchar 	*src, *dest;

	src = srcA;
	dest = destA;
	
	STRATACMD_CLEARSTATUS();

	/* For each sector, if it overlaps any of the destination space */
	/* then erase that sector. */
	for (sector = 0; sector < fdev->sectorcnt; sector++) 
    {
		if ((((uchar *)dest) > (fdev->sectors[sector].end)) ||
		    (((uchar *)dest+bytecnt-1) < (fdev->sectors[sector].begin))) 
        {
			continue;
		}

		addr = (ulong)(fdev->sectors[sector].begin);

		/* Issue the ERASE setup/confirm sequence: */
		STRATACMD_BLOCKERASE(addr);
		STRATACMD_CONFIRM(addr);

		/* Wait for sector erase to complete by polling RSR... */
		WAIT_FOR_WSMS_READY();

		STRATACMD_READARRAY();

		WAIT_FOR_FF(addr);
	}

	for(i = 0; i < bytecnt; i++) 
    {

		/* Flash program setup command */
		STRATACMD_PROGRAM(dest);
		
		/* Write the value */
		FLASH_WRITE(dest,src);

		WAIT_FOR_WSMS_READY();

		STRATACMD_READARRAY();

		WAIT_FOR_DATA(dest,src);

		++dest;
		++src;
	}
	
	/* Now that the re-programming of flash is complete, reset: */
	reset = RESETFUNC();
	reset();

	return(0);	/* won't get here */
}

/* EndIntel28f256_8x1_ewrite():
 * Function place holder to determine the end of the above function.
 */
void
EndIntel28f256_8x1_ewrite(void)
{}

/* Intel28f256_8x1_lock():
 */
int
Intel28f256_8x1_lock(struct flashinfo *fdev,int snum,int operation)
{
	vint i;
	int sector;
	vulong	add, bstat;

	add = (ulong)(fdev->base);

	switch(operation) {
		case FLASH_LOCKABLE:
			return(1);

		case FLASH_UNLOCK:
			STRATACMD_LOCKBIT();
			for(i=0;i<SR_WAIT;i++);
			STRATACMD_CONFIRM(add);
			WAIT_FOR_WSMS_READY();
			STRATACMD_READARRAY();
			for(i=0;i<SR_WAIT;i++);
			return(0);

		case FLASH_LOCK:
			for (sector=0;sector<fdev->sectorcnt;sector++) 
            {
				if ((snum == ALL_SECTORS) || (snum == sector)) 
                {
					STRATACMD_LOCKBIT();
					for(i=0;i<SR_WAIT;i++);
					STRATACMD_SETLOCKCONFIRM(add);
					WAIT_FOR_WSMS_READY();
					STRATACMD_READARRAY();
					for(i=0;i<SR_WAIT;i++);
				}
				add += fdev->sectors[sector].size;
			}
			return(0);

		case FLASH_LOCKQRY:
			STRATACMD_READID();
			bstat = FLASH_READ_BLOCKSTATUS(fdev->sectors[snum].begin);
			STRATACMD_READARRAY();
			if ((bstat & 0x01) == 0x01)
				return(1);
			else
				return(0);

		default:
			return(-1);
	}
}

/* EndIntel28f256_8x1_lock():
 * Function place holder to determine the end of the above function.
 */
void
EndIntel28f256_8x1_lock(void)
{
}
/* Intel28f256_8x1_type():
 * Run the AUTOSELECT algorithm to retrieve the manufacturer and
 * device id of the flash.
 */
 int Intel28f256_8x1_type(struct flashinfo *fdev)
{
   ushort	man, dev;
     unsigned long id;

	STRATACMD_READID();

	man = *(uchar *)fdev->base;		/* manufacturer ID */
	dev = *(uchar *)(fdev->base+2);	/* device ID */
	id = (unsigned long)man;
	id <<= 16;
	id |= (unsigned long)dev;

	fdev->id = id;

	STRATACMD_READARRAY();	
	
	return((int)(fdev->id));


}

/* EndIntel28f256_8x1_type():
 * Function place holder to determine the end of the above function.
 */
void
EndIntel28f256_8x1_type(void)
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
ulong	FlashTypeFbuf[400];
ulong	FlashEraseFbuf[400];
ulong	FlashWriteFbuf[400];
ulong	FlashEwriteFbuf[400];
ulong	FlashLockFbuf[400];
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
	{ DEVICE_28F320J3,	"INTEL-28F320J3" },
	{ DEVICE_28F640J3,	"INTEL-28F640J3" },
	{ DEVICE_28F128J3,	"INTEL-28F128J3" },
	{ DEVICE_28F256J3,	"INTEL-28F256J3" },
	{ 0, 0 },
};

/* Sector size configuration:
 */
ulong	SectorSizes28f256_8x1[] = {
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
	0x20000, 0x20000, 0x20000, 0x20000, 0x20000, 0x20000, 0x20000, 0x20000,
};

struct sectorinfo sinfo256[sizeof(SectorSizes28f256_8x1)/sizeof(int)];

int
FlashBankInit(struct flashinfo *fbnk,int snum)
{
	uchar	*saddr;
	int		i, msize;
	struct	sectorinfo *sinfotbl;

	/* Create the per-sector information table.  The size of the table */
	/* depends on the number of sectors in the device...  */

	if (fbnk->sectors)
		free((char *)fbnk->sectors);
	msize = fbnk->sectorcnt * (sizeof(struct sectorinfo));
	sinfotbl = (struct sectorinfo *)malloc(msize);
	if (!sinfotbl) {
		printf("Can't allocate space (%d bytes) for flash sector info\n",msize);
		return(-1);
	}
	fbnk->sectors = sinfotbl;

	/* Using the above-determined sector count and size table, build */
	/* the sector information table as part of the flash-bank structure: */
	saddr = fbnk->base;
	for(i = 0; i < fbnk->sectorcnt; i++) 
    {
		fbnk->sectors[i].snum = snum+i;
		fbnk->sectors[i].size = SectorSizes28f256_8x1[i];
		fbnk->sectors[i].begin = saddr;
		fbnk->sectors[i].end = fbnk->sectors[i].begin + fbnk->sectors[i].size - 1;
		fbnk->sectors[i].protected = 0;
		saddr += SectorSizes28f256_8x1[i];
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

	if (flashopload((ulong *)Intel28f256_8x1_lock,
		(ulong *)EndIntel28f256_8x1_lock,
		FlashLockFbuf,sizeof(FlashLockFbuf)) < 0)
		return(-1);
	if (flashopload((ulong *)Intel28f256_8x1_type,
		(ulong *)EndIntel28f256_8x1_type,
		FlashTypeFbuf,sizeof(FlashTypeFbuf)) < 0)
		return(-1);
	if (flashopload((ulong *)Intel28f256_8x1_erase,
		(ulong *)EndIntel28f256_8x1_erase,
		FlashEraseFbuf,sizeof(FlashEraseFbuf)) < 0)
		return(-1);
	if (flashopload((ulong *)Intel28f256_8x1_ewrite,
		(ulong *)EndIntel28f256_8x1_ewrite,
		FlashEwriteFbuf,sizeof(FlashEwriteFbuf)) < 0)
		return(-1);
	if (flashopload((ulong *)Intel28f256_8x1_write,
		(ulong *)EndIntel28f256_8x1_write,
		FlashWriteFbuf,sizeof(FlashWriteFbuf)) < 0)
		return(-1);

#endif

	fbnk = &FlashBank[0];
	fbnk->base = (unsigned char *)FLASH_BANK0_BASE_ADDR;
	fbnk->id = DEVICE_28F256J3;
	fbnk->width = FLASH_BANK0_WIDTH;
	fbnk->sectorcnt = (sizeof(SectorSizes28f256_8x1)/sizeof(ulong));
#ifdef FLASH_COPY_TO_RAM
	fbnk->fltype = (int(*)())FlashTypeFbuf;	
	fbnk->flerase = (int(*)())FlashEraseFbuf;
	fbnk->flwrite = (int(*)())FlashWriteFbuf;
	fbnk->flewrite = (int(*)())FlashEwriteFbuf;
	fbnk->fllock = (int(*)())FlashLockFbuf;	
#else
	fbnk->fltype = Intel28f256_8x1_type;
	fbnk->flerase = Intel28f256_8x1_erase;
	fbnk->flwrite = Intel28f256_8x1_write;
	fbnk->flewrite = Intel28f256_8x1_ewrite;
	fbnk->fllock = Intel28f256_8x1_lock;
#endif

	snum += FlashBankInit(fbnk,snum);

	sectorProtect(FLASH_PROTECT_RANGE,1);

	return(0);
}

#endif	/* SINGLE_FLASH_DEVICE */

#endif	/* INCLUDE_FLASH */
