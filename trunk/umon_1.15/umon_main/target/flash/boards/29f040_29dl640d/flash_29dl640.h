#define FLASH_TIMEOUT	1000000
#define WIDTH	2
#define	ftype		    	volatile unsigned short
#define Write_aaaa_to_555()  (*(ftype *)(fdev->base+0xaaa) = 0xaaaa)
#define Write_5555_to_2aa()  (*(ftype *)(fdev->base+0x554) = 0x5555)
#define Write_8080_to_555()  (*(ftype *)(fdev->base+0xaaa) = 0x8080)
#define Write_a0a0_to_555()  (*(ftype *)(fdev->base+0xaaa) = 0xa0a0)
#define Write_9090_to_555()  (*(ftype *)(fdev->base+0xaaa) = 0x9090)
#define Write_f0f0_to_base()  (*(ftype *)(fdev->base) = 0xf0f0)
#define Write_3030_to_(add)   (*(ftype *)(add) = 0x3030)
#define Read_0000()	    	(*(ftype *)(fdev->base))
#define Read_0001()	    	(*(ftype *)(fdev->base+1))
#define Read_0002()	    	(*(ftype *)(fdev->base+2))
#define Read_555()	    	(*(ftype *)(fdev->base+0x555))
#define Is_ff(add)	    	(*(ftype *)add == 0xffff)
#define Is_not_ff(add)	    (*(ftype *)add != 0xffff)
#define D5_Timeout(add)	    ((*(ftype *)add & 0xdf) == 0x20)

#define Fwrite(to,frm)      (*(ftype *)to = *(ftype *)frm)
#define Is_Equal(p1,p2)     (*(ftype *)p1 == *(ftype *)p2)
#define Is_Not_Equal(p1,p2) (*(ftype *)p1 != *(ftype *)p2)
