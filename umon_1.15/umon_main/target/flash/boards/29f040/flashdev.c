/* This file contains all of the flash support code that does not need
   to be relocated to RAM.  Two separate files (flash.c and flashpic.c)
   are maintained because under certain compilers, they may need to be
   compiled with different options to be made position independent.

   NOTE: THESE FUNCTIONS ARE NOT RE-ENTRANT!!!  All of the FLASH routines
   assume they can copy themselves into the array FlashFunc[]; hence, only
   one operation can be active at any time.
*/
#include "cpu.h"
#include "flashdev.h"
#include "flash.h"
#include "config.h"
#include "genlib.h"
#include "stddefs.h"

#if INCLUDE_FLASH

extern int FLASHERASE();
extern int ENDFLASHERASE();
extern int FLASHWRITE();
extern int ENDFLASHWRITE();
extern int FLASHEWRITE();
extern int ENDFLASHEWRITE();
extern int FLASHTYPE();
extern int ENDFLASHTYPE();

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
   NOTE: This is now declared in common/monitor/flash.c
struct	flashinfo FlashBank[FLASHBANKS];
*/

/* FlashNamId[]:
   Used to correlate between the ID and a string representing the name
   of the flash device.
*/
struct flashdesc FlashNamId[] = {
	{ STM_29F040,	"STM-29F040" },		/* SGS-Thompson Microelectronics */
	{ STM_M29W040B,	"STM-M29W040B" },
	{ AMD_29F040,	"AMD-29F040" },		/* AMD */
	{ AMD_29F010,	"AMD-29F010" },
	{ AMD_29LV040,	"AMD-29LV040" }
};

struct sectorinfo sinfo040[8];

#ifdef FLASHRAM_BASE

int
ramSectors[] = {
	0x20000, 0x20000, 0x20000, 0x20000, 0x20000, 0x20000, 0x20000, 0x20000,
	0x20000, 0x20000, 0x20000, 0x20000, 0x20000, 0x20000, 0x20000, 0x20000
};

struct sectorinfo sinfoRAM[sizeof(ramSectors)/sizeof(int)];

#endif

int FlashBankInit(struct flashinfo *,int);

/* FlashInit():
   Initialize data structures for each bank of flash...
*/
int
FlashInit(void)
{
	int		snum;
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
	fbnk->width = FLASH_BANK0_WIDTH;
	fbnk->fltype = (int(*)())FlashTypeFbuf;			/* flashtype(). */
	fbnk->flerase = (int(*)())FlashEraseFbuf;		/* flasherase(). */
	fbnk->flwrite = (int(*)())FlashWriteFbuf;		/* flashwrite(). */
	fbnk->flewrite = (int(*)())FlashEwriteFbuf;		/* flashewrite(). */
	fbnk->sectors = sinfo040;
	snum += FlashBankInit(fbnk,snum);
	sectorProtect(FLASH_PROTECT_RANGE,1);

#ifdef FLASHRAM_BASE
#if FLASHBANKS != 2
#error: Must allocate bank for FLASHRAM
#endif	
	FlashRamInit(snum, sizeof(ramSectors)/sizeof(int),&FlashBank[1],
		sinfoRAM, ramSectors);
#endif
	return(0);
}


/* FlashBankInit():
   Initialize flash structures and determine flash device type.
*/
int
FlashBankInit(struct flashinfo *fbnk, int snum)
{
	int	i, ssize;


	flashtype(fbnk);
	switch(fbnk->id) {
		case AMD_29LV040:
		case STM_M29W040B:
		case STM_29F040:
		case AMD_29F040:
			fbnk->sectorcnt = 8;
			ssize = 0x10000 * fbnk->width;
			fbnk->end = fbnk->base + (0x80000 * fbnk->width) - 1;
			break;
		case AMD_29F010:
			fbnk->sectorcnt = 8;
			ssize = 0x4000 * fbnk->width;
			fbnk->end = fbnk->base + (0x20000 * fbnk->width) - 1;
			break;
		default:
			printf("Flash device id 0x%lx unknown\n", fbnk->id);
			return(-1);
	}
	for(i=0;i<fbnk->sectorcnt;i++) {
		fbnk->sectors[i].snum = snum+i;
		fbnk->sectors[i].size = ssize;
		fbnk->sectors[i].begin = fbnk->base + (i*ssize);
		fbnk->sectors[i].end = fbnk->sectors[i].begin + ssize - 1;
		fbnk->sectors[i].protected = 0;
	}
	return(8);
}

#endif
