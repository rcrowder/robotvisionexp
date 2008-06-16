/* flashdev.c:
 *	This code supports two different platforms:
 *	1.	29F040 boot flash with 2 by-32 banks of AMD29LV800 (4 devices)
 *		and an 8bit battery-backed ram device (DS1743W).  The 2 banks
 *		of AMD29LV800s are assumed to be in contiguous memory space.
 *		And use of BB or BT devices is supported as long as any one
 *		bank has the same set.
 *	2.	29F040 boot flash with 1 by-32 bank of AM29LV160DB (2 devices)
 *		and an 8-bit battery-backed ram device (DS1746WP).
 *
 *	For both platforms, the battery-backed ram device is configured to
 *	"look like flash" for use in TFS.
 */
#include "config.h"
#if INCLUDE_FLASH
#include "cpu.h"
#include "flashdev.h"
#include "flash.h"
#include "genlib.h"
#include "stddefs.h"

extern int	Flashtype29F040_8();
extern int	EndFlashtype29F040_8();
extern int	Flasherase29F040_8();
extern int	EndFlasherase29F040_8();
extern int	Flashwrite29F040_8();
extern int	EndFlashwrite29F040_8();
extern int	Flashewrite29F040_8();
extern int	EndFlashewrite29F040_8();

extern int	Flashtype29LV800B_32();
extern int	EndFlashtype29LV800B_32();
extern int	Flasherase29LV800B_32();
extern int	EndFlasherase29LV800B_32();
extern int	Flashwrite29LV800B_32();
extern int	EndFlashwrite29LV800B_32();
extern int	Flashewrite29LV800B_32();
extern int	EndFlashewrite29LV800B_32();

extern int	FlashCurrentBank;
extern int 	FlashProtectWindow;
extern int	NotUsed();
extern int	flashopload();
extern struct flashinfo FlashBank[FLASHBANKS];

/* FlashXXXFbuf[]:
 *	These arrays will contain the flash operation function that is executing.
 *	Recall that to operate on the flash, you cannot be executing out of it.
 *	The flash functions are copied here, then executed through the function
 *	pointers setup in FlashInit().
*/
ulong	FlashTypeFbuf040[FLASHFUNCSIZE];
ulong	FlashEraseFbuf040[FLASHFUNCSIZE];
ulong	FlashWriteFbuf040[FLASHFUNCSIZE];
ulong	FlashEwriteFbuf040[FLASHFUNCSIZE];
ulong	FlashTypeFbuf800[FLASHFUNCSIZE];
ulong	FlashEraseFbuf800[FLASHFUNCSIZE];
ulong	FlashWriteFbuf800[FLASHFUNCSIZE];
ulong	FlashEwriteFbuf800[FLASHFUNCSIZE+0x100];


