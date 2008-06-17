/*    Copyright (C) 2007  Virtual Cogs Embedded Systems Inc.

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

	www.virtualcogs.com
	Virtual Cogs Embedded Systems Inc., 5694 Highway 7 East, Unit 4, Suite 311
	Markham, ON, Canada L3P 1B4
*/
#ifndef LPC2136
#define LPC2136


typedef unsigned int UINT32;
typedef unsigned short UINT16;
typedef unsigned char UCHAR;
typedef unsigned char UINT8;
typedef unsigned int UINT;
typedef unsigned short USHORT;

//#define (*(volatile UINT32 *)0x)


//system control block
#define	EXTINT		(*(volatile UINT32 *)0xe01fc140)
#define EXTWAKE		(*(volatile UINT32 *)0xe01fc144)
#define EXTMODE		(*(volatile UINT32 *)0xe01fc148)
#define EXTPOLAR	(*(volatile UINT32 *)0xe01fc14c)
#define MEMMAP		(*(volatile UINT32 *)0xe01fc040)
#define PLLCON		(*(volatile UINT32 *)0xe01fc080)
#define PLLCFG		(*(volatile UINT32 *)0xe01fc084)
#define PLLSTAT		(*(volatile UINT32 *)0xe01fc088)
#define PLLFEED		(*(volatile UINT32 *)0xe01fc08c)
#define PCON		(*(volatile UINT32 *)0xe01fc0c0)
#define PCONP		(*(volatile UINT32 *)0xe01fc0c4)
#define VPBDIV		(*(volatile UINT32 *)0xe01fc100)
#define RSID		(*(volatile UINT32 *)0xe01fc180)
#define CSPR		(*(volatile UINT32 *)0xe01fc184)

//MAM
#define MAMCR		(*(volatile UINT32 *)0xe01fc000)
#define MAMTIM		(*(volatile UINT32 *)0xe01fc004)

//vector interupt controller
#define VICIRQSTATUS		(*(volatile UINT32 *)0xfffff000)
#define VICFIQSTATUS		(*(volatile UINT32 *)0xfffff004)
#define VICRAWINTR			(*(volatile UINT32 *)0xfffff008)
#define VICINTSELECT		(*(volatile UINT32 *)0xfffff00c)
#define VICINTENABLE		(*(volatile UINT32 *)0xfffff010)
#define VICINTENCLR			(*(volatile UINT32 *)0xfffff014)
#define VICSOFTINT			(*(volatile UINT32 *)0xfffff018)
#define VICSOFTINTCLEAR		(*(volatile UINT32 *)0xfffff01c)
#define VICPROTECTION		(*(volatile UINT32 *)0xfffff020)
#define VICVECTADDR			(*(volatile UINT32 *)0xfffff030)
#define VICDEFVECTADDR		(*(volatile UINT32 *)0xfffff034)
#define VICVECTADDR0		(*(volatile UINT32 *)0xfffff100)
#define VICVECTADDR1		(*(volatile UINT32 *)0xfffff104)
#define VICVECTADDR2		(*(volatile UINT32 *)0xfffff108)
#define VICVECTADDR3		(*(volatile UINT32 *)0xfffff10c)
#define VICVECTADDR4		(*(volatile UINT32 *)0xfffff110)
#define VICVECTADDR5		(*(volatile UINT32 *)0xfffff114)
#define VICVECTADDR6		(*(volatile UINT32 *)0xfffff118)
#define VICVECTADDR7		(*(volatile UINT32 *)0xfffff11C)
#define VICVECTADDR8		(*(volatile UINT32 *)0xfffff120)
#define VICVECTADDR9		(*(volatile UINT32 *)0xfffff124)
#define VICVECTADDR10		(*(volatile UINT32 *)0xfffff128)
#define VICVECTADDR11		(*(volatile UINT32 *)0xfffff12C)
#define VICVECTADDR12		(*(volatile UINT32 *)0xfffff130)
#define VICVECTADDR13		(*(volatile UINT32 *)0xfffff134)
#define VICVECTADDR14		(*(volatile UINT32 *)0xfffff138)
#define VICVECTADDR15		(*(volatile UINT32 *)0xfffff13C)
#define VICVECTCNTL0		(*(volatile UINT32 *)0xfffff200)
#define VICVECTCNTL1		(*(volatile UINT32 *)0xfffff204)
#define VICVECTCNTL2		(*(volatile UINT32 *)0xfffff208)
#define VICVECTCNTL3		(*(volatile UINT32 *)0xfffff20C)
#define VICVECTCNTL4		(*(volatile UINT32 *)0xfffff210)
#define VICVECTCNTL5		(*(volatile UINT32 *)0xfffff214)
#define VICVECTCNTL6		(*(volatile UINT32 *)0xfffff218)
#define VICVECTCNTL7		(*(volatile UINT32 *)0xfffff21C)
#define VICVECTCNTL8		(*(volatile UINT32 *)0xfffff220)
#define VICVECTCNTL9		(*(volatile UINT32 *)0xfffff224)
#define VICVECTCNTL10		(*(volatile UINT32 *)0xfffff228)
#define VICVECTCNTL11		(*(volatile UINT32 *)0xfffff22C)
#define VICVECTCNTL12		(*(volatile UINT32 *)0xfffff230)
#define VICVECTCNTL13		(*(volatile UINT32 *)0xfffff234)
#define VICVECTCNTL14		(*(volatile UINT32 *)0xfffff238)
#define VICVECTCNTL15		(*(volatile UINT32 *)0xfffff23C)

