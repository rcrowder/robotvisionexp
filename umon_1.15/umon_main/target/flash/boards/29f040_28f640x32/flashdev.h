/* flash.h:
   Header file for use with 29F800B for 16 bit mode.
*/
#define FLASH_TIMEOUT		1000000
#define FLASHFUNCSIZE		320

/* Manufacturer and device ids... */
#define INTEL28F320J5		0x00890014		/* 32  Mbit (5 volt strata) */
#define INTEL28F640J5		0x00890015		/* 64  Mbit (5 volt strata) */
#define INTEL28F320J3		0x00890016		/* 32  Mbit (3 volt strata) */
#define INTEL28F640J3		0x00890017		/* 64  Mbit (3 volt strata) */
#define INTEL28F128J3		0x00890018		/* 128 Mbit (3 volt strata) */

#define SGS29F040			0x0020e2
#define AMD29F040			0x0001a4
#define AMD29F010			0x000120
#define AMD29LV040			0x00014f

#define Fwrite(to,frm)      (*(ftype *)to = *(ftype *)frm)
#define Is_Equal(p1,p2)     (*(ftype *)p1 == *(ftype *)p2)
#define Is_Not_Equal(p1,p2) (*(ftype *)p1 != *(ftype *)p2)
