/*********************************************************************
 *
 * Copyright:
 *	MOTOROLA, INC. All Rights Reserved.  
 *  You are hereby granted a copyright license to use, modify, and
 *  distribute the SOFTWARE so long as this entire notice is
 *  retained without alteration in any modified and/or redistributed
 *  versions, and that such modified versions are clearly identified
 *  as such. No licenses are granted by implication, estoppel or
 *  otherwise under any patents or trademarks of Motorola, Inc. This 
 *  software is provided on an "AS IS" basis and without warranty.
 *
 *  To the maximum extent permitted by applicable law, MOTOROLA 
 *  DISCLAIMS ALL WARRANTIES WHETHER EXPRESS OR IMPLIED, INCLUDING 
 *  IMPLIED WARRANTIES OF MERCHANTABILITY OR FITNESS FOR A PARTICULAR
 *  PURPOSE AND ANY WARRANTY AGAINST INFRINGEMENT WITH REGARD TO THE 
 *  SOFTWARE (INCLUDING ANY MODIFIED VERSIONS THEREOF) AND ANY 
 *  ACCOMPANYING WRITTEN MATERIALS.
 * 
 *  To the maximum extent permitted by applicable law, IN NO EVENT
 *  SHALL MOTOROLA BE LIABLE FOR ANY DAMAGES WHATSOEVER (INCLUDING 
 *  WITHOUT LIMITATION, DAMAGES FOR LOSS OF BUSINESS PROFITS, BUSINESS 
 *  INTERRUPTION, LOSS OF BUSINESS INFORMATION, OR OTHER PECUNIARY
 *  LOSS) ARISING OF THE USE OR INABILITY TO USE THE SOFTWARE.   
 * 
 *  Motorola assumes no responsibility for the maintenance and support
 *  of this software
 ********************************************************************/

/*
 * File:		MCF5272.h
 * Purpose:		MCF5272 definitions
 *
 * Notes:
 */

#ifndef _CPU_MCF5272_H
#define _CPU_MCF5272_H

/***********************************************************************/
/*
 * Misc. Defines
 */

#ifdef	FALSE
#undef	FALSE
#endif
#define FALSE	(0)

#ifdef	TRUE
#undef	TRUE
#endif
#define	TRUE	(1)

#ifdef	NULL
#undef	NULL
#endif
#define NULL	(0)

/***********************************************************************/
/*
 * The basic data types
 */

#ifndef ASSEMBLY_ONLY
typedef unsigned char		uint8;  /*  8 bits */
typedef unsigned short int	uint16; /* 16 bits */
typedef unsigned long int	uint32; /* 32 bits */

typedef signed char			int8;   /*  8 bits */
typedef signed short int	int16;  /* 16 bits */
typedef signed long int		int32;  /* 32 bits */
#endif

/***********************************************************************/
/*
 * Common M68K & ColdFire definitions
 */

#define ADDRESS			uint32
#define INSTRUCTION		uint16
#define ILLEGAL			0x4AFC
#define CPU_WORD_SIZE	16

/***********************************************************************/

/*
 * Routines and macros for accessing Input/Output devices
 */

#define cpu_iord_8(ADDR)        *((volatile uint8 *)(ADDR))
#define cpu_iord_16(ADDR)       *((volatile uint16 *)(ADDR))
#define cpu_iord_32(ADDR)       *((volatile uint32 *)(ADDR))

#define cpu_iowr_8(ADDR,DATA)   *((volatile uint8 *)(ADDR)) = (DATA)
#define cpu_iowr_16(ADDR,DATA)  *((volatile uint16 *)(ADDR)) = (DATA)
#define cpu_iowr_32(ADDR,DATA)  *((volatile uint32 *)(ADDR)) = (DATA)

/***********************************************************************/

#define MCF5200_SR_T		(0x8000)
#define MCF5200_SR_S		(0x2000)
#define MCF5200_SR_M		(0x1000)
#define MCF5200_SR_IPL		(0x0700)
#define MCF5200_SR_IPL_0	(0x0000)
#define MCF5200_SR_IPL_1	(0x0100)
#define MCF5200_SR_IPL_2	(0x0200)
#define MCF5200_SR_IPL_3	(0x0300)
#define MCF5200_SR_IPL_4	(0x0400)
#define MCF5200_SR_IPL_5	(0x0500)
#define MCF5200_SR_IPL_6	(0x0600)
#define MCF5200_SR_IPL_7	(0x0700)
#define MCF5200_SR_X		(0x0010)
#define MCF5200_SR_N		(0x0008)
#define MCF5200_SR_Z		(0x0004)
#define MCF5200_SR_V		(0x0002)
#define MCF5200_SR_C		(0x0001)

/***********************************************************************/

/*
 * The ColdFire family of processors has a simplified exception stack
 * frame that looks like the following:
 *
 *              3322222222221111 111111
 *              1098765432109876 5432109876543210
 *           8 +----------------+----------------+
 *             |         Program Counter         |
 *           4 +----------------+----------------+
 *             |FS/Fmt/Vector/FS|      SR        |
 *   SP -->  0 +----------------+----------------+
 *
 * The stack self-aligns to a 4-byte boundary at an exception, with
 * the FS/Fmt/Vector/FS field indicating the size of the adjustment
 * (SP += 0,1,2,3 bytes).
 */

#define MCF5200_RD_SF_FORMAT(PTR)	\
	((*((uint16 *)(PTR)) >> 12) & 0x00FF)

#define MCF5200_RD_SF_VECTOR(PTR)	\
	((*((uint16 *)(PTR)) >>  2) & 0x00FF)

#define MCF5200_RD_SF_FS(PTR)		\
	( ((*((uint16 *)(PTR)) & 0x0C00) >> 8) | (*((uint16 *)(PTR)) & 0x0003) )

#define MCF5200_SF_SR(PTR)	*((uint16 *)(PTR)+1)
#define MCF5200_SF_PC(PTR)	*((uint32 *)(PTR)+1)

#if 0
typedef struct
{
	uint16	SR;
	uint16	FS_FMT_VECTOR_FS;
	uint32	PC;
} MCF5200_STACK_FRAME;
#endif


/**********************************************************************
*
* Macro for computing address of on-chip peripheral registers
*
***********************************************************************/

#define Mcf5272_addr(IMMP,OFFSET)   ((void *)&((uint8 *)IMMP)[OFFSET])

/**********************************************************************
*
* Macros for accessing the on-chip I/O resources
*
***********************************************************************/

#define Mcf5272_iord8(IMMP,OFFSET)		\
    *(volatile unsigned char *)(Mcf5272_addr(IMMP,OFFSET))
#define Mcf5272_iord16(IMMP,OFFSET)		\
    *(volatile unsigned short *)(Mcf5272_addr(IMMP,OFFSET))
#define Mcf5272_iord32(IMMP,OFFSET)		\
    *(volatile unsigned long *)(Mcf5272_addr(IMMP,OFFSET))

#define Mcf5272_iowr8(IMMP,OFFSET,DATA)  \
    *(volatile unsigned char *)(Mcf5272_addr(IMMP,OFFSET)) = (DATA)
#define Mcf5272_iowr16(IMMP,OFFSET,DATA)  \
    *(volatile unsigned short *)(Mcf5272_addr(IMMP,OFFSET)) = (DATA)
#define Mcf5272_iowr32(IMMP,OFFSET,DATA)  \
    *(volatile unsigned long *)(Mcf5272_addr(IMMP,OFFSET)) = (DATA)

/**********************************************************************
*
* CPU Space Registers
*
***********************************************************************/

/* Bit level definitions and macros */
#define MCF5272_CACR_CENB		(0x80000000)
#define MCF5272_CACR_CFRZ		(0x08000000)
#define MCF5272_CACR_CINV		(0x01000000)
#define MCF5272_CACR_CMOD		(0x00000200)
#define MCF5272_CACR_CWRP		(0x00000020)
#define MCF5272_CACR_CLNF_00	(0x00000000)
#define MCF5272_CACR_CLNF_01	(0x00000001)
#define MCF5272_CACR_CLNF_10	(0x00000002)

#define MCF5272_ACR_BASE(a)		((a)&0xFF000000)
#define MCF5272_ACR_MASK(a)		(((a)&0xFF000000) >> 8)
#define MCF5272_ACR_EN			(0x00008000)
#define MCF5272_ACR_S_USER		(0x00000000)
#define MCF5272_ACR_S_SUPER		(0x00002000)
#define MCF5272_ACR_S_IGNORE	(0x00006000)
#define MCF5272_ACR_ENIB		(0x00000080)
#define MCF5272_ACR_CM			(0x00000040)
#define MCF5272_ACR_WP			(0x00000004)

#define MCF5272_SRAMBAR_BASE(a)	((a)&0xFFFFF000)
#define MCF5272_SRAMBAR_WP		(0x00000100)
#define MCF5272_SRAMBAR_CI		(0x00000020)
#define MCF5272_SRAMBAR_SC		(0x00000010)
#define MCF5272_SRAMBAR_SD		(0x00000008)
#define MCF5272_SRAMBAR_UC		(0x00000004)
#define MCF5272_SRAMBAR_UD		(0x00000002)
#define MCF5272_SRAMBAR_V		(0x00000001)

#define MCF5272_ROMBAR_BASE(a)	((a)&0xFFFFF000)
#define MCF5272_ROMBAR_WP		(0x00000100)
#define MCF5272_ROMBAR_CI		(0x00000080)
#define MCF5272_ROMBAR_SC		(0x00000040)
#define MCF5272_ROMBAR_SD		(0x00000020)
#define MCF5272_ROMBAR_UC		(0x00000004)
#define MCF5272_ROMBAR_UD		(0x00000002)
#define MCF5272_ROMBAR_V		(0x00000001)

#define MCF5272_MBAR_BASE(a)	((a)&0xFFFFFC00)
#define MCF5272_MBAR_SC			(0x00000010)
#define MCF5272_MBAR_SD			(0x00000008)
#define MCF5272_MBAR_UC			(0x00000004)
#define MCF5272_MBAR_UD			(0x00000002)
#define MCF5272_MBAR_V			(0x00000001)

/**********************************************************************
*
* System Configuration Registers
*
***********************************************************************/

/* Offsets of the registers from the MBAR */
#define MCF5272_SIM_MBAR		(0x0000)
#define MCF5272_SIM_SCR			(0x0004)
#define MCF5272_SIM_SPR			(0x0006)
#define MCF5272_SIM_PMR			(0x0008)
#define MCF5272_SIM_ALPR		(0x000E)
#define MCF5272_SIM_DIR			(0x0010)

/* Read access macros for general use */
#define MCF5272_RD_SIM_MBAR(IMMP)	Mcf5272_iord32(IMMP,MCF5272_SIM_MBAR)
#define MCF5272_RD_SIM_SCR(IMMP)	Mcf5272_iord16(IMMP,MCF5272_SIM_SCR)
#define MCF5272_RD_SIM_SPR(IMMP)	Mcf5272_iord16(IMMP,MCF5272_SIM_SPR)
#define MCF5272_RD_SIM_PMR(IMMP)	Mcf5272_iord32(IMMP,MCF5272_SIM_PMR)
#define MCF5272_RD_SIM_DIR(IMMP)	Mcf5272_iord32(IMMP,MCF5272_SIM_DIR)

/* Write access macros for general use */
#define MCF5272_WR_SIM_SCR(IMMP,DATA)	\
	Mcf5272_iowr16(IMMP,MCF5272_SIM_SCR,DATA)
#define MCF5272_WR_SIM_SPR(IMMP,DATA)	\
	Mcf5272_iowr16(IMMP,MCF5272_SIM_SPR,DATA)
#define MCF5272_WR_SIM_PMR(IMMP,DATA)	\
	Mcf5272_iowr32(IMMP,MCF5272_SIM_PMR,DATA)
#define MCF5272_WR_SIM_ALPR(IMMP,DATA)	\
	Mcf5272_iowr16(IMMP,MCF5272_SIM_ALPR,DATA)

/* Bit level definitions and macros */
#define MCF5272_SIM_SCR_HRST		0x1000
#define MCF5272_SIM_SCR_DRAMRST		0x3000
#define MCF5272_SIM_SCR_SWTR		0x2000
#define MCF5272_SIM_SCR_AR			0x0080
#define MCF5272_SIM_SCR_SOFT_RES	0x0040
#define MCF5272_SIM_SCR_HWWD_128	0x0000
#define MCF5272_SIM_SCR_HWWD_256	0x0001
#define MCF5272_SIM_SCR_HWWD_512	0x0002
#define MCF5272_SIM_SCR_HWWD_1024	0x0003
#define MCF5272_SIM_SCR_HWWD_2048	0x0004
#define MCF5272_SIM_SCR_HWWD_4096	0x0005
#define MCF5272_SIM_SCR_HWWD_8192	0x0006
#define MCF5272_SIM_SCR_HWWD_16384	0x0007

#define MCF5272_SIM_SPR_ADC			0x8000
#define MCF5272_SIM_SPR_ADCEN		0x0080
#define MCF5272_SIM_SPR_WPV 		0x4000
#define MCF5272_SIM_SPR_WPVEN		0x0040
#define MCF5272_SIM_SPR_SMV			0x2000
#define MCF5272_SIM_SPR_SMVEN		0x0020
#define MCF5272_SIM_SPR_SBE			0x1000
#define MCF5272_SIM_SPR_SBEEN		0x0010
#define MCF5272_SIM_SPR_HWT			0x0800
#define MCF5272_SIM_SPR_HWTEN		0x0008
#define MCF5272_SIM_SPR_RPV 		0x0400
#define MCF5272_SIM_SPR_RPVEN		0x0004
#define MCF5272_SIM_SPR_EXT			0x0200
#define MCF5272_SIM_SPR_EXTEN		0x0002
#define MCF5272_SIM_SPR_SUV			0x0100
#define MCF5272_SIM_SPR_SUVEN		0x0001

#define MCF5272_SIM_PMR_BDMPDN      0x80000000
#define MCF5272_SIM_PMR_ENETPDN     0x04000000
#define MCF5272_SIM_PMR_PLIPPDN     0x02000000
#define MCF5272_SIM_PMR_DRAMPDN     0x01000000
#define MCF5272_SIM_PMR_DMAPDN      0x00800000
#define MCF5272_SIM_PMR_PWMPDN      0x00400000
#define MCF5272_SIM_PMR_QSPIPDN     0x00200000
#define MCF5272_SIM_PMR_TIMERPDN    0x00100000
#define MCF5272_SIM_PMR_GPIOPDN     0x00080000
#define MCF5272_SIM_PMR_USBPDN      0x00040000
#define MCF5272_SIM_PMR_UART1PDN    0x00020000
#define MCF5272_SIM_PMR_UART0PDN    0x00010000
#define MCF5272_SIM_PMR_USBWK       0x00000400
#define MCF5272_SIM_PMR_UART1WK     0x00000200
#define MCF5272_SIM_PMR_UART0WK     0x00000100
#define MCF5272_SIM_PMR_MOS         0x00000020
#define MCF5272_SIM_PMR_SLPEN       0x00000010

/**********************************************************************
*
* Interrupt Controller Registers
*
***********************************************************************/

/* Offsets of the registers from the MBAR */
#define MCF5272_SIM_ICR1		(0x0020)
#define MCF5272_SIM_ICR2		(0x0024)
#define MCF5272_SIM_ICR3		(0x0028)
#define MCF5272_SIM_ICR4		(0x002C)
#define MCF5272_SIM_ISR			(0x0030)
#define MCF5272_SIM_PITR		(0x0034)
#define MCF5272_SIM_PIWR		(0x0038)
#define MCF5272_SIM_PIVR		(0x003F)

/* Read access macros for general use */
#define MCF5272_RD_SIM_ICR1(IMMP)	Mcf5272_iord32(IMMP,MCF5272_SIM_ICR1)
#define MCF5272_RD_SIM_ICR2(IMMP)	Mcf5272_iord32(IMMP,MCF5272_SIM_ICR2)
#define MCF5272_RD_SIM_ICR3(IMMP)	Mcf5272_iord32(IMMP,MCF5272_SIM_ICR3)
#define MCF5272_RD_SIM_ICR4(IMMP)	Mcf5272_iord32(IMMP,MCF5272_SIM_ICR4)
#define MCF5272_RD_SIM_ISR(IMMP)	Mcf5272_iord32(IMMP,MCF5272_SIM_ISR)
#define MCF5272_RD_SIM_PITR(IMMP)	Mcf5272_iord32(IMMP,MCF5272_SIM_PITR)
#define MCF5272_RD_SIM_PIWR(IMMP)	Mcf5272_iord32(IMMP,MCF5272_SIM_PIWR)
#define MCF5272_RD_SIM_PIVR(IMMP)	Mcf5272_iord8(IMMP,MCF5272_SIM_PIVR)

/* Write access macros for general use */
#define MCF5272_WR_SIM_ICR1(IMMP,DATA)	\
	Mcf5272_iowr32(IMMP,MCF5272_SIM_ICR1,DATA)
