/* This file contains all of the flash support code that does not need
   to be relocated to RAM.  Two separate files (flash.c and flashpic.c)
   are maintained because under certain compilers, they may need to be
   compiled with different options to be made position independent.

   NOTE: THESE FUNCTIONS ARE NOT RE-ENTRANT!!!  All of the FLASH routines
   assume they can copy themselves into the array FlashFunc[]; hence, only
   one operation can be active at any time.
*/
#include "config.h"
#if INCLUDE_FLASH
#include "cpu.h"
#include "flashdev.h"
#include "flash.h"
#include "stddefs.h"

extern int FlashtypeStrata_8x32();
extern int EndFlashtypeStrata_8x32();
extern int FlasheraseStrata_8x32();
extern int EndFlasheraseStrata_8x32();
extern int FlashwriteStrata_8x32();
extern int EndFlashwriteStrata_8x32();
extern int FlashewriteStrata_8x32();
extern int EndFlashewriteStrata_8x32();
extern int FlashlockStrata_8x32();
extern int EndFlashlockStrata_8x32();

/* FlashXXXFbufYYY[]:
	Where XXX is the function to be stored in the buffer and YYY is the
	device that the function is to operate on.
	These arrays are loaded with the flash operation function (TYPE, ERASE,
	WRITE & EWRITE) that must run in different memory space than the device
	that is being operated on.  Recall that to operate on the flash, you
	cannot be executing out of it.
	The flash functions are copied here, then executed through a function
	pointer flashfunc which is set (in FlashInit) to point to the buffer.
*/

#ifdef FLASH_COPY_TO_RAM
long    FlashTypeFbufStrata[FLASHFUNCSIZE];
long    FlashWriteFbufStrata[FLASHFUNCSIZE*2];
long    FlashEraseFbufStrata[FLASHFUNCSIZE];
long    FlashEwriteFbufStrata[FLASHFUNCSIZE];
long    FlashLockFbufStrata[FLASHFUNCSIZE];
#endif

/* FlashNamId[]:
   Used to correlate between the ID and a string representing the name
   of the flash device.
*/
struct flashdesc FlashNamId[] = {
	{ DEVICE_28F320J3,	"INTEL-28F320J3" },
	{ DEVICE_28F640J3,	"INTEL-28F640J3" },
	{ DEVICE_28F128J3,	"INTEL-28F128J3" },
	{ 0,	0 }
};

/* Sector Size table:
 * 28F320J3A:  32 128Kbyte sectors
 * 28F640J3A:  64 128Kbyte sectors
 * 28F128J3A: 128 128Kbyte sectors
 *
 * This configuration is 4 side-by-side 8-bit devices.
 * Since we have 4 in parallel, then each 32-bit sector
 * is 512Kbytes...
 */

int	SectorSizesStrata[] = {
		0x80000, 	0x80000,	0x80000,	0x80000,
		0x80000, 	0x80000,	0x80000,	0x80000,
		0x80000, 	0x80000,	0x80000,	0x80000,
		0x80000, 	0x80000,	0x80000,	0x80000,
		0x80000, 	0x80000,	0x80000,	0x80000,
		0x80000, 	0x80000,	0x80000,	0x80000,
		0x80000, 	0x80000,	0x80000,	0x80000,
		0x80000, 	0x80000,	0x80000,	0x80000,
#if SUPPORT640 | SUPPORT128
		0x80000, 	0x80000,	0x80000,	0x80000,
		0x80000, 	0x80000,	0x80000,	0x80000,
		0x80000, 	0x80000,	0x80000,	0x80000,
		0x80000, 	0x80000,	0x80000,	0x80000,
		0x80000, 	0x80000,	0x80000,	0x80000,
		0x80000, 	0x80000,	0x80000,	0x80000,
		0x80000, 	0x80000,	0x80000,	0x80000,
		0x80000, 	0x80000,	0x80000,	0x80000,
#endif
#if SUPPORT128
		0x80000, 	0x80000,	0x80000,	0x80000,
		0x80000, 	0x80000,	0x80000,	0x80000,
		0x80000, 	0x80000,	0x80000,	0x80000,
		0x80000, 	0x80000,	0x80000,	0x80000,
		0x80000, 	0x80000,	0x80000,	0x80000,
		0x80000, 	0x80000,	0x80000,	0x80000,
		0x80000, 	0x80000,	0x80000,	0x80000,
		0x80000, 	0x80000,	0x80000,	0x80000,
		0x80000, 	0x80000,	0x80000,	0x80000,
		0x80000, 	0x80000,	0x80000,	0x80000,
		0x80000, 	0x80000,	0x80000,	0x80000,
		0x80000, 	0x80000,	0x80000,	0x80000,
		0x80000, 	0x80000,	0x80000,	0x80000,
		0x80000, 	0x80000,	0x80000,	0x80000,
		0x80000, 	0x80000,	0x80000,	0x80000,
		0x80000, 	0x80000,	0x80000,	0x80000,
#endif
};

