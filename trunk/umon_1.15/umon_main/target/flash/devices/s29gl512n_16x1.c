/* s29gl512n_16x1.c
 * Device interface for Spansion S29GL512N flash device configured for
 * x16 mode with 1 device in parallel.  
 * The device contains 512 128Kbyte sectors.
 * There are four different memory configurations that this driver
 * handles:
 * 1. A single S29GL512N_16x1 device 
 * 2. Piggybacked S29GL512N_16x1 devices (2 devices).
 * 3. 70GL01GN: 2 S29GL512N_16x1 devices in one package.
 * 4. GL01GP: This is a single device with double the density of the 
 *    S29GL512N_16x1 device.
 */
#include "config.h"

#if INCLUDE_FLASH

#include "stddefs.h"
#include "genlib.h"	
#include "cpu.h"
#include "cpuio.h"
#include "flash.h"			/* Part of monitor common code */
#include "s29gl512n_16x1.h"

#define ftype 					volatile unsigned short
#define Is_ff(add)			   	(*(ftype *)add == 0xffff)
#define Is_not_ff(add)	    	(*(ftype *)add != 0xffff)
#define Is_Equal(p1,p2) 	    (*(ftype *)p1 == *(ftype *)p2)
#define Is_Not_Equal(p1,p2)		(*(ftype *)p1 != *(ftype *)p2)
#define Fwrite(to,frm)  	    (*(ftype *)to = *(ftype *)frm)
#define D5_Timeout(add)	   		((*(ftype *)(add) & 0x0020) == 0x0020)

#define SECTOR_TOTAL			(512*2)
#define SECTOR_SIZE				0x20000
#define SPANSION_29GL512N_ID	0x00012223
#define SPANSION_29GL1024N_ID	0x00012228

#define D6	0x0040
#define D5	0x0020
#define D3	0x0008

#define SECTOR_ERASE(add)		{ \
	*(ftype *)((fdev->base+(0x555<<1))) = 0x00aa; \
	*(ftype *)((fdev->base+(0x2aa<<1))) = 0x0055; \
	*(ftype *)((fdev->base+(0x555<<1))) = 0x0080; \
	*(ftype *)((fdev->base+(0x555<<1))) = 0x00aa; \
	*(ftype *)((fdev->base+(0x2aa<<1))) = 0x0055; \
	*(ftype *)(add) = 0x0030; }
	

#define FLASH_WRITE(dest,src)	{ 	\
	*(ftype *)((fdev->base+(0x555<<1))) = 0x00aa; \
	*(ftype *)((fdev->base+(0x2aa<<1))) = 0x0055; \
	*(ftype *)((fdev->base+(0x555<<1))) = 0x00a0; \
	Fwrite((ftype *)(dest),src); }

#define AUTO_SELECT()			{	\
	*(ftype *)((fdev->base+(0x555<<1))) = 0x00aa; \
	*(ftype *)((fdev->base+(0x2aa<<1))) = 0x0055; \
	*(ftype *)((fdev->base+(0x555<<1))) = 0x0090; }

#define READ_RESET() 			{	\
	ftype val;	\
	*(ftype *)(fdev->base) = 0x00f0; \
	val = *(ftype *)(fdev->base); }

#define WHILE_D6_TOGGLES(a) \
	while ((*(ftype *)(a) & D6) != (*(ftype *)(a) & D6))


/* S29gl512n_16x1_erase():
 * Based on the 'snum' value, erase the appropriate sector(s).
 * Return 0 if success, else -1.
 */
int
S29gl512n_16x1_erase(struct flashinfo *fdev,int snum)
{
	ulong	add;
	int		ret;

	ret = 0;
	add = (ulong)(fdev->sectors[snum].begin);


	SECTOR_ERASE(add);

	/* Wait for sector erase to complete or timeout..
	 * DQ7 polling: wait for D7 to be 1.
	 * DQ6 toggling: wait for D6 to not toggle.
	 * DQ5 timeout: if DQ7 is 0, and DQ5 = 1, timeout.
	 */
	while(1) {
		WATCHDOG_MACRO;
		if (*(ftype *)(add) == 0xffff) {
			if (*(ftype *)(add) == 0xffff) 
				break;
		}
		if (D5_Timeout(add)) {
			if (*(ftype *)(add) != 0xffff) 
				ret = -1;
			break;
		}
	}

	/* If the erase failed for some reason, then issue the read/reset
	 * command sequence prior to returning...
	 */
	if (ret == -1)  
		READ_RESET();

	return(ret);
}

/* EndS29gl512n_16x1_erase():
 * Function place holder to determine the end of the above function.
 */
void
EndS29gl512n_16x1_erase(void)
{
}

/* S29gl512n_16x1_write():
 * Copy specified number of bytes from source to destination.  The destination
 * address is assumed to be flash space.
 */