#define MCF5272_WR_SIM_ICR2(IMMP,DATA)	\
	Mcf5272_iowr32(IMMP,MCF5272_SIM_ICR2,DATA)
#define MCF5272_WR_SIM_ICR3(IMMP,DATA)	\
	Mcf5272_iowr32(IMMP,MCF5272_SIM_ICR3,DATA)
#define MCF5272_WR_SIM_ICR4(IMMP,DATA)	\
	Mcf5272_iowr32(IMMP,MCF5272_SIM_ICR4,DATA)
#define MCF5272_WR_SIM_PITR(IMMP,DATA)	\
	Mcf5272_iowr32(IMMP,MCF5272_SIM_PITR,DATA)
#define MCF5272_WR_SIM_PIWR(IMMP,DATA)	\
	Mcf5272_iowr32(IMMP,MCF5272_SIM_PIWR,DATA)
#define MCF5272_WR_SIM_PIVR(IMMP,DATA)	\
	Mcf5272_iowr8(IMMP,MCF5272_SIM_PIVR,DATA)

/* Bit level definitions and macros */
#define MCF5272_SIM_ICR_INT1_IL(a)	( 0x80000000 | (((a)&0x07)<<28)	)
#define MCF5272_SIM_ICR_INT2_IL(a)	( 0x08000000 | (((a)&0x07)<<24)	)
#define MCF5272_SIM_ICR_INT3_IL(a)	( 0x00800000 | (((a)&0x07)<<20)	)
#define MCF5272_SIM_ICR_INT4_IL(a)	( 0x00080000 | (((a)&0x07)<<16)	)
#define MCF5272_SIM_ICR_TMR0_IL(a)	( 0x00008000 | (((a)&0x07)<<12)	)
#define MCF5272_SIM_ICR_TMR1_IL(a)	( 0x00000800 | (((a)&0x07)<<8) )
#define MCF5272_SIM_ICR_TMR2_IL(a)	( 0x00000080 | (((a)&0x07)<<4) )
#define MCF5272_SIM_ICR_TMR3_IL(a)	( 0x00000008 | ((a)&0x07) )
#define MCF5272_SIM_ICR_TMR_IL(a,x)	( (0x8 | ((a)&0x07)) << ((3-x)*4))

#define MCF5272_SIM_ICR_UART0_IL(a)	( 0x80000000 | (((a)&0x07)<<28)	)
#define MCF5272_SIM_ICR_UART1_IL(a)	( 0x08000000 | (((a)&0x07)<<24)	)
#define MCF5272_SIM_ICR_PLIP_IL(a)	( 0x00800000 | (((a)&0x07)<<20)	)
#define MCF5272_SIM_ICR_PLIA_IL(a)	( 0x00080000 | (((a)&0x07)<<16)	)
#define MCF5272_SIM_ICR_USB0_IL(a)	( 0x00008000 | (((a)&0x07)<<12)	)
#define MCF5272_SIM_ICR_USB1_IL(a)	( 0x00000800 | (((a)&0x07)<<8) )
#define MCF5272_SIM_ICR_USB2_IL(a)	( 0x00000800 | (((a)&0x07)<<4) )
#define MCF5272_SIM_ICR_USB3_IL(a)	( 0x00000800 | ((a)&0x07) )

#define MCF5272_SIM_ICR_USB4_IL(a)	( 0x80000000 | (((a)&0x07)<<28)	)
#define MCF5272_SIM_ICR_USB5_IL(a)	( 0x08000000 | (((a)&0x07)<<24)	)
#define MCF5272_SIM_ICR_USB6_IL(a)	( 0x00800000 | (((a)&0x07)<<20)	)
#define MCF5272_SIM_ICR_USB7_IL(a)	( 0x00080000 | (((a)&0x07)<<16)	)
#define MCF5272_SIM_ICR_DMA_IL(a)	( 0x00008000 | (((a)&0x07)<<12)	)
#define MCF5272_SIM_ICR_ERX_IL(a)	( 0x00000800 | (((a)&0x07)<<8) )
#define MCF5272_SIM_ICR_ETX_IL(a)	( 0x00000080 | (((a)&0x07)<<4) )
#define MCF5272_SIM_ICR_ENTC_IL(a)	( 0x00000008 | ((a)&0x07) )

#define MCF5272_SIM_ICR_QSPI_IL(a)	( 0x80000000 | (((a)&0x07)<<28)	)
#define MCF5272_SIM_ICR_INT5_IL(a)	( 0x08000000 | (((a)&0x07)<<24)	)
#define MCF5272_SIM_ICR_INT6_IL(a)	( 0x00800000 | (((a)&0x07)<<20)	)
#define MCF5272_SIM_ICR_SWTO_IL(a)	( 0x00080000 | (((a)&0x07)<<16)	)

#define MCF5272_SIM_PITR_POS_EDGE	(0xF0000060)
#define MCF5272_SIM_PITR_NEG_EDGE	(0x00000000)

#define MCF5272_SIM_PIWR_INT1			(0x80000000)
#define MCF5272_SIM_PIWR_INT2			(0x40000000)
#define MCF5272_SIM_PIWR_INT3			(0x20000000)
#define MCF5272_SIM_PIWR_INT4			(0x10000000)
#define MCF5272_SIM_PIWR_TMR0			(0x08000000)
#define MCF5272_SIM_PIWR_TMR1			(0x04000000)
#define MCF5272_SIM_PIWR_TMR2			(0x02000000)
#define MCF5272_SIM_PIWR_TMR3			(0x01000000)
#define MCF5272_SIM_PIWR_UART0		(0x00800000)
#define MCF5272_SIM_PIWR_UART1		(0x00400000)
#define MCF5272_SIM_PIWR_PLIP			(0x00200000)
#define MCF5272_SIM_PIWR_PLIA			(0x00100000)
#define MCF5272_SIM_PIWR_USB_0			(0x00080000)
#define MCF5272_SIM_PIWR_USB_1			(0x00040000)
#define MCF5272_SIM_PIWR_USB_2			(0x00020000)
#define MCF5272_SIM_PIWR_USB_3			(0x00010000)
#define MCF5272_SIM_PIWR_USB_4			(0x00008000)
#define MCF5272_SIM_PIWR_USB_5			(0x00004000)
#define MCF5272_SIM_PIWR_USB_6			(0x00002000)
#define MCF5272_SIM_PIWR_USB_7			(0x00001000)
#define MCF5272_SIM_PIWR_DMA			(0x00000800)
#define MCF5272_SIM_PIWR_ERx			(0x00000400)
#define MCF5272_SIM_PIWR_ETx			(0x00000200)
#define MCF5272_SIM_PIWR_ENTC			(0x00000100)
#define MCF5272_SIM_PIWR_QSPI			(0x00000080)
#define MCF5272_SIM_PIWR_INT5			(0x00000040)
#define MCF5272_SIM_PIWR_INT6			(0x00000020)
#define MCF5272_SIM_PIWR_SWTO			(0x00000010)

#define MCF5272_SIM_PIVR_IL(a)		(((a)&0x07)<<5)
#define MCF5272_SIM_PIVR_NORMAL		(0x40)

/**********************************************************************
*
* Chip Select Registers
*
***********************************************************************/

/* Offsets of the registers from the MBAR */
#define MCF5272_CS_CSBR0	(0x0040)
#define MCF5272_CS_CSOR0	(0x0044)
#define MCF5272_CS_CSBR1	(0x0048)
#define MCF5272_CS_CSOR1	(0x004C)
#define MCF5272_CS_CSBR2	(0x0050)
#define MCF5272_CS_CSOR2	(0x0054)
#define MCF5272_CS_CSBR3	(0x0058)
#define MCF5272_CS_CSOR3	(0x005C)
#define MCF5272_CS_CSBR4	(0x0060)
#define MCF5272_CS_CSOR4	(0x0064)
#define MCF5272_CS_CSBR5	(0x0068)
#define MCF5272_CS_CSOR5	(0x006C)
#define MCF5272_CS_CSBR6	(0x0070)
#define MCF5272_CS_CSOR6	(0x0074)
#define MCF5272_CS_CSBR7	(0x0078)
#define MCF5272_CS_CSOR7	(0x007C)

/* Read access macros for general use */
#define MCF5272_RD_CS_CSBR0(IMMP)	Mcf5272_iord32(IMMP,MCF5272_CS_CSBR0)
#define MCF5272_RD_CS_CSOR0(IMMP)	Mcf5272_iord32(IMMP,MCF5272_CS_CSOR0)
#define MCF5272_RD_CS_CSBR1(IMMP)	Mcf5272_iord32(IMMP,MCF5272_CS_CSBR1)
#define MCF5272_RD_CS_CSOR1(IMMP)	Mcf5272_iord32(IMMP,MCF5272_CS_CSOR1)
#define MCF5272_RD_CS_CSBR2(IMMP)	Mcf5272_iord32(IMMP,MCF5272_CS_CSBR2)
#define MCF5272_RD_CS_CSOR2(IMMP)	Mcf5272_iord32(IMMP,MCF5272_CS_CSOR2)
#define MCF5272_RD_CS_CSBR3(IMMP)	Mcf5272_iord32(IMMP,MCF5272_CS_CSBR3)
#define MCF5272_RD_CS_CSOR3(IMMP)	Mcf5272_iord32(IMMP,MCF5272_CS_CSOR3)
#define MCF5272_RD_CS_CSBR4(IMMP)	Mcf5272_iord32(IMMP,MCF5272_CS_CSBR4)
#define MCF5272_RD_CS_CSOR4(IMMP)	Mcf5272_iord32(IMMP,MCF5272_CS_CSOR4)
#define MCF5272_RD_CS_CSBR5(IMMP)	Mcf5272_iord32(IMMP,MCF5272_CS_CSBR5)
#define MCF5272_RD_CS_CSOR5(IMMP)	Mcf5272_iord32(IMMP,MCF5272_CS_CSOR5)
#define MCF5272_RD_CS_CSBR6(IMMP)	Mcf5272_iord32(IMMP,MCF5272_CS_CSBR6)
#define MCF5272_RD_CS_CSOR6(IMMP)	Mcf5272_iord32(IMMP,MCF5272_CS_CSOR6)
#define MCF5272_RD_CS_CSBR7(IMMP)	Mcf5272_iord32(IMMP,MCF5272_CS_CSBR7)
#define MCF5272_RD_CS_CSOR7(IMMP)	Mcf5272_iord32(IMMP,MCF5272_CS_CSOR7)

/* Write access macros for general use */
#define MCF5272_WR_CS_CSBR0(IMMP,DATA)	\
	Mcf5272_iowr32(IMMP,MCF5272_CS_CSBR0,DATA)
#define MCF5272_WR_CS_CSOR0(IMMP,DATA)	\
	Mcf5272_iowr32(IMMP,MCF5272_CS_CSOR0,DATA)
#define MCF5272_WR_CS_CSBR1(IMMP,DATA)	\
	Mcf5272_iowr32(IMMP,MCF5272_CS_CSBR1,DATA)
#define MCF5272_WR_CS_CSOR1(IMMP,DATA)	\
	Mcf5272_iowr32(IMMP,MCF5272_CS_CSOR1,DATA)
#define MCF5272_WR_CS_CSBR2(IMMP,DATA)	\
	Mcf5272_iowr32(IMMP,MCF5272_CS_CSBR2,DATA)
#define MCF5272_WR_CS_CSOR2(IMMP,DATA)	\
	Mcf5272_iowr32(IMMP,MCF5272_CS_CSOR2,DATA)
#define MCF5272_WR_CS_CSBR3(IMMP,DATA)	\
	Mcf5272_iowr32(IMMP,MCF5272_CS_CSBR3,DATA)
#define MCF5272_WR_CS_CSOR3(IMMP,DATA)	\
	Mcf5272_iowr32(IMMP,MCF5272_CS_CSOR3,DATA)
#define MCF5272_WR_CS_CSBR4(IMMP,DATA)	\
	Mcf5272_iowr32(IMMP,MCF5272_CS_CSBR4,DATA)
#define MCF5272_WR_CS_CSOR4(IMMP,DATA)	\
	Mcf5272_iowr32(IMMP,MCF5272_CS_CSOR4,DATA)
#define MCF5272_WR_CS_CSBR5(IMMP,DATA)	\
	Mcf5272_iowr32(IMMP,MCF5272_CS_CSBR5,DATA)
#define MCF5272_WR_CS_CSOR5(IMMP,DATA)	\
	Mcf5272_iowr32(IMMP,MCF5272_CS_CSOR5,DATA)
#define MCF5272_WR_CS_CSBR6(IMMP,DATA)	\
	Mcf5272_iowr32(IMMP,MCF5272_CS_CSBR6,DATA)
#define MCF5272_WR_CS_CSOR6(IMMP,DATA)	\
	Mcf5272_iowr32(IMMP,MCF5272_CS_CSOR6,DATA)
#define MCF5272_WR_CS_CSBR7(IMMP,DATA)	\
	Mcf5272_iowr32(IMMP,MCF5272_CS_CSBR7,DATA)
#define MCF5272_WR_CS_CSOR7(IMMP,DATA)	\
	Mcf5272_iowr32(IMMP,MCF5272_CS_CSOR7,DATA)

/* Bit level definitions and macros */
#define MCF5272_CS_BR_BASE(a)			((a)&0xFFFFF000)

#define MCF5272_CS_OR_MASK_32M			(0xFE000000)
#define MCF5272_CS_OR_MASK_16M			(0xFF000000)
#define MCF5272_CS_OR_MASK_8M			(0xFF800000)
#define MCF5272_CS_OR_MASK_4M			(0xFFC00000)
#define MCF5272_CS_OR_MASK_2M			(0xFFE00000)
#define MCF5272_CS_OR_MASK_1M			(0xFFF00000)
#define MCF5272_CS_OR_MASK_512K			(0xFFF80000)
#define MCF5272_CS_OR_MASK_256K			(0xFFFC0000)
#define MCF5272_CS_OR_MASK_128K			(0xFFFE0000)
#define MCF5272_CS_OR_MASK_64K			(0xFFFF0000)
#define MCF5272_CS_OR_MASK_32K			(0xFFFF8000)
#define MCF5272_CS_OR_MASK_16K			(0xFFFFC000)
#define MCF5272_CS_OR_MASK_8K			(0xFFFFE000)
#define MCF5272_CS_OR_MASK_4K			(0xFFFFF000)
#define MCF5272_CS_OR_WS_MASK			(0x007C)
#define MCF5272_CS_OR_WS(a)				(((a)&0x1F)<<2)
#define MCF5272_CS_OR_ASET				(0x8000)
#define MCF5272_CS_OR_RDAH				(0x4000)
#define MCF5272_CS_OR_WRAH				(0x2000)
#define MCF5272_CS_OR_BRST				(0x0100)
#define MCF5272_CS_OR_RD				(0x0003)
#define MCF5272_CS_OR_WR				(0x0001)

#define MCF5272_CS_BR_PS_8				(0x0100)
#define MCF5272_CS_BR_PS_16				(0x0200)
#define MCF5272_CS_BR_PS_32				(0x0000)
#define MCF5272_CS_BR_PS_LINE			(0x0300)
#define MCF5272_CS_BR_ROM				(0x0000)
#define MCF5272_CS_BR_SRAM				(0x0000)
#define MCF5272_CS_BR_SRAM_8			(0x0C00)
#define MCF5272_CS_BR_SDRAM				(0x0400)
#define MCF5272_CS_BR_ISA				(0x0800)
#define MCF5272_CS_BR_SV				(0x0080)
#define MCF5272_CS_BR_EN				(0x0001)

/**********************************************************************
*
* Ports Registers Description
*
***********************************************************************/

/* Offsets of the registers from the MBAR */
#define MCF5272_GPIO_PACNT		(0x0080)
#define MCF5272_GPIO_PADDR		(0x0084)
#define MCF5272_GPIO_PADAT		(0x0086)
#define MCF5272_GPIO_PBCNT		(0x0088)
#define MCF5272_GPIO_PBDDR		(0x008C)
#define MCF5272_GPIO_PBDAT		(0x008E)
#define MCF5272_GPIO_PCDDR		(0x0094)
#define MCF5272_GPIO_PCDAT		(0x0096)
#define MCF5272_GPIO_PDCNT		(0x0098)

