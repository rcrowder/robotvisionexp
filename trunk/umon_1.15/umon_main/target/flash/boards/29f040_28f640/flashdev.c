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

typedef unsigned char uchar;
typedef unsigned short ushort;
typedef unsigned long ulong;
typedef volatile unsigned char vuchar;
typedef volatile unsigned short vushort;
typedef volatile unsigned long vulong;
typedef volatile unsigned int vuint;
typedef volatile int vint;

extern int Flashtype29F040_8();
extern int EndFlashtype29F040_8();
extern int Flasherase29F040_8();
extern int EndFlasherase29F040_8();
extern int Flashwrite29F040_8();
extern int EndFlashwrite29F040_8();
extern int Flashewrite29F040_8();
extern int EndFlashewrite29F040_8();

extern int Flashtype28F640_16();
extern int EndFlashtype28F640_16();
extern int Flasherase28F640_16();
extern int EndFlasherase28F640_16();
extern int Flashwrite28F640_16();
extern int EndFlashwrite28F640_16();
extern int Flashewrite28F640_16();
extern int EndFlashewrite28F640_16();


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
long    FlashTypeFbuf040[FLASHFUNCSIZE];
long    FlashWriteFbuf040[FLASHFUNCSIZE];
long    FlashEraseFbuf040[FLASHFUNCSIZE];
long    FlashEwriteFbuf040[FLASHFUNCSIZE];
#if COPY_640OPS_TO_RAM
long    FlashTypeFbuf640[FLASHFUNCSIZE];
long    FlashWriteFbuf640[FLASHFUNCSIZE];
long    FlashEraseFbuf640[FLASHFUNCSIZE];
long    FlashEwriteFbuf640[FLASHFUNCSIZE];
#endif

extern int		NotUsed();

/* FlashNamId[]:
   Used to correlate between the ID and a string representing the name
   of the flash device.
*/
struct flashdesc FlashNamId[] = {
	{ INTEL28F640,	"INTEL-28F640" },
	{ INTEL28F320,	"INTEL-28F320" },
	{ SGS29F040,	"SGS-29F040" },
	{ AMD29F040,	"AMD-29F040" },
	{ AMD29F010,	"AMD-29F010" },
	{ AMD29LV040,	"AMD-29LV040" },
	{ 0,	0 }
};

int	SectorSizes29F040_8[] = {
		0x10000, 0x10000, 0x10000, 0x10000,
		0x10000, 0x10000, 0x10000, 0x10000,
};

#if APC_28F640_HARDWARE_BUG
int	SectorSizes28F640_16[] = {
		0x20000, 	0x20000,	0x20000,	0x20000,
		0x20000, 	0x20000,	0x20000,	0x20000,
		0x20000, 	0x20000,	0x20000,	0x20000,
		0x20000, 	0x20000,	0x20000,	0x20000,
		0x20000, 	0x20000,	0x20000,	0x20000,
		0x20000, 	0x20000,	0x20000,	0x20000,
		0x20000, 	0x20000,	0x20000,	0x20000,
		0x20000, 	0x20000,	0x20000,	0x20000,
};
#else
int	SectorSizes28F640_16[] = {
		0x20000, 	0x20000,	0x20000,	0x20000,
		0x20000, 	0x20000,	0x20000,	0x20000,
		0x20000, 	0x20000,	0x20000,	0x20000,
		0x20000, 	0x20000,	0x20000,	0x20000,
		0x20000, 	0x20000,	0x20000,	0x20000,
		0x20000, 	0x20000,	0x20000,	0x20000,
		0x20000, 	0x20000,	0x20000,	0x20000,
		0x20000, 	0x20000,	0x20000,	0x20000,
		0x20000, 	0x20000,	0x20000,	0x20000,
		0x20000, 	0x20000,	0x20000,	0x20000,
		0x20000, 	0x20000,	0x20000,	0x20000,
		0x20000, 	0x20000,	0x20000,	0x20000,
		0x20000, 	0x20000,	0x20000,	0x20000,
		0x20000, 	0x20000,	0x20000,	0x20000,
		0x20000, 	0x20000,	0x20000,	0x20000,
		0x20000, 	0x20000,	0x20000,	0x20000,
};
#endif

