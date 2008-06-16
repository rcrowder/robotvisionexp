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
#include "genlib.h"

typedef unsigned char uchar;
typedef unsigned short ushort;
typedef unsigned long ulong;
typedef volatile unsigned char vuchar;
typedef volatile unsigned short vushort;
typedef volatile unsigned long vulong;
typedef volatile unsigned int vuint;
typedef volatile int vint;

extern int Flashlock16();
extern int EndFlashlock16();
extern int Flashtype16();
extern int EndFlashtype16();
extern int Flashwrite16();
extern int EndFlashwrite16();
extern int Flashewrite16();
extern int EndFlashewrite16();
extern int Flasherase16();
extern int EndFlasherase16();

extern int		FlashProtectWindow;
extern int		FlashCurrentBank;
extern int		flashopload(), flashtype();
extern struct	flashinfo FlashBank[FLASHBANKS];

/* FlashXXXFbuf[]:
   These arrays will contain the flash operation function that is executing.
   Recall that to operate on the flash, you cannot be executing out of it.
   The flash functions are copied here, then executed through the function
   pointer flashfunc which is set to point to FlashFunc.
*/
ulong    FlashLockFbuf[FLASHFUNCSIZE];
ulong    FlashTypeFbuf[FLASHFUNCSIZE];
ulong    FlashEraseFbuf[FLASHFUNCSIZE];
ulong    FlashWriteFbuf[FLASHFUNCSIZE];
ulong    FlashEwriteFbuf[FLASHFUNCSIZE];

/* FlashNamId[]:
   Used to correlate between the ID and a string representing the name
   of the flash device.
*/
struct flashdesc FlashNamId[] = {
		{ INTEL28F800B,	"INTEL-28F800B" },
		{ INTEL28F160B,	"INTEL-28F160B" },
		{ INTEL28F320B,	"INTEL-28F320B" },
		{ 0,				0 },
};

int	SectorSizes800B[] = {	/* 23 sectors */
	0x2000, 0x2000, 0x2000, 0x2000, 0x2000, 0x2000, 0x2000, 0x2000,
	0x10000, 0x10000, 0x10000, 0x10000, 0x10000, 0x10000, 0x10000, 0x10000,
	0x10000, 0x10000, 0x10000, 0x10000, 0x10000, 0x10000, 0x10000
};

int	SectorSizes160B[] = {	/* 39 sectors */
	0x2000, 0x2000, 0x2000, 0x2000, 0x2000, 0x2000, 0x2000, 0x2000,
	0x10000, 0x10000, 0x10000, 0x10000, 0x10000, 0x10000, 0x10000, 0x10000,
	0x10000, 0x10000, 0x10000, 0x10000, 0x10000, 0x10000, 0x10000, 0x10000,
	0x10000, 0x10000, 0x10000, 0x10000, 0x10000, 0x10000, 0x10000, 0x10000,
	0x10000, 0x10000, 0x10000, 0x10000, 0x10000, 0x10000, 0x10000
};

int	SectorSizes320B[] = {	/* 71 sectors */
	0x2000, 0x2000, 0x2000, 0x2000, 0x2000, 0x2000, 0x2000, 0x2000,
	0x10000, 0x10000, 0x10000, 0x10000, 0x10000, 0x10000, 0x10000, 0x10000,
	0x10000, 0x10000, 0x10000, 0x10000, 0x10000, 0x10000, 0x10000, 0x10000,
	0x10000, 0x10000, 0x10000, 0x10000, 0x10000, 0x10000, 0x10000, 0x10000,
	0x10000, 0x10000, 0x10000, 0x10000, 0x10000, 0x10000, 0x10000, 0x10000,
	0x10000, 0x10000, 0x10000, 0x10000, 0x10000, 0x10000, 0x10000, 0x10000,
	0x10000, 0x10000, 0x10000, 0x10000, 0x10000, 0x10000, 0x10000, 0x10000,
	0x10000, 0x10000, 0x10000, 0x10000, 0x10000, 0x10000, 0x10000, 0x10000,
	0x10000, 0x10000, 0x10000, 0x10000, 0x10000, 0x10000, 0x10000
};

/* FlashBankInit():
   Initialize flash structures and determine flash device type.
*/
int
FlashBankInit(struct flashinfo *fbnk,int snum)
{
	uchar	*saddr;
	int	i, *sizetable, msize;
	struct	sectorinfo *sinfotbl;

	sizetable = 0;

	/* Based on the flash bank ID returned, load a sector count and a */
	/* sector size-information table... */
	flashtype(fbnk);
	switch(fbnk->id) {
		case INTEL28F800B:
			fbnk->sectorcnt = 23;
			sizetable = SectorSizes800B;
			break;
		case INTEL28F160B:
			fbnk->sectorcnt = 39;
			sizetable = SectorSizes160B;
			break;
		case INTEL28F320B:
			fbnk->sectorcnt = 71;
			sizetable = SectorSizes320B;
			break;
		default:
			printf("Unrecognized flashid: 0x%04x\n",(ushort)(fbnk->id));
			return(-1);
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

/* FlashInit():
   Initialize data structures for each bank of flash...
*/
int
FlashInit()
{
	int	snum;
	struct	flashinfo *fbnk;

	snum = 0;
	FlashCurrentBank = 0;

	/* Copy functions to ram space... */
	/* Note that this MUST be done when cache is disabled to assure that */
	/* the RAM is occupied by the designated block of code. */

	if (flashopload((ulong *)FLASHLOCK,(ulong *)ENDFLASHLOCK,
		FlashLockFbuf,sizeof(FlashLockFbuf)) < 0)
		return(-1);
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
	fbnk->width = FLASH_BANK0_WIDTH;
	fbnk->fltype = (int(*)())FlashTypeFbuf;			/* flashtype(). */
	fbnk->flerase = (int(*)())FlashEraseFbuf;		/* flasherase(). */
	fbnk->flwrite = (int(*)())FlashWriteFbuf;		/* flashwrite(). */
	fbnk->flewrite = (int(*)())FlashEwriteFbuf;		/* flashewrite(). */
	fbnk->fllock = (int(*)())FlashLockFbuf;			/* flashlock(). */
	snum += FlashBankInit(fbnk,snum);

	/* This line should probably be removed once TFS is working: */
	flashlock(fbnk,ALL_SECTORS,FLASH_UNLOCK);

	sectorProtect(FLASH_PROTECT_RANGE,1);

	/* Additional flash memory banks would be initialized here... */
	return(0);
}

#endif
