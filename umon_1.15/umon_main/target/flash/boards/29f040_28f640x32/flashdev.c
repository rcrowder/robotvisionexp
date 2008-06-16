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

#ifndef COPY_040_OPS_TO_RAM
#define COPY_040_OPS_TO_RAM 1
#endif

#ifndef COPY_STRATA_OPS_TO_RAM
#define COPY_STRATA_OPS_TO_RAM 1
#endif

extern int Flashtype29F040_8();
extern int EndFlashtype29F040_8();
extern int Flasherase29F040_8();
extern int EndFlasherase29F040_8();
extern int Flashwrite29F040_8();
extern int EndFlashwrite29F040_8();
extern int Flashewrite29F040_8();
extern int EndFlashewrite29F040_8();

extern int FlashtypeStrata_32();
extern int EndFlashtypeStrata_32();
extern int FlasheraseStrata_32();
extern int EndFlasheraseStrata_32();
extern int FlashwriteStrata_32();
extern int EndFlashwriteStrata_32();
extern int FlashlockStrata_32();
extern int EndFlashlockStrata_32();
extern int FlashewriteStrata_32();
extern int EndFlashewriteStrata_32();

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
#if COPY_040_OPS_TO_RAM
long    FlashTypeFbuf040[FLASHFUNCSIZE];
long    FlashWriteFbuf040[FLASHFUNCSIZE];
long    FlashEraseFbuf040[FLASHFUNCSIZE];
long    FlashEwriteFbuf040[FLASHFUNCSIZE];
#endif

#if COPY_STRATA_OPS_TO_RAM
long    FlashTypeFbuf640[FLASHFUNCSIZE];
long    FlashWriteFbuf640[FLASHFUNCSIZE];
long    FlashEraseFbuf640[FLASHFUNCSIZE];
long    FlashEwriteFbuf640[FLASHFUNCSIZE];
long    FlashLockFbuf640[FLASHFUNCSIZE];
#endif

extern int		NotUsed();

/* FlashNamId[]:
   Used to correlate between the ID and a string representing the name
   of the flash device.
*/
struct flashdesc FlashNamId[] = {
	{ INTEL28F320J5,	"INTEL-28F320J5" },
	{ INTEL28F640J5,	"INTEL-28F640J5" },
	{ INTEL28F320J3,	"INTEL-28F320J3" },
	{ INTEL28F640J3,	"INTEL-28F640J3" },
	{ INTEL28F128J3,	"INTEL-28F128J3" },
	{ SGS29F040,		"SGS-29F040" },
	{ AMD29F040,		"AMD-29F040" },
	{ AMD29LV040,		"AMD-29LV040" },
	{ 0,	0 }
};

/* Simple 8-bit 29f040...
 * 8 64K sectors.
 */
int	SectorSizes29F040_8[] = {
		0x10000, 0x10000, 0x10000, 0x10000,
		0x10000, 0x10000, 0x10000, 0x10000,
};

/* This configuration is 2 side-by-side 28F640J3 devices.
 * Each device has 64 128Kbyte sectors, so since we have 2
 * in parallel, then each 32-bit sector is 256Kbytes...
 */
int	SectorSizes28F640_32[] = {
		0x40000, 	0x40000,	0x40000,	0x40000,
		0x40000, 	0x40000,	0x40000,	0x40000,
		0x40000, 	0x40000,	0x40000,	0x40000,
		0x40000, 	0x40000,	0x40000,	0x40000,
		0x40000, 	0x40000,	0x40000,	0x40000,
		0x40000, 	0x40000,	0x40000,	0x40000,
		0x40000, 	0x40000,	0x40000,	0x40000,
		0x40000, 	0x40000,	0x40000,	0x40000,
		0x40000, 	0x40000,	0x40000,	0x40000,
		0x40000, 	0x40000,	0x40000,	0x40000,
		0x40000, 	0x40000,	0x40000,	0x40000,
		0x40000, 	0x40000,	0x40000,	0x40000,
		0x40000, 	0x40000,	0x40000,	0x40000,
		0x40000, 	0x40000,	0x40000,	0x40000,
		0x40000, 	0x40000,	0x40000,	0x40000,
 		0x40000, 	0x40000,	0x40000,	0x40000,
};

struct sectorinfo sinfo640[sizeof(SectorSizes28F640_32)/sizeof(int)];
struct sectorinfo sinfo040[sizeof(SectorSizes29F040_8)/sizeof(int)];

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

	/* Copy functions to ram space... */
	/* Note that this MUST be done when cache is disabled to assure that */
	/* the RAM is occupied by the designated block of code. */

#if COPY_040_OPS_TO_RAM
	if (flashopload((ulong *)Flashtype29F040_8,
		(ulong *)EndFlashtype29F040_8,
		FlashTypeFbuf040,sizeof(FlashTypeFbuf040)) < 0)
		return(-1);
	if (flashopload((ulong *)Flasherase29F040_8,
		(ulong *)EndFlasherase29F040_8,
		FlashEraseFbuf040,sizeof(FlashEraseFbuf040)) < 0)
		return(-1);
	if (flashopload((ulong *)Flashewrite29F040_8,
		(ulong *)EndFlashewrite29F040_8,
		FlashEwriteFbuf040,sizeof(FlashEwriteFbuf040)) < 0)
		return(-1);
	if (flashopload((ulong *)Flashwrite29F040_8,
		(ulong *)EndFlashwrite29F040_8,
		FlashWriteFbuf040,sizeof(FlashWriteFbuf040)) < 0)
		return(-1);
