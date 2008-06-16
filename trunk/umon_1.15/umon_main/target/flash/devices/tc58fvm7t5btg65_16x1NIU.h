/* Manufacturer and device ids for top/bottom boot devices.
 */
#define tc58fvm7t5btg65 0x98001b
#define tc58fvm7b5btg65 0x98001d

/* Declarations for TC58FVM7B5BTG65 device interface functions:
 */
extern int tc58fvm7t5btg65_16x1_erase(struct flashinfo *,int);
extern void Endtc58fvm7t5btg65_16x1_erase(void);

extern int tc58fvm7t5btg65_16x1_write(struct flashinfo *,unsigned char *,unsigned char *,long);
extern void Endtc58fvm7t5btg65_16x1_write(void);

extern int tc58fvm7t5btg65_16x1_ewrite(struct flashinfo *,volatile unsigned short *,volatile unsigned short *,int);
extern void Endtc58fvm7t5btg65_16x1_ewrite(void);

extern int tc58fvm7t5btg65_16x1_type(struct flashinfo *);
extern void Endtc58fvm7t5btg65_16x1_type(void);