/* Read access macros for general use */
#define MCF5272_RD_GPIO_PACNT(IMMP)	Mcf5272_iord32(IMMP,MCF5272_GPIO_PACNT)
#define MCF5272_RD_GPIO_PADDR(IMMP)	Mcf5272_iord16(IMMP,MCF5272_GPIO_PADDR)
#define MCF5272_RD_GPIO_PADAT(IMMP)	Mcf5272_iord16(IMMP,MCF5272_GPIO_PADAT)
#define MCF5272_RD_GPIO_PBCNT(IMMP)	Mcf5272_iord32(IMMP,MCF5272_GPIO_PBCNT)
#define MCF5272_RD_GPIO_PBDDR(IMMP)	Mcf5272_iord16(IMMP,MCF5272_GPIO_PBDDR)
#define MCF5272_RD_GPIO_PBDAT(IMMP)	Mcf5272_iord16(IMMP,MCF5272_GPIO_PBDAT)
#define MCF5272_RD_GPIO_PCDDR(IMMP)	Mcf5272_iord16(IMMP,MCF5272_GPIO_PCDDR)
#define MCF5272_RD_GPIO_PCDAT(IMMP)	Mcf5272_iord16(IMMP,MCF5272_GPIO_PCDAT)
#define MCF5272_RD_GPIO_PDCNT(IMMP)	Mcf5272_iord32(IMMP,MCF5272_GPIO_PDCNT)

/* Write access macros for general use */
#define MCF5272_WR_GPIO_PACNT(IMMP,DATA)	\
	Mcf5272_iowr32(IMMP,MCF5272_GPIO_PACNT,DATA)
#define MCF5272_WR_GPIO_PADDR(IMMP,DATA)	\
	Mcf5272_iowr16(IMMP,MCF5272_GPIO_PADDR,DATA)
#define MCF5272_WR_GPIO_PADAT(IMMP,DATA)	\
	Mcf5272_iowr16(IMMP,MCF5272_GPIO_PADAT,DATA)
#define MCF5272_WR_GPIO_PBCNT(IMMP,DATA)	\
	Mcf5272_iowr32(IMMP,MCF5272_GPIO_PBCNT,DATA)
#define MCF5272_WR_GPIO_PBDDR(IMMP,DATA)	\
	Mcf5272_iowr16(IMMP,MCF5272_GPIO_PBDDR,DATA)
#define MCF5272_WR_GPIO_PBDAT(IMMP,DATA)	\
	Mcf5272_iowr16(IMMP,MCF5272_GPIO_PBDAT,DATA)
#define MCF5272_WR_GPIO_PCDDR(IMMP,DATA)	\
	Mcf5272_iowr16(IMMP,MCF5272_GPIO_PCDDR,DATA)
#define MCF5272_WR_GPIO_PCDAT(IMMP,DATA)	\
	Mcf5272_iowr16(IMMP,MCF5272_GPIO_PCDAT,DATA)
#define MCF5272_WR_GPIO_PDCNT(IMMP,DATA)	\
	Mcf5272_iowr32(IMMP,MCF5272_GPIO_PDCNT,DATA)

/* Bit level definitions and macros */
#define MCF5272_GPIO_DDR15_INPUT	(~0x80)
#define MCF5272_GPIO_DDR15_OUTPUT	( 0x80)
#define MCF5272_GPIO_DDR14_INPUT	(~0x40)
#define MCF5272_GPIO_DDR14_OUTPUT	( 0x40)
#define MCF5272_GPIO_DDR13_INPUT	(~0x20)
#define MCF5272_GPIO_DDR13_OUTPUT	( 0x20)
#define MCF5272_GPIO_DDR12_INPUT	(~0x10)
#define MCF5272_GPIO_DDR12_OUTPUT	( 0x10)
#define MCF5272_GPIO_DDR11_INPUT	(~0x08)
#define MCF5272_GPIO_DDR11_OUTPUT	( 0x08)
#define MCF5272_GPIO_DDR10_INPUT	(~0x04)
#define MCF5272_GPIO_DDR10_OUTPUT	( 0x04)
#define MCF5272_GPIO_DDR9_INPUT		(~0x02)
#define MCF5272_GPIO_DDR9_OUTPUT	( 0x02)
#define MCF5272_GPIO_DDR8_INPUT		(~0x01)
#define MCF5272_GPIO_DDR8_OUTPUT	( 0x01)
#define MCF5272_GPIO_DDR7_INPUT		(~0x80)
#define MCF5272_GPIO_DDR7_OUTPUT	( 0x80)
#define MCF5272_GPIO_DDR6_INPUT		(~0x40)
#define MCF5272_GPIO_DDR6_OUTPUT	( 0x40)
#define MCF5272_GPIO_DDR5_INPUT		(~0x20)
#define MCF5272_GPIO_DDR5_OUTPUT	( 0x20)
#define MCF5272_GPIO_DDR4_INPUT		(~0x10)
#define MCF5272_GPIO_DDR4_OUTPUT	( 0x10)
#define MCF5272_GPIO_DDR3_INPUT		(~0x08)
#define MCF5272_GPIO_DDR3_OUTPUT	( 0x08)
#define MCF5272_GPIO_DDR2_INPUT		(~0x04)
#define MCF5272_GPIO_DDR2_OUTPUT	( 0x04)
#define MCF5272_GPIO_DDR1_INPUT		(~0x02)
#define MCF5272_GPIO_DDR1_OUTPUT	( 0x02)
#define MCF5272_GPIO_DDR0_INPUT		(~0x01)
#define MCF5272_GPIO_DDR0_OUTPUT	( 0x01)

#define MCF5272_GPIO_DAT15			( 0x80)
#define MCF5272_GPIO_DAT14			( 0x40)
#define MCF5272_GPIO_DAT13			( 0x20)
#define MCF5272_GPIO_DAT12			( 0x10)
#define MCF5272_GPIO_DAT11			( 0x08)
#define MCF5272_GPIO_DAT10			( 0x04)
#define MCF5272_GPIO_DAT9			( 0x02)
#define MCF5272_GPIO_DAT8			( 0x01)
#define MCF5272_GPIO_DAT7			( 0x80)
#define MCF5272_GPIO_DAT6			( 0x40)
#define MCF5272_GPIO_DAT5			( 0x20)
#define MCF5272_GPIO_DAT4			( 0x10)
#define MCF5272_GPIO_DAT3			( 0x08)
#define MCF5272_GPIO_DAT2			( 0x04)
#define MCF5272_GPIO_DAT1			( 0x02)
#define MCF5272_GPIO_DAT0			( 0x01)


/**********************************************************************
*
* QSPI Module Registers Description
*
***********************************************************************/

/* Offsets of the registers from the MBAR */
#define MCF5272_QSPI_QMR		(0x00A0)
#define MCF5272_QSPI_QDLYR		(0x00A4)
#define MCF5272_QSPI_QWR		(0x00A8)
#define MCF5272_QSPI_QIR		(0x00AC)
#define MCF5272_QSPI_QAR		(0x00B0)
#define MCF5272_QSPI_QDR		(0x00B4)

/* Read access macros for general use */
#define MCF5272_RD_QSPI_QMR(IMMP)	\
	Mcf5272_iord16(IMMP,MCF5272_QSPI_QMR)
#define MCF5272_RD_QSPI_QDLYR(IMMP)	\
	Mcf5272_iord16(IMMP,MCF5272_QSPI_QDLYR)
#define MCF5272_RD_QSPI_QWR(IMMP)	\
	Mcf5272_iord16(IMMP,MCF5272_QSPI_QWR)
#define MCF5272_RD_QSPI_QIR(IMMP)		\
	Mcf5272_iord16(IMMP,MCF5272_QSPI_QIR)
#define MCF5272_RD_QSPI_QAR(IMMP)	\
	Mcf5272_iord16(IMMP,MCF5272_QSPI_QAR)
#define MCF5272_RD_QSPI_QDR(IMMP)	\
	Mcf5272_iord16(IMMP,MCF5272_QSPI_QDR)

/* Write access macros for general use */
#define MCF5272_WR_QSPI_QMR(IMMP,DATA)	\
	Mcf5272_iowr16(IMMP,MCF5272_QSPI_QMR,DATA)
#define MCF5272_WR_QSPI_QDLYR(IMMP,DATA)	\
	Mcf5272_iowr16(IMMP,MCF5272_QSPI_QDLYR,DATA)
#define MCF5272_WR_QSPI_QWR(IMMP,DATA)	\
	Mcf5272_iowr16(IMMP,MCF5272_QSPI_QWR,DATA)
#define MCF5272_WR_QSPI_QIR(IMMP,DATA)		\
	Mcf5272_iowr16(IMMP,MCF5272_QSPI_QIR,DATA)
#define MCF5272_WR_QSPI_QAR(IMMP,DATA)	\
	Mcf5272_iowr16(IMMP,MCF5272_QSPI_QAR,DATA)
#define MCF5272_WR_QSPI_QDR(IMMP,DATA)	\
	Mcf5272_iowr16(IMMP,MCF5272_QSPI_QDR,DATA)


/**********************************************************************
*
* PWM Module Registers Description
*
***********************************************************************/

/* Offsets of the registers from the MBAR */
#define MCF5272_PWM_PWMCR1		(0x00C0)
#define MCF5272_PWM_PWMCR2		(0x00C4)
#define MCF5272_PWM_PWMCR3		(0x00C8)
#define MCF5272_PWM_PWMWD1		(0x00D0)
#define MCF5272_PWM_PWMWD2		(0x00D4)
#define MCF5272_PWM_PWMWD3		(0x00D8)

/* Read access macros for general use */
#define MCF5272_RD_PWM_PWMCR1(IMMP)	Mcf5272_iord8(IMMP,MCF5272_PWM_PWMCR1)
#define MCF5272_RD_PWM_PWMCR2(IMMP)	Mcf5272_iord8(IMMP,MCF5272_PWM_PWMCR2)
#define MCF5272_RD_PWM_PWMCR3(IMMP)	Mcf5272_iord8(IMMP,MCF5272_PWM_PWMCR3)
#define MCF5272_RD_PWM_PWMWD1(IMMP)	Mcf5272_iord8(IMMP,MCF5272_PWM_PWMWD1)
#define MCF5272_RD_PWM_PWMWD2(IMMP)	Mcf5272_iord8(IMMP,MCF5272_PWM_PWMWD2)
#define MCF5272_RD_PWM_PWMWD3(IMMP)	Mcf5272_iord8(IMMP,MCF5272_PWM_PWMWD3)

/* Write access macros for general use */
#define MCF5272_WR_PWM_PWMCR1(IMMP,DATA)	\
	Mcf5272_iowr8(IMMP,MCF5272_PWM_PWMCR1,DATA)
#define MCF5272_WR_PWM_PWMCR2(IMMP,DATA)	\
	Mcf5272_iowr8(IMMP,MCF5272_PWM_PWMCR2,DATA)
#define MCF5272_WR_PWM_PWMCR3(IMMP,DATA)	\
	Mcf5272_iowr8(IMMP,MCF5272_PWM_PWMCR3,DATA)
#define MCF5272_WR_PWM_PWMWD1(IMMP,DATA)	\
	Mcf5272_iowr8(IMMP,MCF5272_PWM_PWMWD1,DATA)
#define MCF5272_WR_PWM_PWMWD2(IMMP,DATA)	\
	Mcf5272_iowr8(IMMP,MCF5272_PWM_PWMWD2,DATA)
#define MCF5272_WR_PWM_PWMWD3(IMMP,DATA)	\
	Mcf5272_iowr8(IMMP,MCF5272_PWM_PWMWD3,DATA)

/**********************************************************************
*
* DMA Module Registers Description
*
***********************************************************************/

/* Offsets of the registers from the MBAR */
#define MCF5272_DMA_DCMR		(0x00E0)
#define MCF5272_DMA_DCIR		(0x00E6)
#define MCF5272_DMA_DBCR		(0x00E8)
#define MCF5272_DMA_DSAR		(0x00EC)
#define MCF5272_DMA_DDAR		(0x00F0)

/* Read access macros for general use */
#define MCF5272_RD_DMA_DCMR(IMMP)	Mcf5272_iord32(IMMP,MCF5272_DMA_DCMR)
#define MCF5272_RD_DMA_DCIR(IMMP)	Mcf5272_iord16(IMMP,MCF5272_DMA_DCIR)
#define MCF5272_RD_DMA_DBCR(IMMP)	Mcf5272_iord32(IMMP,MCF5272_DMA_DBCR)
#define MCF5272_RD_DMA_DSAR(IMMP)	Mcf5272_iord32(IMMP,MCF5272_DMA_DSAR)
#define MCF5272_RD_DMA_DDAR(IMMP)	Mcf5272_iord32(IMMP,MCF5272_DMA_DDAR)

/* Write access macros for general use */
#define MCF5272_WR_DMA_DCMR(IMMP,DATA)	\
	Mcf5272_iowr32(IMMP,MCF5272_DMA_DCMR,DATA)
#define MCF5272_WR_DMA_DCIR(IMMP,DATA)	\
	Mcf5272_iowr16(IMMP,MCF5272_DMA_DCIR,DATA)
#define MCF5272_WR_DMA_DBCR(IMMP,DATA)	\
	Mcf5272_iowr32(IMMP,MCF5272_DMA_DBCR,DATA)
#define MCF5272_WR_DMA_DSAR(IMMP,DATA)	\
	Mcf5272_iowr32(IMMP,MCF5272_DMA_DSAR,DATA)
#define MCF5272_WR_DMA_DDAR(IMMP,DATA)	\
	Mcf5272_iowr32(IMMP,MCF5272_DMA_DDAR,DATA)

/**********************************************************************
*
* USART Module Registers Description
*
***********************************************************************/

/* Offsets of the registers from the MBAR */
#define MCF5272_UART0_UMR		(0x0100)	/* RW */
#define MCF5272_UART0_USR		(0x0104)	/* USR RO, UCSR WO */
#define MCF5272_UART0_UCR		(0x0108)	/* WO */
#define MCF5272_UART0_UBUF		(0x010C)	/* URB RO, UTB WO */
#define MCF5272_UART0_UCCR		(0x0110)	/* UCCR RO, UACR WO */
#define MCF5272_UART0_UISR		(0x0114)	/* UISR RO, UIMR WO */
#define MCF5272_UART0_UBG1		(0x0118)	/* WO */
#define MCF5272_UART0_UBG2		(0x011C)	/* WO */
#define MCF5272_UART0_UABR1		(0x0120)	/* RO */
#define MCF5272_UART0_UABR2		(0x0124)	/* RO */
#define MCF5272_UART0_UTFCSR	(0x0128)	/* RW */
#define MCF5272_UART0_URFCSR	(0x012C)	/* RW */
#define MCF5272_UART0_UIP		(0x0134)	/* RO */
#define MCF5272_UART0_UOP1		(0x0138)	/* WO */
#define MCF5272_UART0_UOP0		(0x013C)	/* WO */

#define MCF5272_UART1_UMR		(0x0140)	/* RW */
#define MCF5272_UART1_USR		(0x0144)	/* USR RO, UCSR WO */
#define MCF5272_UART1_UCR		(0x0148)	/* WO */
#define MCF5272_UART1_UBUF		(0x014C)	/* URB RO, UTB WO */
#define MCF5272_UART1_UCCR		(0x0150)	/* UCCR RO, UACR WO */
#define MCF5272_UART1_UISR		(0x0154)	/* UISR RO, UIMR WO */
#define MCF5272_UART1_UBG1		(0x0158)	/* WO */
#define MCF5272_UART1_UBG2		(0x015C)	/* WO */
#define MCF5272_UART1_UABR1		(0x0160)	/* RO */
#define MCF5272_UART1_UABR2		(0x0164)	/* RO */
#define MCF5272_UART1_UTFCSR	(0x0168)	/* RW */
#define MCF5272_UART1_URFCSR	(0x016C)	/* RW */
#define MCF5272_UART1_UIP		(0x0174)	/* RO */
#define MCF5272_UART1_UOP1		(0x0178)	/* WO */
#define MCF5272_UART1_UOP0		(0x017C)	/* WO */

/* Read access macros for general use */
#define MCF5272_RD_UART0_UMR(IMMP)	\
	Mcf5272_iord8(IMMP,MCF5272_UART0_UMR)
#define MCF5272_RD_UART0_USR(IMMP)	\
	Mcf5272_iord8(IMMP,MCF5272_UART0_USR)
#define MCF5272_RD_UART0_URB(IMMP)	\
	Mcf5272_iord8(IMMP,MCF5272_UART0_UBUF)
#define MCF5272_RD_UART0_UCCR(IMMP)	\
	Mcf5272_iord8(IMMP,MCF5272_UART0_UCCR)
#define MCF5272_RD_UART0_UISR(IMMP)	\
	Mcf5272_iord8(IMMP,MCF5272_UART0_UISR)
#define MCF5272_RD_UART0_UABR1(IMMP)	\
	Mcf5272_iord8(IMMP,MCF5272_UART0_UABR1)
#define MCF5272_RD_UART0_UABR2(IMMP)	\
	Mcf5272_iord8(IMMP,MCF5272_UART1_UABR2)
#define MCF5272_RD_UART0_UTFCSR(IMMP)	\
	Mcf5272_iord8(IMMP,MCF5272_UART0_UTFCSR)
#define MCF5272_RD_UART0_URFCSR(IMMP)	\
	Mcf5272_iord8(IMMP,MCF5272_UART0_URFCSR)
#define MCF5272_RD_UART0_UIP(IMMP)	\
	Mcf5272_iord8(IMMP,MCF5272_UART0_UIP)

