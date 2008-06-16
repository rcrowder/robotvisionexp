/* Manufacturer and device ids for top/bottom boot devices.
 */
#define AMD29DL160DT			0x000122C4	/* Top */
#define AMD29DL160DB			0x00012249	/* Bottom */

/* Declarations for AM29LV160D device interface functions:
 */
extern int Am29lv160d_16x1_erase(struct flashinfo *,int);
extern void End_Am29lv160d_16x1_erase(void);

extern int Am29lv160d_16x1_write(struct flashinfo *,unsigned char *,unsigned char *,long);
extern void End_Am29lv160d_16x1_write(void);

extern int Am29lv160d_16x1_ewrite(struct flashinfo *,unsigned char *,unsigned char *,long);
extern void End_Am29lv160d_16x1_ewrite(void);

extern int Am29lv160d_16x1_type(struct flashinfo *);
extern void End_Am29lv160d_16x1_type(void);

