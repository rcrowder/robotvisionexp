/* 
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

typedef unsigned char uchar;
typedef unsigned short ushort;
typedef unsigned long ulong;
typedef volatile unsigned char vuchar;
typedef volatile unsigned short vushort;
typedef volatile unsigned long vulong;
typedef volatile unsigned int vuint;
typedef volatile int vint;

extern int Flashtype();
extern int EndFlashtype();
extern int Flashwrite();
extern int EndFlashwrite();
extern int Flashewrite();
extern int EndFlashewrite();
extern int Flasherase();
extern int EndFlasherase();

extern int      FlashProtectWindow;
extern int      FlashCurrentBank;
extern int      flashopload(), flashtype();
extern struct   flashinfo FlashBank[FLASHBANKS];

#ifndef NO_PIC
/*
 * flashpic.o is really PIC
 */

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

#else /* NO_PIC */
/*
 * can't rely on compiler producing PIC.
 * use poor man's relocation scheme
 */

ulong    *FlashTypeFbuf;
ulong    *FlashEraseFbuf;
ulong    *FlashWriteFbuf;
ulong    *FlashEwriteFbuf;

#endif /* of NO_PIC */

/* 
 * FlashNamId[]:
 * Used to correlate between the ID and a string representing the name
 * of the flash device.
 */
struct flashdesc FlashNamId[] = {
	{ AMD29LV640D,	"AMD29LV640D" },
#ifdef FLASHRAM_BASE
	{ FLASHRAM,		"FLASH-RAM" },
#endif
	{ 0, 0 },
};

int SectorSizesLV640D[] = {   /* There are 128 sectors*/ 
    0x20000, 0x20000, 0x20000, 0x20000, 0x20000, 0x20000, 0x20000, 0x20000,
    0x20000, 0x20000, 0x20000, 0x20000, 0x20000, 0x20000, 0x20000, 0x20000,
    0x20000, 0x20000, 0x20000, 0x20000, 0x20000, 0x20000, 0x20000, 0x20000,
    0x20000, 0x20000, 0x20000, 0x20000, 0x20000, 0x20000, 0x20000, 0x20000,
    0x20000, 0x20000, 0x20000, 0x20000, 0x20000, 0x20000, 0x20000, 0x20000,
    0x20000, 0x20000, 0x20000, 0x20000, 0x20000, 0x20000, 0x20000, 0x20000,
    0x20000, 0x20000, 0x20000, 0x20000, 0x20000, 0x20000, 0x20000, 0x20000,
    0x20000, 0x20000, 0x20000, 0x20000, 0x20000, 0x20000, 0x20000, 0x20000,
    0x20000, 0x20000, 0x20000, 0x20000, 0x20000, 0x20000, 0x20000, 0x20000,
    0x20000, 0x20000, 0x20000, 0x20000, 0x20000, 0x20000, 0x20000, 0x20000,
    0x20000, 0x20000, 0x20000, 0x20000, 0x20000, 0x20000, 0x20000, 0x20000,
    0x20000, 0x20000, 0x20000, 0x20000, 0x20000, 0x20000, 0x20000, 0x20000,
    0x20000, 0x20000, 0x20000, 0x20000, 0x20000, 0x20000, 0x20000, 0x20000,
    0x20000, 0x20000, 0x20000, 0x20000, 0x20000, 0x20000, 0x20000, 0x20000,
    0x20000, 0x20000, 0x20000, 0x20000, 0x20000, 0x20000, 0x20000, 0x20000,
    0x20000, 0x20000, 0x20000, 0x20000, 0x20000, 0x20000, 0x20000, 0x20000,
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
    uchar   *saddr;
    int i, *sizetable, msize;
    struct  sectorinfo *sinfotbl;

    /* Based on the flash bank ID returned, load a sector count and a */
    /* sector size-information table... */
    flashtype(fbnk);
    switch(fbnk->id) {
        case AMD29LV640D:
            fbnk->sectorcnt = 128;
            sizetable = SectorSizesLV640D;
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
    struct  flashinfo *fbnk;

    snum = 0;
    FlashCurrentBank = 0;


#ifndef NO_PIC

    /*
     * flashpic.o is really PIC
     */

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

#else /* NO_PIC */

    /*
     * can't rely on compiler producing PIC.
     * use poor man's relocation scheme
     */
    
    {
        /* move code form flash to dram, where it is relocated 
           to run. this can also be done much earlier, e.g. in
           'start.c' */

        extern char _etext, _rtext, _erdata;

        char *src = &_etext;
        char *dst = &_rtext;

        while ( dst < &_erdata ) {
            *dst++ = *src++;
        }
    }

    FlashTypeFbuf = (ulong *)FLASHTYPE;
    FlashEraseFbuf = (ulong *)FLASHERASE;
    FlashEwriteFbuf = (ulong *)FLASHEWRITE;
    FlashWriteFbuf = (ulong *)FLASHWRITE;

#endif /* of NO_PIC */

    fbnk = &FlashBank[0];
    fbnk->base = (unsigned char *)FLASH_BANK0_BASE_ADDR;
    fbnk->width = FLASH_BANK0_WIDTH;
    fbnk->fltype = (int(*)())FlashTypeFbuf;     /* flashtype(). */
    fbnk->flerase = (int(*)())FlashEraseFbuf;   /* flasherase(). */
    fbnk->flwrite = (int(*)())FlashWriteFbuf;   /* flashwrite(). */
    fbnk->flewrite = (int(*)())FlashEwriteFbuf; /* flashewrite(). */
    snum += FlashBankInit(fbnk,snum);

    sectorProtect(FLASH_PROTECT_RANGE,1);

#ifdef FLASHRAM_BASE
	FlashRamInit(snum, sizeof(ramSectors)/sizeof(int),
		&FlashBank[FLASHRAM_BANKNUM], sinfoRAM, ramSectors);
#endif

    /* Additional flash memory banks would be initialized here... */
    return(0);
}

#endif /* of INCLUDE_FLASH */