struct sectorinfo sinfo640[sizeof(SectorSizes28F640_16)/sizeof(int)];
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

#if COPY_640OPS_TO_RAM
	if (flashopload((ulong *)Flashtype28F640_16,
		(ulong *)EndFlashtype28F640_16,
		FlashTypeFbuf640,sizeof(FlashTypeFbuf640)) < 0)
		return(-1);
	if (flashopload((ulong *)Flasherase28F640_16,
		(ulong *)EndFlasherase28F640_16,
		FlashEraseFbuf640,sizeof(FlashEraseFbuf640)) < 0)
		return(-1);
	if (flashopload((ulong *)Flashewrite28F640_16,
		(ulong *)EndFlashewrite28F640_16,
		FlashEwriteFbuf640,sizeof(FlashEwriteFbuf640)) < 0)
		return(-1);
	if (flashopload((ulong *)Flashwrite28F640_16,
		(ulong *)EndFlashwrite28F640_16,
		FlashWriteFbuf640,sizeof(FlashWriteFbuf640)) < 0)
		return(-1);
#endif

	/* Initialize each bank of flash... */
	fbnk = &FlashBank[0];
	fbnk->id = AMD29F040;						/* Device id. */
	fbnk->base = (uchar *)FLASH_BANK0_BASE_ADDR;/* Base address of bank. */
	fbnk->end = fbnk->base + 0x7ffff;			/* End address of bank. */
	fbnk->sectorcnt = 8;						/* Number of sectors. */
	fbnk->width = 1;							/* Width (in bytes). */
	fbnk->fltype = (int(*)())FlashTypeFbuf040;		/* Flashtype(). */
	fbnk->flerase = (int(*)())FlashEraseFbuf040;	/* Flasherase(). */
	fbnk->flwrite = (int(*)())FlashWriteFbuf040;	/* Flashwrite(). */
	fbnk->flewrite = (int(*)())FlashEwriteFbuf040;	/* Flashewrite(). */
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
	fbnk->id = INTEL28F640;						/* Device id. */
	fbnk->base = (uchar *)FLASH_BANK1_BASE_ADDR;/* Base address of bank. */
												/* Number of sectors: */
	fbnk->sectorcnt = sizeof(SectorSizes28F640_16)/sizeof(int);
												/* End address of bank: */
	fbnk->end = fbnk->base + 0x20000*fbnk->sectorcnt;
	fbnk->width = 2;							/* Width (in bytes). */
#if COPY_640OPS_TO_RAM
	fbnk->fltype = (int(*)())FlashTypeFbuf640;		/* Flashtype(). */
	fbnk->flerase = (int(*)())FlashEraseFbuf640;	/* Flasherase(). */
	fbnk->flwrite = (int(*)())FlashWriteFbuf640;	/* Flashwrite(). */
	fbnk->flewrite = (int(*)())FlashEwriteFbuf640;	/* Flashewrite(). */
#else
	fbnk->fltype = Flashtype28F640_16;		/* Flashtype(). */
	fbnk->flerase = Flasherase28F640_16;	/* Flasherase(). */
	fbnk->flwrite = Flashwrite28F640_16;	/* Flashwrite(). */
	fbnk->flewrite = Flashewrite28F640_16;	/* Flashewrite(). */
#endif
	fbnk->sectors = sinfo640;				/* Pointer to sector size table. */
	begin = fbnk->base;
	for(i=0;i<fbnk->sectorcnt;i++,snum++) {
		sinfo640[i].snum = snum;
		sinfo640[i].size = SectorSizes28F640_16[i];
		sinfo640[i].begin = begin;
		sinfo640[i].end =
		    sinfo640[i].begin + sinfo640[i].size - 1;
		sinfo640[i].protected = 0;
		begin += SectorSizes28F640_16[i];
	}

	/* Protect the sectors specifed by FLASH_PROTECT_RANGE: */
	sectorProtect(FLASH_PROTECT_RANGE,1);

	return(0);
}

#endif
