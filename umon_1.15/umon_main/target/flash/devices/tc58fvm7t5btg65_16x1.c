/* tc58fvm7t5btg65_16x1.c:
 *tc58fvm7t5btg65_16x1 flash interface
 */
#include "config.h"
#if INCLUDE_FLASH
#include "stddefs.h"
#include "cpu.h"
#include "flash.h"
#include "tc58fvm7t5btg65_16x1.h"

/* Add all device specific macros here.  
 *
 * Note that the flash_template.h file is ONLY used to declare the
 * function prototypes in this file.  Put all other device specific
 * macros here.
 *
 * Define 'ftype' to be vuchar, vushort or vulong depending on the
 * width of the flash bank in this driver.
 */
#define ftype 					volatile unsigned short
#define Is_ff(add)			   	(*(ftype *)add == 0xffff)
#define Is_not_ff(add)	    	(*(ftype *)add != 0xffff)
#define Is_Equal(p1,p2) 	    (*(ftype *)p1 == *(ftype *)p2)
#define Is_Not_Equal(p1,p2)		(*(ftype *)p1 != *(ftype *)p2)
#define Fwrite(to,frm)  	    (*(ftype *)to = *(ftype *)frm)
#define D5_Timeout(add)	   		((*(ftype *)add & 0x00df) == 0x0020)


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
		*(ftype *)(fdev->base) = 0x00f0; \
		val = *(ftype *)(fdev->base); }

/*************************************************************************
 *
 * tc58fvm7t5btg65_16x1_erase():
 * Erase the specified sector on the device specified by the flash info
 * poniter.  Return 0 if success, else negative to indicate device-specific
 * error or reason for failure.
 */
int
tc58fvm7t5btg65_16x1_erase(struct flashinfo *fdev,int snum)
{
	ftype	val;
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
				SECTOR_ERASE(add);

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

	/* If the erase failed for some reason, then issue the read/reset
	 * command sequence prior to returning...
	 */
	if (ret == -1)  {
		READ_RESET();
	}

	return(ret);
}

/* Endtc58fvm7t5btg65_16x1_erase():
 * Function place holder to determine the end of the above function.
 */
void
Endtc58fvm7t5btg65_16x1_erase(void)
{}

/*************************************************************************
 *
 * tc58fvm7t5btg65_16x1_write():
 * Write the specified block of data.  Return 0 if success, else negative
 * to indicate device-specific error or reason for failure.
 */