#define MCF5272_RD_UART1_UMR(IMMP)	\
	Mcf5272_iord8(IMMP,MCF5272_UART1_UMR)
#define MCF5272_RD_UART1_USR(IMMP)	\
	Mcf5272_iord8(IMMP,MCF5272_UART1_USR)
#define MCF5272_RD_UART1_URB(IMMP)	\
	Mcf5272_iord8(IMMP,MCF5272_UART1_UBUF)
#define MCF5272_RD_UART1_UCCR(IMMP)	\
	Mcf5272_iord8(IMMP,MCF5272_UART1_UCCR)
#define MCF5272_RD_UART1_UISR(IMMP)	\
	Mcf5272_iord8(IMMP,MCF5272_UART1_UISR)
#define MCF5272_RD_UART1_UABR1(IMMP)	\
	Mcf5272_iord8(IMMP,MCF5272_UART1_UABR1)
#define MCF5272_RD_UART1_UABR2(IMMP)	\
	Mcf5272_iord8(IMMP,MCF5272_UART1_UABR2)
#define MCF5272_RD_UART1_UTFCSR(IMMP)	\
	Mcf5272_iord8(IMMP,MCF5272_UART1_UTFCSR)
#define MCF5272_RD_UART1_URFCSR(IMMP)	\
	Mcf5272_iord8(IMMP,MCF5272_UART1_URFCSR)
#define MCF5272_RD_UART1_UIP(IMMP)	\
	Mcf5272_iord8(IMMP,MCF5272_UART1_UIP)

/* Write access macros for general use */
#define MCF5272_WR_UART0_UMR(IMMP,DATA)	\
	Mcf5272_iowr8(IMMP,MCF5272_UART0_UMR,DATA)
#define MCF5272_WR_UART0_UCSR(IMMP,DATA)	\
	Mcf5272_iowr8(IMMP,MCF5272_UART0_USR,DATA)
#define MCF5272_WR_UART0_UCR(IMMP,DATA)	\
	Mcf5272_iowr8(IMMP,MCF5272_UART0_UCR,DATA)
#define MCF5272_WR_UART0_UTB(IMMP,DATA)	\
	Mcf5272_iowr8(IMMP,MCF5272_UART0_UBUF,DATA)
#define MCF5272_WR_UART0_UACR(IMMP,DATA)	\
	Mcf5272_iowr8(IMMP,MCF5272_UART0_UCCR,DATA)
#define MCF5272_WR_UART0_UIMR(IMMP,DATA)	\
	Mcf5272_iowr8(IMMP,MCF5272_UART0_UISR,DATA)
#define MCF5272_WR_UART0_UBG1(IMMP,DATA)	\
	Mcf5272_iowr8(IMMP,MCF5272_UART0_UBG1,DATA)
#define MCF5272_WR_UART0_UBG2(IMMP,DATA)	\
	Mcf5272_iowr8(IMMP,MCF5272_UART0_UBG2,DATA)
#define MCF5272_WR_UART0_UTFCSR(IMMP,DATA)	\
	Mcf5272_iowr8(IMMP,MCF5272_UART0_UTFCSR,DATA)
#define MCF5272_WR_UART0_URFCSR(IMMP,DATA)	\
	Mcf5272_iowr8(IMMP,MCF5272_UART0_URFCSR,DATA)
#define MCF5272_WR_UART0_UOP1(IMMP,DATA)	\
	Mcf5272_iowr8(IMMP,MCF5272_UART0_UOP1,DATA)
#define MCF5272_WR_UART0_UOP0(IMMP,DATA)	\
	Mcf5272_iowr8(IMMP,MCF5272_UART0_UOP0,DATA)

#define MCF5272_WR_UART1_UMR(IMMP,DATA)	\
	Mcf5272_iowr8(IMMP,MCF5272_UART1_UMR,DATA)
#define MCF5272_WR_UART1_UCSR(IMMP,DATA)	\
	Mcf5272_iowr8(IMMP,MCF5272_UART1_USR,DATA)
#define MCF5272_WR_UART1_UCR(IMMP,DATA)	\
	Mcf5272_iowr8(IMMP,MCF5272_UART1_UCR,DATA)
#define MCF5272_WR_UART1_UTB(IMMP,DATA)	\
	Mcf5272_iowr8(IMMP,MCF5272_UART1_UBUF,DATA)
#define MCF5272_WR_UART1_UACR(IMMP,DATA)	\
	Mcf5272_iowr8(IMMP,MCF5272_UART1_UCCR,DATA)
#define MCF5272_WR_UART1_UIMR(IMMP,DATA)	\
	Mcf5272_iowr8(IMMP,MCF5272_UART1_UISR,DATA)
#define MCF5272_WR_UART1_UBG1(IMMP,DATA)	\
	Mcf5272_iowr8(IMMP,MCF5272_UART1_UBG1,DATA)
#define MCF5272_WR_UART1_UBG2(IMMP,DATA)	\
	Mcf5272_iowr8(IMMP,MCF5272_UART1_UBG2,DATA)
#define MCF5272_WR_UART1_UTFCSR(IMMP,DATA)	\
	Mcf5272_iowr8(IMMP,MCF5272_UART1_UTFCSR,DATA)
#define MCF5272_WR_UART1_URFCSR(IMMP,DATA)	\
	Mcf5272_iowr8(IMMP,MCF5272_UART1_URFCSR,DATA)
#define MCF5272_WR_UART1_UOP1(IMMP,DATA)	\
	Mcf5272_iowr8(IMMP,MCF5272_UART1_UOP1,DATA)
#define MCF5272_WR_UART1_UOP0(IMMP,DATA)	\
	Mcf5272_iowr8(IMMP,MCF5272_UART1_UOP0,DATA)

/* Bit level definitions and macros */
#define MCF5272_UART_UMR1_RXRTS			(0x80)
#define MCF5272_UART_UMR1_RXIRQ			(0x40)
#define MCF5272_UART_UMR1_ERR			(0x20)
#define MCF5272_UART_UMR1_PM_MULTI_ADDR	(0x1C)
#define MCF5272_UART_UMR1_PM_MULTI_DATA	(0x18)
#define MCF5272_UART_UMR1_PM_NONE		(0x10)
#define MCF5272_UART_UMR1_PM_FORCE_HI	(0x0C)
#define MCF5272_UART_UMR1_PM_FORCE_LO	(0x08)
#define MCF5272_UART_UMR1_PM_ODD		(0x04)
#define MCF5272_UART_UMR1_PM_EVEN		(0x00)
#define MCF5272_UART_UMR1_BC_5			(0x00)
#define MCF5272_UART_UMR1_BC_6			(0x01)
#define MCF5272_UART_UMR1_BC_7			(0x02)
#define MCF5272_UART_UMR1_BC_8			(0x03)

#define MCF5272_UART_UMR2_CM_NORMAL	  	(0x00)
#define MCF5272_UART_UMR2_CM_ECHO	  	(0x40)
#define MCF5272_UART_UMR2_CM_LOCAL_LOOP	(0x80)
#define MCF5272_UART_UMR2_CM_REMOTE_LOOP	(0xC0)
#define MCF5272_UART_UMR2_TXRTS		 	(0x20)
#define MCF5272_UART_UMR2_TXCTS		 	(0x10)
#define MCF5272_UART_UMR2_STOP_BITS_1 	(0x07)
#define MCF5272_UART_UMR2_STOP_BITS_15	(0x08)
#define MCF5272_UART_UMR2_STOP_BITS_2 	(0x0F)
#define MCF5272_UART_UMR2_STOP_BITS(a)   ((a)&0x0f)	/* Stop Bit Length */

#define MCF5272_UART_USR_RB				(0x80)
#define MCF5272_UART_USR_FE				(0x40)
#define MCF5272_UART_USR_PE				(0x20)
#define MCF5272_UART_USR_OE				(0x10)
#define MCF5272_UART_USR_TXEMP			(0x08)
#define MCF5272_UART_USR_TXRDY			(0x04)
#define MCF5272_UART_USR_FFULL			(0x02)
#define MCF5272_UART_USR_RXRDY			(0x01)

#define MCF5272_UART_UCSR_RCS(a)	(((a)&0x0f)<<4)	/* Rx Clk Select */
#define MCF5272_UART_UCSR_TCS(a)		((a)&0x0f)	/* Tx Clk Select */


#define MCF5272_UART_UCR_NONE			(0x00)
#define MCF5272_UART_UCR_STOP_BREAK		(0x70)
#define MCF5272_UART_UCR_START_BREAK	(0x60)
#define MCF5272_UART_UCR_RESET_BKCHGINT	(0x50)
#define MCF5272_UART_UCR_RESET_ERROR	(0x40)
#define MCF5272_UART_UCR_RESET_TX		(0x30)
#define MCF5272_UART_UCR_RESET_RX		(0x20)
#define MCF5272_UART_UCR_RESET_MR		(0x10)
#define MCF5272_UART_UCR_TX_DISABLED	(0x08)
#define MCF5272_UART_UCR_TX_ENABLED		(0x04)
#define MCF5272_UART_UCR_RX_DISABLED	(0x02)
#define MCF5272_UART_UCR_RX_ENABLED		(0x01)

#define MCF5272_UART_UCCR_COS			(0x10)
#define MCF5272_UART_UCCR_CTS			(0x01)

#define MCF5272_UART_UACR_BRG			(0x80)
#define MCF5272_UART_UACR_CTMS_TIMER	(0x60)
#define MCF5272_UART_UACR_IEC			(0x01)

#define MCF5272_UART_UISR_COS			(0x80)
#define MCF5272_UART_UISR_DB			(0x04)
#define MCF5272_UART_UISR_RXRDY			(0x02)
#define MCF5272_UART_UISR_TXRDY			(0x01)

#define MCF5272_UART_UIMR_COS			(0x80)
#define MCF5272_UART_UIMR_DB			(0x04)
#define MCF5272_UART_UIMR_FFULL			(0x02)
#define MCF5272_UART_UIMR_TXRDY			(0x01)


/**********************************************************************
*
* SDRAM Controller Module Registers Description
*
***********************************************************************/

/* Offsets of the registers from the MBAR */
#define MCF5272_SDRAMC_SDCCR		(0x0180)
#define MCF5272_SDRAMC_SDCTR		(0x0184)

/* Read access macros for general use */
#define MCF5272_RD_SDRAMC_SDCCR(IMMP)	\
	Mcf5272_iord32(IMMP,MCF5272_SDRAMC_SDCCR)
#define MCF5272_RD_SDRAMC_SDCTR(IMMP)	\
	Mcf5272_iord32(IMMP,MCF5272_SDRAMC_SDCTR)

/* Write access macros for general use */
#define MCF5272_WR_SDRAMC_SDCCR(IMMP,DATA)	\
	Mcf5272_iowr32(IMMP,MCF5272_SDRAMC_SDCCR,DATA)
#define MCF5272_WR_SDRAMC_SDCTR(IMMP,DATA)	\
	Mcf5272_iowr32(IMMP,MCF5272_SDRAMC_SDCTR,DATA)

/* Bit level definitions and macros */
#define MCF5272_SDRAMC_SDCCR_MCAS_A7	(0x0 << 13)
#define MCF5272_SDRAMC_SDCCR_MCAS_A8	(0x1 << 13)
#define MCF5272_SDRAMC_SDCCR_MCAS_A9	(0x2 << 13)
#define MCF5272_SDRAMC_SDCCR_MCAS_A10	(0x3 << 13)
#define MCF5272_SDRAMC_SDCCR_BALOC_A19	(0x0 << 8)
#define MCF5272_SDRAMC_SDCCR_BALOC_A20	(0x1 << 8)
#define MCF5272_SDRAMC_SDCCR_BALOC_A21	(0x2 << 8)
#define MCF5272_SDRAMC_SDCCR_BALOC_A22	(0x3 << 8)
#define MCF5272_SDRAMC_SDCCR_BALOC_A23	(0x4 << 8)
#define MCF5272_SDRAMC_SDCCR_BALOC_A24	(0x5 << 8)
#define MCF5272_SDRAMC_SDCCR_BALOC_A25	(0x6 << 8)
#define MCF5272_SDRAMC_SDCCR_BALOC_A26	(0x7 << 8)
#define MCF5272_SDRAMC_SDCCR_GSL		(0x00000080)
#define MCF5272_SDRAMC_SDCCR_REG		(0x00000010)
#define MCF5272_SDRAMC_SDCCR_INV		(0x00000008)
#define MCF5272_SDRAMC_SDCCR_SLEEP		(0x00000004)
#define MCF5272_SDRAMC_SDCCR_ACT		(0x00000002)
#define MCF5272_SDRAMC_SDCCR_INIT		(0x00000001)

#define MCF5272_SDRAMC_SDCTR_RTP_66MHz	(0x3D << 10)
#define MCF5272_SDRAMC_SDCTR_RTP_48MHz	(0x2B << 10)
#define MCF5272_SDRAMC_SDCTR_RTP_33MHz	(0x1D << 10)
#define MCF5272_SDRAMC_SDCTR_RTP_25MHz	(0x16 << 10)
#define MCF5272_SDRAMC_SDCTR_RC(x)		((x & 0x3) << 8)
#define MCF5272_SDRAMC_SDCTR_RP(x)		((x & 0x3) << 4)
#define MCF5272_SDRAMC_SDCTR_RCD(x)		((x & 0x3) << 2)
#define MCF5272_SDRAMC_SDCTR_CLT_2		(0x00000001)
#define MCF5272_SDRAMC_SDCTR_CLT_3		(0x00000002)
#define MCF5272_SDRAMC_SDCTR_CLT_4		(0x00000003)

/**********************************************************************
*
* Timer Module Registers Description
*
***********************************************************************/

/* Offsets of the registers from the MBAR */
#define MCF5272_TIMER0_TMR		(0x0200)
#define MCF5272_TIMER0_TRR		(0x0204)
#define MCF5272_TIMER0_TCR		(0x0208)
#define MCF5272_TIMER0_TCN		(0x020C)
#define MCF5272_TIMER0_TER		(0x0210)
#define MCF5272_TIMER1_TMR		(0x0220)
#define MCF5272_TIMER1_TRR		(0x0224)
#define MCF5272_TIMER1_TCR		(0x0228)
#define MCF5272_TIMER1_TCN		(0x022C)
#define MCF5272_TIMER1_TER		(0x0230)
#define MCF5272_TIMER2_TMR		(0x0240)
#define MCF5272_TIMER2_TRR		(0x0244)
#define MCF5272_TIMER2_TCR		(0x0248)
#define MCF5272_TIMER2_TCN		(0x024C)
#define MCF5272_TIMER2_TER		(0x0250)
#define MCF5272_TIMER3_TMR		(0x0260)
#define MCF5272_TIMER3_TRR		(0x0264)
#define MCF5272_TIMER3_TCR		(0x0268)
#define MCF5272_TIMER3_TCN		(0x026C)
#define MCF5272_TIMER3_TER		(0x0270)
#define MCF5272_TIMER_WRRR		(0x0280)
#define MCF5272_TIMER_WIRR		(0x0284)
#define MCF5272_TIMER_WCR		(0x0288)
#define MCF5272_TIMER_WER		(0x028C)

/* Read access macros for general use */
#define MCF5272_RD_TIMER0_TMR(IMMP)	Mcf5272_iord16(IMMP,MCF5272_TIMER0_TMR)
#define MCF5272_RD_TIMER0_TRR(IMMP)	Mcf5272_iord16(IMMP,MCF5272_TIMER0_TRR)
#define MCF5272_RD_TIMER0_TCR(IMMP)	Mcf5272_iord16(IMMP,MCF5272_TIMER0_TCR)
#define MCF5272_RD_TIMER0_TCN(IMMP)	Mcf5272_iord16(IMMP,MCF5272_TIMER0_TCN)
#define MCF5272_RD_TIMER0_TER(IMMP)	Mcf5272_iord16(IMMP,MCF5272_TIMER0_TER)

#define MCF5272_RD_TIMER1_TMR(IMMP)	Mcf5272_iord16(IMMP,MCF5272_TIMER1_TMR)
#define MCF5272_RD_TIMER1_TRR(IMMP)	Mcf5272_iord16(IMMP,MCF5272_TIMER1_TRR)
#define MCF5272_RD_TIMER1_TCR(IMMP)	Mcf5272_iord16(IMMP,MCF5272_TIMER1_TCR)
#define MCF5272_RD_TIMER1_TCN(IMMP)	Mcf5272_iord16(IMMP,MCF5272_TIMER1_TCN)
#define MCF5272_RD_TIMER1_TER(IMMP)	Mcf5272_iord16(IMMP,MCF5272_TIMER1_TER)

