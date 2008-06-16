/* Manufacturer and device ids for top/bottom boot devices.
 */
#define M29W320DT				0x002022ca	/* Top */
#define M29W320DB 				0x002022cb	/* Bottom */
#define M29W320ET 				0x00202256	/* Top */
#define M29W320EB 				0x00202257	/* Bottom */

/* Declarations for M29W320X device interface functions:
 */
extern int M29w320d_16x1_erase(struct flashinfo *,int);
extern void End_M29w320d_16x1_erase(void);

extern int M29w320d_16x1_write(struct flashinfo *,unsigned char *,unsigned char *,long);
extern void End_M29w320d_16x1_write(void);

extern int M29w320d_16x1_ewrite(struct flashinfo *,unsigned char *,unsigned char *,long);
extern void End_M29w320d_16x1_ewrite(void);

extern int M29w320d_16x1_type(struct flashinfo *);
extern void End_M29w320d_16x1_type(void);

