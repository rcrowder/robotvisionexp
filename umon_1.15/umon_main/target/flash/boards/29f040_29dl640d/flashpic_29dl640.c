/* flashpic_29dl640.c:
 * These functions may be relocated to ram, so be careful! 
 */
#include "config.h"
#if INCLUDE_FLASH
#include "cpu.h"
#include "flash_29dl640.h"
#include "flash.h"
#include "stddefs.h"


/* Flasherase():
   Based on the 'snum' value, erase the appropriate sector(s).
   Return 0 if success, else -1.
*/
int
Flasherase29dl640(struct flashinfo *fdev,int snum)
{
	ftype	val;
	volatile ulong	add, *lp, *lp1;
	int		ret, sector;

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
			/* If the space within the sector is all FF, then it
			 * doesn't need to be erased, so don't...
			 */
			lp = (ulong *)fdev->sectors[sector].begin; 
			lp1 = (ulong *)((char *)lp + fdev->sectors[sector].size - 1); 
			while(lp <= lp1) {
				if (*lp++ != 0xffffffff) {		/* Erased? */
					/* Issue the sector erase command sequence: */
					Write_aaaa_to_555();
					Write_5555_to_2aa();
					Write_8080_to_555();
					Write_aaaa_to_555();
					Write_5555_to_2aa();
					Write_3030_to_(add);
	
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
		}
		add += fdev->sectors[sector].size;
	}

	/* If the erase failed for some reason, then issue the read/reset */
	/* command sequence prior to returning... */
	if (ret == -1) {
		Write_f0f0_to_base();
		val = Read_555();
	}
	return(ret);
}

/* EndFlasherase():
   Function place holder to determine the "end" of the
   sectorerase() function.
*/
void
EndFlasherase29dl640()
{}


/* Flashwrite():
   Return 0 if successful, else -1.
   Note: this assumes that source & destination properly aligned
   based on the width of the flash bank.
*/
int
Flashwrite29dl640(struct flashinfo *fdev,ftype *dest,ftype *src,long bytecnt)
{
	int	i, ret;
	ftype	val;

	/* Each pass through this loop writes 'fdev->width' bytes... */
	ret = 0;
	for (i=0;i<bytecnt;i+=fdev->width) {

		/* Flash write command */
		Write_aaaa_to_555();
		Write_5555_to_2aa();
		Write_a0a0_to_555();

		/* Write the value */
		Fwrite(dest,src);

		/* Wait for write to complete or timeout. */
		while(1) {
			if (Is_Equal(dest,src)) {
				if (Is_Equal(dest,src))
					break;
			}
			/* Check D5 for timeout... */
			if (D5_Timeout(dest)) {
				if (Is_Not_Equal(dest,src))
					ret = -1;
				goto done;
			}
		}
		dest++; src++;
	}

done:
	/* Read/reset command: */
	Write_f0f0_to_base();
	val = Read_555();
	return(ret);
}

/* EndFlashwrite():
	Function place holder to determine the "end" of the
	Flashwrite() function.
*/
void
EndFlashwrite29dl640()
{}

/* Ewrite():
   Erase all sectors that are part of the address space to be written,
   then write the data to that address space.  This is basically a
   concatenation of flasherase and flashwrite done in one step.  This is
   necessary primarily for re-writing the bootcode; because after the boot
   code is erased, there is nowhere to return so the re-write must be done
   while executing out of ram also.
*/

int
Flashewrite29dl640(struct flashinfo *fdev,ftype *dest,ftype *src,int bytecnt)
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
		Write_aaaa_to_555();
		Write_5555_to_2aa();
		Write_8080_to_555();
		Write_aaaa_to_555();
		Write_5555_to_2aa();
		Write_3030_to_(add);
	
		/* Wait for sector erase to complete or timeout.. */
		/* DQ7 polling: wait for D7 to be 1. */
		/* DQ6 toggling: wait for D6 to not toggle. */
		/* DQ5 timeout: if DQ7 is 0, and DQ5 = 1, timeout. */
		while(1) {
			if (Is_ff(add)) {
				if (Is_ff(add))
					break;
			}
			/* Check D5 for timeout... */
			/* In this case, there is nothing to return to */
			/* because the flash was just erased, so just break.*/
			if (D5_Timeout(add)) {
				goto quit;
			}
		}
		add += fdev->sectors[i].size;
	}

	/* Read/reset command: */
	Write_f0f0_to_base();
	val = Read_555();

	for(i=0;i<bytecnt;i+=fdev->width) {
		/* Write command: */
		Write_aaaa_to_555();
		Write_5555_to_2aa();
		Write_a0a0_to_555();
		Fwrite(dest,src);

		while(1) {
			if (Is_Equal(dest,src)) {
				if (Is_Equal(dest,src))
					break;
			}
			/* Check D5 for timeout... */
			if (D5_Timeout(dest)) {
				if (Is_Not_Equal(dest,src))
					goto quit;
				continue;
			}
		}

		dest++; src++;
	}

quit:
	/* Issue the read/reset command sequence: */
	Write_f0f0_to_base();
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
EndFlashewrite29dl640()
{}


/* Flashtype():
   Use the AUTOSELECT command sequence to determine the type of device.
   Note: there is one additional step that I found necessary to keep
   SGS29040 device happy... For some reason after issuing the read/reset
   command and returning (to code that actually executes out of the FLASH
   device) I was consistently getting an illegal opcode exception at
   the return location in the flash.  It appears that the SGS part needs
   a bit of time after the read/reset to be able to fetch an instruction.
   Reading a value in the flash (stored in val) prior to issuing the
   command sequence, then waiting for that read to be the same after 
   issuing the read/reset, assures the algorithm of not returning unless
   the flash device is readable.  Note that I found this ONLY to be necessary
   for the signature read command of SGS flash.
*/

int
Flashtype29dl640(struct flashinfo *fdev)
{
	ushort	man, dev;

	/* Issue the autoselect command sequence: */
	Write_aaaa_to_555();
	Write_5555_to_2aa();
	Write_9090_to_555();
	
	man = (ushort)Read_0000();	/* manufacturer ID */
	dev = (ushort)Read_0002();	/* device ID */
	man &= 0xff;
	dev &= 0xff;
	man <<=8;
	dev |= man;

	fdev->id = (ushort)dev;

	/* Issue the read/reset command sequence: */
	Write_f0f0_to_base();

	return((int)(fdev->id));
}

/* EndFlashtype():
	Function place holder to determine the "end" of the
	Flashtype() function.
*/
void
EndFlashtype29dl640()
{}
#endif
