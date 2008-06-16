/* flash.h:
 *	Header file for use with 29F040 in 8 bit mode and 2
 *	29LV800BBs combined for 32 bit mode.
 */
#define FLASH_TIMEOUT		1000000

#define GANG				0x02000000
#define FLASHFUNCSIZE		320

/* Manufacturer and device ids... */
#define AMD_29F800T			0x000122d6
#define AMD_29F800B			0x00012258
#define AMD_29LV800BT32		(GANG | 0x000122da)	/* 2 parts in 32-bit cfg */
#define AMD_29LV800BB32		(GANG | 0x0001225b)	/* 2 parts in 32-bit cfg */
#define AMD_29LV800BBBT		0x225b22da			/* 2 parts 1 BB & 1 BT (bad) */
#define AMD_29LV800BTBB		0x22da225b			/* 2 parts 1 BT & 1 BB (bad) */
#define AMD_29LV160BB32		(GANG | 0x00012249) /* 2 parts in 32-bit cfg */
#define STM_29F040			0x20e2
#define STM_M29W040B		0x20e3
#define AMD_29F040			0x01a4
#define AMD_29LV040			0x014f
#define AMD_29F010			0x0120
#define FLASHRAM			0x9999 /* This is fake, ram has no device id */

#define Fwrite(to,frm)      (*(ftype *)to = *(ftype *)frm)
#define Is_Equal(p1,p2)     (*(ftype *)p1 == *(ftype *)p2)
#define Is_Not_Equal(p1,p2) (*(ftype *)p1 != *(ftype *)p2)
