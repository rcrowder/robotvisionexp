/* flashpic.c:
   This file contains the flash-support code that is relocated to
   RAM prior to execution.  This file currently only supports the
   device when configured in 16-bit mode.
*/
#include "config.h"
#if INCLUDE_FLASH
typedef unsigned char uchar;
typedef unsigned short ushort;
typedef unsigned long ulong;
typedef volatile unsigned char vuchar;
typedef volatile unsigned short vushort;
typedef volatile unsigned long vulong;
typedef volatile unsigned int vuint;
typedef volatile int vint;

#include "cpu.h"
#include "flashdev.h"
#include "flash.h"

#define WSMS	0x0080
#define ESS		0x0040
#define ES		0x0020
#define PSS		0x0004
#define PS		0x0010

extern struct flashinfo	Fdev;
extern int FlashProtectWindow;

int
Flashlock16(struct flashinfo *fdev,int snum,int operation)
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
				Write_d0_to_(add);
				break;
			case FLASH_LOCK:
				Write_01_to_(add);
				break;
			case FLASH_LOCKDWN:
				Write_2f_to_(add);
				break;
			}
		}
		add += fdev->sectors[sector].size;
	}
	Write_FF_to_base();		/* Go to read-array mode */
	return(0);
}

void
EndFlashlock16()
{}

/* Flasherase():
   Based on the 'snum' value, erase the appropriate sector(s).
   Write the "Erase Setup" and "Erase Confirm" commands...
   Return 0 if success, else -1.
*/
int
Flasherase16(struct flashinfo *fdev,int snum)
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
				Write_20_to_base();		/* setup */
				Write_d0_to_(add);		/* confirm */

				/* Wait for sector erase to complete by polling RSR... */
				Write_70_to_base();
				while(1) {
					ushort rsr;

					rsr = Read_0000();
					if (!(rsr & WSMS))	/* Wait till ready */
						continue;
					if (rsr & ESS) 		/* Should not be suspended */
						ret = -1;
					if (rsr & ES) 		/* Should not have error */
						ret = -1;
					break;
				}
				Write_50_to_base();		/* Clear status register */
				Write_FF_to_base();		/* Go to read-array mode */
			}
			if (ret == -1)
				break;
		}
		add += fdev->sectors[sector].size;
	}
	return(ret);
}

/* EndFlasherase():
   Function place holder to determine the "end" of the
   sectorerase() function.
*/
void
EndFlasherase16()
{
}


/* Flashwrite():
   Return 0 if successful, else -1.
*/
int
Flashwrite16(struct flashinfo *fdev,uchar *dest,uchar *src,long bytecnt)
{
	int		i, ret;
	long	cnt;
	uchar 	*src1;
	ftype	val;

	/* If destination address is not properly aligned, then build a fake   */
	/* source buffer based on the current value in dest[-1] and src[0].    */
	/* Then call this function to do that 2-byte operation.  Once that     */
	/* completes, simply increment dest and src by 1 and continue in this  */
	/* context. */
	if (NotAligned(dest)) {
		uchar buf[2];
		buf[0] = *(dest-1);
		buf[1] = *src;
		Flashwrite16(fdev,dest-1,buf,2);
		dest++; src++; bytecnt--;
	}

	/* Each pass through this loop writes 'fdev->width' bytes... */
	ret = 0;
	cnt = bytecnt & ~1;
	src1 = (uchar *)&val;

onemore:
	for (i=0;i<cnt;i+=fdev->width) {

	   /* Just in case src is not aligned... */
		src1[0] = src[0];
		src1[1] = src[1];

		/* Flash program setup command */
		Write_40_to_base();
		  
		/* Write the value */
		Fwrite(dest,src1);

		/* Wait for write to complete or timeout. */
		Write_70_to_base();
		while(1) {
			ushort rsr;

			rsr = Read_0000();
			if (!(rsr & WSMS))	/* Wait till ready */
				continue;
			if (rsr & PSS) 		/* Should not be suspended */
				ret = -1;
			if (rsr & PS) 		/* Should not have error */
				ret = -1;
			break;
		}
		Write_50_to_base();		/* Clear status register */
		Write_FF_to_base();		/* Go to read-array mode */
		dest += fdev->width; 
		src += fdev->width;
	}

	/* If bytecount was odd... */
	/* If cnt != bytecnt then bytecnt is odd, so one more byte must be */
	/* written to flash.  To do this, the one byte must be combined with */
	/* the next byte that is already stored in flash; then re-written... */
	if (cnt != bytecnt) {
		val = (ftype)*dest | ((ftype)(*src) << 8);
		src = (uchar *)&val;
		bytecnt = cnt = 1;
		goto onemore;
	}

	return(ret);
}

