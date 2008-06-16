/* intel28f128_16x2.c:
 * Support for INTEL 28f128.
 * A 16-bit device in a 2-device-wide bank.
 */
#include "config.h"
#if INCLUDE_FLASH
#include "stddefs.h"
#include "genlib.h"
#include "cpu.h"
#include "flash.h"
#include "intel28f128_16x2.h"

#define SR_WAIT	100000
#define WR_WAIT	50

#define DEV_WIDTH	4

#define WSMS	0x00800080
#define WSMS_L	0x00000080
#define WSMS_H	0x00800000
#define ESS		0x00400040
#define ES		0x00200020
#define PSS		0x00040004
#define PS		0x00100010

extern struct flashinfo	Fdev;
extern int FlashProtectWindow;

/* Manufacturer and device ids... */
#define INTEL28F128J	0x00890018

#define	ftype			    volatile unsigned long

#define FWrite(to,frm)		(*(ftype *)(to) = *(ftype *)(frm))
#define Write_to(add, val)	(*(ftype *)(add) = (val))
#define Write_01_to(add)	(*(ftype *)(add) = 0x00010001)
#define Write_20_to(add)	(*(ftype *)(add) = 0x00200020)
#define Write_40_to(add)	(*(ftype *)(add) = 0x00400040)
#define Write_60_to(add)	(*(ftype *)(add) = 0x00600060)
#define Write_70_to(add)	(*(ftype *)(add) = 0x00700070)
#define Write_d0_to(add)	(*(ftype *)(add) = 0x00d000d0)

#define Write_50_to_base()	(*(ftype *)(fdev->base) = 0x00500050)
#define Write_60_to_base()	(*(ftype *)(fdev->base) = 0x00600060)
#define Write_70_to_base()	(*(ftype *)(fdev->base) = 0x00700070)
#define Write_90_to_base()	(*(ftype *)(fdev->base) = 0x00900090)
#define Write_d0_to_base()	(*(ftype *)(fdev->base) = 0x00d000d0)
#define Write_ff_to_base()	(*(ftype *)(fdev->base) = 0x00FF00FF)

#define Read_from(add)		(*(ftype *)(add))
#define Read_0000_from_base()	(*(ftype *)(fdev->base+(0x00000000<<2)))
#define Read_0001_from_base()	(*(ftype *)(fdev->base+(0x00000001<<2)))
#define Read_5555_from_base()	(*(ftype *)(fdev->base+(0x5555<<1)))

#define Is_ff(add)		(*(ftype *)(add) == 0xFFFFFFFF)
#define Is_not_ff(add)		(*(ftype *)(add) != 0xFFFFFFFF)
#define Is_Equal(p1,p2)		(*(ftype *)(p1) == *(ftype *)(p2))
#define Is_Not_Equal(p1,p2)	(*(ftype *)(p1) != *(ftype *)(p2))
#define Not32BitAligned(ptr)	((long)ptr & 3)

/* Intel28f128_16x2_erase():
 * Based on the 'snum' value, erase the appropriate sector(s).
 * Return 0 if success, else -1.
 */
int
Intel28f128_16x2_erase(struct flashinfo *fdev,int snum)
{
	int	ret, sector;
	volatile int i;

	ret = 0;

	/* Erase the request sector(s): */
	for (sector=0; sector < fdev->sectorcnt; sector++) {
		if ((!FlashProtectWindow) && (fdev->sectors[sector].protected)) {
			continue;
		}
		if ((snum == ALL_SECTORS) || (snum == sector)) {
			register ulong *lp, *lp1;
			int	noterased;

			/* See if the sector is already erased: */
			noterased = 0;
			lp = (ulong *)fdev->sectors[sector].begin; 
			lp1 = (ulong *)((char *)lp + fdev->sectors[sector].size - 1); 
			while(lp <= lp1) {
				if (*lp++ != 0xffffffff) {
					noterased = 1;
					break;
				}
			}
			if (noterased) {
				/* Issue the setup/confirm sequence: */
				Write_20_to(fdev->sectors[sector].begin);/* setup */
				Write_d0_to(fdev->sectors[sector].begin);/* confirm */

				/* Wait for sector erase to complete by polling RSR... */
				while(1) {
					ulong rsr;

					Write_70_to_base();
					rsr = Read_0000_from_base();
					if (!(rsr & WSMS_H && rsr & WSMS_L)) { /* Wait till ready */
						for (i=0; i<SR_WAIT; i++);
						continue;
					}
					if (rsr & ESS) { /* Should not be suspended */
						ret = -1;
					}
					if (rsr & ES) { /* Should not have error */
						ret = -1;
					}
					break;
				}
				Write_50_to_base();	/* Clear status register */
				Write_ff_to_base();	/* Go to read-array mode */
			}
			if (ret == -1)
				break;
		}
	}
	for (i=0; i<SR_WAIT; i++);
	return(ret);
}

