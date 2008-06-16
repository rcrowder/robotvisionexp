/* 
 * flashpic.c:
 * This started off as the flash driver contributed by IAN.  I has been
 * modified slightly.
 *
 */
#include "config.h"
#if INCLUDE_FLASH
#include "stddefs.h"
#include "cpu.h"
#include "flashdev.h"
#include "flash.h"

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

int
Flashlock32(struct flashinfo *fdev,int snum,int operation)
{
	ulong	add;
	int	sector;

	add = (ulong)(fdev->base);

	/* Lock the requested sector(s): */
	for (sector=0;sector<fdev->sectorcnt;sector++) {
		if ((snum == ALL_SECTORS) || (snum == sector)) {
			/* Issue the appropriate command sequence: */
			Write_60_to_base();	
			switch(operation) {
			case FLASH_UNLOCK:
				Write_d0_to(add);
				break;
			case FLASH_LOCK:
				Write_01_to(add);
				break;
			case FLASH_LOCKDWN:
				return(-1);
				break;
			}
		}
		add += fdev->sectors[sector].size;
	}
	Write_ff_to_base();		/* Go to read-array mode */
	return(0);
}

void
EndFlashlock32()
{}

/* Flasherase():
   Based on the 'snum' value, erase the appropriate sector(s).
   Write the "Erase Setup" and "Erase Confirm" commands...
   Return 0 if success, else -1.
*/
int
Flasherase32(struct flashinfo *fdev,int snum)
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
					if (! (rsr & WSMS_H && rsr & WSMS_L)) { /* Wait till ready */
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

/* EndFlasherase():
   Function place holder to determine the "end" of the
   sectorerase() function.
*/
void
EndFlasherase32()
{
}


/* Flashwrite():
   Return 0 if successful, else -1.
*/
int
Flashwrite32(struct flashinfo *fdev,uchar *dest,uchar *src,long bytecnt)
{
	ftype			stat;
	int				rval;
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

		if (Flashwrite32(fdev,tmpdest,(uchar *)buf4,4) == -1)
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
			rval = -1;
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
			rval = -1;
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

/* EndFlashwrite():
	Function place holder to determine the "end" of the
	Flashwrite() function.
*/
void
EndFlashwrite32()
{
}

/* Ewrite():
   Erase all sectors that are part of the address space to be written,
   then write the data to that address space.  This is basically a
   concatenation of flasherase and flashwrite done in one step.  This is
   necessary primarily for re-writing the bootcode; because after the boot
   code is erased, there is nowhere to return so the re-write must be done
   while executing out of ram also.
*/

int
Flashewrite32(struct flashinfo *fdev,uchar *destA,uchar *srcA,int bytecnt)
{
	int	    sector, i;
	void	(*reset)();
	uchar	*src1;
	ftype	val;
	uchar   *src, *dest;
	ulong   rsr;


ewrite_again:
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
		do {
			Write_70_to_base();
			rsr = Read_0000_from_base();
		} while (! (rsr & WSMS_H && rsr & WSMS_L));

		Write_50_to_base();		/* Clear status register */
		Write_ff_to_base();		/* Go to read-array mode */
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

/* EndFlashewrite():
	Function place holder to determine the "end" of the
	FlashEraseAndWrite() function.
*/
void
EndFlashewrite32()
{
}

/* Flashtype():
   Use the ReadCOnfiguration command (90H) to read manufacturer code (0000)
   and device id code (0001).
*/
int
Flashtype32(struct flashinfo *fdev)
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

/* EndFlashtype():
	Function place holder to determine the "end" of the
	Flashtype() function.
*/
void
EndFlashtype32()
{
}
#endif
