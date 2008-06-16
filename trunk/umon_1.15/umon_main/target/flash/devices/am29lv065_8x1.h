/* Manufacturer and device ids...
 */
#define AMD_29LV065             0x0193
#define AMD_29LV065_NUM_SECTORS 128
#define AMD_29LV065_SECTOR_SIZE 0x10000                         /* size of sector in each chip in bytes (if chips in parallel, will be multiplied by number of chips in parallel elsewhere) */
#define AMD_29LV065_TOTAL_SIZE  \
  (AMD_29LV065_NUM_SECTORS*AMD_29LV065_SECTOR_SIZE)             /* in bytes, per chip */


/* Position-independent function declarations...
 */
extern int Am29lv065_8x1_erase(struct flashinfo *fdev,int snum);
extern void EndAm29lv065_8x1_erase(void);

extern int Am29lv065_8x1_write(struct flashinfo *fdev,volatile unsigned char *dest,volatile unsigned char *src,long bytecnt);
extern void EndAm29lv065_8x1_write(void);

extern int Am29lv065_8x1_ewrite(struct flashinfo *fdev,volatile unsigned char *dest,volatile unsigned char *src,int bytecnt);
extern void EndAm29lv065_8x1_ewrite(void);

extern int Am29lv065_8x1_type(struct flashinfo *fdev);
extern void EndAm29lv065_8x1_type(void);