struct sectorinfo sinfoStrata[sizeof(SectorSizesStrata)/sizeof(int)];

/* FlashInit():
   Initialize data structures for each bank of flash...
*/
int
FlashInit()
{
	int	i, snum;
	uchar	*begin;
	struct	flashinfo *fbnk;

	snum = 0;
	FlashCurrentBank = 0;

#ifdef FLASH_COPY_TO_RAM
	/* Copy functions to ram space... */
	/* Note that this MUST be done when cache is disabled to assure that */
	/* the RAM is occupied by the designated block of code. */

	if (flashopload((ulong *)FlashtypeStrata_8x32,
		(ulong *)EndFlashtypeStrata_8x32,
		FlashTypeFbufStrata,sizeof(FlashTypeFbufStrata)) < 0)
		return(-1);
	if (flashopload((ulong *)FlasheraseStrata_8x32,
		(ulong *)EndFlasheraseStrata_8x32,
		FlashEraseFbufStrata,sizeof(FlashEraseFbufStrata)) < 0)
		return(-1);
	if (flashopload((ulong *)FlashewriteStrata_8x32,
		(ulong *)EndFlashewriteStrata_8x32,
		FlashEwriteFbufStrata,sizeof(FlashEwriteFbufStrata)) < 0)
		return(-1);
	if (flashopload((ulong *)FlashwriteStrata_8x32,
		(ulong *)EndFlashwriteStrata_8x32,
		FlashWriteFbufStrata,sizeof(FlashWriteFbufStrata)) < 0)
		return(-1);
	if (flashopload((ulong *)FlashlockStrata_8x32,
		(ulong *)EndFlashlockStrata_8x32,
		FlashLockFbufStrata,sizeof(FlashLockFbufStrata)) < 0)
		return(-1);
#endif

	/* Initialize each bank of flash... */
	fbnk = &FlashBank[0];
	fbnk->id = DEVICE_28F320J3;					/* Device id. */
	fbnk->base = (uchar *)FLASH_BANK0_BASE_ADDR;/* Base address of bank. */
												/* Number of sectors: */
	fbnk->sectorcnt = sizeof(SectorSizesStrata)/sizeof(int);
												/* End address of bank: */
	fbnk->end = fbnk->base + (0x80000 * fbnk->sectorcnt) - 1;
	fbnk->width = 4;							/* Width (in bytes). */
#ifdef FLASH_COPY_TO_RAM
	fbnk->fltype = (int(*)())FlashTypeFbufStrata;
	fbnk->flerase = (int(*)())FlashEraseFbufStrata;
	fbnk->flwrite = (int(*)())FlashWriteFbufStrata;
	fbnk->flewrite = (int(*)())FlashEwriteFbufStrata;
	fbnk->fllock = (int(*)())FlashLockFbufStrata;
#else
	fbnk->fltype = FlashtypeStrata_8x32;
	fbnk->flerase = FlasheraseStrata_8x32;
	fbnk->flwrite =FlashwriteStrata_8x32;
	fbnk->flewrite = FlashewriteStrata_8x32;
	fbnk->fllock = FlashlockStrata_8x32;
#endif
	fbnk->sectors = sinfoStrata;				/* Ptr to sector size tbl. */
	begin = fbnk->base;


	for(i=0;i<fbnk->sectorcnt;i++,snum++) {
		sinfoStrata[i].snum = snum;
		sinfoStrata[i].size = SectorSizesStrata[i];
		sinfoStrata[i].begin = begin;
		sinfoStrata[i].end = sinfoStrata[i].begin + sinfoStrata[i].size - 1;
		sinfoStrata[i].protected = 0;
		begin += SectorSizesStrata[i];
	}

	/* Protect the sectors specifed by FLASH_PROTECT_RANGE: */
	sectorProtect(FLASH_PROTECT_RANGE,1);

	return(0);
}

#endif
