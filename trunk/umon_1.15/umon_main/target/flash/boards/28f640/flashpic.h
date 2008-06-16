#define ftype               volatile unsigned short

#define Write_20_to_base()  (*(ftype *)(fdev->base) = 0x0020)
#define Write_40_to_base()  (*(ftype *)(fdev->base) = 0x0040)
#define Write_50_to_base()  (*(ftype *)(fdev->base) = 0x0050)
#define Write_60_to_base()  (*(ftype *)(fdev->base) = 0x0060)
#define Write_70_to_base()  (*(ftype *)(fdev->base) = 0x0070)
#define Write_90_to_base()  (*(ftype *)(fdev->base) = 0x0090)
#define Write_ff_to_base()  (*(ftype *)(fdev->base) = 0x00ff)
#define Write_0f_to_(add)   (*(ftype *)add = 0x000f)
#define Write_20_to_(add)   (*(ftype *)add = 0x0020)
#define Write_40_to_(add)   (*(ftype *)add = 0x0040)
#define Write_d0_to_(add)   (*(ftype *)add = 0x00d0)
#define Write_e8_to_(add)   (*(ftype *)add = 0x00e8)
#define Read_0000()         (*(ftype *)(fdev->base))
#define Read_0001()         (*(ftype *)(fdev->base+2))

#define WBS     0x0080
#define WSMS    0x0080
#define ESS     0x0040
#define ECLBS   0x0020
#define PSLBS   0x0010
#define VPENS   0x0008
#define RSVD2   0x0004
#define DPS     0x0002
#define RSVD0   0x0001

