/* 
 * flashdev.c
 *
 * Support for Intel Strata Flash, in 2 x 16 configuration.
 *
 * by Thomas E. Arvanitis (tharvan@inaccessnetworks.com)
 *
 * No lock support.
 *
 * This file contains all of the flash support code that does not need
 * to be relocated to RAM.  Two separate files (flash.c and flashpic.c)
 * are maintained because under certain compilers, they may need to be
 * compiled with different options to be made position independent.
 *
 * NOTE: THESE FUNCTIONS ARE NOT RE-ENTRANT!!!  All of the FLASH routines
 * assume they can copy themselves into the array FlashFunc[]; hence, only
 * one operation can be active at any time.
*/
#include "config.h"
#if INCLUDE_FLASH
#include "cpu.h"
#include "flashdev.h"
#include "flash.h"
#include "genlib.h"
#include "stddefs.h"

extern int Flashlock32();
extern int EndFlashlock32();
extern int Flashtype32();
extern int EndFlashtype32();
extern int Flashwrite32();
extern int EndFlashwrite32();
extern int Flashewrite32();
extern int EndFlashewrite32();
extern int Flasherase32();
extern int EndFlasherase32();

extern int		FlashProtectWindow;
extern int		FlashCurrentBank;
extern int		flashopload(), flashtype();
extern struct	flashinfo FlashBank[FLASHBANKS];

#ifndef NO_PIC

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

#endif


/* 
 * FlashNamId[]:
 * Used to correlate between the ID and a string representing the name
 * of the flash device.
 */
struct flashdesc FlashNamId[] = {
	{ INTEL28F128J,	"INTEL-28F128J" },
#ifdef FLASHRAM_BASE
	{ FLASHRAM,			"FLASH-RAM" },
#endif
	{ 0, 0 },
};

int	SectorSizes128J[] = {	/* There are 128 sectors*/ 
	0x40000, 0x40000, 0x40000, 0x40000, 0x40000, 0x40000, 0x40000, 0x40000,
#ifndef PLATFORM_PB1500
	0x40000, 0x40000, 0x40000, 0x40000, 0x40000, 0x40000, 0x40000, 0x40000,
	0x40000, 0x40000, 0x40000, 0x40000, 0x40000, 0x40000, 0x40000, 0x40000,
	0x40000, 0x40000, 0x40000, 0x40000, 0x40000, 0x40000, 0x40000, 0x40000,
	0x40000, 0x40000, 0x40000, 0x40000, 0x40000, 0x40000, 0x40000, 0x40000,
	0x40000, 0x40000, 0x40000, 0x40000, 0x40000, 0x40000, 0x40000, 0x40000,
	0x40000, 0x40000, 0x40000, 0x40000, 0x40000, 0x40000, 0x40000, 0x40000,
	0x40000, 0x40000, 0x40000, 0x40000, 0x40000, 0x40000, 0x40000, 0x40000,
	0x40000, 0x40000, 0x40000, 0x40000, 0x40000, 0x40000, 0x40000, 0x40000,
	0x40000, 0x40000, 0x40000, 0x40000, 0x40000, 0x40000, 0x40000, 0x40000,
	0x40000, 0x40000, 0x40000, 0x40000, 0x40000, 0x40000, 0x40000, 0x40000,
	0x40000, 0x40000, 0x40000, 0x40000, 0x40000, 0x40000, 0x40000, 0x40000,
	0x40000, 0x40000, 0x40000, 0x40000, 0x40000, 0x40000, 0x40000, 0x40000,
	0x40000, 0x40000, 0x40000, 0x40000, 0x40000, 0x40000, 0x40000, 0x40000,
	0x40000, 0x40000, 0x40000, 0x40000, 0x40000, 0x40000, 0x40000, 0x40000,
	0x40000, 0x40000, 0x40000, 0x40000, 0x40000, 0x40000, 0x40000, 0x40000,
#endif
};

#ifdef FLASHRAM_BASE

int
ramSectors[] = {
	0x10000, 0x10000, 0x10000, 0x10000, 0x10000, 0x10000, 0x10000, 0x10000,
	0x10000, 0x10000, 0x10000, 0x10000, 0x10000, 0x10000, 0x10000, 0x10000,
};

struct sectorinfo sinfoRAM[sizeof(ramSectors)/sizeof(int)];

#endif

/* 
 * FlashBankInit():
 * Initialize flash structures and determine flash device type.
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
		case INTEL28F128J:
			fbnk->sectorcnt = (sizeof(SectorSizes128J)/sizeof(int));
			sizetable = SectorSizes128J;
			break;
		default:
			printf("Unrecognized flashid: 0x%08lx\n",fbnk->id);
			return(-1);
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

/* 
 * FlashInit():
 * Initialize data structures for each bank of flash...
 */
int
FlashInit()
{
	int		snum;
	struct	flashinfo *fbnk;

	snum = 0;
	FlashCurrentBank = 0;

#ifndef NO_PIC

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

#endif

	fbnk = &FlashBank[0];
	fbnk->base = (unsigned char *)FLASH_BANK0_BASE_ADDR;
	fbnk->width = FLASH_BANK0_WIDTH;
#ifndef NO_PIC
	fbnk->fltype = (int(*)())FlashTypeFbuf;		/* flashtype(). */
	fbnk->flerase = (int(*)())FlashEraseFbuf;	/* flasherase(). */
	fbnk->flwrite = (int(*)())FlashWriteFbuf;	/* flashwrite(). */
	fbnk->flewrite = (int(*)())FlashEwriteFbuf;	/* flashewrite(). */
#else
	fbnk->fltype = FLASHTYPE;
	fbnk->flerase = FLASHERASE;
	fbnk->flwrite = FLASHWRITE;
	fbnk->flewrite = FLASHEWRITE;
#endif

/*	fbnk->fllock = (int(*)())FlashLockFbuf;	flashlock(). */
	snum += FlashBankInit(fbnk,snum);

	sectorProtect(FLASH_PROTECT_RANGE,1);

#ifdef FLASHRAM_BASE
	FlashRamInit(snum, sizeof(ramSectors)/sizeof(int),
		&FlashBank[FLASHRAM_BANKNUM], sinfoRAM, ramSectors);
#endif
	return(0);
}

#endif /* of INCLUDE_FLASH */
