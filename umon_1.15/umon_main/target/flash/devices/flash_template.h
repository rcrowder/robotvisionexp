extern int Flashtemplate_erase(struct flashinfo *fdev,int snum);
extern void EndFlashtemplate_erase(void);

extern int Flashtemplate_write(struct flashinfo *fdev,
	unsigned char *dest,unsigned char *src,long bytecnt);
extern void EndFlashtemplate_write(void);

extern int Flashtemplate_ewrite(struct flashinfo *fdev,
	unsigned char *dest,unsigned char *src,long bytecnt);
extern void EndFlashtemplate_ewrite(void);

extern int Flashtemplate_lock(struct flashinfo *fdev,int snum,int operation);
extern void EndFlashtemplate_lock(void);

extern int Flashtemplate_type(struct flashinfo *fdev);
extern void EndFlashtemplate_type(void);