#endif

#if COPY_STRATA_OPS_TO_RAM
	if (flashopload((ulong *)FlashtypeStrata_32,
		(ulong *)EndFlashtypeStrata_32,
		FlashTypeFbuf640,sizeof(FlashTypeFbuf640)) < 0)
		return(-1);
	if (flashopload((ulong *)FlasheraseStrata_32,
		(ulong *)EndFlasheraseStrata_32,
		FlashEraseFbuf640,sizeof(FlashEraseFbuf640)) < 0)
		return(-1);
	if (flashopload((ulong *)FlashewriteStrata_32,
		(ulong *)EndFlashewriteStrata_32,
		FlashEwriteFbuf640,sizeof(FlashEwriteFbuf640)) < 0)
		return(-1);
	if (flashopload((ulong *)FlashwriteStrata_32,
		(ulong *)EndFlashwriteStrata_32,
		FlashWriteFbuf640,sizeof(FlashWriteFbuf640)) < 0)
		return(-1);
	if (flashopload((ulong *)FlashlockStrata_32,
		(ulong *)EndFlashlockStrata_32,
		FlashLockFbuf640,sizeof(FlashLockFbuf640)) < 0)
		return(-1);
#endif

	/* Initialize each bank of flash... */
	fbnk = &FlashBank[0];
	fbnk->id = AMD29LV040;						/* Device id. */
	fbnk->base = (uchar *)FLASH_BANK0_BASE_ADDR;/* Base address of bank. */
	fbnk->end = fbnk->base + 0x7ffff;			/* End address of bank. */
	fbnk->sectorcnt = 8;						/* Number of sectors. */
	fbnk->width = 1;							/* Width (in bytes). */
#if COPY_040_OPS_TO_RAM
	fbnk->fltype = (int(*)())FlashTypeFbuf040;		/* Flashtype(). */
	fbnk->flerase = (int(*)())FlashEraseFbuf040;	/* Flasherase(). */
	fbnk->flwrite = (int(*)())FlashWriteFbuf040;	/* Flashwrite(). */
	fbnk->flewrite = (int(*)())FlashEwriteFbuf040;	/* Flashewrite(). */
#else
	fbnk->fltype = Flashtype29F040_8;
	fbnk->flerase = Flasherase29F040_8;
	fbnk->flwrite = Flashwrite29F040_8;
	fbnk->flewrite = Flashewrite29F040_8;
#endif
	fbnk->fllock = FlashLockNotSupported;
	fbnk->sectors = sinfo040;
	begin = fbnk->base;
	for(i=0;i<fbnk->sectorcnt;i++,snum++) {
		sinfo040[i].snum = snum;
		sinfo040[i].size = SectorSizes29F040_8[i];
		sinfo040[i].begin = begin;
		sinfo040[i].end =
		    sinfo040[i].begin + sinfo040[i].size - 1;
		begin += SectorSizes29F040_8[i];
	}

	fbnk = &FlashBank[1];
	fbnk->id = INTEL28F640J3;					/* Device id. */
	fbnk->base = (uchar *)FLASH_BANK1_BASE_ADDR;/* Base address of bank. */
												/* Number of sectors: */
	fbnk->sectorcnt = sizeof(SectorSizes28F640_32)/sizeof(int);
												/* End address of bank: */
	fbnk->end = fbnk->base + 0x40000*fbnk->sectorcnt;
	fbnk->width = 4;							/* Width (in bytes). */
#if COPY_STRATA_OPS_TO_RAM
	fbnk->fltype = (int(*)())FlashTypeFbuf640;		/* Flashtype(). */
	fbnk->flerase = (int(*)())FlashEraseFbuf640;	/* Flasherase(). */
	fbnk->flwrite = (int(*)())FlashWriteFbuf640;	/* Flashwrite(). */
	fbnk->flewrite = (int(*)())FlashEwriteFbuf640;	/* Flashewrite(). */
	fbnk->fllock = (int(*)())FlashLockFbuf640;		/* Flashlock(). */
#else
	fbnk->fltype = FlashtypeStrata_32;			/* Flashtype(). */
	fbnk->flerase = FlasheraseStrata_32;		/* Flasherase(). */
	fbnk->flwrite = FlashwriteStrata_32;		/* Flashwrite(). */
	fbnk->flewrite = FlashewriteStrata_32;		/* Flashewrite(). */
	fbnk->fllock = FlashlockStrata_32;			/* Flashelock(). */
#endif
	fbnk->sectors = sinfo640;					/* Ptr to sector size tbl. */
	begin = fbnk->base;
	for(i=0;i<fbnk->sectorcnt;i++,snum++) {
		sinfo640[i].snum = snum;
		sinfo640[i].size = SectorSizes28F640_32[i];
		sinfo640[i].begin = begin;
		sinfo640[i].end =
		    sinfo640[i].begin + sinfo640[i].size - 1;
		sinfo640[i].protected = 0;
		begin += SectorSizes28F640_32[i];
	}

	/* Protect the sectors specifed by FLASH_PROTECT_RANGE: */
	sectorProtect(FLASH_PROTECT_RANGE,1);

	return(0);
}

#endif