/* EndIntel28f128_16x2_erase():
 * Function place holder to determine the end of the above function.
 */
void
EndIntel28f128_16x2_erase(void)
{}


/* Intel28f128_16x2_write():
 * Copy specified number of bytes from source to destination.  The destination
 * address is assumed to be flash space.
 */
int
Intel28f128_16x2_write(struct flashinfo *fdev,uchar *dest,uchar *src,
	long bytecnt)
{
	ftype			stat;
	volatile long	cnt, buf4[1];
	volatile int	i, tot, ret, delta;
	volatile uchar	*bp4;

#ifdef FLASH_TRACE
	printf("fwrite(dest=0x%lx,src=0x%lx,size=%d)\n",
		(ulong)dest,(ulong)src,bytecnt);
#endif

	/* If the destination address is not properly aligned, then build a
	 * fake source buffer using bytes below dest.  Then call this function
	 * recursively to do that operation.
	 */
	if (Not32BitAligned(dest)) {
		uchar	*tmpdest;

		delta = (long)dest & 3;
		tmpdest = dest - delta;

		bp4 = (uchar *)buf4;
		for(i=0;delta>0;i++,delta--) {
			*bp4++ = *(dest-delta);
		}
		for(;i<4;i++) {
			if (bytecnt > 0) {
				*bp4++ = *src++;
				bytecnt--;
			}
			else {
				*bp4++ = *dest;
			}
			dest++;
		}
#ifdef FLASH_TRACE
		printf("pre-align:  0x%lx = %08lx\n",(ulong)tmpdest,buf4[0]);
#endif

		if (Intel28f128_16x2_write(fdev,tmpdest,(uchar *)buf4,4) == -1)
			return(-1);
	}

#ifdef FLASH_TRACE
	printf("bc=%d\n",bytecnt);
#endif
	ret = 0;
	cnt = bytecnt & ~3;

	for (tot=0;tot<cnt;tot+=4) {
		bp4 = (uchar *)buf4;
		*bp4++ = *src++;				/* Just in case src is not aligned... */
		*bp4++ = *src++;				
		*bp4++ = *src++;				
		*bp4++ = *src++;				

#ifdef FLASH_TRACE
		printf("fwrite1:    0x%lx = %08lx\n",(ulong)dest,buf4[0]);
#endif

		Write_40_to(dest);	/* flash program setup */
		FWrite(dest, buf4); /* write value */
		do {
			Write_70_to_base();
			stat = Read_0000_from_base();
		} while ( ! (stat & WSMS_H && stat & WSMS_L) );
		Write_50_to_base(); /* Clear status register */
		Write_ff_to_base(); /* Go to read-array mode */
		if ( Is_Not_Equal(dest, buf4)) {
			ret = -1;
			break;
		}
		dest += 4; 
	}

onemore:
	for (;tot<cnt;tot+=4) {
		bp4 = (uchar *)buf4;
		*bp4++ = *src++;				/* Just in case src is not aligned... */
		*bp4++ = *src++;
		*bp4++ = *src++;
		*bp4++ = *src++;

#ifdef FLASH_TRACE
		printf("fwrite2:    0x%lx = %08lx\n",(ulong)dest,buf4[0]);
#endif
		Write_40_to(dest);	/* flash program setup */
		FWrite(dest, buf4); /* write value */
		do {
			Write_70_to_base();
			stat = Read_0000_from_base();
		} while ( ! (stat & WSMS_H && stat & WSMS_L) );
		Write_50_to_base(); /* Clear status register */
		Write_ff_to_base(); /* Go to read-array mode */
		if (Is_Not_Equal(dest,buf4)) {
			ret = -1;
			break;
		}

		dest += 4; 
	}

	/* If cnt != bytecnt then bytecnt is not mod4, so one more write must be
	 * be done.  To do this, we must combine the source data with data that
	 * is already in the flash above the intended final address...
	 */
	if (cnt != bytecnt) {
		bp4 = (uchar *)buf4;
		for(delta=0;cnt != bytecnt;delta++,cnt++)
			*bp4++ = *src++;
		for(;delta != 4;delta++)
			*bp4++ = *(dest+delta);
		src = (uchar *)buf4;

		tot = cnt-1;
		goto onemore;
	}
	return(ret);
}