int
tc58fvm7t5btg65_16x1_write(struct flashinfo *fdev,uchar *dest,uchar *src,long bytecnt)
{
	ftype	val;
	long	cnt;
	int		i, ret;
	uchar 	*src1;

	ret = 0;
	cnt = bytecnt & ~1;
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

	/* Each pass through this loop writes 'fdev->width' bytes...
	 */

	for (i=0;i<cnt;i+=fdev->width) {

		/* Just in case src is not aligned... */
		src1[0] = src[0];
		src1[1] = src[1];

		FLASH_WRITE(dest,src1);
		
		/* Wait for write to complete or timeout. */
		while(1) {
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
	if (cnt != bytecnt) {
		src1[0] = *src;
		src1[1] = dest[1];

		FLASH_WRITE(dest,src1);

		/* Wait for write to complete or timeout. */
		while(1) {
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

/* Endtc58fvm7t5btg65_16x1_write():
 * Function place holder to determine the end of the above function.
 */
void
Endtc58fvm7t5btg65_16x1_write(void)
{}

/*************************************************************************
 *
 * tc58fvm7t5btg65_16x1_ewrite():
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
tc58fvm7t5btg65_16x1_ewrite(struct flashinfo *fdev,uchar *dest,uchar *src,long bytecnt)
{
	int	i;
	ulong	add;
	void	(*reset)();
	ftype	val, *src1, *dest1;

	add = (ulong)(fdev->base);
	src1 = src;
	dest1 = dest;

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
			if (Is_Equal(dest,src)) {
				if (Is_Equal(dest,src))
					break;
			}
		}

		dest++; 
		src++;
	}

	/* Issue the read/reset command sequence: */
	*(ftype *)(fdev->base + 0x555) = 0x00f0;
	val = *(ftype *)(fdev->base + 0x555);

	/* Wait till flash is readable, or timeout: */
	for(i=0;i<FLASH_TIMEOUT;i++) {
		if (Is_Equal(dest1,src1))
			break;
	}

	/* Now that the re-programming of flash is complete, reset: */
	//reset = RESETFUNC();
	//reset();

	return(0);	/* won't get here */
}

/* Endtc58fvm7t5btg65_16x1_ewrite():
 * Function place holder to determine the end of the above function.
 */
void
Endtc58fvm7t5btg65_16x1_ewrite(void)
{}

/*************************************************************************
 *
 * tc58fvm7t5btg65_16x1_lock():
 * Deal with the flash devices's ability to do a per-sector lock/unlock/query.
 * Note that if the device does  not support lock, then the FLASH_LOCKABLE
 * operation should return 0 and all others should return -1.
 */
int
tc58fvm7t5btg65_16x1_lock(struct flashinfo *fdev,int snum,int operation)
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

/* Endtc58fvm7t5btg65_16x1_lock():
 * Function place holder to determine the end of the above function.
 */
void
Endtc58fvm7t5btg65_16x1_lock(void)
{
}

/* tc58fvm7t5btg65_16x1_type():
 * Run the AUTOSELECT algorithm to retrieve the manufacturer and
 * device id of the flash.
 */
int
tc58fvm7t5btg65_16x1_type(struct flashinfo *fdev)
{
    ushort  man, dev;
    ulong   id;

	// Send the ID command sequence
	(*(ftype *)(fdev->base + (0x555 << 1)) = 0x00aa);	
	(*(ftype *)(fdev->base + (0x2aa << 1)) = 0x0055);
	(*(ftype *)(fdev->base + (0x555 << 1)) = 0x0090);

	// Read in the values
	man = (*(ftype *)(fdev->base));
	dev = (*(ftype *)(fdev->base + 2));


	id = man;
	id <<= 16;
	id |= dev;

	fdev->id = id;
	
	// Read/reset command
	(*(ftype *)(fdev->base) = 0x00f0);	

	return((int)fdev->id);
}

/* Endtc58fvm7t5btg65_16x1_type():
 * Function place holder to determine the end of the above function.
 */
void
Endtc58fvm7t5btg65_16x1_type(void)
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
#define FLASH_FBUF_SIZE 1024
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
	{ tc58fvm7t5btg65,	"tc58fvm7t5btg65" },
	{ 0, 0 },
};


/* Sector size configuration:
 * Each entry in this table represents the size of a sector in the 
 * flash bank, so establish them appropriately...
 */
int	SectorSizestc58fvm7t5btg65_16x1[] = {


	//BK0
	0x10000, 	0x10000,	0x10000,	0x10000,	0x10000, 	0x10000,	0x10000,	0x10000,
	0x10000, 	0x10000,	0x10000,	0x10000,	0x10000, 	0x10000,	0x10000,	0x10000,
	
	//BK1
	0x10000, 	0x10000,	0x10000,	0x10000,	0x10000, 	0x10000,	0x10000,	0x10000,
	0x10000, 	0x10000,	0x10000,	0x10000,	0x10000, 	0x10000,	0x10000,	0x10000,
	
	//BK2
	0x10000, 	0x10000,	0x10000,	0x10000,	0x10000, 	0x10000,	0x10000,	0x10000,
	0x10000, 	0x10000,	0x10000,	0x10000,	0x10000, 	0x10000,	0x10000,	0x10000,
	
	//BK3
	0x10000, 	0x10000,	0x10000,	0x10000,	0x10000, 	0x10000,	0x10000,	0x10000,
	0x10000, 	0x10000,	0x10000,	0x10000,	0x10000, 	0x10000,	0x10000,	0x10000,
	
	//BK4
	0x10000, 	0x10000,	0x10000,	0x10000,	0x10000, 	0x10000,	0x10000,	0x10000,
	0x10000, 	0x10000,	0x10000,	0x10000,	0x10000, 	0x10000,	0x10000,	0x10000,
	
	//BK5
	0x10000, 	0x10000,	0x10000,	0x10000,	0x10000, 	0x10000,	0x10000,	0x10000,
	0x10000, 	0x10000,	0x10000,	0x10000,	0x10000, 	0x10000,	0x10000,	0x10000,
	
	//BK6
	0x10000, 	0x10000,	0x10000,	0x10000,	0x10000, 	0x10000,	0x10000,	0x10000,
	0x10000, 	0x10000,	0x10000,	0x10000,	0x10000, 	0x10000,	0x10000,	0x10000,
	
	//BK7
	0x10000, 	0x10000,	0x10000,	0x10000,	0x10000, 	0x10000,	0x10000,	0x10000,
	0x10000, 	0x10000,	0x10000,	0x10000,	0x10000, 	0x10000,	0x10000,	0x10000,
	
	//BK8
	0x10000, 	0x10000,	0x10000,	0x10000,	0x10000, 	0x10000,	0x10000,	0x10000,
	0x10000, 	0x10000,	0x10000,	0x10000,	0x10000, 	0x10000,	0x10000,	0x10000,
	
	//BK9
	0x10000, 	0x10000,	0x10000,	0x10000,	0x10000, 	0x10000,	0x10000,	0x10000,
	0x10000, 	0x10000,	0x10000,	0x10000,	0x10000, 	0x10000,	0x10000,	0x10000,
	
	//BK10
	0x10000, 	0x10000,	0x10000,	0x10000,	0x10000, 	0x10000,	0x10000,	0x10000,
	0x10000, 	0x10000,	0x10000,	0x10000,	0x10000, 	0x10000,	0x10000,	0x10000,
	
	//BK11
	0x10000, 	0x10000,	0x10000,	0x10000,	0x10000, 	0x10000,	0x10000,	0x10000,
	0x10000, 	0x10000,	0x10000,	0x10000,	0x10000, 	0x10000,	0x10000,	0x10000,
	
	//BK12
	0x10000, 	0x10000,	0x10000,	0x10000,	0x10000, 	0x10000,	0x10000,	0x10000,
	0x10000, 	0x10000,	0x10000,	0x10000,	0x10000, 	0x10000,	0x10000,	0x10000,
	
	//BK13
	0x10000, 	0x10000,	0x10000,	0x10000,	0x10000, 	0x10000,	0x10000,	0x10000,
	0x10000, 	0x10000,	0x10000,	0x10000,	0x10000, 	0x10000,	0x10000,	0x10000,
	
	//BK14
	0x10000, 	0x10000,	0x10000,	0x10000,	0x10000, 	0x10000,	0x10000,	0x10000,
	0x10000, 	0x10000,	0x10000,	0x10000,	0x10000, 	0x10000,	0x10000,	0x10000,
	
	//BK15
	0x10000, 	0x10000,	0x10000,	0x10000,	0x10000, 	0x10000,	0x10000,	0x10000,
	0x10000, 	0x10000,	0x10000,	0x10000,	0x10000, 	0x10000,	0x10000,	

	// Boot block
	0x02000,	0x02000, 	0x02000,	0x02000,	0x02000,	0x02000, 	0x02000,	0x02000,

	
};

struct sectorinfo sinfoFlash[sizeof(SectorSizestc58fvm7t5btg65_16x1)/sizeof(int)];

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
		fbnk->sectors[i].size = SectorSizestc58fvm7t5btg65_16x1[i];
		fbnk->sectors[i].begin = saddr;
		fbnk->sectors[i].end =
		    fbnk->sectors[i].begin + fbnk->sectors[i].size - 1;
		fbnk->sectors[i].protected = 0;
		saddr += SectorSizestc58fvm7t5btg65_16x1[i];
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

	if (flashopload((ulong *)tc58fvm7t5btg65_16x1_lock,
		(ulong *)Endtc58fvm7t5btg65_16x1_lock,
		FlashLockFbuf,sizeof(FlashLockFbuf)) < 0)
		return(-1);
	if (flashopload((ulong *)tc58fvm7t5btg65_16x1_type,
		(ulong *)Endtc58fvm7t5btg65_16x1_type,
		FlashTypeFbuf,sizeof(FlashTypeFbuf)) < 0)
		return(-1);
	if (flashopload((ulong *)tc58fvm7t5btg65_16x1_erase,
		(ulong *)Endtc58fvm7t5btg65_16x1_erase,
		FlashEraseFbuf,sizeof(FlashEraseFbuf)) < 0)
		return(-1);
	if (flashopload((ulong *)tc58fvm7t5btg65_16x1_ewrite,
		(ulong *)Endtc58fvm7t5btg65_16x1_ewrite,
		FlashEwriteFbuf,sizeof(FlashEwriteFbuf)) < 0)
		return(-1);
	if (flashopload((ulong *)tc58fvm7t5btg65_16x1_write,
		(ulong *)Endtc58fvm7t5btg65_16x1_write,
		FlashWriteFbuf,sizeof(FlashWriteFbuf)) < 0)
		return(-1);

#endif

	fbnk = &FlashBank[0];
	fbnk->base = (unsigned char *)FLASH_BANK0_BASE_ADDR;
	fbnk->id = tc58fvm7t5btg65;
	fbnk->width = FLASH_BANK0_WIDTH;
	fbnk->sectorcnt = (sizeof(SectorSizestc58fvm7t5btg65_16x1)/sizeof(int));

#ifdef FLASH_COPY_TO_RAM
	fbnk->fltype = (int(*)())FlashTypeFbuf;	
	fbnk->flerase = (int(*)())FlashEraseFbuf;
	fbnk->flwrite = (int(*)())FlashWriteFbuf;
	fbnk->flewrite = (int(*)())FlashEwriteFbuf;
	fbnk->fllock = (int(*)())FlashLockFbuf;	
#else
	fbnk->fltype = tc58fvm7t5btg65_16x1_type;
	fbnk->flerase = tc58fvm7t5btg65_16x1_erase;
	fbnk->flwrite = tc58fvm7t5btg65_16x1_write;
	fbnk->flewrite = tc58fvm7t5btg65_16x1_ewrite;
	fbnk->fllock = tc58fvm7t5btg65_16x1_lock;
#endif

	snum += FlashBankInit(fbnk,snum);

	sectorProtect(FLASH_PROTECT_RANGE,1);

	return(0);
}

#endif	/* SINGLE_FLASH_DEVICE */

#endif	/* INCLUDE_FLASH */
