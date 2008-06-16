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

extern int Flashtype29F800B_16();
extern int EndFlashtype29F800B_16();
extern int Flasherase29F800B_16();
extern int EndFlasherase29F800B_16();
extern int Flashwrite29F800B_16();
extern int EndFlashwrite29F800B_16();
extern int Flashewrite29F800B_16();
extern int EndFlashewrite29F800B_16();


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
long    FlashTypeFbuf800[FLASHFUNCSIZE];
long    FlashWriteFbuf800[FLASHFUNCSIZE];
long    FlashEraseFbuf800[FLASHFUNCSIZE];
long    FlashEwriteFbuf800[FLASHFUNCSIZE];

extern int		NotUsed();

/* FlashNamId[]:
   Used to correlate between the ID and a string representing the name
   of the flash device.
*/
struct flashdesc FlashNamId[] = {
	{ AMD29F800B,	"AMD-29F800B" },
	{ AMD29F800T,	"AMD-29F800T" },
	{ SGS29F040,	"SGS-29F040" },
	{ AMD29F040,	"AMD-29F040" },
	{ AMD29F010,	"AMD-29F010" },
	{ FLASHRAM,		"FLASH-RAM" },
	{ 0,	0 }
};

int	SectorSizes29F040_8[] = {
		0x10000, 0x10000, 0x10000, 0x10000,
		0x10000, 0x10000, 0x10000, 0x10000,
};

int	SectorSizes29F800B_16[] = {
		0x4000,		0x2000,		0x2000,		0x8000,
		0x10000, 	0x10000,	0x10000,	0x10000,
		0x10000, 	0x10000,	0x10000,	0x10000,
		0x10000, 	0x10000,	0x10000,	0x10000,
		0x10000, 	0x10000,	0x10000
};

struct sectorinfo sinfo800[19], sinfo040[8];
struct sectorinfo sinfoRAM[(FLASHRAM_SIZE/FLASHRAM_SSIZE)];

/* FlashInit():
   Initialize data structures for each bank of flash...
*/
int
FlashInit()
{
	int	i, snum;
	uchar	*begin;
	struct	flashinfo *fbnk;

	FlashCurrentBank = 0;
	snum = 0;

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

	if (flashopload((ulong *)Flashtype29F800B_16,
		(ulong *)EndFlashtype29F800B_16,
		FlashTypeFbuf800,sizeof(FlashTypeFbuf800)) < 0)
		return(-1);
	if (flashopload((ulong *)Flasherase29F800B_16,
		(ulong *)EndFlasherase29F800B_16,
		FlashEraseFbuf800,sizeof(FlashEraseFbuf800)) < 0)
		return(-1);
	if (flashopload((ulong *)Flashewrite29F800B_16,
		(ulong *)EndFlashewrite29F800B_16,
		FlashEwriteFbuf800,sizeof(FlashEwriteFbuf800)) < 0)
		return(-1);
	if (flashopload((ulong *)Flashwrite29F800B_16,
		(ulong *)EndFlashwrite29F800B_16,
		FlashWriteFbuf800,sizeof(FlashWriteFbuf800)) < 0)
		return(-1);

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
		/* All sectors of bank 0 whose base is less than FLASH_PROTECT_SIZE */
		/* are marked as protected... */
		if (begin < (uchar *)(FLASH_BANK0_BASE_ADDR + FLASH_PROTECT_SIZE))
			sinfo040[i].protected = 1;
		else
			sinfo040[i].protected = 0;
		
		begin += SectorSizes29F040_8[i];
	}

	fbnk = &FlashBank[1];
	fbnk->id = AMD29F800B;						/* Device id. */
	fbnk->base = (uchar *)FLASH_BANK1_BASE_ADDR;/* Base address of bank. */
	fbnk->end = fbnk->base + 0xfffff;			/* End address of bank. */
	fbnk->sectorcnt = 19;						/* Number of sectors. */
	fbnk->width = 2;							/* Width (in bytes). */
	fbnk->fltype = (int(*)())FlashTypeFbuf800;		/* Flashtype(). */
	fbnk->flerase = (int(*)())FlashEraseFbuf800;	/* Flasherase(). */
	fbnk->flwrite = (int(*)())FlashWriteFbuf800;	/* Flashwrite(). */
	fbnk->flewrite = (int(*)())FlashEwriteFbuf800;	/* Flashewrite(). */
	fbnk->sectors = sinfo800;				/* Pointer to sector size table. */
	begin = fbnk->base;
	for(i=0;i<fbnk->sectorcnt;i++,snum++) {
		sinfo800[i].snum = snum;
		sinfo800[i].size = SectorSizes29F800B_16[i];
		sinfo800[i].begin = begin;
		sinfo800[i].end =
		    sinfo800[i].begin + sinfo800[i].size - 1;
		sinfo800[i].protected = 0;
		begin += SectorSizes29F800B_16[i];
	}

	if (FLASHBANKS < 3)
		return(0);

	/* This is a TFS FLASH/RAM bank... */
	fbnk = &FlashBank[2];
	fbnk->id = FLASHRAM;					/* Device id. */
	fbnk->base = (uchar *)FLASHRAM_BASE;	/* Base address of bank. */
	fbnk->end = (uchar *)FLASHRAM_END;		/* End address of bank. */
	fbnk->sectorcnt = (FLASHRAM_SIZE/FLASHRAM_SSIZE);/* Number of sectors. */
	fbnk->width = 0;						/* Width (in bytes). */
	fbnk->fltype = NotUsed;					/* Flashtype() function. */
	fbnk->flerase = NotUsed;				/* Flasherase() function. */
	fbnk->flwrite = NotUsed;				/* Flashwrite() function. */
	fbnk->flewrite = NotUsed;				/* Flashewrite() function. */
	fbnk->sectors = sinfoRAM;				/* Pointer to sector size table. */
	begin = fbnk->base;
	for(i=0;i<fbnk->sectorcnt;i++,snum++) {
		sinfoRAM[i].snum = snum;
		sinfoRAM[i].size = FLASHRAM_SSIZE;
		sinfoRAM[i].begin = begin;
		sinfoRAM[i].end = sinfoRAM[i].begin + sinfoRAM[i].size - 1;
		sinfoRAM[i].protected = 0;
		begin += FLASHRAM_SSIZE;
	}
	return(0);
}

#endif
