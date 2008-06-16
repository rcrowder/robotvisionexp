/* flashpic.c:
   This file contains the flash-support code that is relocated to
   RAM prior to execution.
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

/* Flasherase():
   Based on the 'snum' value, erase the appropriate sector(s).
   Return 0 if success, else -1.
*/
int
FLASHERASE(struct flashinfo *fdev,int snum)
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
		val = Read_555();
	}
	return(ret);
}

/* EndFlasherase():
   Function place holder to determine the "end" of the
   sectorerase() function.
*/
void
ENDFLASHERASE()
{
}

/* Flashwrite():
   Return 0 if successful, else -1.
*/
int
FLASHWRITE(struct flashinfo *fdev,uchar *dest,uchar *src,long bytecnt)
{
	ftype	val;
	long	cnt;
	int		i, ret, offby;
	uchar 	*src1, *tdest, buf[4];

	/* If destination address is not properly aligned, then build a fake   */
	/* source buffer based on the current value in dest[-1] and src[0].    */
	/* Then call this function to do that 4-byte operation.  Once that     */
	/* completes, simply increment dest and src by 1 and continue in this  */
	/* context. */
	offby = NotAligned(dest);
	if (offby) {
		tdest = dest - offby;
		for(i=0;i<offby;i++)
			buf[i] = tdest[i];
		for(i=offby;i<4 && bytecnt;i++,bytecnt--)
			buf[i] = *src++;
		while(i<4)
			buf[i] = tdest[i++];
		if (FLASHWRITE(fdev,tdest,buf,4) < 0)
			return(-1);
		dest += (4-offby);
	}

	/* Each pass through this loop writes 'fdev->width' bytes... */
	ret = 0;
	cnt = bytecnt & ~3;
	src1 = (uchar *)&val;

	for (i=0;i<cnt;i+=fdev->width) {

		/* Flash write command */
		Write_aa_to_555();
		Write_55_to_2aa();
		Write_a0_to_555();
		
		/* Just in case src is not aligned... */
		src1[0] = src[0];
		src1[1] = src[1];
		src1[2] = src[2];
		src1[3] = src[3];

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

	/* If bytecount was not mod4... */
	/* If cnt != bytecnt then bytecnt is wasn't mod4, so one more pass must */
	/* be made.  To do this, the additional bytes must be combined with */
	/* the next bytes already stored in flash; then re-written... */
	if (cnt != bytecnt) {
		offby = bytecnt - cnt;
		src1 = (uchar *)&val;
		src1[0] = dest[0];
		src1[1] = dest[1];
		src1[2] = dest[2];
		src1[3] = dest[3];
		
		for(i=0;i<offby;i++)
			src1[i] = src[i];
		
		if (FLASHWRITE(fdev,dest,src1,4) < 0)
			return(-1);
	}

done:
	/* Read/reset command: */
	Write_f0_to_555();
	val = Read_555();
	return(ret);
}

/* EndFlashwrite():
	Function place holder to determine the "end" of the
	Flashwrite() function.
*/
void
ENDFLASHWRITE()
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
FLASHEWRITE(struct flashinfo *fdev,ftype *dest,ftype *src,int bytecnt)
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

		/* inline putchar...
		while((*(ushort *)TXBD0_0 & 0x8000));
		xbuf[0] = (uchar)(0x41+i);
		*(ushort *)TXBD0_0 |= 0x8000;
		*/

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

	/* inline putchar...
	while((*(ushort *)TXBD0_0 & 0x8000));
	xbuf[0] = (uchar)'!';
	*(ushort *)TXBD0_0 |= 0x8000;
	*/

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
	Function place holder to determine the "end" of the
	FlashEraseAndWrite() function.
*/
void
ENDFLASHEWRITE()
{
}


/* Flashtype():
   Use the AUTOSELECT command sequence to determine the type of device.
*/

int
FLASHTYPE(struct flashinfo *fdev)
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
ENDFLASHTYPE()
{
}
#endif
