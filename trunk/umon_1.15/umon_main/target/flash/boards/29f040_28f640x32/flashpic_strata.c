/* flashpic_strata.c:
 * This file contains the portion of the flash driver code that can be
 * relocated (pic =  position independent code) to RAM space so that it
 * is not executing out of the same flash device that it is operating on.
 *
 * This code is written for 2 Strata devices in 16-bit mode in parallel.
 * This forms one 32-bit wide flash bank.
 */
#include "config.h"
#if INCLUDE_FLASH
#include "genlib.h"
#include "stddefs.h"
#include "flashdev.h"
#include "flash.h"
#include "flash_strata.h"

#define USE_WRITE_BUFFER 1

/* When this is used in an environment where it is NOT relocated, 
 * then it is safe to turn on trace and call printf for debugging.
 * This can be changed to a #define initialized to zero to save
 * space if printf is not going to be used.
 */
int STRATA_TRACE;

/* Flasherase():
 * Based on the 'snum' value, erase the appropriate sector(s).
 * Write the "Erase Setup" and "Erase Confirm" commands...
 * Return 0 if success, else -1.
 */
int
FlasheraseStrata_32(struct flashinfo *fdev,int snum)
{
	ftype			stat;
	volatile ulong	add, *lp, *lp1;
	volatile int	ret, sector, outofrange;

	ret = 0;
	outofrange = 1;
	add = (ulong)(fdev->base);

	if (STRATA_TRACE)
		printf("erase(%d)\n",snum);


	/* Start operation off by clearing the status register. */
	Write_50_to_base();	

	/* Erase the request sector(s): */
	for (sector=0;sector<fdev->sectorcnt;sector++) {
		if ((snum == ALL_SECTORS) || (snum == sector)) {

			/* Skip sector if it is protected.
			 */
			if ((!FlashProtectWindow) &&
			    (fdev->sectors[sector].protected)) {
				add += fdev->sectors[sector].size;
				continue;
			}

			/* As long as we enter this block of code once, we know
			 * that the sector number requested was within the range
			 * of the specified bank.
			 */
			outofrange = 0;

			/* If the space within the sector is all FF, then it
			 * doesn't need to be erased, so don't...
			 */
			lp = (ulong *)fdev->sectors[sector].begin; 
			lp1 = (ulong *)((char *)lp + fdev->sectors[sector].size - 1); 
			while(lp <= lp1) {
				if (*lp++ != 0xffffffff) {		/* Erased? */
					Write_20_to_base();			/* setup */
					Write_d0_to_(add);			/* confirm */
					WAIT_FOR_WSMS_READY();		/* Wait for WSMS ready */
					stat = Read_0000();
					if (stat & ECLBS) {			/* Check for error */
						Write_50_to_base();		/* Clear status register */
						printf("Eerr 0x%lx\n",(ulong)stat);
						Write_ff_to_base();		/* Go to read-array mode */
						return(-1);
					}
					Write_ff_to_base();			/* Go to read-array mode */
					WAIT_FOR_FF(add);			/* Wait for read == 0xff */
					break;
				}
			}
			
			if (ret == -1)
				break;
		}
		add += fdev->sectors[sector].size;
	}
	if (outofrange)
		ret = -1;
	return(ret);
}

/* EndFlasherase():
 *	Function place holder to determine the "end" of the
 *	sectorerase() function.
 */
void
EndFlasheraseStrata_32()
{
}


/* Flashwrite():
 * This device has a 32-byte write buffer.  The only gotcha for this is 
 * that the destination address into the flash must be on a 32-byte boundary,
 * so we an only use the buffer when the destination address has the lower 5
 * bits clear.  To do this, we do however many "individual" writes are
 * necessary to get the destination address 32-byte aligned; then, if there
 * are at least 32 bytes left, we use the write-buffer mechanism for each
 * remaining block of 32 bytes.
 */
int
FlashwriteStrata_32(struct flashinfo *fdev,uchar *dest,uchar *src,long bytecnt)
{
	ftype			stat;
	volatile long	cnt, buf4[1];
	volatile int	i, tot, ret, delta;
	volatile uchar	*bp4;
#if USE_WRITE_BUFFER
	volatile uchar	*block, *bp64;
	volatile long	buf64[16];
#endif

	/* Start operation off by clearing the status register. */
	Write_50_to_base();	

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
			*bp4++ = *src++;
			bytecnt--;
			dest++;
		}
		if (STRATA_TRACE)
			printf("pre-align:  0x%lx = %08lx\n",(ulong)tmpdest,buf4[0]);

		if (FlashwriteStrata_32(fdev,tmpdest,(uchar *)buf4,4) == -1)
			return(-1);
	}

	/* This device supports a write-buffer mode.  The buffer is 32 bytes
	 * (16 words) and greatly speeds up the programming process.  This
	 * This function starts up with the word-at-a-time mode and uses it
	 * until the destination address is mod32.  Then for each successive
	 * 32-byte block, the write buffer is used. When there is less than 32
	 * bytes left, the algorithm falls back to the one-word-at-a-time mode
	 * to finish up.
	 */
	ret = 0;
	cnt = bytecnt & ~3;

	for (tot=0;tot<cnt;tot+=4) {
#if USE_WRITE_BUFFER
		if (((ulong)dest & 0x3f) == 0)	/* Break out when 6 LSBs are zero */
			break;
#endif

		bp4 = (uchar *)buf4;
		*bp4++ = *src++;				/* Just in case src is not aligned... */
		*bp4++ = *src++;				
		*bp4++ = *src++;				
		*bp4++ = *src++;				

		if (STRATA_TRACE)
			printf("fwrite1:    0x%lx = %08lx\n",(ulong)dest,buf4[0]);

		Write_40_to_(dest);				/* Flash program setup command */
		Fwrite(dest,buf4);				/* Write the value */
		WAIT_FOR_WSMS_READY();			/* Wait for WSMS ready */
		stat = Read_0000();
		if (stat & PSLBS) {				/* Check for error */
			Write_50_to_base();			/* Clear status register */
			Write_ff_to_base();			/* Go to read-array mode */
			printf("Werr1 0x%lx\n",(ulong)stat);
			return(-1);
		}
		do {
			Write_ff_to_base();				/* Go to read-array mode */
		} WAIT_FOR_WRITE(dest,buf4);		
		dest += 4; 
	}

