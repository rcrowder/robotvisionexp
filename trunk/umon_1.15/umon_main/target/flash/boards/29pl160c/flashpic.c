/* flashpic.c:
 * This file contains the flash-support code that is relocated to
 * RAM prior to execution.
 */
#include "config.h"
#if INCLUDE_FLASH

#include "stddefs.h"
#include "cpu.h"
#include "flashdev.h"
#include "flash.h"

/* Flasherase():
 * Based on the 'snum' value, erase the appropriate sector(s).
 * Return 0 if success, else -1.
 */
int
Flasherase16(struct flashinfo *fdev,int snum)
{
	ftype	val;
	ulong	add;
	int	ret, sector;

	ret = 0;
	add = (ulong)(fdev->base);

*(int *)0xb8000 = snum;
*(int *)0xb8004 = 0xdeadbeef;

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
*(int *)0xb8008 = 0;
			if (noterased) {
*(int *)0xb8008 = 1;
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

	/* If the erase failed for some reason, then issue the read/reset
	 * command sequence prior to returning...
	 */
	if (ret == -1) {
*(int *)0xb800c = 0x2;
		Write_f0_to_555();
		val = Read_555();
	}
*(int *)0xb800c = 0x3;
	return(ret);
}

/* EndFlasherase():
 * Function place holder to determine the "end" of the
 * sectorerase() function.
 */
void
EndFlasherase16()
{
}

/* Flashwrite():
 * Return 0 if successful, else -1.
 */
int
Flashwrite16(struct flashinfo *fdev,uchar *dest,uchar *src,long bytecnt)
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
	if (NotAligned(dest)) {
		dest--;
		
		src1[0] = *dest;
		src1[1] = *src;

		/* Flash write command */
		Write_aa_to_555();
		Write_55_to_2aa();
		Write_a0_to_555();
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

		dest += 2;
		src++;
		bytecnt--;
	}

	/* Each pass through this loop writes 'fdev->width' bytes...
	 */

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

	/* Similar to the front end of this function, if the byte count is not
	 * even, then we have one byte left to write, so we need to write a 
	 * 16-bit value by writing the last byte, plus whatever is already in
	 * the next flash location.
	 */
	if (cnt != bytecnt) {
		src1[0] = *src;
		src1[1] = dest[1];

		/* Flash write command */
		Write_aa_to_555();
		Write_55_to_2aa();
		Write_a0_to_555();
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
	}

done:
	/* Read/reset command: */
	Write_f0_to_555();
	return(ret);
}

/* EndFlashwrite():
 *	Function place holder to determine the "end" of the
 *	Flashwrite() function.
 */
void
EndFlashwrite16()
{
}

/* Ewrite():
 * Erase all sectors that are part of the address space to be written,
 * then write the data to that address space.  This is basically a
 * concatenation of flasherase and flashwrite done in one step.  This is
 * necessary primarily for re-writing the bootcode; because after the boot
 * code is erased, there is nowhere to return so the re-write must be done
 * while executing out of ram also.
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

	/* For each sector, if it overlaps any of the destination space
	 * then erase that sector.
	 */
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
#if 0
			/* Check D5 for timeout...
			 * In this case, there is nothing to return to
			 * because the flash was just erased, so just break.
			 */
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
	val = Read_555();

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
	val = Read_555();

	/* Wait till flash is readable, or timeout: */
	for(i=0;i<FLASH_TIMEOUT;i++) {
		if (Is_Equal(dest1,src1))
			break;
	}

	/* Now that the re-programming of flash is complete, reset: */
	reset = RESETFUNC();
	reset();

	return(0);	/* won't get here */
}

/* EndFlashewrite():
 *	Function place holder to determine the "end" of the
 *	FlashEraseAndWrite() function.
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
