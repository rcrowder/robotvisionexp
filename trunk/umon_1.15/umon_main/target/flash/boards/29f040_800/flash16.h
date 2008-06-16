#define	ftype			    volatile unsigned short
#define Write_aa_to_555()  (*(ftype *)(fdev->base+(0x555<<1)) = 0xaaaa)
#define Write_55_to_2aa()  (*(ftype *)(fdev->base+(0x2aa<<1)) = 0x5555)
#define Write_80_to_555()  (*(ftype *)(fdev->base+(0x555<<1)) = 0x8080)
#define Write_a0_to_555()  (*(ftype *)(fdev->base+(0x555<<1)) = 0xa0a0)
#define Write_f0_to_555()  (*(ftype *)(fdev->base+(0x555<<1)) = 0xf0f0)
#define Write_90_to_555()  (*(ftype *)(fdev->base+(0x555<<1)) = 0x9090)
#define Write_30_to_(add)   (*(ftype *)add = 0x3030)
#define Read_0000()		    (*(ftype *)(fdev->base+(0x0000<<1)))
#define Read_0001()		    (*(ftype *)(fdev->base+(0x0001<<1)))
#define Read_5555()		    (*(ftype *)(fdev->base+(0x5555<<1)))
#define Is_ff(add)		    (*(ftype *)add == 0xffff)
#define Is_not_ff(add)	    (*(ftype *)add != 0xffff)
#define D5_Timeout(add)	    ((*(ftype *)add & 0xdfdf) == 0x2020)
