/* flashdev.h:
   Header file for use with 28F640 for 16 bit mode.
*/
#define FLASH_TIMEOUT       1000000
#define FLASHFUNCSIZE       320

/* Manufacturer and device ids... */
#define INTEL28F640     0x150089
#define INTEL28F320     0x140089

#define Fwrite(to,frm)      (*(ftype *)to = *(ftype *)frm)
#define Is_Equal(p1,p2)     (*(ftype *)p1 == *(ftype *)p2)
#define Is_Not_Equal(p1,p2) (*(ftype *)p1 != *(ftype *)p2)
#define NotAligned(ptr)     ((long)ptr & 1)