//pin connect block
#define PINSEL0		(*(volatile UINT32 *)0xe002c000)
#define PINSEL1		(*(volatile UINT32 *)0xe002c004)
#define PINSEL2		(*(volatile UINT32 *)0xe002c014)

//gpio
#define IO0PIN		(*(volatile UINT32 *)0xE0028000)
#define IO0SET		(*(volatile UINT32 *)0xE0028004)
#define IO0DIR		(*(volatile UINT32 *)0xE0028008)
#define IO0CLR		(*(volatile UINT32 *)0xE002800C)
#define IO1PIN		(*(volatile UINT32 *)0xE0028010)
#define IO1SET		(*(volatile UINT32 *)0xE0028014)
#define IO1DIR		(*(volatile UINT32 *)0xE0028018)
#define IO1CLR		(*(volatile UINT32 *)0xE002801C)

//uart0
#define U0RHR		(*(volatile UINT32 *)0xE000C000)
#define U0THR		(*(volatile UINT32 *)0xE000C000)
#define U0IER		(*(volatile UINT32 *)0xE000C004)
#define U0IIR		(*(volatile UINT32 *)0xE000C008)
#define U0FCR		(*(volatile UINT32 *)0xE000C008)
#define U0LCR		(*(volatile UINT32 *)0xE000C00C)
#define U0LSR		(*(volatile UINT32 *)0xE000C014)
#define U0SCR		(*(volatile UINT32 *)0xE000C01C)
#define U0DLL		(*(volatile UINT32 *)0xE000C000)
#define U0DLM		(*(volatile UINT32 *)0xE000C004)

//uart1
#define U1RHR		(*(volatile UINT32 *)0xE0010000)
#define U1THR		(*(volatile UINT32 *)0xE0010000)
#define U1IER		(*(volatile UINT32 *)0xE0010004)
#define U1IIR		(*(volatile UINT32 *)0xE0010008)
#define U1FCR		(*(volatile UINT32 *)0xE0010008)
#define U1LCR		(*(volatile UINT32 *)0xE001000C)
#define U1MCR		(*(volatile UINT32 *)0xE0010010)
#define U1LSR		(*(volatile UINT32 *)0xE0010014)
#define U1MSR		(*(volatile UINT32 *)0xE0010018)
#define U1SCR		(*(volatile UINT32 *)0xE001001C)
#define U1DLL		(*(volatile UINT32 *)0xE0010000)
#define U1DLM		(*(volatile UINT32 *)0xE0010004)

