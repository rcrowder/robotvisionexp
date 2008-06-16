extern int Intel28f128_16x1_erase(struct flashinfo *fdev,int snum);

extern void EndIntel28f128_16x1_erase(void);

extern int Intel28f128_16x1_write(struct flashinfo *fdev,
	unsigned char *dest,unsigned char *src,long bytecnt);

extern void EndIntel28f128_16x1_write(void);

extern int Intel28f128_16x1_ewrite(struct flashinfo *fdev,
	unsigned char *dest,unsigned char *src,long bytecnt);

extern void EndIntel28f128_16x1_ewrite(void);

extern int Intel28f128_16x1_lock(struct flashinfo *fdev,
	int snum,int operation);

extern void EndIntel28f128_16x1_lock(void);

extern int Intel28f128_16x1_type(struct flashinfo *fdev);

extern void EndIntel28f128_16x1_type(void);
