/* FLASHFUNCSIZE:
	Size of the array used by the flash operations to copy functions into
	RAM space for execution out of non-flash memory.
*/
#define FLASHFUNCSIZE		350

#define FLASH_TIMEOUT		1000000

/* Manufacturer and device ids... */
#define AMD29PL160C		0x00012245

#define WIDTH			2
#define WIDTH_IN_BYTES	2
#define WIDTH_IN_BITS	16
#define WIDTH_MASK		1
#define	ftype			    volatile unsigned short
#define	NotAligned(ptr)	    ((long)ptr & 1)
#define Write_aa_to_555()  (*(ftype *)(fdev->base+(0x555<<1)) = 0xaaaa)
#define Write_55_to_2aa()  (*(ftype *)(fdev->base+(0x2aa<<1)) = 0x5555)
#define Write_80_to_555()  (*(ftype *)(fdev->base+(0x555<<1)) = 0x8080)
#define Write_a0_to_555()  (*(ftype *)(fdev->base+(0x555<<1)) = 0xa0a0)
#define Write_f0_to_555()  (*(ftype *)(fdev->base+(0x555<<1)) = 0xf0f0)
#define Write_90_to_555()  (*(ftype *)(fdev->base+(0x555<<1)) = 0x9090)
#define Write_30_to_(add)   (*(ftype *)add = 0x3030)
#define Read_555()		    (*(ftype *)(fdev->base+(0x555<<1)))
#define Read_0000()		    (*(ftype *)(fdev->base+(0x0000<<1)))
#define Read_0001()		    (*(ftype *)(fdev->base+(0x0001<<1)))
#define Is_ff(add)		    (*(ftype *)add == 0xffff)
#define Is_not_ff(add)	    (*(ftype *)add != 0xffff)
#define D5_Timeout(add)	    ((*(ftype *)add & 0xdfdf) == 0x2020)

#define Fwrite(to,frm)      (*(ftype *)to = *(ftype *)frm)
#define Is_Equal(p1,p2)     (*(ftype *)p1 == *(ftype *)p2)
#define Is_Not_Equal(p1,p2) (*(ftype *)p1 != *(ftype *)p2)

extern int Flasherase16();
extern int Flashwrite16();
extern int Flashewrite16();
extern int Flashtype16();
extern void EndFlasherase16();
extern void EndFlashwrite16();
extern void EndFlashewrite16();
extern void EndFlashtype16();
