extern int Intel28f640_8x4_erase(struct flashinfo *fdev,int snum);

extern void EndIntel28f640_8x4_erase(void);

extern int Intel28f640_8x4_write(struct flashinfo *fdev,
	unsigned char *dest,unsigned char *src,long bytecnt);

extern void EndIntel28f640_8x4_write(void);

extern int Intel28f640_8x4_ewrite(struct flashinfo *fdev,
	unsigned char *dest,unsigned char *src,long bytecnt);

extern void EndIntel28f640_8x4_ewrite(void);

extern int Intel28f640_8x4_lock(struct flashinfo *fdev,int snum,int operation);

extern void EndIntel28f640_8x4_lock(void);

extern int Intel28f640_8x4_type(struct flashinfo *fdev);

extern void EndIntel28f640_8x4_type(void);
