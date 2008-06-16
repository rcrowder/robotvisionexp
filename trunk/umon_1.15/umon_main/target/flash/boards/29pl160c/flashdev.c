/* This file contains all of the flash support code that does not need
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
#include "stddefs.h"
#include "genlib.h"
#include "flashdev.h"
#include "flash.h"

/* FlashXXXFbuf[]:
 * These arrays will contain the flash operation function that is executing.
 * Recall that to operate on the flash, you cannot be executing out of it.
 * The flash functions are copied here, then executed through the function
 * pointer flashfunc which is set to point to FlashFunc.
 */
ulong    FlashTypeFbuf[FLASHFUNCSIZE];
ulong    FlashEraseFbuf[FLASHFUNCSIZE];
ulong    FlashWriteFbuf[FLASHFUNCSIZE];
ulong    FlashEwriteFbuf[FLASHFUNCSIZE];

/* FlashBank[]:
 * This structure contains all of the information that must be made available
 * to the various flash operation commands.  It is initialized by flashtype()
 * and used thereafter by the other operations.
 */
struct	flashinfo FlashBank[FLASHBANKS];

/* FlashNamId[]:
 * Used to correlate between the ID and a string representing the name
 * of the flash device. 
 */
struct flashdesc FlashNamId[] = {
		{ AMD29PL160C,	"AMD-29PL160C" }, 
		{ 0,	(char *)0 },
};

/* SectorSizes160C:
 *	There are a total of 11 sectors for this part.  This table reflects the
 *	size of each sector based on the fact that this configuration is for a
 *	x16 configuration.
 */
int	SectorSizes160C[] = {
		0x4000,  0x2000,  0x2000,  0x38000, 0x40000,
		0x40000, 0x40000, 0x40000, 0x40000, 0x40000,
		0x40000
};

struct sectorinfo sinfo160[sizeof(SectorSizes160C)/sizeof(int)];

/* FlashInit():
 * Initialize data structures for each bank of flash...
 */
int
FlashInit()
{
	int	i,	snum, fid;
	uchar	*begin;
	struct	flashinfo *fbnk;

	snum = 0;
	FlashCurrentBank = 0;

	/* Copy functions to ram space... */
	/* Note that this MUST be done when cache is disabled to assure that */
	/* the RAM is occupied by the designated block of code. */

	if (flashopload((ulong *)Flashtype16,(ulong *)EndFlashtype16,
		FlashTypeFbuf,sizeof(FlashTypeFbuf)) < 0)
		return(-1);
	if (flashopload((ulong *)Flasherase16,(ulong *)EndFlasherase16,
		FlashEraseFbuf,sizeof(FlashEraseFbuf)) < 0)
		return(-1);
	if (flashopload((ulong *)Flashewrite16,(ulong *)EndFlashewrite16,
		FlashEwriteFbuf,sizeof(FlashEwriteFbuf)) < 0)
		return(-1);
	if (flashopload((ulong *)Flashwrite16,(ulong *)EndFlashwrite16,
		FlashWriteFbuf,sizeof(FlashWriteFbuf)) < 0)
		return(-1);

	fbnk = &FlashBank[0];
	fbnk->id = AMD29PL160C;
	fbnk->base = (unsigned char *)FLASH_BANK0_BASE_ADDR;
	fbnk->end = fbnk->base + 0x1fffff;
	fbnk->sectorcnt = sizeof(SectorSizes160C)/sizeof(int);
	fbnk->width = FLASH_BANK0_WIDTH;
	fbnk->fltype = (int(*)())FlashTypeFbuf;			/* flashtype(). */
	fbnk->flerase = (int(*)())FlashEraseFbuf;		/* flasherase(). */
	fbnk->flwrite = (int(*)())FlashWriteFbuf;		/* flashwrite(). */
	fbnk->flewrite = (int(*)())FlashEwriteFbuf;		/* flashewrite(). */
	fbnk->sectors = sinfo160;

	/* The AMD29PL160C is the only device expected in this system, so
	 * there is no need to switch on the device type to configure the
	 * appropriate device; however, it is worth it to verify that the
	 * id of the device is what we expect.  This is a good sanity test
	 * to verify that write-access to the flash is working properly.
	 */
	fid = flashtype(fbnk);
	if (fid != AMD29PL160C)
		printf("Flashinit(): bad flashid (0x%lx)\n",fid);

	begin = fbnk->base;
	for(i=0;i<fbnk->sectorcnt;i++,snum++) {
		int	ssize;

		ssize = SectorSizes160C[i];
		fbnk->sectors[i].snum = snum;
		fbnk->sectors[i].size = ssize;
		fbnk->sectors[i].begin = begin;
		fbnk->sectors[i].end =
		    fbnk->sectors[i].begin + ssize - 1;
		fbnk->sectors[i].protected = 0;
		begin += ssize;
	}

	sectorProtect(FLASH_PROTECT_RANGE,1);

	/* Additional flash memory banks would be initialized here... */
	return(0);
}

#endif
