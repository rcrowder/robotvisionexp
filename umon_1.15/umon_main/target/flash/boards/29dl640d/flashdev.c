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
#include "stddefs.h"
#include "genlib.h"
#include "flashdev.h"
#include "flash.h"

extern int Flashtype();
extern int EndFlashtype();
extern int Flashwrite();
extern int EndFlashwrite();
extern int Flashewrite();
extern int EndFlashewrite();
extern int Flasherase();
extern int EndFlasherase();

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
		{ AMD29DL640D,	"AMD-29DL640D" },
		{ 0, (char *)0 },
};

int	SectorSizes640D[] = {
	0x2000,  0x2000,  0x2000,  0x2000,  0x2000,  0x2000,  0x2000,  0x2000,
	0x10000, 0x10000, 0x10000, 0x10000, 0x10000, 0x10000, 0x10000, 0x10000,
	0x10000, 0x10000, 0x10000, 0x10000, 0x10000, 0x10000, 0x10000, 0x10000,
	0x10000, 0x10000, 0x10000, 0x10000, 0x10000, 0x10000, 0x10000, 0x10000,
	0x10000, 0x10000, 0x10000, 0x10000, 0x10000, 0x10000, 0x10000, 0x10000,
	0x10000, 0x10000, 0x10000, 0x10000, 0x10000, 0x10000, 0x10000, 0x10000,
	0x10000, 0x10000, 0x10000, 0x10000, 0x10000, 0x10000, 0x10000, 0x10000,
	0x10000, 0x10000, 0x10000, 0x10000, 0x10000, 0x10000, 0x10000, 0x10000,
	0x10000, 0x10000, 0x10000, 0x10000, 0x10000, 0x10000, 0x10000, 0x10000,
	0x10000, 0x10000, 0x10000, 0x10000, 0x10000, 0x10000, 0x10000, 0x10000,
	0x10000, 0x10000, 0x10000, 0x10000, 0x10000, 0x10000, 0x10000, 0x10000,
	0x10000, 0x10000, 0x10000, 0x10000, 0x10000, 0x10000, 0x10000, 0x10000,
	0x10000, 0x10000, 0x10000, 0x10000, 0x10000, 0x10000, 0x10000, 0x10000,
	0x10000, 0x10000, 0x10000, 0x10000, 0x10000, 0x10000, 0x10000, 0x10000,
	0x10000, 0x10000, 0x10000, 0x10000, 0x10000, 0x10000, 0x10000, 0x10000,
	0x10000, 0x10000, 0x10000, 0x10000, 0x10000, 0x10000, 0x10000, 0x10000,
	0x10000, 0x10000, 0x10000, 0x10000, 0x10000, 0x10000,
	0x2000,  0x2000,  0x2000,  0x2000,  0x2000,  0x2000,  0x2000,  0x2000
};

struct sectorinfo sinfo640[sizeof(SectorSizes640D)/sizeof(int)];

/* FlashInit():
   Initialize data structures for each bank of flash...
*/
int
FlashInit()
{
	int	i,	snum;
	uchar	*begin;
	struct	flashinfo *fbnk;

	snum = 0;
	FlashCurrentBank = 0;

	/* Copy functions to ram space... */
	/* Note that this MUST be done when cache is disabled to assure that */
	/* the RAM is occupied by the designated block of code. */

	if (flashopload((ulong *)Flashtype,(ulong *)EndFlashtype,
		FlashTypeFbuf,sizeof(FlashTypeFbuf)) < 0)
		return(-1);
	if (flashopload((ulong *)Flasherase,(ulong *)EndFlasherase,
		FlashEraseFbuf,sizeof(FlashEraseFbuf)) < 0)
		return(-1);
	if (flashopload((ulong *)Flashewrite,(ulong *)EndFlashewrite,
		FlashEwriteFbuf,sizeof(FlashEwriteFbuf)) < 0)
		return(-1);
	if (flashopload((ulong *)Flashwrite,(ulong *)EndFlashwrite,
		FlashWriteFbuf,sizeof(FlashWriteFbuf)) < 0)
		return(-1);

	fbnk = &FlashBank[0];
	fbnk->base = (unsigned char *)FLASH_BANK0_BASE_ADDR;
	fbnk->end = fbnk->base + FLASH_BANK0_SIZE - 1;
	fbnk->sectorcnt = (sizeof(SectorSizes640D)/sizeof(int));
	fbnk->width = FLASH_BANK0_WIDTH;
	fbnk->fltype = (int(*)())FlashTypeFbuf;			/* flashtype(). */
	fbnk->flerase = (int(*)())FlashEraseFbuf;		/* flasherase(). */
	fbnk->flwrite = (int(*)())FlashWriteFbuf;		/* flashwrite(). */
	fbnk->flewrite = (int(*)())FlashEwriteFbuf;		/* flashewrite(). */
	fbnk->sectors = sinfo640;
	fbnk->id = flashtype(fbnk);

	begin = fbnk->base;
	for(i=0;i<fbnk->sectorcnt;i++,snum++) {
		int	ssize;

		ssize = SectorSizes640D[i];
		fbnk->sectors[i].snum = snum;
		fbnk->sectors[i].size = ssize;
		fbnk->sectors[i].begin = begin;
		fbnk->sectors[i].end = fbnk->sectors[i].begin + ssize - 1;
		fbnk->sectors[i].protected = 0;
		begin += ssize;
	}

	sectorProtect(FLASH_PROTECT_RANGE,1);

	/* Additional flash memory banks would be initialized here... */
	return(0);
}

#endif