#define MCF5272_RD_TIMER2_TMR(IMMP)	Mcf5272_iord16(IMMP,MCF5272_TIMER2_TMR)
#define MCF5272_RD_TIMER2_TRR(IMMP)	Mcf5272_iord16(IMMP,MCF5272_TIMER2_TRR)
#define MCF5272_RD_TIMER2_TCR(IMMP)	Mcf5272_iord16(IMMP,MCF5272_TIMER2_TCR)
#define MCF5272_RD_TIMER2_TCN(IMMP)	Mcf5272_iord16(IMMP,MCF5272_TIMER2_TCN)
#define MCF5272_RD_TIMER2_TER(IMMP)	Mcf5272_iord16(IMMP,MCF5272_TIMER2_TER)

#define MCF5272_RD_TIMER3_TMR(IMMP)	Mcf5272_iord16(IMMP,MCF5272_TIMER3_TMR)
#define MCF5272_RD_TIMER3_TRR(IMMP)	Mcf5272_iord16(IMMP,MCF5272_TIMER3_TRR)
#define MCF5272_RD_TIMER3_TCR(IMMP)	Mcf5272_iord16(IMMP,MCF5272_TIMER3_TCR)
#define MCF5272_RD_TIMER3_TCN(IMMP)	Mcf5272_iord16(IMMP,MCF5272_TIMER3_TCN)
#define MCF5272_RD_TIMER3_TER(IMMP)	Mcf5272_iord16(IMMP,MCF5272_TIMER3_TER)

#define MCF5272_RD_TIMER_TMR(IMMP,NUM)	\
Mcf5272_iord16(IMMP,MCF5272_TIMER0_TMR + (NUM * 0x20))
#define MCF5272_RD_TIMER_TRR(IMMP,NUM)	\
Mcf5272_iord16(IMMP,MCF5272_TIMER0_TRR + (NUM * 0x20))
#define MCF5272_RD_TIMER_TCR(IMMP,NUM)	\
Mcf5272_iord16(IMMP,MCF5272_TIMER0_TCR + (NUM * 0x20))
#define MCF5272_RD_TIMER_TCN(IMMP,NUM)	\
Mcf5272_iord16(IMMP,MCF5272_TIMER0_TCN + (NUM * 0x20))
#define MCF5272_RD_TIMER_TER(IMMP,NUM)	\
Mcf5272_iord8(IMMP,MCF5272_TIMER0_TER + (NUM * 0x20))

#define MCF5272_RD_TIMER_WRRR(IMMP)	Mcf5272_iord16(IMMP,MCF5272_TIMER_WRRR)
#define MCF5272_RD_TIMER_WIRR(IMMP)	Mcf5272_iord16(IMMP,MCF5272_TIMER_WIRR)
#define MCF5272_RD_TIMER_WCR(IMMP)	Mcf5272_iord16(IMMP,MCF5272_TIMER_WCR)
#define MCF5272_RD_TIMER_WER(IMMP)	Mcf5272_iord16(IMMP,MCF5272_TIMER_WER)

/* Write access macros for general use */
#define MCF5272_WR_TIMER0_TMR(IMMP,DATA)	\
	Mcf5272_iowr16(IMMP,MCF5272_TIMER0_TMR,DATA)
#define MCF5272_WR_TIMER0_TRR(IMMP,DATA)	\
	Mcf5272_iowr16(IMMP,MCF5272_TIMER0_TRR,DATA)
#define MCF5272_WR_TIMER0_TCN(IMMP,DATA)	\
	Mcf5272_iowr16(IMMP,MCF5272_TIMER0_TCN,DATA)
#define MCF5272_WR_TIMER0_TER(IMMP,DATA)	\
	Mcf5272_iowr16(IMMP,MCF5272_TIMER0_TER,DATA)
#define MCF5272_WR_TIMER1_TMR(IMMP,DATA)	\
	Mcf5272_iowr16(IMMP,MCF5272_TIMER1_TMR,DATA)
#define MCF5272_WR_TIMER1_TRR(IMMP,DATA)	\
	Mcf5272_iowr16(IMMP,MCF5272_TIMER1_TRR,DATA)
#define MCF5272_WR_TIMER1_TCN(IMMP,DATA)	\
	Mcf5272_iowr16(IMMP,MCF5272_TIMER1_TCN,DATA)
#define MCF5272_WR_TIMER1_TER(IMMP,DATA)	\
	Mcf5272_iowr16(IMMP,MCF5272_TIMER1_TER,DATA)
#define MCF5272_WR_TIMER2_TMR(IMMP,DATA)	\
	Mcf5272_iowr16(IMMP,MCF5272_TIMER2_TMR,DATA)
#define MCF5272_WR_TIMER2_TRR(IMMP,DATA)	\
	Mcf5272_iowr16(IMMP,MCF5272_TIMER2_TRR,DATA)
#define MCF5272_WR_TIMER2_TCN(IMMP,DATA)	\
	Mcf5272_iowr16(IMMP,MCF5272_TIMER2_TCN,DATA)
#define MCF5272_WR_TIMER2_TER(IMMP,DATA)	\
	Mcf5272_iowr16(IMMP,MCF5272_TIMER2_TER,DATA)
#define MCF5272_WR_TIMER3_TMR(IMMP,DATA)	\
	Mcf5272_iowr16(IMMP,MCF5272_TIMER3_TMR,DATA)
#define MCF5272_WR_TIMER3_TRR(IMMP,DATA)	\
	Mcf5272_iowr16(IMMP,MCF5272_TIMER3_TRR,DATA)
#define MCF5272_WR_TIMER3_TCN(IMMP,DATA)	\
	Mcf5272_iowr16(IMMP,MCF5272_TIMER3_TCN,DATA)
#define MCF5272_WR_TIMER3_TER(IMMP,DATA)	\
	Mcf5272_iowr16(IMMP,MCF5272_TIMER3_TER,DATA)
#define MCF5272_WR_TIMER_WRRR(IMMP,DATA)	\
	Mcf5272_iowr16(IMMP,MCF5272_TIMER_WRRR,DATA)
#define MCF5272_WR_TIMER_WIRR(IMMP,DATA)	\
	Mcf5272_iowr16(IMMP,MCF5272_TIMER_WIRR,DATA)
#define MCF5272_WR_TIMER_WCR(IMMP,DATA)	\
	Mcf5272_iowr16(IMMP,MCF5272_TIMER_WCR,DATA)
#define MCF5272_WR_TIMER_WER(IMMP,DATA)	\
	Mcf5272_iowr16(IMMP,MCF5272_TIMER_WER,DATA)

#define MCF5272_WR_TIMER_TMR(IMMP,NUM,DATA)	\
	Mcf5272_iowr16(IMMP,MCF5272_TIMER0_TMR + (NUM * 0x20),DATA)
#define MCF5272_WR_TIMER_TRR(IMMP,NUM,DATA)	\
	Mcf5272_iowr16(IMMP,MCF5272_TIMER0_TRR + (NUM * 0x20),DATA)
#define MCF5272_WR_TIMER_TCN(IMMP,NUM,DATA)	\
	Mcf5272_iowr16(IMMP,MCF5272_TIMER0_TCN + (NUM * 0x20),DATA)
#define MCF5272_WR_TIMER_TER(IMMP,NUM,DATA)	\
	Mcf5272_iowr8(IMMP,MCF5272_TIMER0_TER + (NUM * 0x20),DATA)

/* Bit level definitions and macros */
#define MCF5272_TIMER_TMR_PS(a)		(((a)&0x00FF)<<8)
#define MCF5272_TIMER_TMR_CE_ANY	(0x00C0)
#define MCF5272_TIMER_TMR_CE_RISE	(0x0080)
#define MCF5272_TIMER_TMR_CE_FALL	(0x0040)
#define MCF5272_TIMER_TMR_CE_NONE	(0x0000)
#define MCF5272_TIMER_TMR_OM		(0x0020)
#define MCF5272_TIMER_TMR_ORI		(0x0010)
#define MCF5272_TIMER_TMR_FRR		(0x0008)
#define MCF5272_TIMER_TMR_CLK_TIN	(0x0006)
#define MCF5272_TIMER_TMR_CLK_DIV16	(0x0004)
#define MCF5272_TIMER_TMR_CLK_MSCLK	(0x0002)
#define MCF5272_TIMER_TMR_CLK_STOP	(0x0000)
#define MCF5272_TIMER_TMR_RST		(0x0001)

#define MCF5272_TIMER_TER_REF		(0x02)
#define MCF5272_TIMER_TER_CAP		(0x01)

/**********************************************************************
*
* PLI Module Registers Description
*
***********************************************************************/

/* Offsets of the registers from the MBAR */
#define MCF5272_PLI_P0B1RR			(0x0300)
#define MCF5272_PLI_P1B1RR			(0x0304)
#define MCF5272_PLI_P2B1RR			(0x0308)
#define MCF5272_PLI_P3B1RR			(0x030C)
#define MCF5272_PLI_P0B2RR			(0x0310)
#define MCF5272_PLI_P1B2RR			(0x0314)
#define MCF5272_PLI_P2B2RR			(0x0318)
#define MCF5272_PLI_P3B2RR			(0x031C)
#define MCF5272_PLI_P0DRR			(0x0320)
#define MCF5272_PLI_P1DRR			(0x0321)
#define MCF5272_PLI_P2DRR			(0x0322)
#define MCF5272_PLI_P3DRR			(0x0323)
#define MCF5272_PLI_P0B1TR			(0x0328)
#define MCF5272_PLI_P1B1TR			(0x032C)
#define MCF5272_PLI_P2B1TR			(0x0330)
#define MCF5272_PLI_P3B1TR			(0x0334)
#define MCF5272_PLI_P0B2TR			(0x0338)
#define MCF5272_PLI_P1B2TR			(0x033C)
#define MCF5272_PLI_P2B2TR			(0x0340)
#define MCF5272_PLI_P3B2TR			(0x0344)
#define MCF5272_PLI_P0DTR			(0x0348)
#define MCF5272_PLI_P1DTR			(0x0349)
#define MCF5272_PLI_P2DTR			(0x034A)
#define MCF5272_PLI_P3DTR			(0x034B)
#define MCF5272_PLI_P0CR			(0x0350)
#define MCF5272_PLI_P1CR			(0x0352)
#define MCF5272_PLI_P2CR			(0x0354)
#define MCF5272_PLI_P3CR			(0x0356)
#define MCF5272_PLI_P0ICR			(0x0358)
#define MCF5272_PLI_P1ICR			(0x035A)
#define MCF5272_PLI_P2ICR			(0x035C)
#define MCF5272_PLI_P3ICR			(0x035E)
#define MCF5272_PLI_P0GMR			(0x0360)
#define MCF5272_PLI_P1GMR			(0x0362)
#define MCF5272_PLI_P2GMR			(0x0364)
#define MCF5272_PLI_P3GMR			(0x0366)
#define MCF5272_PLI_P0GMT			(0x0368)
#define MCF5272_PLI_P1GMT			(0x036A)
#define MCF5272_PLI_P2GMT			(0x036C)
#define MCF5272_PLI_P3GMT			(0x036E)
#define MCF5272_PLI_PGMTS			(0x0371)
#define MCF5272_PLI_PGMTA			(0x0372)
#define MCF5272_PLI_P0GCIR			(0x0374)
#define MCF5272_PLI_P1GCIR			(0x0375)
#define MCF5272_PLI_P2GCIR			(0x0376)
#define MCF5272_PLI_P3GCIR			(0x0377)
#define MCF5272_PLI_P0GCIT			(0x0378)
#define MCF5272_PLI_P1GCIT			(0x0379)
#define MCF5272_PLI_P2GCIT			(0x037A)
#define MCF5272_PLI_P3GCIT			(0x037E)
#define MCF5272_PLI_PGCITSR			(0x037F)
#define MCF5272_PLI_PDSUSR			(0x0382)
#define MCF5272_PLI_PDCSR			(0x0383)
#define MCF5272_PLI_P0PSR			(0x0384)
#define MCF5272_PLI_P1PSR			(0x0386)
#define MCF5272_PLI_P2PSR			(0x0388)
#define MCF5272_PLI_P3PSR			(0x038A)
#define MCF5272_PLI_PASR			(0x038C)
#define MCF5272_PLI_PLCR			(0x038F)
#define MCF5272_PLI_PDRQR			(0x0392)
#define MCF5272_PLI_P0SDR			(0x0394)
#define MCF5272_PLI_P1SDR			(0x0396)
#define MCF5272_PLI_P2SDR			(0x0398)
#define MCF5272_PLI_P3SDR			(0x039A)
#define MCF5272_PLI_PCKSELR			(0x039E)

