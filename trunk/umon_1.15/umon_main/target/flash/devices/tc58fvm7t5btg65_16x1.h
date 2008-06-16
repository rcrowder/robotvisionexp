//Device IDs
#define tc58fvm7t5btg65 0x98001b
#define tc58fvm7b5btg65 0x98001d


extern int tc58fvm7t5btg65_16x1_erase(struct flashinfo *fdev,int snum);
extern void Endtc58fvm7t5btg65_16x1_erase(void);

extern int tc58fvm7t5btg65_16x1_write(struct flashinfo *fdev,
	unsigned char *dest,unsigned char *src,long bytecnt);
extern void Endtc58fvm7t5btg65_16x1_write(void);

extern int tc58fvm7t5btg65_16x1_ewrite(struct flashinfo *fdev,
	unsigned char *dest,unsigned char *src,long bytecnt);
extern void Endtc58fvm7t5btg65_16x1_ewrite(void);

extern int tc58fvm7t5btg65_16x1_lock(struct flashinfo *fdev,int snum,int operation);
extern void Endtc58fvm7t5btg65_16x1_lock(void);

extern int tc58fvm7t5btg65_16x1_type(struct flashinfo *fdev);
extern void Endtc58fvm7t5btg65_16x1_type(void);