#if USE_WRITE_BUFFER
	/* If tot is less than cnt, then the upper loop reached a point where
	 * the destination address had the 5 LSBs low.  This means that we can
	 * use the write-buffer for all remaining 32-byte blocks...  Note that
	 * each 32-byte block is actually 64 because we have two devices in
	 * parallel.
	 */
	if ((cnt-tot) >= 64) {
		while((cnt-tot) >= 64) {
	
			Write_ff_to_base();			/* Go to read-array mode */

			bp64 = (uchar *)buf64;		/* Copy next buffer's worth of data */
			for(i=0;i<64;i++)			/* into local buffer just in case */
				*bp64++ = *src++;		/* the source is this flash device. */
			bp64 = (uchar *)buf64;
	
			do {
				Write_e8_to_(dest);	
			} while (WBUF_NOT_AVAIL(dest));

			block = dest;
			Write_0f_to_(dest);	
			for(i=0;i<16;i++) {
				bp4 = (uchar *)buf4;
				*bp4++ = *bp64++;		/* Just in case src is not aligned... */
				*bp4++ = *bp64++;				
				*bp4++ = *bp64++;				
				*bp4++ = *bp64++;				
				if (STRATA_TRACE)
					printf("fwrite2:    0x%lx = %08lx\n",(ulong)dest,buf4[0]);

				Fwrite(dest,buf4);		/* Write the value */
				dest += 4; 
			}
			Write_d0_to_(block);		/* Write-confirm command */
			tot += 64;
			Write_70_to_base();			/* Read Status Register */
			WAIT_FOR_WSMS_READY();		/* Wait for idle state machine */
		}
		stat = Read_0000();				/* Check for error */
		if (stat & PSLBS) {		
			Write_50_to_base();			/* Clear status register */
			Write_ff_to_base();			/* Go to read-array mode */
			printf("Werr2 0x%lx\n",stat);
			return(-1);
		}
		Write_ff_to_base();				/* Go to read-array mode */
	}
#endif

onemore:
	for (;tot<cnt;tot+=4) {
		bp4 = (uchar *)buf4;
		*bp4++ = *src++;				/* Just in case src is not aligned... */
		*bp4++ = *src++;
		*bp4++ = *src++;
		*bp4++ = *src++;

		if (STRATA_TRACE)
			printf("fwrite3:    0x%lx = %08lx\n", (ulong)dest,buf4[0]);

		Write_40_to_(dest);				/* Flash program setup command */
		Fwrite(dest,buf4);				/* Write the value */
		WAIT_FOR_WSMS_READY();			/* Wait for WSMS ready */
		stat = Read_0000();
		if (stat & PSLBS) {				/* Check for error */
			Write_50_to_base();			/* Clear status register */
			Write_ff_to_base();			/* Go to read-array mode */
			printf("Werr3 0x%lx\n",stat);
			return(-1);
		}
		do {
			Write_ff_to_base();				/* Go to read-array mode */
		} WAIT_FOR_WRITE(dest,buf4);		
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
		if (STRATA_TRACE)
			printf("post-align: 0x%lx = %08lx\n",(ulong)dest,buf4[0]);

		tot = cnt-1;
		goto onemore;
	}
	return(ret);
}

/* EndFlashwrite():
 * Function place holder to determine the "end" of the
 * Flashwrite() function.
 */
void
EndFlashwriteStrata_32()
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
FlashewriteStrata_32(struct flashinfo *fdev,ftype *dest,ftype *src,int bytecnt)
{
	return(-1);
}

/* EndFlashewrite():
 * Function place holder to determine the "end" of the
 * FlashEraseAndWrite() function.
 */
void
EndFlashewriteStrata_32()
{
}

/* FlashlockStrata_32():
 */
int
FlashlockStrata_32(struct flashinfo *fdev,int snum,int operation)
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
	Write_ff_to_base();		/* Go to read-array mode */
	return(0);
}

/* EndFlashlock():
 * Function place holder to determine the "end" of the
 * Flashlock() function.
 */
void
EndFlashlockStrata_32()
{
}

/* Flashtype():
 * Use the ReadConfiguration command (90H) to read manufacturer code (0000)
 * and device id code (0001).
 */

int
FlashtypeStrata_32(struct flashinfo *fdev)
{
	ushort	man, dev;
	ulong	id;

	/* Start operation off by clearing the status register. */
	Write_50_to_base();	

	/* Issue the read array command: */
	Write_ff_to_base();

	/* Issue the read configuration command: */
	Write_90_to_base();

	man = (ushort)Read_0000();	/* manufacturer ID */
	dev = (ushort)Read_0001();	/* device ID */
	if (STRATA_TRACE)
		printf("Strata man-id/dev-id: 0x%x 0x%x\n",man,dev);

	id = man;
	id <<= 16;
	id |= dev;

	fdev->id = id;

	/* Issue the read array command: */
	Write_ff_to_base();
	return((int)(fdev->id));
}

/* EndFlashtype():
 * Function place holder to determine the "end" of the
 * Flashtype() function.
 */
void
EndFlashtypeStrata_32()
{
}
#endif