/* Read access macros for general use */
#define MCF5272_RD_PLI_P0B1RR(IMMP)	Mcf5272_iord32(IMMP,MCF5272_PLI_P0B1RR)
#define MCF5272_RD_PLI_P1B1RR(IMMP)	Mcf5272_iord32(IMMP,MCF5272_PLI_P1B1RR)
#define MCF5272_RD_PLI_P2B1RR(IMMP)	Mcf5272_iord32(IMMP,MCF5272_PLI_P2B1RR)
#define MCF5272_RD_PLI_P3B1RR(IMMP)	Mcf5272_iord32(IMMP,MCF5272_PLI_P3B1RR)
#define MCF5272_RD_PLI_P0B2RR(IMMP)	Mcf5272_iord32(IMMP,MCF5272_PLI_P0B2RR)
#define MCF5272_RD_PLI_P1B2RR(IMMP)	Mcf5272_iord32(IMMP,MCF5272_PLI_P1B2RR)
#define MCF5272_RD_PLI_P2B2RR(IMMP)	Mcf5272_iord32(IMMP,MCF5272_PLI_P2B2RR)
#define MCF5272_RD_PLI_P3B2RR(IMMP)	Mcf5272_iord32(IMMP,MCF5272_PLI_P3B2RR)
#define MCF5272_RD_PLI_P0DRR(IMMP)	Mcf5272_iord8(IMMP,MCF5272_PLI_P0DRR)
#define MCF5272_RD_PLI_P1DRR(IMMP)	Mcf5272_iord8(IMMP,MCF5272_PLI_P1DRR)
#define MCF5272_RD_PLI_P2DRR(IMMP)	Mcf5272_iord8(IMMP,MCF5272_PLI_P2DRR)
#define MCF5272_RD_PLI_P3DRR(IMMP)	Mcf5272_iord8(IMMP,MCF5272_PLI_P3DRR)
#define MCF5272_RD_PLI_P0B1TR(IMMP)	Mcf5272_iord32(IMMP,MCF5272_PLI_P0B1TR)
#define MCF5272_RD_PLI_P1B1TR(IMMP)	Mcf5272_iord32(IMMP,MCF5272_PLI_P1B1TR)
#define MCF5272_RD_PLI_P2B1TR(IMMP)	Mcf5272_iord32(IMMP,MCF5272_PLI_P2B1TR)
#define MCF5272_RD_PLI_P3B1TR(IMMP)	Mcf5272_iord32(IMMP,MCF5272_PLI_P3B1TR)
#define MCF5272_RD_PLI_P0B2TR(IMMP)	Mcf5272_iord32(IMMP,MCF5272_PLI_P0B2TR)
#define MCF5272_RD_PLI_P1B2TR(IMMP)	Mcf5272_iord32(IMMP,MCF5272_PLI_P1B2TR)
#define MCF5272_RD_PLI_P2B2TR(IMMP)	Mcf5272_iord32(IMMP,MCF5272_PLI_P2B2TR)
#define MCF5272_RD_PLI_P3B2TR(IMMP)	Mcf5272_iord32(IMMP,MCF5272_PLI_P3B2TR)
#define MCF5272_RD_PLI_P0DTR(IMMP)	Mcf5272_iord8(IMMP,MCF5272_PLI_P0DTR)
#define MCF5272_RD_PLI_P1DTR(IMMP)	Mcf5272_iord8(IMMP,MCF5272_PLI_P1DTR)
#define MCF5272_RD_PLI_P2DTR(IMMP)	Mcf5272_iord8(IMMP,MCF5272_PLI_P2DTR)
#define MCF5272_RD_PLI_P3DTR(IMMP)	Mcf5272_iord8(IMMP,MCF5272_PLI_P3DTR)
#define MCF5272_RD_PLI_P0CR(IMMP)	Mcf5272_iord16(IMMP,MCF5272_PLI_P0CR)
#define MCF5272_RD_PLI_P1CR(IMMP)	Mcf5272_iord16(IMMP,MCF5272_PLI_P1CR)
#define MCF5272_RD_PLI_P2CR(IMMP)	Mcf5272_iord16(IMMP,MCF5272_PLI_P2CR)
#define MCF5272_RD_PLI_P3CR(IMMP)	Mcf5272_iord16(IMMP,MCF5272_PLI_P3CR)
#define MCF5272_RD_PLI_P0ICR(IMMP)	Mcf5272_iord16(IMMP,MCF5272_PLI_P0ICR)
#define MCF5272_RD_PLI_P1ICR(IMMP)	Mcf5272_iord16(IMMP,MCF5272_PLI_P1ICR)
#define MCF5272_RD_PLI_P2ICR(IMMP)	Mcf5272_iord16(IMMP,MCF5272_PLI_P2ICR)
#define MCF5272_RD_PLI_P3ICR(IMMP)	Mcf5272_iord16(IMMP,MCF5272_PLI_P3ICR)
#define MCF5272_RD_PLI_P0GMR(IMMP)	Mcf5272_iord16(IMMP,MCF5272_PLI_P0GMR)
#define MCF5272_RD_PLI_P1GMR(IMMP)	Mcf5272_iord16(IMMP,MCF5272_PLI_P1GMR)
#define MCF5272_RD_PLI_P2GMR(IMMP)	Mcf5272_iord16(IMMP,MCF5272_PLI_P2GMR)
#define MCF5272_RD_PLI_P3GMR(IMMP)	Mcf5272_iord16(IMMP,MCF5272_PLI_P3GMR)
#define MCF5272_RD_PLI_P0GMT(IMMP)	Mcf5272_iord16(IMMP,MCF5272_PLI_P0GMT)
#define MCF5272_RD_PLI_P1GMT(IMMP)	Mcf5272_iord16(IMMP,MCF5272_PLI_P1GMT)
#define MCF5272_RD_PLI_P2GMT(IMMP)	Mcf5272_iord16(IMMP,MCF5272_PLI_P2GMT)
#define MCF5272_RD_PLI_P3GMT(IMMP)	Mcf5272_iord16(IMMP,MCF5272_PLI_P3GMT)
#define MCF5272_RD_PLI_PGMTS(IMMP)	Mcf5272_iord8(IMMP,MCF5272_PLI_PGMTS)
#define MCF5272_RD_PLI_PGMTA(IMMP)	Mcf5272_iord8(IMMP,MCF5272_PLI_PGMTA)
#define MCF5272_RD_PLI_P0GCIR(IMMP) Mcf5272_iord8(IMMP,MCF5272_PLI_P0GCIR)
#define MCF5272_RD_PLI_P1GCIR(IMMP) Mcf5272_iord8(IMMP,MCF5272_PLI_P1GCIR)
#define MCF5272_RD_PLI_P2GCIR(IMMP) Mcf5272_iord8(IMMP,MCF5272_PLI_P2GCIR)
#define MCF5272_RD_PLI_P3GCIR(IMMP) Mcf5272_iord8(IMMP,MCF5272_PLI_P3GCIR)
#define MCF5272_RD_PLI_P0GCIT(IMMP) Mcf5272_iord8(IMMP,MCF5272_PLI_P0GCIT)
#define MCF5272_RD_PLI_P1GCIT(IMMP) Mcf5272_iord8(IMMP,MCF5272_PLI_P1GCIT)
#define MCF5272_RD_PLI_P2GCIT(IMMP) Mcf5272_iord8(IMMP,MCF5272_PLI_P2GCIT)
#define MCF5272_RD_PLI_P3GCIT(IMMP) Mcf5272_iord8(IMMP,MCF5272_PLI_P3GCIT)
#define MCF5272_RD_PLI_PGCITSR(IMMP) Mcf5272_iord8(IMMP,MCF5272_PLI_PGCITSR)
#define MCF5272_RD_PLI_PDSUSR(IMMP)	Mcf5272_iord8(IMMP,MCF5272_PLI_PDSUSR)
#define MCF5272_RD_PLI_PDCSR(IMMP)	Mcf5272_iord8(IMMP,MCF5272_PLI_PDCSR)
#define MCF5272_RD_PLI_P0PSR(IMMP)	Mcf5272_iord16(IMMP,MCF5272_PLI_P0PSR)
#define MCF5272_RD_PLI_P1PSR(IMMP)	Mcf5272_iord16(IMMP,MCF5272_PLI_P1PSR)
#define MCF5272_RD_PLI_P2PSR(IMMP)	Mcf5272_iord16(IMMP,MCF5272_PLI_P2PSR)
#define MCF5272_RD_PLI_P3PSR(IMMP)	Mcf5272_iord16(IMMP,MCF5272_PLI_P3PSR)
#define MCF5272_RD_PLI_PASR(IMMP)	Mcf5272_iord16(IMMP,MCF5272_PLI_PASR,
#define MCF5272_RD_PLI_PLCR(IMMP)	Mcf5272_iord8(IMMP,MCF5272_PLI_PLCR)
#define MCF5272_RD_PLI_PDRQR(IMMP)	Mcf5272_iord16(IMMP,MCF5272_PLI_PDRQR)
#define MCF5272_RD_PLI_P0SDR(IMMP)	Mcf5272_iord16(IMMP,MCF5272_PLI_P0SDR)
#define MCF5272_RD_PLI_P1SDR(IMMP)	Mcf5272_iord16(IMMP,MCF5272_PLI_P1SDR)
#define MCF5272_RD_PLI_P2SDR(IMMP)	Mcf5272_iord16(IMMP,MCF5272_PLI_P2SDR)
#define MCF5272_RD_PLI_P3SDR(IMMP)	Mcf5272_iord16(IMMP,MCF5272_PLI_P3SDR)
#define MCF5272_RD_PLI_PCKSELR(IMMP) Mcf5272_iord16(IMMP,MCF5272_PLI_PCKSELR)

/* Write access macros for general use */
#define MCF5272_WR_PLI_P0B1TR(IMMP,DATA)	\
	Mcf5272_iowr32(IMMP,MCF5272_PLI_P0B1TR,DATA)
#define MCF5272_WR_PLI_P1B1TR(IMMP,DATA)	\
	Mcf5272_iowr32(IMMP,MCF5272_PLI_P1B1TR,DATA)
#define MCF5272_WR_PLI_P2B1TR(IMMP,DATA)	\
	Mcf5272_iowr32(IMMP,MCF5272_PLI_P2B1TR,DATA)
#define MCF5272_WR_PLI_P3B1TR(IMMP,DATA)	\
	Mcf5272_iowr32(IMMP,MCF5272_PLI_P3B1TR,DATA)
#define MCF5272_WR_PLI_P0B2TR(IMMP,DATA)	\
	Mcf5272_iowr32(IMMP,MCF5272_PLI_P0B2TR,DATA)
#define MCF5272_WR_PLI_P1B2TR(IMMP,DATA)	\
	Mcf5272_iowr32(IMMP,MCF5272_PLI_P1B2TR,DATA)
#define MCF5272_WR_PLI_P2B2TR(IMMP,DATA)	\
	Mcf5272_iowr32(IMMP,MCF5272_PLI_P2B2TR,DATA)
#define MCF5272_WR_PLI_P3B2TR(IMMP,DATA)	\
	Mcf5272_iowr32(IMMP,MCF5272_PLI_P3B2TR,DATA)
#define MCF5272_WR_PLI_P0DTR(IMMP,DATA)		\
	Mcf5272_iowr8(IMMP,MCF5272_PLI_P0DTR,DATA)
#define MCF5272_WR_PLI_P1DTR(IMMP,DATA)		\
	Mcf5272_iowr8(IMMP,MCF5272_PLI_P1DTR,DATA)
#define MCF5272_WR_PLI_P2DTR(IMMP,DATA)		\
	Mcf5272_iowr8(IMMP,MCF5272_PLI_P2DTR,DATA)
#define MCF5272_WR_PLI_P3DTR(IMMP,DATA)		\
	Mcf5272_iowr8(IMMP,MCF5272_PLI_P3DTR,DATA)
#define MCF5272_WR_PLI_P0CR(IMMP,DATA)		\
	Mcf5272_iowr16(IMMP,MCF5272_PLI_P0CR,DATA)
#define MCF5272_WR_PLI_P1CR(IMMP,DATA)		\
	Mcf5272_iowr16(IMMP,MCF5272_PLI_P1CR,DATA)
#define MCF5272_WR_PLI_P2CR(IMMP,DATA)		\
	Mcf5272_iowr16(IMMP,MCF5272_PLI_P2CR,DATA)
#define MCF5272_WR_PLI_P3CR(IMMP,DATA)		\
	Mcf5272_iowr16(IMMP,MCF5272_PLI_P3CR,DATA)
#define MCF5272_WR_PLI_P0ICR(IMMP,DATA)	\
	Mcf5272_iowr16(IMMP,MCF5272_PLI_P0ICR,DATA)
#define MCF5272_WR_PLI_P1ICR(IMMP,DATA)	\
	Mcf5272_iowr16(IMMP,MCF5272_PLI_P1ICR,DATA)
#define MCF5272_WR_PLI_P2ICR(IMMP,DATA)	\
	Mcf5272_iowr16(IMMP,MCF5272_PLI_P2ICR,DATA)
#define MCF5272_WR_PLI_P3ICR(IMMP,DATA)	\
	Mcf5272_iowr16(IMMP,MCF5272_PLI_P3ICR,DATA)
#define MCF5272_WR_PLI_P0GMT(IMMP,DATA)	\
	Mcf5272_iowr16(IMMP,MCF5272_PLI_P0GMR,DATA)
#define MCF5272_WR_PLI_P1GMT(IMMP,DATA)	\
	Mcf5272_iowr16(IMMP,MCF5272_PLI_P1GMT,DATA)
#define MCF5272_WR_PLI_P2GMT(IMMP,DATA)	\
	Mcf5272_iowr16(IMMP,MCF5272_PLI_P2GMT,DATA)
#define MCF5272_WR_PLI_P3GMT(IMMP,DATA)	\
	Mcf5272_iowr16(IMMP,MCF5272_PLI_P3GMT,DATA)
#define MCF5272_WR_PLI_PGMTA(IMMP,DATA)	\
	Mcf5272_iowr8(IMMP,MCF5272_PLI_PGMTA,DATA)
#define MCF5272_WR_PLI_P0GCIT(IMMP,DATA)	\
	Mcf5272_iowr8(IMMP,MCF5272_PLI_P0GCIT,DATA)
#define MCF5272_WR_PLI_P1GCIT(IMMP,DATA)	\
	Mcf5272_iowr8(IMMP,MCF5272_PLI_P1GCIT,DATA)
#define MCF5272_WR_PLI_P2GCIT(IMMP,DATA)	\
	Mcf5272_iowr8(IMMP,MCF5272_PLI_P2GCIT,DATA)
#define MCF5272_WR_PLI_P3GCIT(IMMP,DATA)	\
	Mcf5272_iowr8(IMMP,MCF5272_PLI_P3GCIT,DATA)
#define MCF5272_WR_PLI_PLCR(IMMP,DATA)		\
	Mcf5272_iowr8(IMMP,MCF5272_PLI_PLCR,DATA)
#define MCF5272_WR_PLI_PDRQR(IMMP,DATA)		\
	Mcf5272_iowr16(IMMP,MCF5272_PLI_PDRQR,DATA)
#define MCF5272_WR_PLI_P0SDR(IMMP,DATA)		\
	Mcf5272_iowr16(IMMP,MCF5272_PLI_P0SDR,DATA)
#define MCF5272_WR_PLI_P1SDR(IMMP,DATA)		\
	Mcf5272_iowr16(IMMP,MCF5272_PLI_P1SDR,DATA)
#define MCF5272_WR_PLI_P2SDR(IMMP,DATA)		\
	Mcf5272_iowr16(IMMP,MCF5272_PLI_P2SDR,DATA)
#define MCF5272_WR_PLI_P3SDR(IMMP,DATA)		\
	Mcf5272_iowr16(IMMP,MCF5272_PLI_P3SDR,DATA)
#define MCF5272_WR_PLI_PCKSELR(IMMP,DATA)	\
	Mcf5272_iowr16(IMMP,MCF5272_PLI_PCKSELR,DATA)


/**********************************************************************
*
* Fast Ethernet Controller Module Register Descriptions
*
***********************************************************************/

/* Offsets of the registers from the MBAR */
#define MCF5272_FEC_ECR			(0x0840)
#define MCF5272_FEC_EIR			(0x0844)
#define MCF5272_FEC_IMR			(0x0848)
#define MCF5272_FEC_IVSR		(0x084C)
#define MCF5272_FEC_RDAR		(0x0850)
#define MCF5272_FEC_TDAR		(0x0854)
#define MCF5272_FEC_MMFR		(0x0880)
#define MCF5272_FEC_MSCR		(0x0884)
#define MCF5272_FEC_FRBR		(0x08CC)
#define MCF5272_FEC_FRSR		(0x08D0)
#define MCF5272_FEC_TFWR		(0x08E4)
#define MCF5272_FEC_FTSR		(0x08EC)
#define MCF5272_FEC_RCR			(0x0944)
#define MCF5272_FEC_MFLR		(0x0948)
#define MCF5272_FEC_TCR			(0x0984)
#define MCF5272_FEC_MALR		(0x0C00)
#define MCF5272_FEC_MAUR		(0x0C04)
#define MCF5272_FEC_HTUR		(0x0C08)
#define MCF5272_FEC_HTLR		(0x0C0C)
#define MCF5272_FEC_ERDSR		(0x0C10)
#define MCF5272_FEC_ETDSR		(0x0C14)
#define MCF5272_FEC_EMRBR		(0x0C18)
#define MCF5272_FEC_EFIFO		(0x0C40)	/* 0x0C40 - 0x0DFF */

/* Read access macros for general use */
#define MCF5272_RD_FEC_ECR(IMMP)			\
	Mcf5272_iord32(IMMP,MCF5272_FEC_ECR)
#define MCF5272_RD_FEC_EIR(IMMP)			\
	Mcf5272_iord32(IMMP,MCF5272_FEC_EIR)
#define MCF5272_RD_FEC_IMR(IMMP)			\
	Mcf5272_iord32(IMMP,MCF5272_FEC_IMR)
#define MCF5272_RD_FEC_IVSR(IMMP)			\
	Mcf5272_iord32(IMMP,MCF5272_FEC_IVSR)
#define MCF5272_RD_FEC_RDAR(IMMP)	\
	Mcf5272_iord32(IMMP,MCF5272_FEC_RDAR)
#define MCF5272_RD_FEC_TDAR(IMMP)	\
	Mcf5272_iord32(IMMP,MCF5272_FEC_TDAR)
#define MCF5272_RD_FEC_MMFR(IMMP)		\
	Mcf5272_iord32(IMMP,MCF5272_FEC_MMFR)
#define MCF5272_RD_FEC_MSCR(IMMP)		\
	Mcf5272_iord32(IMMP,MCF5272_FEC_MSCR)
#define MCF5272_RD_FEC_FRBR(IMMP)		\
	Mcf5272_iord32(IMMP,MCF5272_FEC_FRBR)
#define MCF5272_RD_FEC_FRSR(IMMP)		\
	Mcf5272_iord32(IMMP,MCF5272_FEC_FRSR)
#define MCF5272_RD_FEC_TFWR(IMMP)			\
	Mcf5272_iord32(IMMP,MCF5272_FEC_TFWR)
#define MCF5272_RD_FEC_FTSR(IMMP)		\
	Mcf5272_iord32(IMMP,MCF5272_FEC_FTSR)
#define MCF5272_RD_FEC_RCR(IMMP)		\
	Mcf5272_iord32(IMMP,MCF5272_FEC_RCR)
#define MCF5272_RD_FEC_MFLR(IMMP)	\
	Mcf5272_iord32(IMMP,MCF5272_FEC_MFLR)
#define MCF5272_RD_FEC_TCR(IMMP)		\
	Mcf5272_iord32(IMMP,MCF5272_FEC_TCR)
#define MCF5272_RD_FEC_MALR(IMMP)		\
	Mcf5272_iord32(IMMP,MCF5272_FEC_MALR)
#define MCF5272_RD_FEC_MAUR(IMMP)		\
	Mcf5272_iord32(IMMP,MCF5272_FEC_MAUR)
#define MCF5272_RD_FEC_HTUR(IMMP)		\
	Mcf5272_iord32(IMMP,MCF5272_FEC_HTUR)
#define MCF5272_RD_FEC_HTLR(IMMP)		\
	Mcf5272_iord32(IMMP,MCF5272_FEC_HTLR)
#define MCF5272_RD_FEC_ERDSR(IMMP)	\
	Mcf5272_iord32(IMMP,MCF5272_FEC_ERDSR)
#define MCF5272_RD_FEC_ETDSR(IMMP)	\
	Mcf5272_iord32(IMMP,MCF5272_FEC_ETDSR)
#define MCF5272_RD_FEC_EMRBR(IMMP)	\
	Mcf5272_iord32(IMMP,MCF5272_FEC_EMRBR)

/* Write access macros for general use */
#define MCF5272_WR_FEC_ECR(IMMP,DATA)		\
	Mcf5272_iowr32(IMMP,MCF5272_FEC_ECR,DATA)
#define MCF5272_WR_FEC_EIR(IMMP,DATA)		\
	Mcf5272_iowr32(IMMP,MCF5272_FEC_EIR,DATA)
#define MCF5272_WR_FEC_IMR(IMMP,DATA)			\
	Mcf5272_iowr32(IMMP,MCF5272_FEC_IMR,DATA)
