#define	ftype				volatile unsigned char
#define Write_aa_to_555()	(*(ftype *)(fdev->base+0x555) = 0xaa)
#define Write_55_to_2aa()	(*(ftype *)(fdev->base+0x2aa) = 0x55)
#define Write_80_to_555()	(*(ftype *)(fdev->base+0x555) = 0x80)
#define Write_a0_to_555()	(*(ftype *)(fdev->base+0x555) = 0xa0)
#define Write_f0_to_555()	(*(ftype *)(fdev->base+0x555) = 0xf0)
#define Write_90_to_555()	(*(ftype *)(fdev->base+0x555) = 0x90)
#define Write_30_to_(add)	(*(ftype *)add = 0x30)
#define Read_0000()			(*(ftype *)(fdev->base))
#define Read_0001()			(*(ftype *)(fdev->base+1))
#define Read_5555()			(*(ftype *)(fdev->base+0x5555))
#define Is_ff(add)			(*(ftype *)add == 0xff)
#define Is_not_ff(add)		(*(ftype *)add != 0xff)
#define D5_Timeout(add)		((*(ftype *)add & 0xdf) == 0x20)
