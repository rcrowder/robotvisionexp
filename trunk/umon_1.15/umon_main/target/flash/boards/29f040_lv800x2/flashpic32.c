/* flashpic32.c:
 * This file contains the 32-bit flash-support code that is relocated to
 * RAM prior to execution.
 */
#include "config.h"
#if INCLUDE_FLASH
#include "stddefs.h"
#include "cpu.h"
#include "flashdev.h"
#include "flash.h"
#include "flash32.h"

extern struct flashinfo	Fdev;
extern int FlashProtectWindow;

/* Flasherase():
 * Based on the 'snum' value, erase the appropriate sector(s).
 * Return 0 if success, else -1.
 */
int
Flasherase29LV800B_32(fdev,snum)
struct	flashinfo *fdev;
int	snum;
{
    ftype	val;
    ulong	add;
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
		    int				noterased;
		    register ulong	*lp, *lp1;
	
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
		
				/* Wait for sector erase to complete or timeout..
				 * DQ7 polling: wait for D7 to be 1.
				 * DQ5 timeout: if DQ7 is 0, and DQ5 == 1, timeout.
				 */
				for (;;) {
				    val = *(ftype *)add;
				    if ((val & 0x00800080) == 0x00800080) {
						break;
				    }
	
				    /* Check for D5 timeout in upper flash if it's not done */
				    if (((val & 0x00800000) != 0x00800000) &&
						((val & 0x00200000) != 0)) {
		
						/* Check D7 again since D7 and D5 are asynchronous */
						val = *(ftype *)add;
						if ((val & 0x00800000) != 0x00800000) {
						    ret = -1;
						    break;
						}
				    }
	
				    /* Check for D5 timeout in lower flash if it's not done */
				    if (((val & 0x00000080) != 0x00000080) &&
						((val & 0x00000020) != 0)) {
		
						/* Check D7 again since D7 and D5 are asynchronous */
						val = *(ftype *)add;
						if ((val & 0x00000080) != 0x00000080) {
						    ret = -1;
						    break;
						}
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
		Write_f0_to_555();
		val = Read_555();
    }
    return(ret);
}

/* EndFlasherase():
 * Function place holder to determine the "end" of the
 * sectorerase() function.
 */
void
EndFlasherase29LV800B_32()
{
}

union alignedsrc {
	ftype	val;
	uchar	buf[sizeof(ftype)];
};

/* Flashwrite():
 * Return 0 if successful, else -1.
 */
int
Flashwrite29LV800B_32(fdev,dest,src,cnt)
struct	flashinfo *fdev;
uchar	*src;
register uchar *dest;
long	cnt;
{
    union alignedsrc	asrc;
    register ftype		tmp, d7test;
    unsigned long	prefetch;
    int			bidx;
    int			timeout = 0;

    prefetch = (unsigned long) dest & 3;
    dest = (uchar *) ((unsigned long) dest & ~3ul);

	/* Unlock Bypass Command: */
	Write_aa_to_555();
	Write_55_to_2aa();
	Write_20_to_555();

    while (timeout == 0 && cnt > 0) {
		/* First fill the aligned source buffer from the source.  If the
		 * destination is not 4-byte aligned, also grab bytes from the
		 * flash prior to the destination to allow all 32 bits to be
		 * written at once.  If the source length is too short to fill all
		 * 4 bytes, grab bytes from the flash following the destination to
		 * allow all 32 bits to be written at once.
		 */
		bidx = 0;
		while (prefetch) {
		    asrc.buf[bidx] = dest[bidx];
		    bidx++;
		    prefetch--;
		}
		while (bidx < 4) {
		    if (cnt > 0) {
				asrc.buf[bidx] = *src++;
				cnt--;
		    }
			else {
				asrc.buf[bidx] = dest[bidx];
		    }
		    bidx++;
		}
	
		/* Now asrc.val has the 4-bytes to write to the flash. */
	
		/* Bypass Program command */
		Write_a0_to_555();
	
		/* Write the value */
		Fwrite(dest, &asrc.val);
		d7test = asrc.val & 0x00800080;
	
		/* Wait for flash write to complete (each device might complete
		 * at a different time).
		 */
		for (;;) {
		    tmp = *(ftype *)dest;
		    if ((tmp & 0x00800080) == d7test) {
				break;
		    }
	
		    /* Check for D5 timeout in upper flash if it's not done */
		    if ((tmp & 0x00800000) != (d7test & 0x00800000) &&
				(tmp & 0x00200000) != 0) {
	
				/* Check D7 once more since D7 and D5 are asynchronous */
				tmp = *(ftype *)dest;
				if ((tmp & 0x00800000) != (d7test & 0x00800000)) {
				    timeout = -1;
				    break;
				}
		    }
	
		    /* Check for D5 timeout in lower flash if it's not done */
		    if ((tmp & 0x00000080) != (d7test & 0x00000080) &&
				(tmp & 0x00000020) != 0) {
	
				/* Check D7 once more since D7 and D5 are asynchronous */
				tmp = *(ftype *)dest;
				if ((tmp & 0x00000080) != (d7test & 0x00000080)) {
				    timeout = -1;
				    break;
				}
		    }
		}

		/* Finished that 4-byte write, go on to the next (per cnt) */
		dest += 4;
    }	/* End while cnt > 0 */

    /* Unlock Bypass Reset command: */
    Write_90_to_555();
    Write_00_to_555();
    tmp = Read_555();
    return(timeout);
}

/* EndFlashwrite():
 *	Function place holder to determine the "end" of the
 *	Flashwrite() function.
 */
void
EndFlashwrite29LV800B_32()
{
}

/* Ewrite():
 * Erase all sectors that are part of the address space to be written,
 * then write the data to that address space.  This is basically a
 * concatenation of flasherase and flashwrite done in one step.  This is
 * necessary primarily for re-writing the bootcode; because after the boot
 * code is erased, there is nowhere to return so the re-write must be done
 * while executing out of ram also.
 *
 * Note that this routine is restricted to writing full words only.
*/

int
Flashewrite29LV800B_32(fdev,dest,src,bytecnt)
struct	flashinfo *fdev;
ftype	*src, *dest;
int	bytecnt;
{
    int		i;
    ulong	add;
    void	(*reset)();
    ftype	val, d7test, *src1, *dest1;

    if (bytecnt <= 0) {		/* sanity check */
		return -1;
    }
    add = (ulong)(fdev->base);
    src1 = src;
    dest1 = dest;

	/* For each sector, if it overlaps any of the destination space
	 * then erase that sector.
	 */
    for (i=0;i<fdev->sectorcnt;i++) {
		if ((((uchar *)dest) > (fdev->sectors[i].end)) ||
		    (((uchar *)dest+(bytecnt-1)) < (fdev->sectors[i].begin))) {
	
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
		 * DQ5 timeout: if DQ7 is 0, and DQ5 = 1, timeout.
		 */
		for (;;) {
		    val = *(ftype *)add;
		    if ((val & 0x00800080) == 0x00800080) {
				break;
		    }
	
			/* Check for D5 timeout in upper flash if it's not done
			 * In this case, there is nothing to return to
			 * because the flash was just erased, so just break.
			 */
		    if (((val & 0x00800000) != 0x00800000) &&
				((val & 0x00200000) != 0)) {
	
		    	/* Check D7 once more since D7 and D5 are asynchronous */
				val = *(ftype *)add;
				if ((val & 0x00800000) != 0x00800000) {
				    reset = RESETFUNC();
				    reset();
				}
		    }
	
		    /* Check for D5 timeout in lower flash if it's not done
		     * In this case, there is nothing to return to
		     * because the flash was just erased, so just break.
			 */
		    if (((val & 0x00000080) != 0x00000080) &&
				((val & 0x00000020) != 0)) {

				/* Check D7 once more since D7 and D5 are asynchronous */
				val = *(ftype *)add;
				if ((val & 0x00000080) != 0x00000080) {
				    reset = RESETFUNC();
				    reset();
				}
			}
	
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

		d7test = *src & 0x00800080;

		/* Wait for flash write to complete (each device might complete
		 * at a different time).
		 */
		for (;;) {
		    val = *(ftype *)dest;
		    if ((val & 0x00800080) == d7test) {
				break;
		    }

			/* Check for D5 timeout in upper flash if it's not done
			 * In this case, there is nothing to return to
			 * because the flash was just erased, so just break.
			 */
		    if ((val & 0x00800000) != (d7test & 0x00800000)
				&& (val & 0x00200000) != 0) {
	
				/* Check D7 once more since D7 and D5 are asynchronous */
				val = *(ftype *)dest;
				if ((val & 0x00800000) != (d7test & 0x00800000)) {
				    reset = RESETFUNC();
				    reset();
				}
	    	}

		    /* Check for D5 timeout in lower flash if it's not done
			 * In this case, there is nothing to return to
			 * because the flash was just erased, so just break.
			 */
		    if ((val & 0x00000080) != (d7test & 0x00000080) &&
				(val & 0x00000020) != 0) {
	
				/* Check D7 once more since D7 and D5 are asynchronous */
				val = *(ftype *)dest;
				if ((val & 0x00000080) != (d7test & 0x00000080)) {
				    reset = RESETFUNC();
				    reset();
				}
			}
		}
		dest++; 
		src++;
    }

    /* Issue the read/reset command sequence: */
    Write_f0_to_555();
    val = Read_555();

    /* Wait till flash is readable, or timeout: */
    for(i=0;i<FLASH_TIMEOUT;i++) {
		if (Is_Equal(dest1,src1)) {
		    break;
		}
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
EndFlashewrite29LV800B_32()
{
}

/* Flashtype():
 *	Use the AUTOSELECT command sequence to determine the type of device.
 *	This code is checking a device type for either a set of 29LV800BB
 *	or 29LV800BT devices.  It starts by assuming that the devices (2
 *	16-bit devices in parallel) are the same (gang is set to GANG).
 *	If after reading the device id, it is determined that the devices are
 *	not the same, then we return the long value containing the parallel
 *	device ids.  The goal is to catch the case where manufacturing
 *	folks may have installed a 29LV800BB and 29LV800BT in one bank.
*/

int
Flashtype29LV800B_32(struct flashinfo *fdev)
{
    ftype	val;
    ulong	man, dev, gang;
    ulong	id;

    val = Read_000();

    /* Issue the autoselect command sequence: */
    Write_aa_to_555();
    Write_55_to_2aa();
    Write_90_to_555();

    gang = GANG;		/* Assume two identical parts */
    val = Read_000();
    man = val & 0xff;	/* manufacturer ID lower device */
    if (man != (val & 0x00ff0000) >> 16)
		gang = 0;

	/* Read device id.  If upper and lower 16-bit id values do not match,
	 * return the value read now.  This allows the calling function to
	 * analyze the return value and attempt to draw a conclusion about the
	 * mismatched set of devices.
	 */
    val = Read_001();
    dev = val & 0xffff;	/* device ID lower device */
    if (dev != (val & 0xffff0000) >> 16) {
    	fdev->id = val;
		return(val);
    }
    id = gang;
    id |= man << 16;
    id |= dev;

    fdev->id = id;

    /* Issue the read/reset command sequence: */
    Write_f0_to_555();
    val = Read_000();
    return((int)(fdev->id));
}

/* EndFlashtype():
 *	Function place holder to determine the "end" of the
 *	Flashtype() function.
*/
void
EndFlashtype29LV800B_32(void)
{
}

#endif
