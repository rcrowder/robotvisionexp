/* based on LPC214x.h from NXP (Philips)                 */
/* extended with FIO-Registers by Mike Anton             */
/* extended for LPC23xx/24xx MCI, PINSEL4, PCONP by mth  */

#ifndef LPC2000_regs_h
#define LPC2000_regs_h

/* Pin Connect Block */
#define PINSEL0        (*((volatile unsigned long *) 0xE002C000))
#define PINSEL1        (*((volatile unsigned long *) 0xE002C004))

/* General Purpose Input/Output (GPIO) */
#define IOPIN0         (*((volatile unsigned long *) 0xE0028000))
#define IOSET0         (*((volatile unsigned long *) 0xE0028004))
#define IODIR0         (*((volatile unsigned long *) 0xE0028008))
#define IOCLR0         (*((volatile unsigned long *) 0xE002800C))
#define IOPIN1         (*((volatile unsigned long *) 0xE0028010))
#define IOSET1         (*((volatile unsigned long *) 0xE0028014))
#define IODIR1         (*((volatile unsigned long *) 0xE0028018))
#define IOCLR1         (*((volatile unsigned long *) 0xE002801C))

/* Local bus accessible registers - Enhanced GPIO features */
#define FIODIR0     		(*((volatile unsigned long *) (0x3FFFC000)))
#define FIOMASK0    		(*((volatile unsigned long *) (0x3FFFC010)))
#define FIOPIN0     		(*((volatile unsigned long *) (0x3FFFC014)))
#define FIOSET0     		(*((volatile unsigned long *) (0x3FFFC018)))
#define FIOCLR0     		(*((volatile unsigned long *) (0x3FFFC01C))) /* Write only */
#define FIODIR1     		(*((volatile unsigned long *) (0x3FFFC020)))
#define FIOMASK1    		(*((volatile unsigned long *) (0x3FFFC030)))
#define FIOPIN1     		(*((volatile unsigned long *) (0x3FFFC034)))
#define FIOSET1     		(*((volatile unsigned long *) (0x3FFFC038)))
#define FIOCLR1     		(*((volatile unsigned long *) (0x3FFFC03C))) /* Write only */

/* SPI0 (Serial Peripheral Interface 0) */
#define S0SPCR         (*((volatile unsigned char *) 0xE0020000))
#define S0SPSR         (*((volatile unsigned char *) 0xE0020004))
#define S0SPDR         (*((volatile unsigned char *) 0xE0020008))
#define S0SPCCR        (*((volatile unsigned char *) 0xE002000C))
#define S0SPTCR        (*((volatile unsigned char *) 0xE0020010))
#define S0SPTSR        (*((volatile unsigned char *) 0xE0020014))
#define S0SPTOR        (*((volatile unsigned char *) 0xE0020018))
#define S0SPINT        (*((volatile unsigned char *) 0xE002001C))

/* SSP Controller */
#define SSPCR0         (*((volatile unsigned short* ) 0xE0068000))
#define SSPCR1         (*((volatile unsigned char * ) 0xE0068004))
#define SSPDR          (*((volatile unsigned short* ) 0xE0068008))
#define SSPSR          (*((volatile unsigned char * ) 0xE006800C))
#define SSPCPSR        (*((volatile unsigned char * ) 0xE0068010))
#define SSPIMSC        (*((volatile unsigned char * ) 0xE0068014))
#define SSPRIS         (*((volatile unsigned char * ) 0xE0068018))
#define SSPMIS         (*((volatile unsigned char * ) 0xE006801C))
#define SSPICR         (*((volatile unsigned char * ) 0xE0068020))
#define SSPDMACR       (*((volatile unsigned char * ) 0xE0068024))

