/* flash.h:
   Header file for use with 29F800B for 16 bit mode.
*/
#define FLASH_TIMEOUT		1000000
#define FLASHFUNCSIZE		320

/* If this is set to 1, then the 28F640 functions are executed out of RAM. */
#define COPY_640OPS_TO_RAM	0

/* Manufacturer and device ids... */
#define INTEL28F640		0x150089
#define INTEL28F320		0x140089
#define SGS29F040		0x0020e2
#define AMD29F040		0x0001a4
#define AMD29F010		0x000120
#define AMD29LV040		0x00014f

#define Fwrite(to,frm)      (*(ftype *)to = *(ftype *)frm)
#define Is_Equal(p1,p2)     (*(ftype *)p1 == *(ftype *)p2)
#define Is_Not_Equal(p1,p2) (*(ftype *)p1 != *(ftype *)p2)
#define	NotAligned(ptr)	    ((long)ptr & 1)
