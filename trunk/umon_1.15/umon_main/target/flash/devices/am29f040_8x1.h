/* Manufacturer and device ids...
 */
#define STM_29F040		0x20e2
#define STM_M29W040B	0x20e3
#define AMD_29F040		0x01a4
#define AMD_29F010		0x0120
#define AMD_29LV040		0x014f

/* Position-independent function declarations...
 */
extern int Am29f040_8x1_erase(struct flashinfo *fdev,int snum);
extern void EndAm29f040_8x1_erase(void);

extern int Am29f040_8x1_write(struct flashinfo *fdev,unsigned char *dest,unsigned char *src,long bytecnt);
extern void EndAm29f040_8x1_write(void);

extern int Am29f040_8x1_ewrite(struct flashinfo *fdev,unsigned char *dest,unsigned char *src,long bytecnt);
extern void EndAm29f040_8x1_ewrite(void);

extern int Am29f040_8x1_type(struct flashinfo *fdev);
extern void EndAm29f040_8x1_type(void);

extern int Am29f040_8x1_lock(struct flashinfo *fdev, int snum, int operation);
extern void EndAm29f040_8x1_lock(void);
