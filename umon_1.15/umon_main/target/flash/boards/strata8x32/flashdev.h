/* flash.h:
   Header file for use with 29F800B for 16 bit mode.
*/
#define FLASH_TIMEOUT		1000000
#define FLASHFUNCSIZE		320

/* If this is set to 1, then the 28F640 functions are executed out of RAM. */
#define COPY_640OPS_TO_RAM	0

/* Manufacturer and device ids... */
#define MANUFACTURER_INTEL	0x89898989
#define DEVICE_28F320J3		0x16161616		/* 32  Mbit (3 volt strata) */
#define DEVICE_28F640J3		0x17171717		/* 64  Mbit (3 volt strata) */
#define DEVICE_28F128J3		0x18181818		/* 128 Mbit (3 volt strata) */

#define Fwrite(to,frm)      (*(ftype *)to = *(ftype *)frm)
#define Is_Equal(p1,p2)     (*(ftype *)p1 == *(ftype *)p2)
#define Is_Not_Equal(p1,p2) (*(ftype *)p1 != *(ftype *)p2)
#define	ftype			    volatile unsigned long

#define WBS					0x80808080
#define WSMS				0x80808080
#define ESS					0x40404040
#define ECLBS				0x20202020
#define PSLBS				0x10101010
#define VPENS				0x08080808
#define RSVD2				0x04040404
#define DPS					0x02020202
#define RSVD0				0x01010101

#define Write_20_to_base()	(*(ftype *)(fdev->base) = 0x20202020)
#define Write_40_to_base()	(*(ftype *)(fdev->base) = 0x40404040)
#define Write_50_to_base()	(*(ftype *)(fdev->base) = 0x50505050)
#define Write_60_to_base()	(*(ftype *)(fdev->base) = 0x60606060)
#define Write_70_to_base()	(*(ftype *)(fdev->base) = 0x70707070)
#define Write_90_to_base()	(*(ftype *)(fdev->base) = 0x90909090)
#define Write_ff_to_base()	(*(ftype *)(fdev->base) = 0xffffffff)
#define Write_01_to_(add)	(*(ftype *)add = 0x01010101)
#define Write_0f_to_(add)	(*(ftype *)add = 0x0f0f0f0f)
#define Write_20_to_(add)	(*(ftype *)add = 0x20202020)
#define Write_40_to_(add)	(*(ftype *)add = 0x40404040)
#define Write_d0_to_(add)	(*(ftype *)add = 0xd0d0d0d0)
#define Write_e8_to_(add)	(*(ftype *)add = 0xe8e8e8e8)
#define Read_0000()			(*(ftype *)(fdev->base))
#define Read_0001()			(*(ftype *)(fdev->base+4))
#define	Not32BitAligned(ptr)	((long)ptr & 3)

#define WAIT_FOR_WRITE(add,data)	while(*(ftype *)add != *(ftype *)data)
#define WAIT_FOR_FF(add)			while((*(ftype *)add) != 0xffffffff)
#define WAIT_FOR_WSMS_READY()		while((Read_0000() & WSMS) != WSMS)
#define WBUF_NOT_AVAIL(add)			((*(ftype *)add & WBS) != WBS)
