/* flash.h:
   Header file for use with 29LV160 for 32 bit mode (2 devices in x16 mode).
*/
/* FLASHFUNCSIZE:
	Size of the array used by the flash operations to copy functions into
	RAM space for execution out of non-flash memory.
*/
#define FLASHFUNCSIZE		320

#define FLASH_TIMEOUT		1000000

/* Manufacturer and device ids... */
#define SWAPPED_AMD29LV160T	0x0100c422
#define SWAPPED_AMD29LV160B	0x01004922
#define AMD29LV160T			0x000122C4
#define AMD29LV160B			0x00012249

#define WIDTH	4
#define	ftype				volatile unsigned long
#define FLASHERASE			Flasherase32
#define ENDFLASHERASE	    EndFlasherase32
#define FLASHWRITE			Flashwrite32
#define ENDFLASHWRITE	    EndFlashwrite32
#define FLASHEWRITE			Flashewrite32
#define ENDFLASHEWRITE	    EndFlashewrite32
#define FLASHTYPE			Flashtype32
#define ENDFLASHTYPE	    EndFlashtype32
#define Write_aa_to_555()  (*((ftype *)(fdev->base)+0x555) = 0xaaaaaaaa)
#define Write_55_to_2aa()  (*((ftype *)(fdev->base)+0x2aa) = 0x55555555)
#define Write_80_to_555()  (*((ftype *)(fdev->base)+0x555) = 0x80808080)
#define Write_a0_to_555()  (*((ftype *)(fdev->base)+0x555) = 0xa0a0a0a0)
#define Write_f0_to_555()  (*((ftype *)(fdev->base)+0x555) = 0xf0f0f0f0)
#define Write_90_to_555()  (*((ftype *)(fdev->base)+0x555) = 0x90909090)
#define Write_30_to_(add)   (*(ftype *)add = 0x30303030)
#define Read_0000()			(*(ftype *)(fdev->base))
#define Read_0001()			(*((ftype *)(fdev->base)+0x0001))
#define Read_555()			(*((ftype *)(fdev->base)+0x555))
#define Is_ff(add)			(*(ftype *)add == 0xffffffff)
#define Is_not_ff(add)	    (*(ftype *)add != 0xffffffff)
#define D5_Timeout(add)	    ((*(ftype *)add & 0x20202020) == 0x20202020)

#define	NotAligned(ptr)	    ((long)ptr & 3)
#define Fwrite(to,frm)      (*(ftype *)to = *(ftype *)frm)
#define Is_Equal(p1,p2)     (*(ftype *)p1 == *(ftype *)p2)
#define Is_Not_Equal(p1,p2) (*(ftype *)p1 != *(ftype *)p2)