/* EndIntel28f128_16x2_write():
 * Function place holder to determine the end of the above function.
 */
void
EndIntel28f128_16x2_write(void)
{}

/* Intel28f128_16x2_ewrite():
 * Erase all sectors that are part of the address space to be written,
 * then write the data to that address space.  This is basically a
 * concatenation of the above erase & write done in one step.  This is
 * necessary primarily for re-writing the bootcode; because after the boot
 * code is erased, there is nowhere to return so the re-write must be done
 * while executing out of ram also.  It is only needed in systems that are
 * executing the monitor out of the same device that is being updated.
 */
int
Intel28f128_16x2_ewrite(struct flashinfo *fdev,uchar *destA,uchar *srcA,
	long bytecnt)
{
	int	    sector, efail, i;
	void	(*reset)();
	uchar	*src1;
	ftype	val;
	uchar   *src, *dest;
	ulong   rsr;

ewrite_again:
	efail = 0;
	src = srcA;
	dest = destA;
	Write_50_to_base(); /* clear status register */
	Write_ff_to_base(); /* set device in read-array mode */

	/* For each sector, if it overlaps any of the destination space */
	/* then erase that sector. */
	for (sector = 0; sector < fdev->sectorcnt; sector++) {
		if ((((uchar *)dest) > (fdev->sectors[sector].end)) ||
		    (((uchar *)dest+bytecnt-1) < (fdev->sectors[sector].begin))) {
			continue;
		}

		/* Issue the ERASE setup/confirm sequence: */
		Write_20_to(fdev->sectors[sector].begin);/* setup */
		Write_d0_to(fdev->sectors[sector].begin);/* confirm */

		/* Wait for sector erase to complete by polling RSR... */
		while(1) {
			ulong rsr;

			Write_70_to_base();
			rsr = Read_0000_from_base();
			if (!(rsr & WSMS_H && rsr & WSMS_L)) { /* Wait till ready */
				for (i=0; i<SR_WAIT; i++);
				continue;
			}

			if (rsr & (ESS|ES))	/* Failure if suspension or error. */
				efail = 1;

			break;
		}
		Write_50_to_base();		/* Clear status register */
		Write_ff_to_base();		/* Go to read-array mode */

		if (efail)				/* If erase failed, return -1 */
			return(-1);
	}


	src1 = (uchar *)&val;

	for(i = 0; i < bytecnt; i += DEV_WIDTH) {

		/* Just in case src is not aligned... */
		src1[0] = src[0];
		src1[1] = src[1];
		src1[2] = src[2];
		src1[3] = src[3];

		/* Flash program setup command */
		Write_40_to(dest);
		
		/* Write the value */
		FWrite(dest,src1);

		/* Wait for write to complete by polling RSR... */
		do {
			Write_70_to_base();
			rsr = Read_0000_from_base();
		} while (! (rsr & WSMS_H && rsr & WSMS_L));

		Write_50_to_base();		/* Clear status register */
		Write_ff_to_base();		/* Go to read-array mode */

		if (Is_Not_Equal(dest, src1)) {
			goto ewrite_again;
		}
		dest+=DEV_WIDTH;
		src+=DEV_WIDTH;
	}
	
	/* Now that the re-programming of flash is complete, reset: */
	reset = RESETFUNC();
	reset();

	return(0);	/* won't get here */
}