/* FlashNamId[]:
 *	Used to correlate between the ID and a string representing the name
 *	of the flash device.
*/
struct flashdesc FlashNamId[] = {
	{ AMD_29F800B,		"AMD-29F800B" },
	{ AMD_29F800T,		"AMD-29F800T" },
	{ STM_29F040,		"STM-29F040" },
	{ STM_M29W040B,		"STM-M29W040B" },
	{ AMD_29F040,		"AMD-29F040" },
	{ AMD_29F010,		"AMD-29F010" },
	{ AMD_29LV800BT32,	"AMD-29LV800BTx2" },
	{ AMD_29LV800BB32,	"AMD-29LV800BBx2" },
	{ AMD_29LV160BB32,	"AMD-29LV160BBx2" },
	{ FLASHRAM,			"Battery-backed SRAM " }
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

int	SectorSizes29LV800BB_32[] = {
		0x8000,		0x4000,		0x4000,		0x10000,
		0x20000, 	0x20000,	0x20000,	0x20000,
		0x20000, 	0x20000,	0x20000,	0x20000,
		0x20000, 	0x20000,	0x20000,	0x20000,
		0x20000, 	0x20000,	0x20000
};

int	SectorSizes29LV800BT_32[] = {
		0x20000, 	0x20000,	0x20000,	0x20000,
		0x20000, 	0x20000,	0x20000,	0x20000,
		0x20000, 	0x20000,	0x20000,	0x20000,
		0x20000, 	0x20000,	0x20000,	0x10000,
		0x4000,		0x4000,		0x8000
};

int	SectorSizes29LV160BB_32[] = {
		0x8000,		0x4000,		0x4000,		0x10000,
		0x20000, 	0x20000,	0x20000,	0x20000,
		0x20000, 	0x20000,	0x20000,	0x20000,
		0x20000, 	0x20000,	0x20000,	0x20000,
		0x20000, 	0x20000,	0x20000,	0x20000,
		0x20000, 	0x20000,	0x20000,	0x20000,
		0x20000, 	0x20000,	0x20000,	0x20000,
		0x20000, 	0x20000,	0x20000,	0x20000,
		0x20000, 	0x20000,	0x20000,
};

#if PLATFORM_HDIRD | PLATFORM_TXA
int	SectorSizesDS1743[] = {
		0x100, 0x400, 0x400, 0x400, 0x400, 0x700, 0x800-64
};
struct sectorinfo	sinfoRAM[sizeof(SectorSizesDS1743)/sizeof(int)];
#else
int	SectorSizesDS1746[] = {
		0x1000,	0x1000,	0x1000, 0x1000, 0x1000, 0x1000, 0x1000, 0x1000,
		0x1000,	0x1000,	0x1000, 0x1000, 0x1000, 0x1000, 0x1000, 0x1000,
		0x1000,	0x1000,	0x1000, 0x1000, 0x1000, 0x1000, 0x1000, 0x1000,
		0x1000,	0x1000,	0x1000, 0x1000, 0x1000, 0x1000, 0x2000-64
};
struct sectorinfo	sinfoRAM[sizeof(SectorSizesDS1746)/sizeof(int)];
#endif

struct sectorinfo	sinfo800a[sizeof(SectorSizes29LV800BB_32)/sizeof(int)];
struct sectorinfo	sinfo800b[sizeof(SectorSizes29LV800BB_32)/sizeof(int)];
struct sectorinfo	sinfo160[sizeof(SectorSizes29LV160BB_32)/sizeof(int)];
struct sectorinfo	sinfo040[sizeof(SectorSizes29F040_8)/sizeof(int)];


/* FlashBankInit():
 *	Assuming a device is of a particular family, this function can 
 *	be used to initialize some of the device-specific information based
 *	on the device-id read in.
 *	This idea is typically used when we are using the same code to
 *	deal with "similar" devices... For this case the 28LV800BB or 28LV800BT
 *	devices.
 */
int
FlashBankInit(struct flashinfo *fbnk, int snum)
{
    int	    i;
    uchar   *begin;
	struct	sectorinfo *sinfo;

    flashtype(fbnk);
    switch(fbnk->id) {
	    case AMD_29LV800BB32:
			fbnk->sectorcnt = 19;
	    	begin = fbnk->base;
			sinfo = fbnk->sectors;
		    for(i=0;i<fbnk->sectorcnt;i++,snum++,sinfo++) {
				sinfo->snum = snum;
				sinfo->size = SectorSizes29LV800BB_32[i];
				sinfo->begin = begin;
				sinfo->end = sinfo->begin + sinfo->size - 1;
				sinfo->protected = 0;
				begin += sinfo->size;
		    }
			break;
	    case AMD_29LV800BT32:
			fbnk->sectorcnt = 19;
	    	begin = fbnk->base;
			sinfo = fbnk->sectors;
		    for(i=0;i<fbnk->sectorcnt;i++,snum++,sinfo++) {
				sinfo->snum = snum;
				sinfo->size = SectorSizes29LV800BT_32[i];
				sinfo->begin = begin;
				sinfo->end = sinfo->begin + sinfo->size - 1;
				sinfo->protected = 0;
				begin += sinfo->size;
		    }
			break;
	    case AMD_29LV800BBBT:
	    case AMD_29LV800BTBB:
			printf("ERROR: BT/BB (0x%lx) devices in same flash bank.\n",
				fbnk->id);
			return(-1);
	    default:
			printf("ERROR: Flash type 0x%lx unrecognized\n",fbnk->id);
			return(-1);
    }
    return (snum);
}

/* FlashInit():
 *	Initialize data structures for each bank of flash...
*/
int
FlashInit(void)
{
    int		i, snum;
    uchar	*begin;
    struct	flashinfo *fbnk;

    snum = 0;
    FlashCurrentBank = 0;

	/* Copy functions to ram space...
	 * Note that this MUST be done when cache is disabled to assure that
	 * the RAM is occupied by the designated block of code.
	 */

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

	if (flashopload((ulong *)Flashtype29LV800B_32,
		(ulong *)EndFlashtype29LV800B_32,
		FlashTypeFbuf800,sizeof(FlashTypeFbuf800)) < 0)
		return(-1);
	if (flashopload((ulong *)Flasherase29LV800B_32,
		(ulong *)EndFlasherase29LV800B_32,
		FlashEraseFbuf800,sizeof(FlashEraseFbuf800)) < 0)
		return(-1);
	if (flashopload((ulong *)Flashewrite29LV800B_32,
		(ulong *)EndFlashewrite29LV800B_32,
		FlashEwriteFbuf800,sizeof(FlashEwriteFbuf800)) < 0)
		return(-1);
	if (flashopload((ulong *)Flashwrite29LV800B_32,
		(ulong *)EndFlashwrite29LV800B_32,
		FlashWriteFbuf800,sizeof(FlashWriteFbuf800)) < 0)
		return(-1);

	/*
	 * Set up BANK 0:
	 * The device is a 29F040.
	 */
    fbnk = &FlashBank[0];
    fbnk->id = AMD_29F040;
    fbnk->base = (uchar *)FLASH_BANK0_BASE_ADDR;
    fbnk->end = fbnk->base + (FLASH_BANK0_SIZE-1);
    fbnk->sectorcnt = 8;
    fbnk->width = 1;
	/* Establish flash operation function pointers: */
    fbnk->fltype = (int(*)())FlashTypeFbuf040;
    fbnk->flerase = (int(*)())FlashEraseFbuf040;
    fbnk->flwrite = (int(*)())FlashWriteFbuf040;
    fbnk->flewrite = (int(*)())FlashEwriteFbuf040;
    fbnk->sectors = sinfo040;
    begin = fbnk->base;
    for(i=0;i<fbnk->sectorcnt;i++,snum++) {
		sinfo040[i].snum = snum;
		sinfo040[i].size = SectorSizes29F040_8[i];
		sinfo040[i].begin = begin;
		sinfo040[i].end = sinfo040[i].begin + sinfo040[i].size - 1;
		begin += SectorSizes29F040_8[i];
	}

	/* Protect sectors designated by FLASH_PROTECT_RANGE: */
	sectorProtect(FLASH_PROTECT_RANGE,1);

#if PLATFORM_HDIRD | PLATFORM_TXA
	/*
	 * Set up HDIRD/TXA BANK 1:
	 * The device may be a 29LV800BB or 29LV800BT
	 */
    fbnk = &FlashBank[1];
	/* Establish flash-op function pointers: */
    fbnk->fltype = (int(*)())FlashTypeFbuf800;
    fbnk->flerase = (int(*)())FlashEraseFbuf800;
    fbnk->flwrite = (int(*)())FlashWriteFbuf800;
    fbnk->flewrite = (int(*)())FlashEwriteFbuf800;
	/* Establish flash bank characteristics: */
    fbnk->base = (uchar *)FLASH_BANK1_BASE_ADDR;
    fbnk->end = fbnk->base + (FLASH_BANK1_SIZE-1);
    fbnk->width = FLASH_BANK1_WIDTH;
    fbnk->sectors = sinfo800a;
	snum = FlashBankInit(fbnk,snum);
	if (snum == -1)
		return(-1);

	/*
	 * Set up HDIRD/TXA BANK 2:
	 * The device may be a 29LV800BB or 29LV800BT
	 */
    fbnk = &FlashBank[2];
	/* Establish flash-op function pointers: */
    fbnk->fltype = (int(*)())FlashTypeFbuf800;
    fbnk->flerase = (int(*)())FlashEraseFbuf800;
    fbnk->flwrite = (int(*)())FlashWriteFbuf800;
    fbnk->flewrite = (int(*)())FlashEwriteFbuf800;
	/* Establish flash bank characteristics: */
    fbnk->base = (uchar *)FLASH_BANK2_BASE_ADDR;	/* Base address of bank. */
    fbnk->end = fbnk->base + (FLASH_BANK2_SIZE-1);	/* End address of bank. */
    fbnk->width = FLASH_BANK2_WIDTH;				/* Width (in bytes). */
    fbnk->sectors = sinfo800b;				/* Pointer to sector size table. */
	snum = FlashBankInit(fbnk,snum);
	if (snum == -1)
		return(-1);

	/*
	 * Set up HDIRD/TXA BANK 3:
	 * This is the battery backed ram for HDIRD & TXA platforms.
	 */
	snum = FlashRamInit(snum, sizeof(SectorSizesDS1743)/sizeof(int),
		&FlashBank[3], sinfoRAM, SectorSizesDS1743);
#else
	/*
	 * Set up NETDEC BANK 1:
	 * The device is a 29LV160BB
	 */
    fbnk = &FlashBank[1];
    fbnk->id = AMD_29LV160BB32;						/* Device id. */
    fbnk->base = (uchar *)FLASH_BANK1_BASE_ADDR;	/* Base address of bank. */
    fbnk->end = fbnk->base + (FLASH_BANK1_SIZE-1);	/* End address of bank. */
    fbnk->sectorcnt = sizeof(SectorSizes29LV160BB_32)/sizeof(int);
    fbnk->width = FLASH_BANK1_WIDTH;				/* Width (in bytes). */
    fbnk->fltype = (int(*)())FlashTypeFbuf800;	
    fbnk->flerase = (int(*)())FlashEraseFbuf800;
    fbnk->flwrite = (int(*)())FlashWriteFbuf800;
    fbnk->flewrite = (int(*)())FlashEwriteFbuf800;
    fbnk->sectors = sinfo160;		/* Pointer to sector size table. */
    begin = fbnk->base;
    for(i=0;i<fbnk->sectorcnt;i++,snum++) {
		sinfo160[i].snum = snum;
		sinfo160[i].size = SectorSizes29LV160BB_32[i];
		sinfo160[i].begin = begin;
		sinfo160[i].end = sinfo160[i].begin + sinfo160[i].size - 1;
		sinfo160[i].protected = 0;
		begin += SectorSizes29LV160BB_32[i];
    }

	/*
	 * Set up NETDEC BANK 3:
	 * This is the battery backed ram for NETDEC platform.
	 */
	snum = FlashRamInit(snum, sizeof(SectorSizesDS1746)/sizeof(int),
		&FlashBank[2], sinfoRAM, SectorSizesDS1746);
#endif

    return(0);
}

#endif