int
S29gl512n_16x1_write(struct flashinfo *fdev,
	uchar *dest,uchar *src,long bytecnt)
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
			WATCHDOG_MACRO;

			if (*(ftype *)(dest) == *(ushort *)src1) {
				if (*(ftype *)(dest) == *(ushort *)src1)
					break;
			}
			/* Check D5 for timeout... */
			if (D5_Timeout(dest)) {
				if (*(ftype *)(dest) != *(ushort *)src1) {
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
			WATCHDOG_MACRO;

			if (*(ftype *)(dest) == *(ushort *)src1) {
				if (*(ftype *)(dest) == *(ushort *)src1)
					break;
			}
			/* Check D5 for timeout... */
			if (D5_Timeout(dest)) {
				if (*(ftype *)(dest) != *(ushort *)src1) {
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
			WATCHDOG_MACRO;
			if (*(ftype *)(dest) == *(ushort *)src1) {
				if (*(ftype *)(dest) == *(ushort *)src1)
					break;
			}
			/* Check D5 for timeout... */
			if (D5_Timeout(dest)) {
				if (*(ftype *)(dest) != *(ushort *)src1) {
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

/* EndS29gl512n_16x1_write():
 * Function place holder to determine the end of the above function.
 */
void
EndS29gl512n_16x1_write(void)
{}

/* S29gl512n_16x1_ewrite():
 * Erase all sectors that are part of the address space to be written,
 * then write the data to that address space.
 * This function is optimized for speed by using the write buffer and
 * erase delay timer.  It assumes the incoming destination address is
 * aligned on a 32-byte boundary and that the bytecount will be even.
 */
int
S29gl512n_16x1_ewrite(struct flashinfo *fdev,
	uchar *dest,uchar *src,long bytecnt)
{
	int		i;
	ulong	add;


	add = (ulong)(fdev->base);

	/* For each sector, if it overlaps any of the destination space
	 * then erase that sector.
	 */
	for (i=0;i<fdev->sectorcnt;i++) {
		long size;
		long *begin, *end;
		struct sectorinfo *sip;

		sip = &(fdev->sectors[i]);
		begin = (long *)(sip->begin);
		end = (long *)(sip->end);
		size = sip->size;

		/* If this sector is not overlapping the destination, then
		 * don't erase it...
		 */
		if ((((uchar *)dest) > (uchar *)end) ||
		    (((uchar *)dest+bytecnt-1) < (uchar *)begin)) {
			add += size;
			continue;
		}

		/* If this sector is already erased, then don't erase it...
		 */
		while(begin < end) {
			if (*begin != 0xffffffff)
				break;
			begin++;
		}
		if (begin >= end) {
			add += size;
			continue;
		}
		SECTOR_ERASE(add);

		WHILE_D6_TOGGLES(add);

		add += size;
	}

	READ_RESET();

	while(bytecnt > 0) {
		int tot, tot1;
		uchar *sa;

		sa = dest;
		*(ftype *)((fdev->base+(0x555<<1))) = 0x00aa;
		*(ftype *)((fdev->base+(0x2aa<<1))) = 0x0055;

		*(ftype *)(sa) = 0x0025;
		if (bytecnt >= 32)
			tot = tot1 = 32;
		else
			tot = tot1 = bytecnt;
		*(ftype *)(dest) = (tot/2 - 1);
		while(tot > 0) {
			Fwrite((ftype *)(dest),src); 
			dest+=2; 
			src+=2;
			tot -= 2;
		}
  	    *(ftype *)(sa) = 0x0029;

		WHILE_D6_TOGGLES(sa);

		bytecnt -= tot1;
	}

	READ_RESET();

	/* Now that the re-programming of flash is complete, reset: */
	RESETFUNC();

	return(0);	/* won't get here */
}

/* EndS29gl512n_16x1_ewrite():
 * Function place holder to determine the end of the above function.
 */
void
EndS29gl512n_16x1_ewrite(void)
{}


/* S29gl512n_16x1_type():
 * Run the AUTOSELECT algorithm to retrieve the manufacturer and
 * device id of the flash.
 */
int
S29gl512n_16x1_type(struct flashinfo *fdev)
{
	ushort	man, dev;
	ulong	id;


	AUTO_SELECT();

	man = *(ftype *)(fdev->base);			/* manufacturer ID */
	dev = *(ftype *)((fdev->base + 28));	/* device ID */
	id = man;
	id <<= 16;
	id |= dev;

	fdev->id = id;

	READ_RESET();

	return((int)(fdev->id));
}

/* EndS29gl512n_16x1_type():
 * Function place holder to determine the end of the above function.
 */
void
EndS29gl512n_16x1_type(void)
{}

/**************************************************************************
 **************************************************************************
 *
 * The remainder of the code in this file should only included if the
 * target configuration is such that this AM29F040 device is the only
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
ulong    FlashTypeFbuf[400];
ulong    FlashEraseFbuf[400];
ulong    FlashWriteFbuf[400];
ulong    FlashEwriteFbuf[400];
#endif

/* FlashNamId[]:
 * Used to correlate between the ID and a string representing the name
 * of the flash device.
 */
struct flashdesc FlashNamId[] = {
	{ SPANSION_29GL512N_ID,		"Spansion-29GL512N" },
	{ SPANSION_29GL1024N_ID,	"Spansion-29GL1024N" },
	{ 0, (char *)0 },
};


struct sectorinfo sinfo_bank0[SECTOR_TOTAL];
struct sectorinfo sinfo_bank1[SECTOR_TOTAL];

int
flashBankInit(int banknum, int snum, struct sectorinfo *sinfotbl)
{
	int		i;
	uchar	*begin;
	struct	flashinfo *fbnk;

	fbnk = &FlashBank[banknum];
	if (banknum == 0)
		fbnk->base = (unsigned char *)FLASH_BANK0_BASE_ADDR;
	else
		fbnk->base = (unsigned char *)FLASH_BANK0_BASE_ADDR+0x04000000;

	fbnk->width = 2;

#ifdef FLASH_COPY_TO_RAM
	fbnk->fltype = (int(*)())FlashTypeFbuf;
	fbnk->flerase = (int(*)())FlashEraseFbuf;
	fbnk->flwrite = (int(*)())FlashWriteFbuf;
	fbnk->flewrite = (int(*)())FlashEwriteFbuf;
#else
	fbnk->fltype = S29gl512n_16x1_type;
	fbnk->flerase = S29gl512n_16x1_erase;
	fbnk->flwrite = S29gl512n_16x1_write;
	fbnk->flewrite = S29gl512n_16x1_ewrite;
#endif

	/* This device doesn't support flash lock, so set the pointer
	 * to the default FlashLockNotSupported() function...
	 */
	fbnk->fllock = FlashLockNotSupported;

	fbnk->sectors = sinfotbl;
	fbnk->id = flashtype(fbnk);
	switch(fbnk->id) {
		case SPANSION_29GL512N_ID:
			fbnk->end = fbnk->base + 0x04000000 - 1;
			fbnk->sectorcnt = SECTOR_TOTAL/2;
			break;
		case SPANSION_29GL1024N_ID:
			fbnk->end = fbnk->base + 0x08000000 - 1;
			fbnk->sectorcnt = SECTOR_TOTAL;
			break;
		default:
			/* The second bank may legitimately not be populated, so if it
			 * isn't found, indicate unavailable, don't claim an error...
			 */
			printf("Flash bank %d: ",banknum);
			if (banknum == 0)
				printf("invalid id: 0x%lx.\n",fbnk->id);
			else
				printf("not available.\n");
			return(-1);
	}

	begin = fbnk->base;
	for(i=0;i<fbnk->sectorcnt;i++,snum++) {
		fbnk->sectors[i].snum = snum;
		fbnk->sectors[i].size = SECTOR_SIZE;
		fbnk->sectors[i].begin = begin;
		fbnk->sectors[i].end = fbnk->sectors[i].begin + SECTOR_SIZE - 1;
		fbnk->sectors[i].protected = 0;
		begin += SECTOR_SIZE;
	}
	return(snum);
}

/* FlashInit():
 * Initialize data structures for each bank of flash...
 */
int
FlashInit()
{
	int	snum;

	FlashCurrentBank = 0;

	/* If code is in flash, then we must copy the flash ops to RAM.
	 * Note that this MUST be done when cache is disabled to assure
	 * that the RAM is occupied by the designated block of code.
	 */

#ifdef FLASH_COPY_TO_RAM
	if (flashopload((ulong *)S29gl512n_16x1_type,
		(ulong *)EndS29gl512n_16x1_type,
		FlashTypeFbuf,sizeof(FlashTypeFbuf)) < 0)
		return(-1);
	
	if (flashopload((ulong *)S29gl512n_16x1_erase,
		(ulong *)EndS29gl512n_16x1_erase,
		FlashEraseFbuf,sizeof(FlashEraseFbuf)) < 0)
		return(-1);
	
	if (flashopload((ulong *)S29gl512n_16x1_ewrite,
		(ulong *)EndS29gl512n_16x1_ewrite,
		FlashEwriteFbuf,sizeof(FlashEwriteFbuf)) < 0)
		return(-1);
	
	if (flashopload((ulong *)S29gl512n_16x1_write,
		(ulong *)EndS29gl512n_16x1_write,
		FlashWriteFbuf,sizeof(FlashWriteFbuf)) < 0)
		return(-1);
#endif

#ifdef IS_MONOLITHIC_FLASH
	ActiveFlashBanks = 1;
	snum = flashBankInit(0,0,sinfo_bank0);
	if (!IS_MONOLITHIC_FLASH()) {
		if (flashBankInit(1,snum,sinfo_bank1) != -1)
			ActiveFlashBanks++;
	}
#else
	snum = flashBankInit(0,0,sinfo_bank0);
#endif

#ifdef FLASH_PROTECT_RANGE
	sectorProtect(FLASH_PROTECT_RANGE,1);
#endif

#if FLASHRAM_BASE
	FlashRamInit(snum, FLASHRAM_SECTORCOUNT, &FlashBank[FLASHRAM_BANKNUM],
		sinfoRAM, 0);
#endif

	return(0);
}

#endif	/* SINGLE_FLASH_DEVICE */

#endif	/* INCLUDE_FLASH */
