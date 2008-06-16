#define	ftype					volatile unsigned char
#define Write_aa_to_555()		(*(ftype *)(fdev->base+(0x5555<<0)) = 0xaa)
#define Write_55_to_2aa()		(*(ftype *)(fdev->base+(0x2aaa<<0)) = 0x55)
#define Write_80_to_555()		(*(ftype *)(fdev->base+(0x5555<<0)) = 0x80)
#define Write_a0_to_555()		(*(ftype *)(fdev->base+(0x5555<<0)) = 0xa0)
#define Write_f0_to_555()		(*(ftype *)(fdev->base+(0x5555<<0)) = 0xf0)
#define Write_90_to_555()		(*(ftype *)(fdev->base+(0x5555<<0)) = 0x90)
#define Write_30_to_(add)		(*(ftype *)add = 0x30)
#define Read_0000()				(*(ftype *)(fdev->base+(0x0000<<0)))
#define Read_0001()				(*(ftype *)(fdev->base+(0x0001<<0)))
#define Read_5555()				(*(ftype *)(fdev->base+(0x5555<<0)))
#define Is_ff(add)				(*(ftype *)add == 0xff)
#define Is_not_ff(add)			(*(ftype *)add != 0xff)
#define D5_Timeout(add)			((*(ftype *)add & 0x20) != 0)
#define D7_not_set(add)			((*(ftype *)add & 0x80) == 0)
#define	NotAligned(ptr)	    	0

#define Is_D7_Equal(add,src)		\
		((*(ftype *)add & 0x80) == (*(ftype *)src & 0x80))
#define Is_D7_Not_Equal(add,src)	\
	    ((*(ftype *)add & 0x80) != (*(ftype *)src & 0x80))
