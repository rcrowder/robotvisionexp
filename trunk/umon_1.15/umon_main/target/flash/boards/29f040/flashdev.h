/* flash.h:
   This header file supports the ability to use one source file to build
   flash operations for 8, 16 and 32 bit width flash banks.
*/
#include "config.h"

#define FLASH_TIMEOUT			100000000
#define FLASHFUNCSIZE			400

/* Manufacturer and device ids... */
#define STM_29F040		0x20e2
#define STM_M29W040B	0x20e3
#define AMD_29F040		0x01a4
#define AMD_29F010		0x0120
#define AMD_29LV040		0x014f

#if WIDTH8	/* Macros used if bank width is 8 bits */

#define WIDTH	1
#define	ftype		    	volatile unsigned char
#define FLASHERASE	    	Flasherase8
#define ENDFLASHERASE		EndFlasherase8
#define FLASHWRITE	    	Flashwrite8
#define ENDFLASHWRITE		EndFlashwrite8
#define FLASHEWRITE	    	Flashewrite8
#define ENDFLASHEWRITE		EndFlashewrite8
#define FLASHTYPE	    	Flashtype8
#define ENDFLASHTYPE		EndFlashtype8
#define Write_aa_to_5555()  (*(ftype *)(fdev->base+(0x5555<<0)) = 0xaa)
#define Write_55_to_2aaa()  (*(ftype *)(fdev->base+(0x2aaa<<0)) = 0x55)
#define Write_80_to_5555()  (*(ftype *)(fdev->base+(0x5555<<0)) = 0x80)
#define Write_a0_to_5555()  (*(ftype *)(fdev->base+(0x5555<<0)) = 0xa0)
#define Write_f0_to_5555()  (*(ftype *)(fdev->base+(0x5555<<0)) = 0xf0)
#define Write_90_to_5555()  (*(ftype *)(fdev->base+(0x5555<<0)) = 0x90)
#define Write_30_to_(add)   (*(ftype *)add = 0x30)
#define Read_0000()	    	(*(ftype *)(fdev->base+(0x0000<<0)))
#define Read_0001()	    	(*(ftype *)(fdev->base+(0x0001<<0)))
#define Read_5555()	    	(*(ftype *)(fdev->base+(0x5555<<0)))
#define Is_ff(add)	    	(*(ftype *)add == 0xff)
#define Is_not_ff(add)	    (*(ftype *)add != 0xff)
#define D5_Timeout(add)	    ((*(ftype *)add & 0xdf) == 0x20)

#elif WIDTH16	/* Macros used if bank width is 16 bits */

#define WIDTH	2
#define	ftype		    	volatile unsigned short
#define FLASHERASE	    	Flasherase16
#define ENDFLASHERASE		EndFlasherase16
#define FLASHWRITE	    	Flashwrite16
#define ENDFLASHWRITE		EndFlashwrite16
#define FLASHEWRITE	    	Flashewrite16
#define ENDFLASHEWRITE		EndFlashewrite16
#define FLASHTYPE	    	Flashtype16
#define ENDFLASHTYPE		EndFlashtype16
#define Write_aa_to_5555()  (*(ftype *)(fdev->base+(0x5555<<1)) = 0xaaaa)
#define Write_55_to_2aaa()  (*(ftype *)(fdev->base+(0x2aaa<<1)) = 0x5555)
#define Write_80_to_5555()  (*(ftype *)(fdev->base+(0x5555<<1)) = 0x8080)
#define Write_a0_to_5555()  (*(ftype *)(fdev->base+(0x5555<<1)) = 0xa0a0)
#define Write_f0_to_5555()  (*(ftype *)(fdev->base+(0x5555<<1)) = 0xf0f0)
#define Write_90_to_5555()  (*(ftype *)(fdev->base+(0x5555<<1)) = 0x9090)
#define Write_30_to_(add)   (*(ftype *)add = 0x3030)
#define Read_0000()	    	(*(ftype *)(fdev->base+(0x0000<<1)))
#define Read_0001()	    	(*(ftype *)(fdev->base+(0x0001<<1)))
#define Read_5555()	    	(*(ftype *)(fdev->base+(0x5555<<1)))
#define Is_ff(add)	    	(*(ftype *)add == 0xffff)
#define Is_not_ff(add)	    (*(ftype *)add != 0xffff)
#define D5_Timeout(add)	    ((*(ftype *)add & 0xdfdf) == 0x2020)

#elif WIDTH32	/* Macros used if bank width is 32 bits */

#define WIDTH	4
#define	ftype		    	volatile unsigned long
#define FLASHERASE	    	Flasherase32
#define ENDFLASHERASE	    EndFlasherase32
#define FLASHWRITE	    	Flashwrite32
#define ENDFLASHWRITE	    EndFlashwrite32
#define FLASHEWRITE	    	Flashewrite32
#define ENDFLASHEWRITE	    EndFlashewrite32
#define FLASHTYPE	    	Flashtype32
#define ENDFLASHTYPE	    EndFlashtype32
#define Write_aa_to_5555()  (*(ftype *)(fdev->base+(0x5555<<2)) = 0xaaaaaaaa)
#define Write_55_to_2aaa()  (*(ftype *)(fdev->base+(0x2aaa<<2)) = 0x55555555)
#define Write_80_to_5555()  (*(ftype *)(fdev->base+(0x5555<<2)) = 0x80808080)
#define Write_a0_to_5555()  (*(ftype *)(fdev->base+(0x5555<<2)) = 0xa0a0a0a0)
#define Write_f0_to_5555()  (*(ftype *)(fdev->base+(0x5555<<2)) = 0xf0f0f0f0)
#define Write_90_to_5555()  (*(ftype *)(fdev->base+(0x5555<<2)) = 0x90909090)
#define Write_30_to_(add)   (*(ftype *)add = 0x30303030)
#define Read_0000()	    	(*(ftype *)(fdev->base+(0x0000<<2)))
#define Read_0001()	    	(*(ftype *)(fdev->base+(0x0001<<2)))
#define Read_5555()	    	(*(ftype *)(fdev->base+(0x5555<<2)))
#define Is_ff(add)	    	(*(ftype *)add == 0xffffffff)
#define Is_not_ff(add)	    (*(ftype *)add != 0xffffffff)
#define D5_Timeout(add)	    ((*(ftype *)add & 0xdfdfdfdf) == 0x20202020)

#else
Width specification illegal or undefined.
#endif

#define Fwrite(to,frm)      (*(ftype *)to = *(ftype *)frm)
#define Is_Equal(p1,p2)     (*(ftype *)p1 == *(ftype *)p2)
#define Is_Not_Equal(p1,p2) (*(ftype *)p1 != *(ftype *)p2)