/* EndFlashwrite():
	Function place holder to determine the "end" of the
	Flashwrite() function.
*/
void
EndFlashwrite16()
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
Flashewrite16(struct flashinfo *fdev,ftype *dest,ftype *src,int bytecnt)
{
	int	i, err;
	ulong	add;
	void	(*reset)();
	ftype	*src1;

	err = 0;
	add = (ulong)(fdev->base);
	src1 = src;

	/* For each sector, if it overlaps any of the destination space */
	/* then erase that sector. */
	for (i=0;i<fdev->sectorcnt;i++) {
		if ((((uchar *)dest) > (fdev->sectors[i].end)) ||
		    (((uchar *)dest+bytecnt-1) < (fdev->sectors[i].begin))) {
			add += fdev->sectors[i].size;
			continue;
		}

		/* Issue the ERASE setup/confirm sequence: */
		Write_20_to_base();		/* setup */
		Write_d0_to_(add);		/* confirm */

		/* Wait for sector erase to complete by polling RSR... */
		Write_70_to_base();
		while(1) {
			ushort rsr;

			rsr = Read_0000();
			if (!(rsr & WSMS))	/* Wait till ready */
				continue;
			if (rsr & ESS) 		/* Should not be suspended */
				err = 1;
			if (rsr & ES) 		/* Should not have error */
				err = 1;
			break;
		}
		Write_50_to_base();		/* Clear status register */
		Write_FF_to_base();		/* Go to read-array mode */
		if (err)
			return(-1);
		add += fdev->sectors[i].size;
	}

	for(i=0;i<bytecnt;i+=fdev->width) {
		/* Flash program setup command */
		Write_40_to_base();
		
		/* Just in case src is not aligned... */
		src1[0] = src[0];
		src1[1] = src[1];

		/* Write the value */
		Fwrite(dest,src1);

		/* Wait for write to complete by polling RSR... */
		Write_70_to_base();
		while(1) {
			ushort rsr;

			rsr = Read_0000();
			if (!(rsr & WSMS))	/* Wait till ready */
				continue;
			if (rsr & PSS) 		/* Should not be suspended */
				err = 1;
			if (rsr & PS) 		/* Should not have error */
				err = 1;
			break;
		}
		Write_50_to_base();		/* Clear status register */
		Write_FF_to_base();		/* Go to read-array mode */
		if (err)
			return(-1);
		dest++; 
		src++;
	}
	
	/* Wait till flash is readable, then reset: */
	while(1) {
		if (Is_Equal((dest-1),src1))
			break;
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
EndFlashewrite16()
{
}

/* Flashtype():
   Use the ReadCOnfiguration command (90H) to read manufacturer code (0000)
   and device id code (0001).
*/

int
Flashtype16(struct flashinfo *fdev)
{
	ftype	val;
	ushort	man, dev;
	ulong	id;

	val = Read_0000();

	/* Issue the read configuration command: */
	Write_90_to_base();

	man = (ushort)Read_0000();	/* manufacturer ID */
	dev = (ushort)Read_0001();	/* device ID */
	id = man;
	id <<= 16;
	id |= dev;

	fdev->id = id;

	/* Issue the read array command: */
	Write_FF_to_base();
	return((int)(fdev->id));
}

/* EndFlashtype():
	Function place holder to determine the "end" of the
	Flashtype() function.
*/
void
EndFlashtype16()
{
}
#endif
