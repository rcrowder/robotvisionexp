/* Manufacturer and device ids for top/bottom boot devices.
 */
#define AM29BDS128H		 0x0001227e

/* Declarations for Am29BDS128H device interface functions:
 */
extern int Am29bds128h_16x2_erase(struct flashinfo *,int);
extern void EndAm29bds128h_16x2_erase(void);

extern int Am29bds128h_16x2_write(struct flashinfo *,unsigned char *,unsigned char *,long);
extern void EndAm29bds128h_16x2_write(void);

extern int Am29bds128h_16x2_ewrite(struct flashinfo *,unsigned char *,unsigned char *,long);
extern void EndAm29bds128h_16x2_ewrite(void);

extern int Am29bds128h_16x2_type(struct flashinfo *);
extern void EndAm29bds128h_16x2_type(void);

