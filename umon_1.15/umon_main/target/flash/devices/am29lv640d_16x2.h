/* Manufacturer and device ids for top/bottom boot devices.
 */
#define AMD29LV640D			 0x000122d7

/* Declarations for AM29LV640D device interface functions:
 */
extern int Am29lv640d_16x2_erase(struct flashinfo *,int);
extern void End_Am29lv640d_16x2_erase(void);

extern int Am29lv640d_16x2_write(struct flashinfo *,unsigned char *,unsigned char *,long);
extern void End_Am29lv640d_16x2_write(void);

extern int Am29lv640d_16x2_ewrite(struct flashinfo *,unsigned char *,unsigned char *,long);
extern void End_Am29lv640d_16x2_ewrite(void);

extern int Am29lv640d_16x2_type(struct flashinfo *);
extern void End_Am29lv640d_16x2_type(void);

