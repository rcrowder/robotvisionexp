/* flash.h:
   This header file supports the ability to use one source file to build
   flash operations for 8, 16 and 32 bit width flash banks.
*/
#include "config.h"

#define FLASH_TIMEOUT			1000000
#define FLASHFUNCSIZE			400

/* Manufacturer and device ids... */
#define AMD29DL640D		0x017e

#define WIDTH	1
#define	ftype		    	volatile unsigned char
#define Write_aa_to_aaa()  (*(ftype *)(fdev->base+0xaaa) = 0xaa)
#define Write_55_to_555()  (*(ftype *)(fdev->base+0x555) = 0x55)
#define Write_80_to_aaa()  (*(ftype *)(fdev->base+0xaaa) = 0x80)
#define Write_a0_to_aaa()  (*(ftype *)(fdev->base+0xaaa) = 0xa0)
#define Write_90_to_aaa()  (*(ftype *)(fdev->base+0xaaa) = 0x90)
#define Write_f0_to_base()  (*(ftype *)(fdev->base) = 0xf0)
#define Write_30_to_(add)   (*(ftype *)add = 0x30)
#define Read_0000()	    	(*(ftype *)(fdev->base))
#define Read_0001()	    	(*(ftype *)(fdev->base+1))
#define Read_0002()	    	(*(ftype *)(fdev->base+2))
#define Read_555()	    	(*(ftype *)(fdev->base+0x555))
#define Is_ff(add)	    	(*(ftype *)add == 0xff)
#define Is_not_ff(add)	    (*(ftype *)add != 0xff)
#define D5_Timeout(add)	    ((*(ftype *)add & 0xdf) == 0x20)

#define Fwrite(to,frm)      (*(ftype *)to = *(ftype *)frm)
#define Is_Equal(p1,p2)     (*(ftype *)p1 == *(ftype *)p2)
#define Is_Not_Equal(p1,p2) (*(ftype *)p1 != *(ftype *)p2)
