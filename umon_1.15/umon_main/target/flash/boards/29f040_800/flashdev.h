/* flash.h:
   Header file for use with 29F800B for 16 bit mode.
*/
#define FLASH_TIMEOUT		1000000
#define FLASHFUNCSIZE		320

/* Manufacturer and device ids... */
#define AMD29F800T	0x000122d6
#define AMD29F800B	0x00012258
#define SGS29F040	0x20e2
#define AMD29F040	0x01a4
#define AMD29F010	0x0120

#define Fwrite(to,frm)      (*(ftype *)to = *(ftype *)frm)
#define Is_Equal(p1,p2)     (*(ftype *)p1 == *(ftype *)p2)
#define Is_Not_Equal(p1,p2) (*(ftype *)p1 != *(ftype *)p2)
#define	NotAligned(ptr)	    ((long)ptr & 1)
