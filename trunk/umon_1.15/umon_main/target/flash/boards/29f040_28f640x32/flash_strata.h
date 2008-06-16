#define	ftype			    volatile unsigned long

#define WBS					0x00800080
#define WSMS				0x00800080
#define ESS					0x00400040
#define ECLBS				0x00200020
#define PSLBS				0x00100010
#define VPENS				0x00080008
#define RSVD2				0x00040004
#define DPS					0x00020002
#define RSVD0				0x00010001

#define Write_20_to_base()	(*(ftype *)(fdev->base) = 0x00200020)
#define Write_40_to_base()	(*(ftype *)(fdev->base) = 0x00400040)
#define Write_50_to_base()	(*(ftype *)(fdev->base) = 0x00500050)
#define Write_60_to_base()	(*(ftype *)(fdev->base) = 0x00600060)
#define Write_70_to_base()	(*(ftype *)(fdev->base) = 0x00700070)
#define Write_90_to_base()	(*(ftype *)(fdev->base) = 0x00900090)
#define Write_ff_to_base()	(*(ftype *)(fdev->base) = 0x00ff00ff)
#define Write_01_to_(add)	(*(ftype *)add = 0x00010001)
#define Write_0f_to_(add)	(*(ftype *)add = 0x000f000f)
#define Write_20_to_(add)	(*(ftype *)add = 0x00200020)
#define Write_2f_to_(add)	(*(ftype *)add = 0x002f002f)
#define Write_40_to_(add)	(*(ftype *)add = 0x00400040)
#define Write_d0_to_(add)	(*(ftype *)add = 0x00d000d0)
#define Write_e8_to_(add)	(*(ftype *)add = 0x00e800e8)
#define Read_0000()			(*(ftype *)(fdev->base))
#define Read_0001()			(*(ftype *)(fdev->base+4))
#define	Not32BitAligned(ptr)	((long)ptr & 3)

#define WAIT_FOR_WRITE(add,data)	while(*(ftype *)add != *(ftype *)data)
#define WAIT_FOR_FF(add)			while((*(ftype *)add) != 0xffffffff)
#define WAIT_FOR_WSMS_READY()		while((Read_0000() & WSMS) != WSMS)
#define WBUF_NOT_AVAIL(add)			((*(ftype *)add & WBS) != WBS)
