extern int iTE28160C3B_16x1_erase(struct flashinfo *fdev,int snum);

extern void EndiTE28160C3B_16x1_erase(void);

extern int iTE28160C3B_16x1_write(struct flashinfo *fdev,
	unsigned char *dest,unsigned char *src,long bytecnt);

extern void EndiTE28160C3B_16x1_write(void);

extern int iTE28160C3B_16x1_ewrite(struct flashinfo *fdev,
	unsigned char *dest,unsigned char *src,long bytecnt);

extern void EndiTE28160C3B_16x1_ewrite(void);

extern int iTE28160C3B_16x1_lock(struct flashinfo *fdev,
	int snum,int operation);

extern void EndiTE28160C3B_16x1_lock(void);

extern int iTE28160C3B_16x1_type(struct flashinfo *fdev);

extern void EndiTE28160C3B_16x1_type(void);
