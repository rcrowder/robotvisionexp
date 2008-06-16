/* Manufacturer and device ids for top/bottom boot devices.
 */
#define AMD29DL160DT			0x000122C4	/* Top */
#define AMD29DL160DB			0x00012249	/* Bottom */

/* Declarations for AM29LV160D device interface functions:
 */
extern int Am29lv160d_8x2_erase(struct flashinfo *,int);
extern void End_Am29lv160d_8x2_erase(void);

extern int Am29lv160d_8x2_write(struct flashinfo *,unsigned char *,unsigned char *,long);
extern void End_Am29lv160d_8x2_write(void);

extern int Am29lv160d_8x2_ewrite(struct flashinfo *,volatile unsigned short *,volatile unsigned short *,int);
extern void End_Am29lv160d_8x2_ewrite(void);

extern int Am29lv160d_8x2_type(struct flashinfo *);
extern void End_Am29lv160d_8x2_type(void);