#define MCF5272_WR_FEC_RDAR(IMMP,DATA)	\
	Mcf5272_iowr32(IMMP,MCF5272_FEC_RDAR,DATA)
#define MCF5272_WR_FEC_TDAR(IMMP,DATA)	\
	Mcf5272_iowr32(IMMP,MCF5272_FEC_TDAR,DATA)
#define MCF5272_WR_FEC_MMFR(IMMP,DATA)		\
	Mcf5272_iowr32(IMMP,MCF5272_FEC_MMFR,DATA)
#define MCF5272_WR_FEC_MSCR(IMMP,DATA)		\
	Mcf5272_iowr32(IMMP,MCF5272_FEC_MSCR,DATA)
#define MCF5272_WR_FEC_FRSR(IMMP,DATA)		\
	Mcf5272_iowr32(IMMP,MCF5272_FEC_FRSR,DATA)
#define MCF5272_WR_FEC_TFWR(IMMP,DATA)		\
	Mcf5272_iowr32(IMMP,MCF5272_FEC_TFWR,DATA)
#define MCF5272_WR_FEC_FTSR(IMMP,DATA)		\
	Mcf5272_iowr32(IMMP,MCF5272_FEC_FTSR,DATA)
#define MCF5272_WR_FEC_RCR(IMMP,DATA)		\
	Mcf5272_iowr32(IMMP,MCF5272_FEC_RCR,DATA)
#define MCF5272_WR_FEC_MFLR(IMMP,DATA)	\
	Mcf5272_iowr32(IMMP,MCF5272_FEC_MFLR,DATA)
#define MCF5272_WR_FEC_TCR(IMMP,DATA)		\
	Mcf5272_iowr32(IMMP,MCF5272_FEC_TCR,DATA)
#define MCF5272_WR_FEC_MALR(IMMP,DATA)		\
	Mcf5272_iowr32(IMMP,MCF5272_FEC_MALR,DATA)
#define MCF5272_WR_FEC_MAUR(IMMP,DATA)		\
	Mcf5272_iowr32(IMMP,MCF5272_FEC_MAUR,DATA)
#define MCF5272_WR_FEC_HTUR(IMMP,DATA)		\
	Mcf5272_iowr32(IMMP,MCF5272_FEC_HTUR,DATA)
#define MCF5272_WR_FEC_HTLR(IMMP,DATA)		\
	Mcf5272_iowr32(IMMP,MCF5272_FEC_HTLR,DATA)
#define MCF5272_WR_FEC_ERDSR(IMMP,DATA)	\
	Mcf5272_iowr32(IMMP,MCF5272_FEC_ERDSR,DATA)
#define MCF5272_WR_FEC_ETDSR(IMMP,DATA)	\
	Mcf5272_iowr32(IMMP,MCF5272_FEC_ETDSR,DATA)
#define MCF5272_WR_FEC_EMRBR(IMMP,DATA)	\
	Mcf5272_iowr32(IMMP,MCF5272_FEC_EMRBR,DATA)


/* Bit level definitions and macros */
#define MCF5272_FEC_ECR_RESET		(0x00000001)
#define MCF5272_FEC_ECR_ETHER_EN	(0x00000002)
#define MCF5272_FEC_EIR_GRA			(0x10000000)
#define MCF5272_FEC_EIR_RXF			(0x02000000)
#define MCF5272_FEC_EIR_TXF			(0x08000000)
#define MCF5272_FEC_IMR_HBEEN		(0x80000000)
#define MCF5272_FEC_IMR_BREN		(0x40000000)
#define MCF5272_FEC_IMR_BTEN		(0x20000000)
#define MCF5272_FEC_IMR_GRAEN		(0x10000000)
#define MCF5272_FEC_IMR_TXFEN		(0x08000000)
#define MCF5272_FEC_IMR_TXBEN		(0x04000000)
#define MCF5272_FEC_IMR_RXFEN		(0x02000000)
#define MCF5272_FEC_IMR_RXBEN		(0x01000000)
#define MCF5272_FEC_IMR_MIIEN		(0x00800000)
#define MCF5272_FEC_IMR_EBERREN		(0x00400000)
#define MCF5272_FEC_RCR_PROM		(0x00000008)
#define MCF5272_FEC_RCR_MII_MODE	(0x00000004)
#define MCF5272_FEC_RCR_DRT			(0x00000002)
#define MCF5272_FEC_RCR_LOOP		(0x00000001)
#define MCF5272_FEC_TCR_GTS			(0x00000001)
#define MCF5272_FEC_TCR_HBC			(0x00000002)
#define MCF5272_FEC_TCR_FDEN		(0x00000004)
#define MCF5272_MMFR_MODE			(0x00004000)
#define MCF5272_MMFR_DUPLEX			(0x00000200)
#define MCF5272_MMFR_LOOP			(0x00004000)
#define MCF5272_MMFR_NEGOTIATE		(0x00001000)
#define MCF5272_MMFR_100			(0x00002000)

/**********************************************************************
*
* USB_ Module Register Description
*
***********************************************************************/

/* Offsets of the registers from the MBAR */
#define MCF5272_USB_FNR			(0x1002)
#define MCF5272_USB_FNMR		(0x1006)
#define MCF5272_USB_RFMR		(0x100A)
#define MCF5272_USB_RFMMR		(0x100E)
#define MCF5272_USB_FAR			(0x1013)
#define MCF5272_USB_ASR			(0x1014)
#define MCF5272_USB_DRR1		(0x1018)
#define MCF5272_USB_DRR2		(0x101C)
#define MCF5272_USB_SPECR		(0x1022)
#define MCF5272_USB_EP0SR		(0x1026)
#define MCF5272_USB_IEP0CFG		(0x1028)
#define MCF5272_USB_OEP0CFG		(0x102C)
#define MCF5272_USB_EP1CFG		(0x1030)
#define MCF5272_USB_EP2CFG		(0x1034)
#define MCF5272_USB_EP3CFG		(0x1038)
#define MCF5272_USB_EP4CFG		(0x103C)
#define MCF5272_USB_EP5CFG		(0x1040)
#define MCF5272_USB_EP6CFG		(0x1044)
#define MCF5272_USB_EP7CFG		(0x1048)
#define MCF5272_USB_EP0CTL		(0x104C)
#define MCF5272_USB_EP1CTL		(0x1052)
#define MCF5272_USB_EP2CTL		(0x1056)
#define MCF5272_USB_EP3CTL		(0x105A)
#define MCF5272_USB_EP4CTL		(0x105E)
#define MCF5272_USB_EP5CTL		(0x1062)
#define MCF5272_USB_EP6CTL		(0x1066)
#define MCF5272_USB_EP7CTL		(0x106A)
#define MCF5272_USB_EP0ISR		(0x106C)
#define MCF5272_USB_EP1ISR		(0x1072)
#define MCF5272_USB_EP2ISR		(0x1076)
#define MCF5272_USB_EP3ISR		(0x107A)
#define MCF5272_USB_EP4ISR		(0x107E)
#define MCF5272_USB_EP5ISR		(0x1082)
#define MCF5272_USB_EP6ISR		(0x1086)
#define MCF5272_USB_EP7ISR		(0x108A)
#define MCF5272_USB_EP0IMR		(0x108C)
#define MCF5272_USB_EP1IMR		(0x1092)
#define MCF5272_USB_EP2IMR		(0x1096)
#define MCF5272_USB_EP3IMR		(0x109A)
#define MCF5272_USB_EP4IMR		(0x109E)
#define MCF5272_USB_EP5IMR		(0x10A2)
#define MCF5272_USB_EP6IMR		(0x10A6)
#define MCF5272_USB_EP7IMR		(0x10AA)
#define MCF5272_USB_EP0DR		(0x10AC)
#define MCF5272_USB_EP1DR		(0x10B0)
#define MCF5272_USB_EP2DR		(0x10B4)
#define MCF5272_USB_EP3DR		(0x10B8)
#define MCF5272_USB_EP4DR		(0x10BC)
#define MCF5272_USB_EP5DR		(0x10C0)
#define MCF5272_USB_EP6DR		(0x10C4)
#define MCF5272_USB_EP7DR		(0x10C8)
#define MCF5272_USB_EP0DPR		(0x10CE)
#define MCF5272_USB_EP1DPR		(0x10D2)
#define MCF5272_USB_EP2DPR		(0x10D6)
#define MCF5272_USB_EP3DPR		(0x10DA)
#define MCF5272_USB_EP4DPR		(0x10DE)
#define MCF5272_USB_EP5DPR		(0x10E2)
#define MCF5272_USB_EP6DPR		(0x10E6)
#define MCF5272_USB_EP7DPR		(0x10EA)
#define MCF5272_USB_CFG_RAM		(0x1400)	/* 0x1400 - 0x17FF */

/* Read access macros for general use */
#define MCF5272_RD_USB_FNR(IMMP)		\
	Mcf5272_iord16(IMMP,MCF5272_USB_FNR)
#define MCF5272_RD_USB_FNMR(IMMP)	\
	Mcf5272_iord16(IMMP,MCF5272_USB_FNMR)
#define MCF5272_RD_USB_RFMR(IMMP)	\
	Mcf5272_iord16(IMMP,MCF5272_USB_RFMR)
#define MCF5272_RD_USB_RFMMR(IMMP)	\
	Mcf5272_iord16(IMMP,MCF5272_USB_RFMMR)
#define MCF5272_RD_USB_FAR(IMMP)		\
	Mcf5272_iord8(IMMP,MCF5272_USB_FAR)
#define MCF5272_RD_USB_ASR(IMMP)		\
	Mcf5272_iord32(IMMP,MCF5272_USB_ASR)
#define MCF5272_RD_USB_DRR1(IMMP)	\
	Mcf5272_iord32(IMMP,MCF5272_USB_DRR1)
#define MCF5272_RD_USB_DRR2(IMMP)	\
	Mcf5272_iord32(IMMP,MCF5272_USB_DRR2)
#define MCF5272_RD_USB_SPECR(IMMP)	\
	Mcf5272_iord16(IMMP,MCF5272_USB_SPECR)
#define MCF5272_RD_USB_EP0SR(IMMP)	\
	Mcf5272_iord16(IMMP,MCF5272_USB_EP0SR)
#define MCF5272_RD_USB_IEP0CFG(IMMP)	\
	Mcf5272_iord32(IMMP,MCF5272_USB_IEP0CFG)
#define MCF5272_RD_USB_OEP0CFG(IMMP)	\
	Mcf5272_iord32(IMMP,MCF5272_USB_OEP0CFG)
#define MCF5272_RD_USB_EPCFG(IMMP,EPN)	\
	Mcf5272_iord32(IMMP,MCF5272_USB_EP1CFG + 4*(EPN-1))
#define MCF5272_RD_USB_EP1CFG(IMMP)	\
	Mcf5272_iord32(IMMP,MCF5272_USB_EP1CFG)
#define MCF5272_RD_USB_EP2CFG(IMMP)	\
	Mcf5272_iord32(IMMP,MCF5272_USB_EP2CFG)
#define MCF5272_RD_USB_EP3CFG(IMMP)	\
	Mcf5272_iord32(IMMP,MCF5272_USB_EP3CFG)
#define MCF5272_RD_USB_EP4CFG(IMMP)	\
	Mcf5272_iord32(IMMP,MCF5272_USB_EP4CFG)
#define MCF5272_RD_USB_EP5CFG(IMMP)	\
	Mcf5272_iord32(IMMP,MCF5272_USB_EP5CFG)
#define MCF5272_RD_USB_EP6CFG(IMMP)	\
	Mcf5272_iord32(IMMP,MCF5272_USB_EP6CFG)
#define MCF5272_RD_USB_EP7CFG(IMMP)	\
	Mcf5272_iord32(IMMP,MCF5272_USB_EP7CFG)
#define MCF5272_RD_USB_EPCTL(IMMP,EPN)	\
	Mcf5272_iord32(IMMP,MCF5272_USB_EP0CTL + 4*EPN)
#define MCF5272_RD_USB_EP0CTL(IMMP)	\
	Mcf5272_iord32(IMMP,MCF5272_USB_EP0CTL)
#define MCF5272_RD_USB_EP1CTL(IMMP)	\
	Mcf5272_iord16(IMMP,MCF5272_USB_EP1CTL)
#define MCF5272_RD_USB_EP2CTL(IMMP)	\
	Mcf5272_iord16(IMMP,MCF5272_USB_EP2CTL)
#define MCF5272_RD_USB_EP3CTL(IMMP)	\
	Mcf5272_iord16(IMMP,MCF5272_USB_EP3CTL)
#define MCF5272_RD_USB_EP4CTL(IMMP)	\
	Mcf5272_iord16(IMMP,MCF5272_USB_EP4CTL)
#define MCF5272_RD_USB_EP5CTL(IMMP)	\
	Mcf5272_iord16(IMMP,MCF5272_USB_EP5CTL)
#define MCF5272_RD_USB_EP6CTL(IMMP)	\
	Mcf5272_iord16(IMMP,MCF5272_USB_EP6CTL)
#define MCF5272_RD_USB_EP7CTL(IMMP)	\
	Mcf5272_iord16(IMMP,MCF5272_USB_EP7CTL)
#define MCF5272_RD_USB_EPISR(IMMP,EPN)	\
	Mcf5272_iord32(IMMP,MCF5272_USB_EP0ISR + 4*EPN)
#define MCF5272_RD_USB_EP0ISR(IMMP)	\
	Mcf5272_iord32(IMMP,MCF5272_USB_EP0ISR)
#define MCF5272_RD_USB_EP1ISR(IMMP)	\
	Mcf5272_iord16(IMMP,MCF5272_USB_EP1ISR)
#define MCF5272_RD_USB_EP2ISR(IMMP)	\
	Mcf5272_iord16(IMMP,MCF5272_USB_EP2ISR)
#define MCF5272_RD_USB_EP3ISR(IMMP)	\
	Mcf5272_iord16(IMMP,MCF5272_USB_EP3ISR)
#define MCF5272_RD_USB_EP4ISR(IMMP)	\
	Mcf5272_iord16(IMMP,MCF5272_USB_EP4ISR)
#define MCF5272_RD_USB_EP5ISR(IMMP)	\
	Mcf5272_iord16(IMMP,MCF5272_USB_EP5ISR)
#define MCF5272_RD_USB_EP6ISR(IMMP)	\
	Mcf5272_iord16(IMMP,MCF5272_USB_EP6ISR)
#define MCF5272_RD_USB_EP7ISR(IMMP)	\
	Mcf5272_iord16(IMMP,MCF5272_USB_EP7ISR)
#define MCF5272_RD_USB_EPIMR(IMMP,EPN)	\
	Mcf5272_iord32(IMMP,MCF5272_USB_EP0IMR + 4*EPN)
#define MCF5272_RD_USB_EP0IMR(IMMP)	\
	Mcf5272_iord32(IMMP,MCF5272_USB_EP0IMR)
#define MCF5272_RD_USB_EP1IMR(IMMP)	\
	Mcf5272_iord16(IMMP,MCF5272_USB_EP1IMR)
#define MCF5272_RD_USB_EP2IMR(IMMP)	\
	Mcf5272_iord16(IMMP,MCF5272_USB_EP2IMR)
#define MCF5272_RD_USB_EP3IMR(IMMP)	\
	Mcf5272_iord16(IMMP,MCF5272_USB_EP3IMR)
#define MCF5272_RD_USB_EP4IMR(IMMP)	\
	Mcf5272_iord16(IMMP,MCF5272_USB_EP4IMR)
#define MCF5272_RD_USB_EP5IMR(IMMP)	\
	Mcf5272_iord16(IMMP,MCF5272_USB_EP5IMR)
#define MCF5272_RD_USB_EP6IMR(IMMP)	\
	Mcf5272_iord16(IMMP,MCF5272_USB_EP6IMR)
#define MCF5272_RD_USB_EP7IMR(IMMP)	\
	Mcf5272_iord16(IMMP,MCF5272_USB_EP7IMR)
#define MCF5272_RD_USB_EPDR(IMMP,EPN,SIZE)	\
	Mcf5272_iord(IMMP,MCF5272_USB_EP0DR + 4*EPN,SIZE)
#define MCF5272_RD_USB_EP0DR(IMMP,SIZE)	\
	Mcf5272_iord(IMMP,MCF5272_USB_EP0DR,SIZE)
#define MCF5272_RD_USB_EP1DR(IMMP,SIZE)	\
	Mcf5272_iord(IMMP,MCF5272_USB_EP1DR,SIZE)
#define MCF5272_RD_USB_EP2DR(IMMP,SIZE)	\
	Mcf5272_iord(IMMP,MCF5272_USB_EP2DR,SIZE)
#define MCF5272_RD_USB_EP3DR(IMMP,SIZE)	\
	Mcf5272_iord(IMMP,MCF5272_USB_EP3DR,SIZE)
#define MCF5272_RD_USB_EP4DR(IMMP,SIZE)	\
	Mcf5272_iord(IMMP,MCF5272_USB_EP4DR,SIZE)