/* Real Time Clock */
/* maybe useful for the efsl time-handling : */
#define ILR            (*((volatile unsigned char *) 0xE0024000))
#define CTC            (*((volatile unsigned short*) 0xE0024004))
#define CCR            (*((volatile unsigned char *) 0xE0024008))
#define CIIR           (*((volatile unsigned char *) 0xE002400C))
#define AMR            (*((volatile unsigned char *) 0xE0024010))
#define CTIME0         (*((volatile unsigned long *) 0xE0024014))
#define CTIME1         (*((volatile unsigned long *) 0xE0024018))
#define CTIME2         (*((volatile unsigned long *) 0xE002401C))
#define SEC            (*((volatile unsigned char *) 0xE0024020))
#define MIN            (*((volatile unsigned char *) 0xE0024024))
#define HOUR           (*((volatile unsigned char *) 0xE0024028))
#define DOM            (*((volatile unsigned char *) 0xE002402C))
#define DOW            (*((volatile unsigned char *) 0xE0024030))
#define DOY            (*((volatile unsigned short*) 0xE0024034))
#define MONTH          (*((volatile unsigned char *) 0xE0024038))
#define YEAR           (*((volatile unsigned short*) 0xE002403C))
#define ALSEC          (*((volatile unsigned char *) 0xE0024060))
#define ALMIN          (*((volatile unsigned char *) 0xE0024064))
#define ALHOUR         (*((volatile unsigned char *) 0xE0024068))
#define ALDOM          (*((volatile unsigned char *) 0xE002406C))
#define ALDOW          (*((volatile unsigned char *) 0xE0024070))
#define ALDOY          (*((volatile unsigned short*) 0xE0024074))
#define ALMON          (*((volatile unsigned char *) 0xE0024078))
#define ALYEAR         (*((volatile unsigned short*) 0xE002407C))
#define PREINT         (*((volatile unsigned short*) 0xE0024080))
#define PREFRAC        (*((volatile unsigned short*) 0xE0024084))


/* LPC23xx/24xx MCI-Support */
#define PCONP          (*((volatile unsigned long *) 0xE01FC0C4))
#define PINSEL4        (*((volatile unsigned long *) 0xE002C010))

/* MultiMedia Card Interface(MCI) Controller */
#define MCI_BASE_ADDR		0xE008C000
#define MCI_POWER      (*(volatile unsigned long *)(MCI_BASE_ADDR + 0x00))
#define MCI_CLOCK      (*(volatile unsigned long *)(MCI_BASE_ADDR + 0x04))
#define MCI_ARGUMENT   (*(volatile unsigned long *)(MCI_BASE_ADDR + 0x08))
#define MCI_COMMAND    (*(volatile unsigned long *)(MCI_BASE_ADDR + 0x0C))
#define MCI_RESP_CMD   (*(volatile unsigned long *)(MCI_BASE_ADDR + 0x10))
#define MCI_RESP0      (*(volatile unsigned long *)(MCI_BASE_ADDR + 0x14))
#define MCI_RESP1      (*(volatile unsigned long *)(MCI_BASE_ADDR + 0x18))
#define MCI_RESP2      (*(volatile unsigned long *)(MCI_BASE_ADDR + 0x1C))
#define MCI_RESP3      (*(volatile unsigned long *)(MCI_BASE_ADDR + 0x20))
#define MCI_DATA_TMR   (*(volatile unsigned long *)(MCI_BASE_ADDR + 0x24))
#define MCI_DATA_LEN   (*(volatile unsigned long *)(MCI_BASE_ADDR + 0x28))
#define MCI_DATA_CTRL  (*(volatile unsigned long *)(MCI_BASE_ADDR + 0x2C))
#define MCI_DATA_CNT   (*(volatile unsigned long *)(MCI_BASE_ADDR + 0x30))
#define MCI_STATUS     (*(volatile unsigned long *)(MCI_BASE_ADDR + 0x34))
#define MCI_CLEAR      (*(volatile unsigned long *)(MCI_BASE_ADDR + 0x38))
#define MCI_MASK0      (*(volatile unsigned long *)(MCI_BASE_ADDR + 0x3C))
#define MCI_MASK1      (*(volatile unsigned long *)(MCI_BASE_ADDR + 0x40))
#define MCI_FIFO_CNT   (*(volatile unsigned long *)(MCI_BASE_ADDR + 0x48))
#define MCI_FIFO       (*(volatile unsigned long *)(MCI_BASE_ADDR + 0x80))

#endif 