/* EndIntel28f128_16x2_ewrite():
 * Function place holder to determine the end of the above function.
 */
void
EndIntel28f128_16x2_ewrite(void)
{}

/* Intel28f128_16x2_lock():
 */
int
Intel28f128_16x2_lock(struct flashinfo *fdev,int snum,int operation)
{
	volatile int i;
	ulong	add, rsr;
	int		sector;

	add = (ulong)(fdev->base);

	if (operation == FLASH_LOCKABLE)
		return(1);

	if (operation == FLASH_LOCKQRY) {
		/* TODO: Write this */
		return(0);
	}

	/* Not applicable for this device.
	 */
	if (operation == FLASH_LOCKDWN)
		return(-1);

	/* For this device the unlock is applied to all
	 * sectors, so no need to enter the loop below.
	 */
	if (operation == FLASH_UNLOCK) {
			Write_60_to_base();
			Write_d0_to_base();

			/* Wait for unlock to complete by polling
			 * status register.
			 */
			do {
				for(i=0;i<SR_WAIT*10;i++);
				rsr = *(ulong *)add;
			} while(!(rsr & WSMS));
			for(i=0;i<SR_WAIT;i++);
			Write_ff_to_base();		/* Go to read-array mode */
		return(0);
	}

	/* Lock the requested sector(s):
	 */
	for (sector=0;sector<fdev->sectorcnt;sector++) {
		if ((snum == ALL_SECTORS) || (snum == sector)) {
			/* Issue the setup/lock command sequence
			 */
			Write_60_to(add);	
			Write_01_to(add);

			/* Wait for lock/unlock to complete by polling
			 * status register.
			 */
			do {
				for(i=0;i<SR_WAIT;i++);
				rsr = *(ulong *)add;
			} while(!(rsr & WSMS));
			Write_ff_to_base();		/* Go to read-array mode */
		}
		add += fdev->sectors[sector].size;
	}
	return(0);
}

/* EndIntel28f128_16x2_lock():
 * Function place holder to determine the end of the above function.
 */
void
EndIntel28f128_16x2_lock(void)
{
}
/* Intel28f128_16x2_type():
 * Run the AUTOSELECT algorithm to retrieve the manufacturer and
 * device id of the flash.
 */
int
Intel28f128_16x2_type(struct flashinfo *fdev)
{
	ushort	man, dev;
	ulong	id;

	/* Issue the read configuration command: */
	Write_90_to_base();

	man = (ushort)Read_0000_from_base();	/* manufacturer ID */
	dev = (ushort)Read_0001_from_base();	/* device ID */
	id = man;
	id <<= 16;
	id |= dev;

	fdev->id = id;

	/* Issue the read array command: */
	Write_ff_to_base();
	
	return((int)(fdev->id));
}

/* EndIntel28f128_16x2_type():
 * Function place holder to determine the end of the above function.
 */
void
EndIntel28f128_16x2_type(void)
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
 * of the flash device.
 */
struct flashdesc FlashNamId[] = {
	{ INTEL28F128J,		"INTEL-28F128J" },
	{ 0, 0 },
};

/* This configuration is 2 side-by-side 28F128J devices.
 * Each device has 128 128Kbyte sectors, so since we have 2
 * in parallel, then each 32-bit sector is 256Kbytes...
 */
