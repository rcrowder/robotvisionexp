extern int Intel28f640_16x2_erase(struct flashinfo *fdev,int snum);

extern void EndIntel28f640_16x2_erase(void);

extern int Intel28f640_16x2_write(struct flashinfo *fdev,
	unsigned char *dest,unsigned char *src,long bytecnt);

extern void EndIntel28f640_16x2_write(void);

extern int Intel28f640_16x2_ewrite(struct flashinfo *fdev,
	unsigned char *dest,unsigned char *src,long bytecnt);

extern void EndIntel28f640_16x2_ewrite(void);

extern int Intel28f640_16x2_lock(struct flashinfo *fdev,int snum,int operation);

extern void EndIntel28f640_16x2_lock(void);

extern int Intel28f640_16x2_type(struct flashinfo *fdev);

extern void EndIntel28f640_16x2_type(void);
