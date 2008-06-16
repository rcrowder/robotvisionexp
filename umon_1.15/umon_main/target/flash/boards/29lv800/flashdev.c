/*******************************************************************************
Copyright (c) Lucent Technologies, 1999.  All rights reserved.

FILENAME:      Flash.c

DESCRIPTION:   Flash utility

================================================================================
EDIT HISTORY:

DATE      INITIALS  ABSTRACT
03-25-99  M. Hunag  Created.

*******************************************************************************/
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

extern int Flashtype16();
extern int EndFlashtype16();
extern int Flashwrite16();
extern int EndFlashwrite16();
extern int Flashewrite16();
extern int EndFlashewrite16();
extern int Flasherase16();
extern int EndFlasherase16();

int	FlashWidthNotSupported();

/* FlashXXXFbuf[]:
   These arrays will contain the flash operation function that is executing.
   Recall that to operate on the flash, you cannot be executing out of it.
   The flash functions are copied here, then executed through the function
   pointer flashfunc which is set to point to FlashFunc.
*/
ulong    FlashTypeFbuf[FLASHFUNCSIZE];
ulong    FlashEraseFbuf[FLASHFUNCSIZE];
ulong    FlashWriteFbuf[FLASHFUNCSIZE];
ulong    FlashEwriteFbuf[FLASHFUNCSIZE];

/* FlashNamId[]:
   Used to correlate between the ID and a string representing the name
   of the flash device.
*/
struct flashdesc FlashNamId[] = {
		{ AMD29LV160BB,	"AM29LV160BB" },
		{ AMD29LV160T,	"AM29LV160T" },
		{ AM29LV800BB,	"AM29LV800BB" },
		{ M29W800B,		"M29W800B" }
};

#if FLASH_29F160
#define SECTORCOUNT 35
int	SectorSizes160[] = {
		0x04000, 0x02000, 0x02000, 0x08000, 0x10000,
		0x10000, 0x10000, 0x10000, 0x10000, 0x10000,
		0x10000, 0x10000, 0x10000, 0x10000, 0x10000,
		0x10000, 0x10000, 0x10000, 0x10000, 0x10000,
		0x10000, 0x10000, 0x10000, 0x10000, 0x10000,
		0x10000, 0x10000, 0x10000, 0x10000, 0x10000,
		0x10000, 0x10000, 0x10000, 0x10000, 0x10000,
};
#elif FLASH_29F800
#define SECTORCOUNT 19
int	SectorSizes800B[] = {
		0x4000, 0x2000,	0x2000, 0x8000,
		0x10000, 0x10000, 0x10000, 0x10000,
		0x10000, 0x10000, 0x10000, 0x10000,
		0x10000, 0x10000, 0x10000, 0x10000,
		0x10000, 0x10000, 0x10000
};
#endif

/* FlashBankInit():
   Initialize flash structures and determine flash device type.
*/
int
FlashBankInit(struct flashinfo *fbnk,int snum)
{
	uchar	*saddr;
	int	i, *sizetable, msize;
	struct	sectorinfo *sinfotbl;

	/* Based on the flash bank ID returned, load a sector count and a */
	/* sector size-information table... */
	flashtype(fbnk);
	switch(fbnk->id) {
		case AMD29LV160BB:
			fbnk->sectorcnt = 35;
			sizetable = SectorSizes160;
			break;
		case 29LV800BB:
			fbnk->sectorcnt = 39;
			sizetable = SectorSizes160B;
			break;
		case INTEL28F320B:
			fbnk->sectorcnt = 71;
			sizetable = SectorSizes320B;
			break;
		default:
			printf("Unrecognized flashid: 0x%04x\n",fbnk->id);
			break;
	}

	/* Create the per-sector information table.  The size of the table */
	/* depends on the number of sectors in the device...  */
	if (fbnk->sectors)
		free((char *)fbnk->sectors);
	msize = fbnk->sectorcnt * (sizeof(struct sectorinfo));
	sinfotbl = (struct sectorinfo *)malloc(msize);
	if (!sinfotbl) {
		printf("Can't allocate space for flash sector information\n");
		return(-1);
	}
	fbnk->sectors = sinfotbl;

	/* Using the above-determined sector count and size table, build */
	/* the sector information table as part of the flash-bank structure: */
	saddr = fbnk->base;
	for(i=0;i<fbnk->sectorcnt;i++) {
		fbnk->sectors[i].snum = snum+i;
		fbnk->sectors[i].size = sizetable[i];
		fbnk->sectors[i].begin = saddr;
		fbnk->sectors[i].end =
		    fbnk->sectors[i].begin + fbnk->sectors[i].size - 1;
		fbnk->sectors[i].protected = 0;
		saddr += sizetable[i];
	}
	fbnk->end = saddr-1;
	return(fbnk->sectorcnt);
}