#define MCF5272_RD_USB_EP5DR(IMMP,SIZE)	\
	Mcf5272_iord(IMMP,MCF5272_USB_EP5DR,SIZE)
#define MCF5272_RD_USB_EP6DR(IMMP,SIZE)	\
	Mcf5272_iord(IMMP,MCF5272_USB_EP6DR,SIZE)
#define MCF5272_RD_USB_EP7DR(IMMP,SIZE)	\
	Mcf5272_iord(IMMP,MCF5272_USB_EP7DR,SIZE)
#define MCF5272_RD_USB_EPDPR(IMMP,EPN)	\
	Mcf5272_iord16(IMMP,MCF5272_USB_EP0DPR + 4*EPN)
#define MCF5272_RD_USB_EP0DPR(IMMP)	\
	Mcf5272_iord16(IMMP,MCF5272_USB_EP0DPR)
#define MCF5272_RD_USB_EP1DPR(IMMP)	\
	Mcf5272_iord16(IMMP,MCF5272_USB_EP1DPR)
#define MCF5272_RD_USB_EP2DPR(IMMP)	\
	Mcf5272_iord16(IMMP,MCF5272_USB_EP2DPR)
#define MCF5272_RD_USB_EP3DPR(IMMP)	\
	Mcf5272_iord16(IMMP,MCF5272_USB_EP3DPR)
#define MCF5272_RD_USB_EP4DPR(IMMP)	\
	Mcf5272_iord16(IMMP,MCF5272_USB_EP4DPR)
#define MCF5272_RD_USB_EP5DPR(IMMP)	\
	Mcf5272_iord16(IMMP,MCF5272_USB_EP5DPR)
#define MCF5272_RD_USB_EP6DPR(IMMP)	\
	Mcf5272_iord16(IMMP,MCF5272_USB_EP6DPR)
#define MCF5272_RD_USB_EP7DPR(IMMP)	\
	Mcf5272_iord16(IMMP,MCF5272_USB_EP7DPR)

/* Write access macros for general use */
#define MCF5272_WR_USB_FNR(IMMP,DATA)	\
	Mcf5272_iowr16(IMMP,MCF5272_USB_FNR,DATA)
#define MCF5272_WR_USB_FNMR(IMMP,DATA)	\
	Mcf5272_iowr16(IMMP,MCF5272_USB_FNMR,DATA)
#define MCF5272_WR_USB_RFMMR(IMMP,DATA)	\
	Mcf5272_iowr16(IMMP,MCF5272_USB_RFMMR,DATA)
#define MCF5272_WR_USB_EP0SR(IMMP,DATA)\
	Mcf5272_iowr32(IMMP,MCF5272_USB_EP0SR,DATA)
#define MCF5272_WR_USB_IEP0CFG(IMMP,DATA)\
	Mcf5272_iowr32(IMMP,MCF5272_USB_IEP0CFG,DATA)
#define MCF5272_WR_USB_OEP0CFG(IMMP,DATA)\
	Mcf5272_iowr32(IMMP,MCF5272_USB_OEP0CFG,DATA)
#define MCF5272_WR_USB_EPCFG(IMMP,EPN,DATA)	\
	Mcf5272_iowr32(IMMP,MCF5272_USB_EP1CFG + 4*(EPN-1),DATA)
#define MCF5272_WR_USB_EP1CFG(IMMP,DATA)	\
	Mcf5272_iowr32(IMMP,MCF5272_USB_EP1CFG,DATA)
#define MCF5272_WR_USB_EP2CFG(IMMP,DATA)	\
	Mcf5272_iowr32(IMMP,MCF5272_USB_EP2CFG,DATA)
#define MCF5272_WR_USB_EP3CFG(IMMP,DATA)	\
	Mcf5272_iowr32(IMMP,MCF5272_USB_EP3CFG,DATA)
#define MCF5272_WR_USB_EP4CFG(IMMP,DATA)	\
	Mcf5272_iowr32(IMMP,MCF5272_USB_EP4CFG,DATA)
#define MCF5272_WR_USB_EP5CFG(IMMP,DATA)	\
	Mcf5272_iowr32(IMMP,MCF5272_USB_EP5CFG,DATA)
#define MCF5272_WR_USB_EP6CFG(IMMP,DATA)	\
	Mcf5272_iowr32(IMMP,MCF5272_USB_EP6CFG,DATA)
#define MCF5272_WR_USB_EP7CFG(IMMP,DATA)	\
	Mcf5272_iowr32(IMMP,MCF5272_USB_EP7CFG,DATA)
#define MCF5272_WR_USB_EPCTL(IMMP,EPN,DATA)	\
	Mcf5272_iowr32(IMMP,MCF5272_USB_EP0CTL + 4*EPN,DATA)
#define MCF5272_WR_USB_EP0CTL(IMMP,DATA)	\
	Mcf5272_iowr32(IMMP,MCF5272_USB_EP0CTL,DATA)
#define MCF5272_WR_USB_EP1CTL(IMMP,DATA)	\
	Mcf5272_iowr16(IMMP,MCF5272_USB_EP1CTL,DATA)
#define MCF5272_WR_USB_EP2CTL(IMMP,DATA)	\
	Mcf5272_iowr16(IMMP,MCF5272_USB_EP2CTL,DATA)
#define MCF5272_WR_USB_EP3CTL(IMMP,DATA)	\
	Mcf5272_iowr16(IMMP,MCF5272_USB_EP3CTL,DATA)
#define MCF5272_WR_USB_EP4CTL(IMMP,DATA)	\
	Mcf5272_iowr16(IMMP,MCF5272_USB_EP4CTL,DATA)
#define MCF5272_WR_USB_EP5CTL(IMMP,DATA)	\
	Mcf5272_iowr16(IMMP,MCF5272_USB_EP5CTL,DATA)
#define MCF5272_WR_USB_EP6CTL(IMMP,DATA)	\
	Mcf5272_iowr16(IMMP,MCF5272_USB_EP6CTL,DATA)
#define MCF5272_WR_USB_EP7CTL(IMMP,DATA)	\
	Mcf5272_iowr16(IMMP,MCF5272_USB_EP7CTL,DATA)
#define MCF5272_WR_USB_EPISR(IMMP,EPN,DATA)	\
	Mcf5272_iowr32(IMMP,MCF5272_USB_EP0ISR + 4*EPN,DATA)
#define MCF5272_WR_USB_EP0ISR(IMMP,DATA)	\
	Mcf5272_iowr32(IMMP,MCF5272_USB_EP0ISR,DATA)
#define MCF5272_WR_USB_EP1ISR(IMMP,DATA)	\
	Mcf5272_iowr16(IMMP,MCF5272_USB_EP1ISR,DATA)
#define MCF5272_WR_USB_EP2ISR(IMMP,DATA)	\
	Mcf5272_iowr16(IMMP,MCF5272_USB_EP2ISR,DATA)
#define MCF5272_WR_USB_EP3ISR(IMMP,DATA)	\
	Mcf5272_iowr16(IMMP,MCF5272_USB_EP3ISR,DATA)
#define MCF5272_WR_USB_EP4ISR(IMMP,DATA)	\
	Mcf5272_iowr16(IMMP,MCF5272_USB_EP4ISR,DATA)
#define MCF5272_WR_USB_EP5ISR(IMMP,DATA)	\
	Mcf5272_iowr16(IMMP,MCF5272_USB_EP5ISR,DATA)
#define MCF5272_WR_USB_EP6ISR(IMMP,DATA)	\
	Mcf5272_iowr16(IMMP,MCF5272_USB_EP6ISR,DATA)
#define MCF5272_WR_USB_EP7ISR(IMMP,DATA)	\
	Mcf5272_iowr16(IMMP,MCF5272_USB_EP7ISR,DATA)
#define MCF5272_WR_USB_EPIMR(IMMP,EPN,DATA)	\
	Mcf5272_iowr32(IMMP,MCF5272_USB_EP0IMR + 4*EPN,DATA)
#define MCF5272_WR_USB_EP0IMR(IMMP,DATA)	\
	Mcf5272_iowr32(IMMP,MCF5272_USB_EP0IMR,DATA)
#define MCF5272_WR_USB_EP1IMR(IMMP,DATA)	\
	Mcf5272_iowr16(IMMP,MCF5272_USB_EP1IMR,DATA)
#define MCF5272_WR_USB_EP2IMR(IMMP,DATA)	\
	Mcf5272_iowr16(IMMP,MCF5272_USB_EP2IMR,DATA)
#define MCF5272_WR_USB_EP3IMR(IMMP,DATA)	\
	Mcf5272_iowr16(IMMP,MCF5272_USB_EP3IMR,DATA)
#define MCF5272_WR_USB_EP4IMR(IMMP,DATA)	\
	Mcf5272_iowr16(IMMP,MCF5272_USB_EP4IMR,DATA)
#define MCF5272_WR_USB_EP5IMR(IMMP,DATA)	\
	Mcf5272_iowr16(IMMP,MCF5272_USB_EP5IMR,DATA)
#define MCF5272_WR_USB_EP6IMR(IMMP,DATA)	\
	Mcf5272_iowr16(IMMP,MCF5272_USB_EP6IMR,DATA)
#define MCF5272_WR_USB_EP7IMR(IMMP,DATA)	\
	Mcf5272_iowr16(IMMP,MCF5272_USB_EP7IMR,DATA)
#define MCF5272_WR_USB_EPDR(IMMP,EPN,SIZE,DATA)	\
	Mcf5272_iowr(IMMP,MCF5272_USB_EP0DR + 4*EPN,SIZE,DATA)
#define MCF5272_WR_USB_EP0DR(IMMP,SIZE,DATA)	\
	Mcf5272_iowr(IMMP,MCF5272_USB_EP0DR,SIZE,DATA)
#define MCF5272_WR_USB_EP1DR(IMMP,SIZE,DATA)	\
	Mcf5272_iowr(IMMP,MCF5272_USB_EP1DR,SIZE,DATA)
#define MCF5272_WR_USB_EP2DR(IMMP,SIZE,DATA)	\
	Mcf5272_iowr(IMMP,MCF5272_USB_EP2DR,SIZE,DATA)
#define MCF5272_WR_USB_EP3DR(IMMP,SIZE,DATA)	\
	Mcf5272_iowr(IMMP,MCF5272_USB_EP3DR,SIZE,DATA)
#define MCF5272_WR_USB_EP4DR(IMMP,SIZE,DATA)	\
	Mcf5272_iowr(IMMP,MCF5272_USB_EP4DR,SIZE,DATA)
#define MCF5272_WR_USB_EP5DR(IMMP,SIZE,DATA)	\
	Mcf5272_iowr(IMMP,MCF5272_USB_EP5DR,SIZE,DATA)
#define MCF5272_WR_USB_EP6DR(IMMP,SIZE,DATA)	\
	Mcf5272_iowr(IMMP,MCF5272_USB_EP6DR,SIZE,DATA)
#define MCF5272_WR_USB_EP7DR(IMMP,SIZE,DATA)	\
	Mcf5272_iowr(IMMP,MCF5272_USB_EP7DR,SIZE,DATA)

/* Macro to return current Alternate Setting for a given Interface */
#define AltSetting(imm, i)	(int8)((MCF5272_RD_USB_ASR(imm) >> (i << 1)) & 3)

/* Bit level definitions and macros */
#define MCF5272_USB_EP0SR_WAKE_ST           0x0800
#define MCF5272_USB_EP0SR_HALT_ST           0x0004
#define MCF5272_USB_EP0SR_DIR               0x0002

#define MCF5272_USB_EP0CTL_DEBUG			0x00040000
#define MCF5272_USB_EP0CTL_WOR_LVL			0x00020000
#define MCF5272_USB_EP0CTL_WOR_EN			0x00010000
#define MCF5272_USB_EP0CTL_CLK_SEL			0x00008000
#define MCF5272_USB_EP0CTL_RESUME			0x00004000
#define MCF5272_USB_EP0CTL_AFE_EN			0x00002000
#define MCF5272_USB_EP0CTL_BUS_PWR			0x00001000
#define MCF5272_USB_EP0CTL_USB_EN			0x00000800
#define MCF5272_USB_EP0CTL_CFG_RAM_VAL		0x00000400
#define MCF5272_USB_EP0CTL_CMD_ERR			0x00000200
#define MCF5272_USB_EP0CTL_CMD_OVER			0x00000100
#define MCF5272_USB_EP0CTL_CRC_ERR			0x00000080
#define MCF5272_USB_EP0CTL_OUT_LVL(x)		((x & 3) << 4)
#define MCF5272_USB_EP0CTL_IN_LVL(x)		((x & 3) << 2)
#define MCF5272_USB_EP0CTL_IN_BUSY			0x00000002

#define MCF5272_USB_EPNCTL_CRC_ERR			0x0080
#define MCF5272_USB_EPNCTL_ISO_MODE			0x0040
#define MCF5272_USB_EPNCTL_FIFO_LVL(x)		((x & 3) << 2)
#define MCF5272_USB_EPNCTL_IN_BUSY			0x0002
#define MCF5272_USB_EPNCTL_STALL			0x0001

#define MCF5272_USB_EP0ISR_DEV_CFG			0x00010000
#define MCF5272_USB_EP0ISR_VEND_REQ			0x00008000
#define MCF5272_USB_EP0ISR_FRM_MAT			0x00004000
#define MCF5272_USB_EP0ISR_ASOF				0x00002000
#define MCF5272_USB_EP0ISR_SOF				0x00001000
#define MCF5272_USB_EP0ISR_WAKE_CHG			0x00000800
#define MCF5272_USB_EP0ISR_RESUME			0x00000400
#define MCF5272_USB_EP0ISR_SUSPEND			0x00000200
#define MCF5272_USB_EP0ISR_RESET			0x00000100
#define MCF5272_USB_EP0ISR_OUT_EOT			0x00000080
#define MCF5272_USB_EP0ISR_OUT_EOP			0x00000040
#define MCF5272_USB_EP0ISR_OUT_LVL			0x00000020
#define MCF5272_USB_EP0ISR_IN_EOT			0x00000010
#define MCF5272_USB_EP0ISR_IN_EOP			0x00000008
#define MCF5272_USB_EP0ISR_UNHALT			0x00000004
#define MCF5272_USB_EP0ISR_HALT				0x00000002
#define MCF5272_USB_EP0ISR_IN_LVL			0x00000001

#define MCF5272_USB_EPNISR_HALT_ST			0x8000
#define MCF5272_USB_EPNISR_DIR				0x4000
#define MCF5272_USB_EPNISR_PRES				0x2000
#define MCF5272_USB_EPNISR_EOT				0x0010
#define MCF5272_USB_EPNISR_EOP				0x0008
#define MCF5272_USB_EPNISR_UNHALT			0x0004
#define MCF5272_USB_EPNISR_HALT				0x0002
#define MCF5272_USB_EPNISR_FIFO_LVL			0x0001

#define MCF5272_USB_EP0IMR_DEV_CFG_EN		0x00010000
#define MCF5272_USB_EP0IMR_VEND_REQ_EN		0x00008000
#define MCF5272_USB_EP0IMR_FRM_MAT_EN		0x00004000
#define MCF5272_USB_EP0IMR_ASOF_EN			0x00002000
#define MCF5272_USB_EP0IMR_SOF_EN			0x00001000
#define MCF5272_USB_EP0IMR_WAKE_CHG_EN		0x00000800
#define MCF5272_USB_EP0IMR_RESUME_EN		0x00000400
#define MCF5272_USB_EP0IMR_SUSPEND_EN		0x00000200
#define MCF5272_USB_EP0IMR_RESET_EN			0x00000100
#define MCF5272_USB_EP0IMR_OUT_EOT_EN		0x00000080
#define MCF5272_USB_EP0IMR_OUT_EOP_EN		0x00000040
#define MCF5272_USB_EP0IMR_OUT_LVL_EN		0x00000020
#define MCF5272_USB_EP0IMR_IN_EOT_EN		0x00000010
#define MCF5272_USB_EP0IMR_IN_EOP_EN		0x00000008
#define MCF5272_USB_EP0IMR_UNHALT_EN		0x00000004
#define MCF5272_USB_EP0IMR_HALT_EN			0x00000002
#define MCF5272_USB_EP0IMR_IN_LVL_EN		0x00000001

#define MCF5272_USB_EPNIMR_EOT_EN			0x0010
#define MCF5272_USB_EPNIMR_EOP_EN			0x0008
#define MCF5272_USB_EPNIMR_UNHALT_EN		0x0004
#define MCF5272_USB_EPNIMR_HALT_EN			0x0002
#define MCF5272_USB_EPNIMR_FIFO_LVL_EN		0x0001

/***********************************************************************/

/* 
 * Define a pointer to the MCF5272 Internal Memory Map 
 */

#ifndef ASSEMBLY_ONLY
typedef uint8	MCF5272_IMM;
#endif

/***********************************************************************/

#endif	/* _CPU_MCF5272_H */
