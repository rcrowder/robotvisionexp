/* Manufacturer and device ids for top/bottom boot devices.
 */
#define S29AL032DT				0x000122f6	/* Top */
#define S29AL032DB 				0x000122f9	/* Bottom */

/* Declarations for S29AL032D device interface functions:
 */
extern int S29al032d_16x1_erase(struct flashinfo *,int);
extern void End_S29al032d_16x1_erase(void);

extern int S29al032d_16x1_write(struct flashinfo *,unsigned char *,unsigned char *,long);
extern void End_S29al032d_16x1_write(void);

extern int S29al032d_16x1_ewrite(struct flashinfo *,unsigned char *,unsigned char *,long);
extern void End_S29al032d_16x1_ewrite(void);

extern int S29al032d_16x1_type(struct flashinfo *);
extern void End_S29al032d_16x1_type(void);

