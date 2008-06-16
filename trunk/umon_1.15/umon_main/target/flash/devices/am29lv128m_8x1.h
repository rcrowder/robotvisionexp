/* Manufacturer and device ids...
 */
 
#define FLASH_TIMEOUT   1000000
 
#define AMD_29LV128M             0x017E1200
#define AMD_29LV128M_NUM_SECTORS 256
#define AMD_29LV128M_SECTOR_SIZE 0x10000                         /* size of sector in each chip in bytes (if chips in parallel, will be multiplied by number of chips in parallel elsewhere) */
#define AMD_29LV128M_TOTAL_SIZE  \
  (AMD_29LV128M_NUM_SECTORS*AMD_29LV128M_SECTOR_SIZE)             /* in bytes, per chip */


/* Position-independent function declarations...
 */
extern int Am29lv128m_8x1_erase(struct flashinfo *fdev,int snum);
extern void EndAm29lv128m_8x1_erase(void);

extern int Am29lv128m_8x1_write(struct flashinfo *fdev,volatile unsigned char *dest,volatile unsigned char *src,long bytecnt);
extern void EndAm29lv128m_8x1_write(void);

extern int Am29lv128m_8x1_ewrite(struct flashinfo *fdev,volatile unsigned char *dest,volatile unsigned char *src,int bytecnt);
extern void EndAm29lv128m_8x1_ewrite(void);

extern int Am29lv128m_8x1_type(struct flashinfo *fdev);
extern void EndAm29lv128m_8x1_type(void);
