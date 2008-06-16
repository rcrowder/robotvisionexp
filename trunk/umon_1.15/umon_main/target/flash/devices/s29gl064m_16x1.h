/* Manufacturer and device ids for top/bottom boot devices.
 */
#define S29GL064M			0x0001227E

/* Declarations for S29GL064M device interface functions:
 */
extern int S29gl064m_16x1_erase(struct flashinfo *,int);
extern void End_S29gl064m_16x1_erase(void);

extern int S29gl064m_16x1_write(struct flashinfo *,unsigned char *,unsigned char *,long);
extern void End_S29gl064m_16x1_write(void);

extern int S29gl064m_16x1_ewrite(struct flashinfo *,volatile unsigned short *,volatile unsigned short *,int);
extern void End_S29gl064m_16x1_ewrite(void);

extern int S29gl064m_16x1_type(struct flashinfo *);
extern void End_S29gl064m_16x1_type(void);

