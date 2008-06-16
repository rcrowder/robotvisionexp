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

extern int FLASHTYPE();
extern int ENDFLASHTYPE();
extern int FLASHWRITE();
extern int ENDFLASHWRITE();
extern int FLASHEWRITE();
extern int ENDFLASHEWRITE();
extern int FLASHERASE();
extern int ENDFLASHERASE();

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

/* FlashBank[]:
   This structure contains all of the information that must be made available
   to the various flash operation commands.  It is initialized by flashtype()
   and used thereafter by the other operations.
*/
struct	flashinfo FlashBank[FLASHBANKS];

/* FlashNamId[]:
   Used to correlate between the ID and a string representing the name
   of the flash device.  */
struct flashdesc FlashNamId[] = {
		{ AMD29LV160B,	"AMD-29LV160B" }, 
		{ AMD29LV160T,	"AMD-29LV160T" },
		{ 0,	(char *)0 },
};

/* SectorSizes160B:
	There are a total of 35 sectors for this part.  This table reflects the
	size of each sector based on the fact that this configuration is for a
	x32 configuration (2 devices are used, each in x16 mode).  */
int	SectorSizes160B[] = {
		0x8000,  0x4000,  0x4000,  0x10000, 0x20000,
		0x20000, 0x20000, 0x20000, 0x20000, 0x20000,
		0x20000, 0x20000, 0x20000, 0x20000, 0x20000,
		0x20000, 0x20000, 0x20000, 0x20000, 0x20000,
		0x20000, 0x20000, 0x20000, 0x20000, 0x20000,
		0x20000, 0x20000, 0x20000, 0x20000, 0x20000,
		0x20000, 0x20000, 0x20000, 0x20000, 0x20000,
};

struct sectorinfo sinfo160[35];

/* FlashInit():
   Initialize data structures for each bank of flash...
*/
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
	fbnk->id = AMD29LV160B;
	fbnk->base = (unsigned char *)FLASH_BANK0_BASE_ADDR;
	fbnk->end = fbnk->base + 0x3fffff;
	fbnk->sectorcnt = sizeof(SectorSizes160B)/sizeof(int);
	fbnk->width = FLASH_BANK0_WIDTH;
	fbnk->fltype = (int(*)())FlashTypeFbuf;			/* flashtype(). */
	fbnk->flerase = (int(*)())FlashEraseFbuf;		/* flasherase(). */
	fbnk->flwrite = (int(*)())FlashWriteFbuf;		/* flashwrite(). */
	fbnk->flewrite = (int(*)())FlashEwriteFbuf;		/* flashewrite(). */
	fbnk->sectors = sinfo160;

	begin = fbnk->base;
	
	for(i=0;i<fbnk->sectorcnt;i++,snum++) {
		int	ssize;

		ssize = SectorSizes160B[i];
		fbnk->sectors[i].snum = snum;
		fbnk->sectors[i].size = ssize;
		fbnk->sectors[i].begin = begin;
		fbnk->sectors[i].end =
		    fbnk->sectors[i].begin + ssize - 1;
		fbnk->sectors[i].protected = 0;
		begin += ssize;
	}

	/* Assume monitor resides in bank0 and that it uses the first 128K */
	/* of code space (actually it uses much less, but different options */
	/* within the monitor are configurable, so assume worst case). */
	/* These "monitor-owned" sectors should be marked as protected. */
	size = 0;
	for (i=0;((i<FlashBank[0].sectorcnt)&(size<FLASH_PROTECT_SIZE));i++) {
		FlashBank[0].sectors[i].protected = 1;
		size += FlashBank[0].sectors[i].size;
	}
	/* Additional flash memory banks would be initialized here... */
	return(0);
}

#endif
