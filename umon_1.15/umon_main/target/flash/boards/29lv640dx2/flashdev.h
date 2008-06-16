/* flash.h:
   This header file supports the commands needed to program the device
   Only 32-bit is supported here
*/
#include "config.h"

#define FLASH_TIMEOUT           1000000
#define FLASHFUNCSIZE           400

/* Manufacturer and device ids... */
#define AMD29LV640D     0x000122d7

#define WIDTH   			4	// only 32-bit supported here
#define ftype               volatile unsigned long
#define Write_aa_to_555()  	(*(ftype *)(fdev->base + (0x555 << 2)) = 0xaaaaaaaa)
#define Write_55_to_2aa()  	(*(ftype *)(fdev->base + (0x2aa << 2)) = 0x55555555)
#define Write_80_to_555()  	(*(ftype *)(fdev->base + (0x555 << 2)) = 0x80808080)
#define Write_90_to_555()  	(*(ftype *)(fdev->base + (0x555 << 2)) = 0x90909090)
#define Write_a0_to_555()  	(*(ftype *)(fdev->base + (0x555 << 2)) = 0xa0a0a0a0)
#define Write_f0_to_base()  (*(ftype *)(fdev->base) = 0xf0f0f0f0)
#define Write_30_to_(add)   (*(ftype *)add = 0x30303030)
#define Read_0000()         (*(ftype *)(fdev->base))
#define Read_0002()         (*(ftype *)(fdev->base + 4))
#define Read_555()          (*(ftype *)(fdev->base + (0x555 << 2)))
#define Is_ff(add)          (*(ftype *)add == 0xffffffff)
#define Is_not_ff(add)      (*(ftype *)add != 0xffffffff)
#define D5_Timeout(add)     ((*(ftype *)add & 0x00df00df) == 0x00200020)

#define Fwrite(to,frm)      (*(ftype *)to = *(ftype *)frm)
#define Is_Equal(p1,p2)     (*(ftype *)p1 == *(ftype *)p2)
#define Is_Not_Equal(p1,p2) (*(ftype *)p1 != *(ftype *)p2)
