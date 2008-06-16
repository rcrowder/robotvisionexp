/* Manufacturer and device ids for top/bottom boot devices.
 */
#define S29WS128N		 0x0001227e

/* Declarations for S29ws128n device interface functions:
 */
extern int S29ws128n_16x2_erase(struct flashinfo *,int);
extern void End_S29ws128n_16x2_erase(void);

extern int S29ws128n_16x2_write(struct flashinfo *,unsigned char *,unsigned char *,long);
extern void End_S29ws128n_16x2_write(void);

extern int S29ws128n_16x2_ewrite(struct flashinfo *,unsigned char *,unsigned char *,long);
extern void End_S29ws128n_16x2_ewrite(void);

extern int S29ws128n_16x2_type(struct flashinfo *);
extern void End_S29ws128n_16x2_type(void);