int	SectorSizes28F128_32[] = {
	0x40000, 0x40000, 0x40000, 0x40000, 0x40000, 0x40000, 0x40000, 0x40000,
	0x40000, 0x40000, 0x40000, 0x40000, 0x40000, 0x40000, 0x40000, 0x40000,
	0x40000, 0x40000, 0x40000, 0x40000, 0x40000, 0x40000, 0x40000, 0x40000,
	0x40000, 0x40000, 0x40000, 0x40000, 0x40000, 0x40000, 0x40000, 0x40000,
	0x40000, 0x40000, 0x40000, 0x40000, 0x40000, 0x40000, 0x40000, 0x40000,
	0x40000, 0x40000, 0x40000, 0x40000, 0x40000, 0x40000, 0x40000, 0x40000,
	0x40000, 0x40000, 0x40000, 0x40000, 0x40000, 0x40000, 0x40000, 0x40000,
	0x40000, 0x40000, 0x40000, 0x40000, 0x40000, 0x40000, 0x40000, 0x40000,
	0x40000, 0x40000, 0x40000, 0x40000, 0x40000, 0x40000, 0x40000, 0x40000,
	0x40000, 0x40000, 0x40000, 0x40000, 0x40000, 0x40000, 0x40000, 0x40000,
	0x40000, 0x40000, 0x40000, 0x40000, 0x40000, 0x40000, 0x40000, 0x40000,
	0x40000, 0x40000, 0x40000, 0x40000, 0x40000, 0x40000, 0x40000, 0x40000,
	0x40000, 0x40000, 0x40000, 0x40000, 0x40000, 0x40000, 0x40000, 0x40000,
	0x40000, 0x40000, 0x40000, 0x40000, 0x40000, 0x40000, 0x40000, 0x40000,
	0x40000, 0x40000, 0x40000, 0x40000, 0x40000, 0x40000, 0x40000, 0x40000,
	0x40000, 0x40000, 0x40000, 0x40000, 0x40000, 0x40000, 0x40000, 0x40000,
};


struct sectorinfo sinfo128[sizeof(SectorSizes28F128_32)/sizeof(int)];

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
		case INTEL28F128J:
			fbnk->sectorcnt = (sizeof(SectorSizes28F128_32)/sizeof(int));
			sizetable = SectorSizes28F128_32;
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

	if (flashopload((ulong *)Intel28f128_16x2_lock,
		(ulong *)EndIntel28f128_16x2_lock,
		FlashLockFbuf,sizeof(FlashLockFbuf)) < 0)
		return(-1);
	if (flashopload((ulong *)Intel28f128_16x2_type,
		(ulong *)EndIntel28f128_16x2_type,
		FlashTypeFbuf,sizeof(FlashTypeFbuf)) < 0)
		return(-1);
	if (flashopload((ulong *)Intel28f128_16x2_erase,
		(ulong *)EndIntel28f128_16x2_erase,
		FlashEraseFbuf,sizeof(FlashEraseFbuf)) < 0)
		return(-1);
	if (flashopload((ulong *)Intel28f128_16x2_ewrite,
		(ulong *)EndIntel28f128_16x2_ewrite,
		FlashEwriteFbuf,sizeof(FlashEwriteFbuf)) < 0)
		return(-1);
	if (flashopload((ulong *)Intel28f128_16x2_write,
		(ulong *)EndIntel28f128_16x2_write,
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
	fbnk->fllock = (int(*)())FlashLockFbuf;		/* flashlock(). */
#else
	fbnk->fltype = Intel28f128_16x2_type;
	fbnk->flerase = Intel28f128_16x2_erase;
	fbnk->fllock = Intel28f128_16x2_lock;
	fbnk->flwrite = Intel28f128_16x2_write;
	fbnk->flewrite = Intel28f128_16x2_ewrite;
#endif

	snum += FlashBankInit(fbnk,snum);

	sectorProtect(FLASH_PROTECT_RANGE,1);

#ifdef FLASHRAM_BASE
	FlashRamInit(snum, FLASHRAM_SECTORCOUNT,
		&FlashBank[FLASHRAM_BANKNUM], sinfoRAM, 0);
#endif
	return(0);
}

#endif	/* SINGLE_FLASH_DEVICE */

#endif	/* INCLUDE_FLASH */
