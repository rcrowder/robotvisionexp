/* flash.h:
*/
#define FLASH_TIMEOUT			1000000
#define FLASHFUNCSIZE			320

/* If this is set to 1, then the 28F640 functions are executed out of RAM. */
#define COPY_640OPS_TO_RAM	0

/* Manufacturer and device ids... */
#define AMD29DL640D		0x0001227e
#define SGS29F040		0x20e2
#define AMD29F040		0x01a4
#define AMD29F010		0x0120
#define AMD29LV040		0x014f

#define Fwrite(to,frm)      (*(ftype *)to = *(ftype *)frm)
#define Is_Equal(p1,p2)     (*(ftype *)p1 == *(ftype *)p2)
#define Is_Not_Equal(p1,p2) (*(ftype *)p1 != *(ftype *)p2)