int
FlashInit()
{
	int	i,	snum;
	uchar	*begin;
	long	size;
	struct	flashinfo *fbnk;

	snum = 0;
	FlashCurrentBank = 0;

	/* Copy functions to ram space... */
	/* Note that this MUST be done when cache is disabled to assure that */
	/* the RAM is occupied by the designated block of code. */

	if (flashopload((ulong *)FLASHTYPE,(ulong *)ENDFLASHTYPE,
		FlashTypeFbuf,sizeof(FlashTypeFbuf)) < 0)
		return(-1);
	if (flashopload((ulong *)FLASHERASE,(ulong *)ENDFLASHERASE,
		FlashEraseFbuf,sizeof(FlashEraseFbuf)) < 0)
		return(-1);
	if (flashopload((ulong *)FLASHEWRITE,(ulong *)ENDFLASHEWRITE,
		FlashEwriteFbuf,sizeof(FlashEwriteFbuf)) < 0)
		return(-1);
	if (flashopload((ulong *)FLASHWRITE,(ulong *)ENDFLASHWRITE,
		FlashWriteFbuf,sizeof(FlashWriteFbuf)) < 0)
		return(-1);

	fbnk = &FlashBank[0];
	fbnk->base = (unsigned char *)FLASH_BANK0_BASE_ADDR;
	fbnk->end = fbnk->base + 0xfffff;			/* End address of bank. */
	fbnk->width = FLASH_BANK0_WIDTH;
	fbnk->fltype = (int(*)())FlashTypeFbuf;			/* flashtype(). */
	fbnk->flerase = (int(*)())FlashEraseFbuf;		/* flasherase(). */
	fbnk->flwrite = (int(*)())FlashWriteFbuf;		/* flashwrite(). */
	fbnk->flewrite = (int(*)())FlashEwriteFbuf;		/* flashewrite(). */
	snum += FlashBankInit(fbnk,snum);

	begin = fbnk->base;
	for(i=0;i<fbnk->sectorcnt;i++,snum++) {
		int	ssize;

		ssize = SectorSizes800B[i];
		fbnk->sectors[i].snum = snum;
		fbnk->sectors[i].size = ssize;
		fbnk->sectors[i].begin = begin;
		fbnk->sectors[i].end = fbnk->sectors[i].begin + ssize- 1;
		fbnk->sectors[i].protected = 0;
		begin += ssize;
	}

	/* Assume monitor resides in bank0 and that it uses the first 128K */
	/* of code space (actually it uses much less, but different options */
	/* within the monitor are configurable, so assume worst case). */
	/* These "monitor-owned" sectors should be marked as protected. */
	size = 0;
	for (i=0;((i<fbnk->sectorcnt)&(size<FLASH_PROTECT_SIZE));i++) {
		fbnk->sectors[i].snum = i;
		fbnk->sectors[i].protected = 1;
		size += fbnk->sectors[i].size;
	}
	/* Additional flash memory banks would be initialized here... */
	return(0);
}

int
FlashWidthNotSupported(fdev)
struct	flashinfo *fdev;
{
	printf("Flashops for %d-bit width not supported.\n",fdev->width*8);
	return(0);
}
#endif
