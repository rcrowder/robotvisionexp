/* flashpic.c:
   This file contains the flash-support code that is relocated to
   RAM prior to execution.  This file supports all bus widths, dependent
   on the definition of WIDTH8, WIDTH16, or WIDTH32 on the command line.
   Refer to flash.h for definition of the width-dependent macros.
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
#include "endian.h"

extern struct flashinfo	Fdev;
extern int FlashProtectWindow;

/* Flasherase():
   Based on the 'snum' value, erase the appropriate sector(s).
   Return 0 if success, else -1.
*/
int
Flasherase16(struct flashinfo *fdev,int snum)
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
				/* Issue the sector erase command sequence: */
				Write_aa_to_555();
				Write_55_to_2aa();
				Write_80_to_555();
				Write_aa_to_555();
				Write_55_to_2aa();
				Write_30_to_(add);

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

	/* If the erase failed for some reason, then issue the read/reset */
	/* command sequence prior to returning... */
	if (ret == -1) {
		Write_f0_to_555();
		val = Read_5555();
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

		/* Flash write command */
		Write_aa_to_555();
		Write_55_to_2aa();
		Write_a0_to_555();
		
		/* Just in case src is not aligned... */
		src1[0] = src[0];
		src1[1] = src[1];

		/* Write the value */
		Fwrite(dest,src1);

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

	/* If bytecount was odd... */
	/* If cnt != bytecnt then bytecnt is odd, so one more byte must be */
	/* written to flash.  To do this, the one byte must be combined with */
	/* the next byte that is already stored in flash; then re-written... */
	if (cnt != bytecnt) {
		val = (ftype)*dest | ((ftype)(*src) << 8);
		src = (uchar *)&val;
		bytecnt = cnt = 1;
		val = ecs(val);
		goto onemore;
	}

done:
	/* Read/reset command: */
	Write_f0_to_555();
	val = Read_5555();
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
	int	i;
	ulong	add;
	void	(*reset)();
	ftype	val, *src1, *dest1;

	add = (ulong)(fdev->base);
	src1 = src;
	dest1 = dest;

	/* For each sector, if it overlaps any of the destination space */
	/* then erase that sector. */
	for (i=0;i<fdev->sectorcnt;i++) {
		if ((((uchar *)dest) > (fdev->sectors[i].end)) ||
		    (((uchar *)dest+bytecnt-1) < (fdev->sectors[i].begin))) {
			add += fdev->sectors[i].size;
			continue;
		}

		/* Sector erase command: */
		Write_aa_to_555();
		Write_55_to_2aa();
		Write_80_to_555();
		Write_aa_to_555();
		Write_55_to_2aa();
		Write_30_to_(add);

		/* Wait for sector erase to complete or timeout.. */
		/* DQ7 polling: wait for D7 to be 1. */
		/* DQ6 toggling: wait for D6 to not toggle. */
		/* DQ5 timeout: if DQ7 is 0, and DQ5 = 1, timeout. */
		while(1) {
			if (Is_ff(add)) {
				if (Is_ff(add))
					break;
			}
#if 0
			/* Check D5 for timeout... */
			/* In this case, there is nothing to return to */
			/* because the flash was just erased, so just break.*/
			if (D5_Timeout(add)) {
				reset = RESETFUNC();
				reset();
			}
#endif
		}
		add += fdev->sectors[i].size;
	}

	/* Read/reset command: */
	Write_f0_to_555();
	val = Read_5555();

	for(i=0;i<bytecnt;i+=fdev->width) {
		/* Write command: */
		Write_aa_to_555();
		Write_55_to_2aa();
		Write_a0_to_555();
		Fwrite(dest,src);

		while(1) {
			if (Is_Equal(dest,src)) {
				if (Is_Equal(dest,src))
					break;
			}
#if 0
			/* Check D5 for timeout... */
			if (D5_Timeout(dest)) {
				if (Is_Not_Equal(dest,src))
					return(-1);
				break;
			}
#endif
		}

		dest++; 
		src++;
	}

	/* Issue the read/reset command sequence: */
	Write_f0_to_555();
	val = Read_5555();

	/* Wait till flash is readable, then reset: */
	while(1) {
		if (Is_Equal(dest1,src1))
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
   Use the AUTOSELECT command sequence to determine the type of device.
*/

int
Flashtype16(struct flashinfo *fdev)
{
	ftype	val;
	ushort	man, dev;
	ulong	id;

	val = Read_0000();

	/* Issue the autoselect command sequence: */
	Write_aa_to_555();
	Write_55_to_2aa();
	Write_90_to_555();

	man = (ushort)Read_0000();	/* manufacturer ID */
	dev = (ushort)Read_0001();	/* device ID */
	id = man;
	id <<= 16;
	id |= dev;

	fdev->id = id;

	/* Issue the read/reset command sequence: */
	Write_f0_to_555();
	val = Read_0000();
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