//I2C
#define I2C0CONSET	(*(volatile UINT32 *)0xE001C000)
#define I2C0STAT	(*(volatile UINT32 *)0xE001C004)
#define I2C0DAT		(*(volatile UINT32 *)0xE001C008)
#define I2C0ADR		(*(volatile UINT32 *)0xE001C00C)
#define I2C0SCLH	(*(volatile UINT32 *)0xE001C010)
#define I2C0SCLL	(*(volatile UINT32 *)0xE001C014)
#define I2C0CONCLR	(*(volatile UINT32 *)0xE001C018)
#define I2C1CONSET	(*(volatile UINT32 *)0xE005C000)
#define I2C1STAT	(*(volatile UINT32 *)0xE005C004)
#define I2C1DAT		(*(volatile UINT32 *)0xE005C008)
#define I2C1ADR		(*(volatile UINT32 *)0xE005C00C)
#define I2C1SCLH	(*(volatile UINT32 *)0xE005C010)
#define I2C1SCLL	(*(volatile UINT32 *)0xE005C014)
#define I2C1CONCLR	(*(volatile UINT32 *)0xE005C018)

//SPI
#define S0SPCR		(*(volatile UINT32 *)0xE0020000)
#define S0SPSR		(*(volatile UINT32 *)0xE0020004)
#define S0SPDR		(*(volatile UINT32 *)0xE0020008)
#define S0SPCCR		(*(volatile UINT32 *)0xE002000C)
#define S0SPINT		(*(volatile UINT32 *)0xE002001C)

//SSP
#define SSPCR0		(*(volatile UINT32 *)0xE0068000)
#define SSPCR1		(*(volatile UINT32 *)0xE0068004)
#define SSPDR		(*(volatile UINT32 *)0xE0068008)
#define SSPSR		(*(volatile UINT32 *)0xE006800c)
#define SSPCPSR		(*(volatile UINT32 *)0xE0068010)
#define SSPIMSC		(*(volatile UINT32 *)0xE0068014)
#define SSPRIS		(*(volatile UINT32 *)0xE0068018)
#define SSPMIS		(*(volatile UINT32 *)0xE006801c)
#define SSPICR		(*(volatile UINT32 *)0xE0068020)

//TIMERS
#define T0IR		(*(volatile UINT32 *)0xE0004000)
#define T0TCR		(*(volatile UINT32 *)0xE0004004)
#define T0TC		(*(volatile UINT32 *)0xE0004008)
#define T0PR		(*(volatile UINT32 *)0xE000400C)
#define T0PC		(*(volatile UINT32 *)0xE0004010)
#define T0MCR		(*(volatile UINT32 *)0xE0004014)
#define T0MR0		(*(volatile UINT32 *)0xE0004018)
#define T0MR1		(*(volatile UINT32 *)0xE000401C)
#define T0MR2		(*(volatile UINT32 *)0xE0004020)
#define T0MR3		(*(volatile UINT32 *)0xE0004024)
#define T0CCR		(*(volatile UINT32 *)0xE0004028)
#define T0CR0		(*(volatile UINT32 *)0xE000402C)
#define T0CR1		(*(volatile UINT32 *)0xE0004030)
#define T0CR2		(*(volatile UINT32 *)0xE0004034)
#define T0CR3		(*(volatile UINT32 *)0xE0004038)
#define T0EMR		(*(volatile UINT32 *)0xE000403C)
#define T0CTCR		(*(volatile UINT32 *)0xE0004070)

#define T1IR		(*(volatile UINT32 *)0xE0008000)
#define T1TCR		(*(volatile UINT32 *)0xE0008004)
#define T1TC		(*(volatile UINT32 *)0xE0008008)
#define T1PR		(*(volatile UINT32 *)0xE000800C)
#define T1PC		(*(volatile UINT32 *)0xE0008010)
#define T1MCR		(*(volatile UINT32 *)0xE0008014)
#define T1MR0		(*(volatile UINT32 *)0xE0008018)
#define T1MR1		(*(volatile UINT32 *)0xE000801C)
#define T1MR2		(*(volatile UINT32 *)0xE0008020)
#define T1MR3		(*(volatile UINT32 *)0xE0008024)
#define T1CCR		(*(volatile UINT32 *)0xE0008028)
#define T1CR0		(*(volatile UINT32 *)0xE000802C)
#define T1CR1		(*(volatile UINT32 *)0xE0008030)
#define T1CR2		(*(volatile UINT32 *)0xE0008034)
#define T1CR3		(*(volatile UINT32 *)0xE0008038)
#define T1EMR		(*(volatile UINT32 *)0xE000803C)
#define T1CTCR		(*(volatile UINT32 *)0xE0008070)

