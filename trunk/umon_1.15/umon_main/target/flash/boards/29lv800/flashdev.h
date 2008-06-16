/*******************************************************************************
Copyright (c) Lucent Technologies, 1999.  All rights reserved.

FILENAME:      Flash.c

DESCRIPTION:   Flash utility

================================================================================
EDIT HISTORY:

DATE      INITIALS  ABSTRACT
03-25-99  M. Hunag  Created.

*******************************************************************************/
/* flash.h:
   Header file for use with 29F800B for 16 bit mode.
*/
/* FLASHFUNCSIZE:
	Size of the array used by the flash operations to copy functions into
	RAM space for execution out of non-flash memory.
*/
#include "config.h"

#define FLASHFUNCSIZE		350

#define FLASH_TIMEOUT		1000000

/* Manufacturer and device ids... */
#ifdef ATLAS_BOARD
#define AMD29F800T	0x000122d6
#define AMD29F800B	0x00012258
#else

#if FLASH_29F160
#define AMD29LV160T		0x000122C4
#define AMD29LV160BB	0x00012249
#elif FLASH_29F800
#define M29W800B	    0x0020005b
#define AM29LV800BB	  0x0001225b
#define AMD29F800B	  0x00012258
#define AMD29F800T	  0x000122d6
#endif

#endif

#define WIDTH			2
#define WIDTH_IN_BYTES	2
#define WIDTH_IN_BITS	16
#define WIDTH_MASK		1
#define	ftype			    volatile unsigned short
#define FLASHERASE		    Flasherase16
#define ENDFLASHERASE	    EndFlasherase16
#define FLASHWRITE		    Flashwrite16
#define ENDFLASHWRITE	    EndFlashwrite16
#define FLASHEWRITE		    Flashewrite16
#define ENDFLASHEWRITE	    EndFlashewrite16
#define FLASHTYPE		    Flashtype16
#define ENDFLASHTYPE	    EndFlashtype16
#define	NotAligned(ptr)	    ((long)ptr & 1)
#define Write_aa_to_555()  (*(ftype *)(fdev->base+(0x555<<1)) = 0xaaaa)
#define Write_55_to_2aa()  (*(ftype *)(fdev->base+(0x2aa<<1)) = 0x5555)
#define Write_80_to_555()  (*(ftype *)(fdev->base+(0x555<<1)) = 0x8080)
#define Write_a0_to_555()  (*(ftype *)(fdev->base+(0x555<<1)) = 0xa0a0)
#define Write_f0_to_555()  (*(ftype *)(fdev->base+(0x555<<1)) = 0xf0f0)
#define Write_90_to_555()  (*(ftype *)(fdev->base+(0x555<<1)) = 0x9090)
#define Write_30_to_(add)   (*(ftype *)add = 0x3030)
#define Read_0000()		    (*(ftype *)(fdev->base+(0x0000<<1)))
#define Read_0001()		    (*(ftype *)(fdev->base+(0x0001<<1)))
#define Read_5555()		    (*(ftype *)(fdev->base+(0x5555<<1)))
#define Is_ff(add)		    (*(ftype *)add == 0xffff)
#define Is_not_ff(add)	    (*(ftype *)add != 0xffff)
#define D5_Timeout(add)	    ((*(ftype *)add & 0xdfdf) == 0x2020)

#define Fwrite(to,frm)      (*(ftype *)to = *(ftype *)frm)
#define Is_Equal(p1,p2)     (*(ftype *)p1 == *(ftype *)p2)
#define Is_Not_Equal(p1,p2) (*(ftype *)p1 != *(ftype *)p2)
