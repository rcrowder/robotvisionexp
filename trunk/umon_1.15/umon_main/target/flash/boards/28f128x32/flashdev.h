/* 
 * flash.h
 *
 * by Thomas E. Arvanits (tharvan@inaccessnetworks.com)
 *
 * Original header file for use with 29F128J3A (intel strata flash)
 * for 16 bit mode, in x2 configuration for 32 bit access.
 *
 * Modified by els for use with alchemy hardware.
 */
#ifndef _flash_h
#define _flash_h

/* 
 * FLASHFUNCSIZE:
 * Size of the array used by the flash operations to copy functions into
 * RAM space for execution out of non-flash memory.
 */
#define FLASHFUNCSIZE		320

#define FLASH_TIMEOUT		1000000

/* Manufacturer and device ids... */
#define INTEL28F128J	0x00890018

#define WIDTH		4
#define WIDTH_IN_BYTES	4
#define WIDTH_IN_BITS	32
#define WIDTH_MASK	1

#define	ftype			    volatile unsigned long

#define FLASHLOCK	    Flashlock32
#define ENDFLASHLOCK	    EndFlashlock32
#define FLASHERASE	    Flasherase32
#define ENDFLASHERASE	    EndFlasherase32
#define FLASHWRITE	    Flashwrite32
#define ENDFLASHWRITE	    EndFlashwrite32
#define FLASHEWRITE	    Flashewrite32
#define ENDFLASHEWRITE	    EndFlashewrite32
#define FLASHTYPE	    Flashtype32
#define ENDFLASHTYPE	    EndFlashtype32

#define FWrite(to,frm)		(*(ftype *)(to) = *(ftype *)(frm))
#define Write_to(add, val)	(*(ftype *)(add) = (val))
#define Write_01_to(add)	(*(ftype *)(add) = 0x00010001)
#define Write_20_to(add)	(*(ftype *)(add) = 0x00200020)
#define Write_40_to(add)	(*(ftype *)(add) = 0x00400040)
#define Write_70_to(add)	(*(ftype *)(add) = 0x00700070)
#define Write_d0_to(add)	(*(ftype *)(add) = 0x00d000d0)

#define Write_50_to_base()	(*(ftype *)(fdev->base) = 0x00500050)
#define Write_60_to_base()	(*(ftype *)(fdev->base) = 0x00600060)
#define Write_70_to_base()	(*(ftype *)(fdev->base) = 0x00700070)
#define Write_90_to_base()	(*(ftype *)(fdev->base) = 0x00900090)
#define Write_ff_to_base()	(*(ftype *)(fdev->base) = 0x00FF00FF)

#define Read_from(add)		(*(ftype *)(add))
#define Read_0000_from_base()	(*(ftype *)(fdev->base+(0x00000000<<2)))
#define Read_0001_from_base()	(*(ftype *)(fdev->base+(0x00000001<<2)))
#define Read_5555_from_base()	(*(ftype *)(fdev->base+(0x5555<<1)))

#define Is_ff(add)		(*(ftype *)(add) == 0xFFFFFFFF)
#define Is_not_ff(add)		(*(ftype *)(add) != 0xFFFFFFFF)

#define Is_Equal(p1,p2)		(*(ftype *)(p1) == *(ftype *)(p2))
#define Is_Not_Equal(p1,p2)	(*(ftype *)(p1) != *(ftype *)(p2))

#define Not32BitAligned(ptr)	((long)ptr & 3)

#endif /* _flash_h */