//PWM
#define PWMIR		(*(volatile UINT32 *)0xE0014000)
#define PWMTCR		(*(volatile UINT32 *)0xE0014004)
#define PWMTC		(*(volatile UINT32 *)0xE0014008)
#define PWMPR		(*(volatile UINT32 *)0xE001400C)
#define PWMPC		(*(volatile UINT32 *)0xE0014010)
#define PWMMCR		(*(volatile UINT32 *)0xE0014014)
#define PWMMR0		(*(volatile UINT32 *)0xE0014018)
#define PWMMR1		(*(volatile UINT32 *)0xE001401C)
#define PWMMR2		(*(volatile UINT32 *)0xE0014020)
#define PWMMR3		(*(volatile UINT32 *)0xE0014024)
#define PWMMR4		(*(volatile UINT32 *)0xE0014040)
#define PWMMR5		(*(volatile UINT32 *)0xE0014044)
#define PWMMR6		(*(volatile UINT32 *)0xE0014048)
#define PWMPCR		(*(volatile UINT32 *)0xE001404C)
#define PWMLER		(*(volatile UINT32 *)0xE0014050)

//ADC
#define AD0CR		(*(volatile UINT32 *)0xE0034000)
#define AD0DR		(*(volatile UINT32 *)0xE0034004)
#define AD1CR		(*(volatile UINT32 *)0xE0060000)
#define AD1DR		(*(volatile UINT32 *)0xE0060004)
#define ADGSR		(*(volatile UINT32 *)0xE0034008)

//DAC
#define DAC			(*(volatile UINT32 *)0xE006c000)

//RTC
#define	ILR			(*(volatile UINT32 *)0xE0024000)
#define	CTC			(*(volatile UINT32 *)0xE0024004)
#define	CCR			(*(volatile UINT32 *)0xE0024008)
#define	CIIR		(*(volatile UINT32 *)0xE002400C)
#define	AMR			(*(volatile UINT32 *)0xE0024010)
#define	CTIME0		(*(volatile UINT32 *)0xE0024014)
#define	CTIME1		(*(volatile UINT32 *)0xE0024018)
#define	CTIME2		(*(volatile UINT32 *)0xE002401C)
#define	SEC			(*(volatile UINT32 *)0xE0024020)
#define	MIN			(*(volatile UINT32 *)0xE0024024)
#define	HOUR		(*(volatile UINT32 *)0xE0024028)
#define	DOM			(*(volatile UINT32 *)0xE002402C)
#define	DOW			(*(volatile UINT32 *)0xE0024030)
#define	DOY			(*(volatile UINT32 *)0xE0024034)
#define	MONTH		(*(volatile UINT32 *)0xE0024038)
#define	YEAR		(*(volatile UINT32 *)0xE002403C)
#define	ALSEC		(*(volatile UINT32 *)0xE0024060)
#define	ALMIN		(*(volatile UINT32 *)0xE0024064)
#define	ALHOUR		(*(volatile UINT32 *)0xE0024068)
#define	ALDOM		(*(volatile UINT32 *)0xE002406C)
#define	ALDOW		(*(volatile UINT32 *)0xE0024070)
#define	ALDOY		(*(volatile UINT32 *)0xE0024074)
#define	ALMON		(*(volatile UINT32 *)0xE0024078)
#define	ALYEAR		(*(volatile UINT32 *)0xE002407C)
#define	PREINT		(*(volatile UINT32 *)0xE0024080)
#define	PREFRAC		(*(volatile UINT32 *)0xE0024084)

//WATCHDOG
#define WDMOD		(*(volatile UINT32 *)0xE0000000)
#define WDTC		(*(volatile UINT32 *)0xE0000004)
#define WDFEED		(*(volatile UINT32 *)0xE0000008)
#define WDTV		(*(volatile UINT32 *)0xE000000C)

#endif
