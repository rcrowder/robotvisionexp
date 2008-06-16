#ifndef MX21
#define MX21

#define NULL ((void*)0)

typedef unsigned int UINT;
typedef unsigned short USHORT;
typedef unsigned char UCHAR;
typedef unsigned int UINT32;
typedef unsigned short UINT16;
typedef unsigned int time_t;

// This Is a generated file. Source is an Excel sheet.  DO NOT EDIT		
//############################################################################		
//#                                                                          #		
//#                    Tahiti specific tht_memory_map.equ                    #		
//#                     Motorola Confidential Proprietary                    #		
//# Purpose:                                                                 #		
//#   This file is used by the ARM ADS assembler to decode the memory map    #		
//#                                                                          #		
//# Description                                                              #		
//#   This file list out each individual memory location for decoding        #		
//#                                                                          #		
//# Initial version writen By                                                #		
//#   JM Kam (jmkam@motorola.com) SPS/WBSG SNP                               #		
//#                                                                          #		
//# Date                                                                     #		
//#   13 Jan 2003                                                            #		
//#                                                                          #		
//# Status                                                                   #		
//#   clock rest module: Added sys control registsers temporarily            #		
//#   Chapters Missing: HAB          EMMA DEC  EMMA ENC                      #		
//#                                                                          #		
//# Revisions                                                                #		
//#    Date       By           Description of changes                        #		
//#  20 Nov 2002  JM Kam       Initial release for Tahiti IC Spec 0.3        #		
//#  21 Dec 2002  JM Kam       Changed over to Tahiti-Lite IC Spec 0.1       #		
//#                            - filled in SCM SMN ROMPATCH  SLCDC  AUDMUX   #		
//#                            - renamed EIM to WEIM                         #		
//#                            - updated DMA  CCM  LCDC  EMMA  RTC  WDOG     #		
//#                                      PWM  UART  FIRI  GPIO               #		
//#                            - temp allocated AIPI1 en 22 to AUDMUX        #		
//#  17 Jan 2003  JM Kam       Changed over to Tahiti-Lite IC Spec 0.2       #		
//#                            - Additions for CRM  NFC  BMI  EMMA  SSI      #		
//#                                     PCMCIA GPIO USBOTG BMI SAHARA        #		
//#                            - Split system control regs away from CRM     #		
//#                              - left in CRM reg space and tagged SYS      #		
//#                            Corrections for (from v0.1)                   #		
//#                            -CRM  FIRI  SLCDC  LCDC                       #		
//#                            Confirm allocated AIPI1 en 22 to AUDMUX       #		
//#  14 Feb 2003  JM Kam       Emma memory map updated for end and dec       #		
//#                            Added new registers for DMA module            #		
//#  19 Feb 2003  JM Kam       Added one register for CSI CSIRXCNT           #		
//#  20 Feb 2003  JM Kam       Added LCDC_LIER and LCDC_LGWDCS               #		
//#                                                                          #		
//############################################################################		
//Memory Map		
//----------		
//$0000_0000 - $0000_3FFF BROM     ()		
//$0000_4000 - $0040_3FFF reserved		
//$0040_4000 - $007F_FFFF BROM     ()		
//$0080_0000 - $0FFF_FFFF reserved		
		
//$1000_0000 - $1000_0FFF AIPI1    ()		
//$1000_1000 - $1000_1FFF DMA      ()		
//$1000_2000 - $1000_2FFF WDOG     ()		
//$1000_3000 - $1000_3FFF GPT1     ()		
//$1000_4000 - $1000_4FFF GPT2     ()		
//$1000_5000 - $1000_5FFF GPT3     ()		
//$1000_6000 - $1000_6FFF PWM      ()		
//$1000_7000 - $1000_7FFF RTC      ()		
//$1000_8000 - $1000_8FFF KPP      ()		
//$1000_9000 - $1000_9FFF OWIRE    ()		
//$1000_A000 - $1000_AFFF UART1    ()		
//$1000_B000 - $1000_BFFF UART2    ()		
//$1000_C000 - $1000_CFFF UART3    ()		
//$1000_D000 - $1000_DFFF UART4    ()		
//$1000_E000 - $1000_EFFF CSPI1    ()		
//$1000_F000 - $1000_FFFF CSPI2    ()		
		
//$1001_0000 - $1001_0FFF SSI1     ()		
//$1001_1000 - $1001_1FFF SSI2     ()		
//$1001_2000 - $1001_2FFF I2C      ()		
//$1001_3000 - $1001_3FFF SDHC1    ()		
//$1001_4000 - $1001_4FFF SDHC2    ()		
//$1001_5000 - $1001_5FFF GPIO     ()		
//$1001_6000 - $1001_6FFF AUDMUX   ()		
//$1001_7000 - $1001_7FFF reserved		
//$1001_8000 - $1001_8FFF reserved		
//$1001_9000 - $1001_9FFF reserved		
//$1001_A000 - $1001_AFFF reserved		
//$1001_B000 - $1001_BFFF reserved		
//$1001_C000 - $1001_CFFF reserved		
//$1001_D000 - $1001_DFFF reserved		
//$1001_E000 - $1001_EFFF reserved		
//$1001_F000 - $1001_FFFF reserved		
		
//$1002_0000 - $1002_0FFF AIPI2    ()		
//$1002_1000 - $1002_1FFF LCDC     ()		
//$1002_2000 - $1002_2FFF SLCDC    ()		
		
//$1002_4000 - $1002_4FFF USBOTG   ()		
//$1002_5000 - $1002_5FFF USBOTG   ()		
//$1002_6000 - $1002_6FFF EMMA     ()		
//$1002_7000 - $1002_7FFF CRM and SYS ()		
//$1002_8000 - $1002_8FFF FIRI     ()		
//$1002_9000 - $1002_9FFF reserved		
//$1002_A000 - $1002_AFFF reserved		
//$1002_B000 - $1002_BFFF reserved		
//$1002_C000 - $1002_CFFF reserved		
//$1002_D000 - $1002_DFFF reserved		
//$1002_E000 - $1002_EFFF reserved		
		
//$1003_0000 - $1003_0FFF reserved		
//$1003_1000 - $1003_1FFF reserved		
//$1003_2000 - $1003_2FFF reserved		
//$1003_3000 - $1003_3FFF reserved		
		
//$1003_5000 - $1003_5FFF reserved		
//$1003_6000 - $1003_6FFF reserved		
//$1003_7000 - $1003_7FFF reserved		
//$1003_8000 - $1003_8FFF reserved		
//$1003_9000 - $1003_9FFF reserved		
//$1003_A000 - $1003_AFFF reserved		
//$1003_B000 - $1003_BFFF reserved		
//$1003_C000 - $1003_CFFF reserved		
//$1003_D000 - $1003_DFFF reserved		
//$1003_E000 - $1003_EFFF JAM      ()		
//$1003_F000 - $1003_FFFF MAX      ()		
		
//$1004_0000 - $1004_0FFF AITC     ()		
//$1004_1000 - $1004_1FFF ROMPATCH ()		
//$1004_2000 - $1004_2FFF SMN      ()		
//$1004_3000 - $1004_3FFF SCM      ()		
		
//$1004_4000 - $7FFF_FFFF reserved		
		
//$8000_0000 - $8000_0FFF CSI      ()		
//$8000_1000 - $9FFF_FFFF reserved		
		
//$A000_0000 - $A000_0FFF BMI      ()		
//$A000_1000 - $BFFF_FFFF reserved		
		
//$C000_0000 - $C3FF_FFFF External Memory (CSD0)		
//$C400_0000 - $C7FF_FFFF External Memory (CSD1)		
//$C800_0000 - $CBFF_FFFF External Memory (CS0)		
//$CC00_0000 - $CFFF_FFFF External Memory (CS1)		
//$D000_0000 - $D0FF_FFFF External Memory (CS2)		
//$D100_0000 - $D1FF_FFFF External Memory (CS3)		
//$D200_0000 - $D2FF_FFFF External Memory (CS4)		
//$D300_0000 - $D3FF_FFFF External Memory (CS5)		
		
//$D400_0000 - $D7FF_FFFF External Memory (PCMCIA/CF)		
//$D800_0000 - $DEFF_FFFF reserved		
//$DF00_0000 - $DF00_0FFF SDRAMC		
//$DF00_1000 - $DF00_1FFF WEIM		
//$DF00_2000 - $DF00_2FFF PCMCIA		
//$DF00_3000 - $DF00_3FFF NFC		
//$DF00_4000 - $DFFF_FFFF reserved		
		
//$E000_0000 - $FFFF_FEFF reserved		
//$FFFF_FF00 - $FFFF_FFFF VRAM		
		
//#########################################		
//# BOOT ROM                              #		
//# $0000_0000 to $0000_3FFF              #		
//# $0040_4000 to $007F_FFFF              #		
//#########################################		
#define BOOTROM1_ADDR_BOT	0x00000000	//  boot rom section 1 bottom address
#define BOOTROM1_PHY_SIZE	0x00004000	//  boot rom section 1 physical size
#define BOOTROM1_ASS_SIZE	0x00004000	//  boot rom section 1 assigned size
		
#define BOOTROM2_ADDR_BOT	0x00404000	//  boot rom section 2 bottom address
#define BOOTROM2_PHY_SIZE	0x003F4000	//  boot rom section 2 physical size
#define BOOTROM2_ASS_SIZE	0x003F4000	//  boot rom section 2 assigned size
		
		
		
		
//#########################################		
//# AIPI1                                 #		
//# $1000_0000 to $1000_0FFF              #		
//#########################################		
#define AIPI1_BASE_ADDR	0x10000000	
#define AIPI1_PSR0	(*(volatile UINT *)(AIPI1_BASE_ADDR+0x00))	//  32bit Peripheral Size Reg 0
#define AIPI1_PSR1	(*(volatile UINT *)(AIPI1_BASE_ADDR+0x04))	//  32bit Peripheral Size Reg 1
#define AIPI1_PAR	(*(volatile UINT *)(AIPI1_BASE_ADDR+0x08))	//  32bit Peripheral Access Reg
		
//#########################################		
//# DMA                                   #		
//# $1000_1000 to $1000_1FFF              #		
//#########################################		
#define DMA_BASE_ADDR	0x10001000	
#define DMA_SYS_BASE	(*(volatile UINT *)(DMA_BASE_ADDR+0x000))	//  base location for system
#define DMA_M2D_BASE	(*(volatile UINT *)(DMA_BASE_ADDR+0x040))	//  base location for 2D memory reg
#define DMA_CH0_BASE	(*(volatile UINT *)(DMA_BASE_ADDR+0x080))	//  base location for channel 0
#define DMA_CH1_BASE	(*(volatile UINT *)(DMA_BASE_ADDR+0x0C0))	//  base location for channel 1
#define DMA_CH2_BASE	(*(volatile UINT *)(DMA_BASE_ADDR+0x100))	//  base location for channel 2
#define DMA_CH3_BASE	(*(volatile UINT *)(DMA_BASE_ADDR+0x140))	//  base location for channel 3
#define DMA_CH4_BASE	(*(volatile UINT *)(DMA_BASE_ADDR+0x180))	//  base location for channel 4
#define DMA_CH5_BASE	(*(volatile UINT *)(DMA_BASE_ADDR+0x1C0))	//  base location for channel 5
#define DMA_CH6_BASE	(*(volatile UINT *)(DMA_BASE_ADDR+0x200))	//  base location for channel 6
#define DMA_CH7_BASE	(*(volatile UINT *)(DMA_BASE_ADDR+0x240))	//  base location for channel 7
#define DMA_CH8_BASE	(*(volatile UINT *)(DMA_BASE_ADDR+0x280))	//  base location for channel 8
#define DMA_CH9_BASE	(*(volatile UINT *)(DMA_BASE_ADDR+0x2C0))	//  base location for channel 9
#define DMA_CH10_BASE	(*(volatile UINT *)(DMA_BASE_ADDR+0x300))	//  base location for channel 10
#define DMA_CH11_BASE	(*(volatile UINT *)(DMA_BASE_ADDR+0x340))	//  base location for channel 11
#define DMA_CH12_BASE	(*(volatile UINT *)(DMA_BASE_ADDR+0x380))	//  base location for channel 12
#define DMA_CH13_BASE	(*(volatile UINT *)(DMA_BASE_ADDR+0x3C0))	//  base location for channel 13
#define DMA_CH14_BASE	(*(volatile UINT *)(DMA_BASE_ADDR+0x400))	//  base location for channel 14
#define DMA_CH15_BASE	(*(volatile UINT *)(DMA_BASE_ADDR+0x440))	//  base location for channel 15
#define DMA_TEST_BASE	(*(volatile UINT *)(DMA_BASE_ADDR+0x480))	//  base location for test registers
		
#define DMA_DCR	(*(volatile UINT *)(DMA_SYS_BASE+0x000))	//  32bit dma control reg
#define DMA_DISR	(*(volatile UINT *)(DMA_SYS_BASE+0x004))	//  32bit dma interrupt status reg
#define DMA_DIMR	(*(volatile UINT *)(DMA_SYS_BASE+0x008))	//  32bit dma interrupt mask reg
#define DMA_DBTOSR	(*(volatile UINT *)(DMA_SYS_BASE+0x00C))	//  32bit dma burst timeout stat reg
#define DMA_DRTOSR	(*(volatile UINT *)(DMA_SYS_BASE+0x010))	//  32bit dma req timeout status reg
#define DMA_DSESR	(*(volatile UINT *)(DMA_SYS_BASE+0x014))	//  32bit dma transfer err status reg
#define DMA_DBOSR	(*(volatile UINT *)(DMA_SYS_BASE+0x018))	//  32bit dma buffer overflow stat reg
#define DMA_DBTOCR	(*(volatile UINT *)(DMA_SYS_BASE+0x01C))	//  32bit dma burst timeout ctrl reg
		
#define DMA_WSRA	(*(volatile UINT *)(DMA_M2D_BASE+0x000))	//  32bit dma W-size A reg
#define DMA_XSRA	(*(volatile UINT *)(DMA_M2D_BASE+0x004))	//  32bit dma X-size A reg
#define DMA_YSRA	(*(volatile UINT *)(DMA_M2D_BASE+0x008))	//  32bit dma Y-size A reg
#define DMA_WSRB	(*(volatile UINT *)(DMA_M2D_BASE+0x00C))	//  32bit dma W-size B reg
#define DMA_XSRB	(*(volatile UINT *)(DMA_M2D_BASE+0x010))	//  32bit dma X-size B reg
#define DMA_YSRB	(*(volatile UINT *)(DMA_M2D_BASE+0x014))	//  32bit dma Y-size B reg
		
#define DMA_SAR0	(*(volatile UINT *)(DMA_CH0_BASE+0x000))	//  32bit dma ch0 source addr reg
#define DMA_DAR0	(*(volatile UINT *)(DMA_CH0_BASE+0x004))	//  32bit dma ch0 dest addr reg
#define DMA_CNTR0	(*(volatile UINT *)(DMA_CH0_BASE+0x008))	//  32bit dma ch0 count reg
#define DMA_CCR0	(*(volatile UINT *)(DMA_CH0_BASE+0x00C))	//  32bit dma ch0 control reg
#define DMA_RSSR0	(*(volatile UINT *)(DMA_CH0_BASE+0x010))	//  32bit dma ch0 req source sel reg
#define DMA_BLR0	(*(volatile UINT *)(DMA_CH0_BASE+0x014))	//  32bit dma ch0 burst lenght reg
#define DMA_RTOR0	(*(volatile UINT *)(DMA_CH0_BASE+0x018))	//  32bit dma ch0 req time out reg
#define DMA_BUCR0	(*(volatile UINT *)(DMA_CH0_BASE+0x018))	//  32bit dma ch0 bus utilization reg
#define DMA_CCNR0	(*(volatile UINT *)(DMA_CH0_BASE+0x01C))	//  32bit dma ch0
		
#define DMA_SAR1	(*(volatile UINT *)(DMA_CH1_BASE+0x000))	//  32bit dma ch1 source addr reg
#define DMA_DAR1	(*(volatile UINT *)(DMA_CH1_BASE+0x004))	//  32bit dma ch1 dest addr reg
#define DMA_CNTR1	(*(volatile UINT *)(DMA_CH1_BASE+0x008))	//  32bit dma ch1 count reg
#define DMA_CCR1	(*(volatile UINT *)(DMA_CH1_BASE+0x00C))	//  32bit dma ch1 control reg
#define DMA_RSSR1	(*(volatile UINT *)(DMA_CH1_BASE+0x010))	//  32bit dma ch1 req source sel reg
#define DMA_BLR1	(*(volatile UINT *)(DMA_CH1_BASE+0x014))	//  32bit dma ch1 burst lenght reg
#define DMA_RTOR1	(*(volatile UINT *)(DMA_CH1_BASE+0x018))	//  32bit dma ch1 req time out reg
#define DMA_BUCR1	(*(volatile UINT *)(DMA_CH1_BASE+0x018))	//  32bit dma ch1 bus utilization reg
#define DMA_CCNR1	(*(volatile UINT *)(DMA_CH1_BASE+0x01C))	//  32bit dma ch1
		
#define DMA_SAR2	(*(volatile UINT *)(DMA_CH2_BASE+0x000))	//  32bit dma ch2 source addr reg
#define DMA_DAR2	(*(volatile UINT *)(DMA_CH2_BASE+0x004))	//  32bit dma ch2 dest addr reg
#define DMA_CNTR2	(*(volatile UINT *)(DMA_CH2_BASE+0x008))	//  32bit dma ch2 count reg
#define DMA_CCR2	(*(volatile UINT *)(DMA_CH2_BASE+0x00C))	//  32bit dma ch2 control reg
#define DMA_RSSR2	(*(volatile UINT *)(DMA_CH2_BASE+0x010))	//  32bit dma ch2 req source sel reg
#define DMA_BLR2	(*(volatile UINT *)(DMA_CH2_BASE+0x014))	//  32bit dma ch2 burst lenght reg
#define DMA_RTOR2	(*(volatile UINT *)(DMA_CH2_BASE+0x018))	//  32bit dma ch2 req time out reg
#define DMA_BUCR2	(*(volatile UINT *)(DMA_CH2_BASE+0x018))	//  32bit dma ch2 bus utilization reg
#define DMA_CCNR2	(*(volatile UINT *)(DMA_CH2_BASE+0x01C))	//  32bit dma ch2
		
#define DMA_SAR3	(*(volatile UINT *)(DMA_CH3_BASE+0x000))	//  32bit dma ch3 source addr reg
#define DMA_DAR3	(*(volatile UINT *)(DMA_CH3_BASE+0x004))	//  32bit dma ch3 dest addr reg
#define DMA_CNTR3	(*(volatile UINT *)(DMA_CH3_BASE+0x008))	//  32bit dma ch3 count reg
#define DMA_CCR3	(*(volatile UINT *)(DMA_CH3_BASE+0x00C))	//  32bit dma ch3 control reg
#define DMA_RSSR3	(*(volatile UINT *)(DMA_CH3_BASE+0x010))	//  32bit dma ch3 req source sel reg
#define DMA_BLR3	(*(volatile UINT *)(DMA_CH3_BASE+0x014))	//  32bit dma ch3 burst lenght reg
#define DMA_RTOR3	(*(volatile UINT *)(DMA_CH3_BASE+0x018))	//  32bit dma ch3 req time out reg
#define DMA_BUCR3	(*(volatile UINT *)(DMA_CH3_BASE+0x018))	//  32bit dma ch3 bus utilization reg
#define DMA_CCNR3	(*(volatile UINT *)(DMA_CH3_BASE+0x01C))	//  32bit dma ch3
		
#define DMA_SAR4	(*(volatile UINT *)(DMA_CH4_BASE+0x000))	//  32bit dma ch4 source addr reg
#define DMA_DAR4	(*(volatile UINT *)(DMA_CH4_BASE+0x004))	//  32bit dma ch4 dest addr reg
#define DMA_CNTR4	(*(volatile UINT *)(DMA_CH4_BASE+0x008))	//  32bit dma ch4 count reg
#define DMA_CCR4	(*(volatile UINT *)(DMA_CH4_BASE+0x00C))	//  32bit dma ch4 control reg
#define DMA_RSSR4	(*(volatile UINT *)(DMA_CH4_BASE+0x010))	//  32bit dma ch4 req source sel reg
#define DMA_BLR4	(*(volatile UINT *)(DMA_CH4_BASE+0x014))	//  32bit dma ch4 burst lenght reg
#define DMA_RTOR4	(*(volatile UINT *)(DMA_CH4_BASE+0x018))	//  32bit dma ch4 req time out reg
#define DMA_BUCR4	(*(volatile UINT *)(DMA_CH4_BASE+0x018))	//  32bit dma ch4 bus utilization reg
#define DMA_CCNR4	(*(volatile UINT *)(DMA_CH4_BASE+0x01C))	//  32bit dma ch4
		
#define DMA_SAR5	(*(volatile UINT *)(DMA_CH5_BASE+0x000))	//  32bit dma ch5 source addr reg
#define DMA_DAR5	(*(volatile UINT *)(DMA_CH5_BASE+0x004))	//  32bit dma ch5 dest addr reg
#define DMA_CNTR5	(*(volatile UINT *)(DMA_CH5_BASE+0x008))	//  32bit dma ch5 count reg
#define DMA_CCR5	(*(volatile UINT *)(DMA_CH5_BASE+0x00C))	//  32bit dma ch5 control reg
#define DMA_RSSR5	(*(volatile UINT *)(DMA_CH5_BASE+0x010))	//  32bit dma ch5 req source sel reg
#define DMA_BLR5	(*(volatile UINT *)(DMA_CH5_BASE+0x014))	//  32bit dma ch5 burst lenght reg
#define DMA_RTOR5	(*(volatile UINT *)(DMA_CH5_BASE+0x018))	//  32bit dma ch5 req time out reg
#define DMA_BUCR5	(*(volatile UINT *)(DMA_CH5_BASE+0x018))	//  32bit dma ch5 bus utilization reg
#define DMA_CCNR5	(*(volatile UINT *)(DMA_CH5_BASE+0x01C))	//  32bit dma ch5
		
#define DMA_SAR6	(*(volatile UINT *)(DMA_CH6_BASE+0x000))	//  32bit dma ch6 source addr reg
#define DMA_DAR6	(*(volatile UINT *)(DMA_CH6_BASE+0x004))	//  32bit dma ch6 dest addr reg
#define DMA_CNTR6	(*(volatile UINT *)(DMA_CH6_BASE+0x008))	//  32bit dma ch6 count reg
#define DMA_CCR6	(*(volatile UINT *)(DMA_CH6_BASE+0x00C))	//  32bit dma ch6 control reg
#define DMA_RSSR6	(*(volatile UINT *)(DMA_CH6_BASE+0x010))	//  32bit dma ch6 req source sel reg
#define DMA_BLR6	(*(volatile UINT *)(DMA_CH6_BASE+0x014))	//  32bit dma ch6 burst lenght reg
#define DMA_RTOR6	(*(volatile UINT *)(DMA_CH6_BASE+0x018))	//  32bit dma ch6 req time out reg
#define DMA_BUCR6	(*(volatile UINT *)(DMA_CH6_BASE+0x018))	//  32bit dma ch6 bus utilization reg
#define DMA_CCNR6	(*(volatile UINT *)(DMA_CH6_BASE+0x01C))	//  32bit dma ch6
		
#define DMA_SAR7	(*(volatile UINT *)(DMA_CH7_BASE+0x000))	//  32bit dma ch7 source addr reg
#define DMA_DAR7	(*(volatile UINT *)(DMA_CH7_BASE+0x004))	//  32bit dma ch7 dest addr reg
#define DMA_CNTR7	(*(volatile UINT *)(DMA_CH7_BASE+0x008))	//  32bit dma ch7 count reg
#define DMA_CCR7	(*(volatile UINT *)(DMA_CH7_BASE+0x00C))	//  32bit dma ch7 control reg
#define DMA_RSSR7	(*(volatile UINT *)(DMA_CH7_BASE+0x010))	//  32bit dma ch7 req source sel reg
#define DMA_BLR7	(*(volatile UINT *)(DMA_CH7_BASE+0x014))	//  32bit dma ch7 burst lenght reg
#define DMA_RTOR7	(*(volatile UINT *)(DMA_CH7_BASE+0x018))	//  32bit dma ch7 req time out reg
#define DMA_BUCR7	(*(volatile UINT *)(DMA_CH7_BASE+0x018))	//  32bit dma ch7 bus utilization reg
#define DMA_CCNR7	(*(volatile UINT *)(DMA_CH7_BASE+0x01C))	//  32bit dma ch7
		
#define DMA_SAR8	(*(volatile UINT *)(DMA_CH8_BASE+0x000))	//  32bit dma ch8 source addr reg
#define DMA_DAR8	(*(volatile UINT *)(DMA_CH8_BASE+0x004))	//  32bit dma ch8 dest addr reg
#define DMA_CNTR8	(*(volatile UINT *)(DMA_CH8_BASE+0x008))	//  32bit dma ch8 count reg
#define DMA_CCR8	(*(volatile UINT *)(DMA_CH8_BASE+0x00C))	//  32bit dma ch8 control reg
#define DMA_RSSR8	(*(volatile UINT *)(DMA_CH8_BASE+0x010))	//  32bit dma ch8 req source sel reg
#define DMA_BLR8	(*(volatile UINT *)(DMA_CH8_BASE+0x014))	//  32bit dma ch8 burst lenght reg
#define DMA_RTOR8	(*(volatile UINT *)(DMA_CH8_BASE+0x018))	//  32bit dma ch8 req time out reg
#define DMA_BUCR8	(*(volatile UINT *)(DMA_CH8_BASE+0x018))	//  32bit dma ch8 bus utilization reg
#define DMA_CCNR8	(*(volatile UINT *)(DMA_CH8_BASE+0x01C))	//  32bit dma ch8
		
#define DMA_SAR9	(*(volatile UINT *)(DMA_CH9_BASE+0x000))	//  32bit dma ch9 source addr reg
#define DMA_DAR9	(*(volatile UINT *)(DMA_CH9_BASE+0x004))	//  32bit dma ch9 dest addr reg
#define DMA_CNTR9	(*(volatile UINT *)(DMA_CH9_BASE+0x008))	//  32bit dma ch9 count reg
#define DMA_CCR9	(*(volatile UINT *)(DMA_CH9_BASE+0x00C))	//  32bit dma ch9 control reg
#define DMA_RSSR9	(*(volatile UINT *)(DMA_CH9_BASE+0x010))	//  32bit dma ch9 req source sel reg
#define DMA_BLR9	(*(volatile UINT *)(DMA_CH9_BASE+0x014))	//  32bit dma ch9 burst lenght reg
#define DMA_RTOR9	(*(volatile UINT *)(DMA_CH9_BASE+0x018))	//  32bit dma ch9 req time out reg
#define DMA_BUCR9	(*(volatile UINT *)(DMA_CH9_BASE+0x018))	//  32bit dma ch9 bus utilization reg
#define DMA_CCNR9	(*(volatile UINT *)(DMA_CH9_BASE+0x01C))	//  32bit dma ch9
		
#define DMA_SAR10	(*(volatile UINT *)(DMA_CH10_BASE+0x000))	//  32bit dma ch10 source addr reg
#define DMA_DAR10	(*(volatile UINT *)(DMA_CH10_BASE+0x004))	//  32bit dma ch10 dest addr reg
#define DMA_CNTR10	(*(volatile UINT *)(DMA_CH10_BASE+0x008))	//  32bit dma ch10 count reg
#define DMA_CCR10	(*(volatile UINT *)(DMA_CH10_BASE+0x00C))	//  32bit dma ch10 control reg
#define DMA_RSSR10	(*(volatile UINT *)(DMA_CH10_BASE+0x010))	//  32bit dma ch10 req source sel reg
#define DMA_BLR10	(*(volatile UINT *)(DMA_CH10_BASE+0x014))	//  32bit dma ch10 burst lenght reg
#define DMA_RTOR10	(*(volatile UINT *)(DMA_CH10_BASE+0x018))	//  32bit dma ch10 req time out reg
#define DMA_BUCR10	(*(volatile UINT *)(DMA_CH10_BASE+0x018))	//  32bit dma ch10 bus utilization reg
#define DMA_CCNR10	(*(volatile UINT *)(DMA_CH10_BASE+0x01C))	//  32bit dma ch10
		
#define DMA_SAR11	(*(volatile UINT *)(DMA_CH11_BASE+0x000))	//  32bit dma ch11 source addr reg
#define DMA_DAR11	(*(volatile UINT *)(DMA_CH11_BASE+0x004))	//  32bit dma ch11 dest addr reg
#define DMA_CNTR11	(*(volatile UINT *)(DMA_CH11_BASE+0x008))	//  32bit dma ch11 count reg
#define DMA_CCR11	(*(volatile UINT *)(DMA_CH11_BASE+0x00C))	//  32bit dma ch11 control reg
#define DMA_RSSR11	(*(volatile UINT *)(DMA_CH11_BASE+0x010))	//  32bit dma ch11 req source sel reg
#define DMA_BLR11	(*(volatile UINT *)(DMA_CH11_BASE+0x014))	//  32bit dma ch11 burst lenght reg
#define DMA_RTOR11	(*(volatile UINT *)(DMA_CH11_BASE+0x018))	//  32bit dma ch11 req time out reg
#define DMA_BUCR11	(*(volatile UINT *)(DMA_CH11_BASE+0x018))	//  32bit dma ch11 bus utilization reg
#define DMA_CCNR11	(*(volatile UINT *)(DMA_CH11_BASE+0x01C))	//  32bit dma ch11
		
#define DMA_SAR12	(*(volatile UINT *)(DMA_CH12_BASE+0x000))	//  32bit dma ch12 source addr reg
#define DMA_DAR12	(*(volatile UINT *)(DMA_CH12_BASE+0x004))	//  32bit dma ch12 dest addr reg
#define DMA_CNTR12	(*(volatile UINT *)(DMA_CH12_BASE+0x008))	//  32bit dma ch12 count reg
#define DMA_CCR12	(*(volatile UINT *)(DMA_CH12_BASE+0x00C))	//  32bit dma ch12 control reg
#define DMA_RSSR12	(*(volatile UINT *)(DMA_CH12_BASE+0x010))	//  32bit dma ch12 req source sel reg
#define DMA_BLR12	(*(volatile UINT *)(DMA_CH12_BASE+0x014))	//  32bit dma ch12 burst lenght reg
#define DMA_RTOR12	(*(volatile UINT *)(DMA_CH12_BASE+0x018))	//  32bit dma ch12 req time out reg
#define DMA_BUCR12	(*(volatile UINT *)(DMA_CH12_BASE+0x018))	//  32bit dma ch12 bus utilization reg
#define DMA_CCNR12	(*(volatile UINT *)(DMA_CH12_BASE+0x01C))	//  32bit dma ch12
		
#define DMA_SAR13	(*(volatile UINT *)(DMA_CH13_BASE+0x000))	//  32bit dma ch13 source addr reg
#define DMA_DAR13	(*(volatile UINT *)(DMA_CH13_BASE+0x004))	//  32bit dma ch13 dest addr reg
#define DMA_CNTR13	(*(volatile UINT *)(DMA_CH13_BASE+0x008))	//  32bit dma ch13 count reg
#define DMA_CCR13	(*(volatile UINT *)(DMA_CH13_BASE+0x00C))	//  32bit dma ch13 control reg
#define DMA_RSSR13	(*(volatile UINT *)(DMA_CH13_BASE+0x010))	//  32bit dma ch13 req source sel reg
#define DMA_BLR13	(*(volatile UINT *)(DMA_CH13_BASE+0x014))	//  32bit dma ch13 burst lenght reg
#define DMA_RTOR13	(*(volatile UINT *)(DMA_CH13_BASE+0x018))	//  32bit dma ch13 req time out reg
#define DMA_BUCR13	(*(volatile UINT *)(DMA_CH13_BASE+0x018))	//  32bit dma ch13 bus utilization reg
#define DMA_CCNR13	(*(volatile UINT *)(DMA_CH13_BASE+0x01C))	//  32bit dma ch13
		
#define DMA_SAR14	(*(volatile UINT *)(DMA_CH14_BASE+0x000))	//  32bit dma ch14 source addr reg
#define DMA_DAR14	(*(volatile UINT *)(DMA_CH14_BASE+0x004))	//  32bit dma ch14 dest addr reg
#define DMA_CNTR14	(*(volatile UINT *)(DMA_CH14_BASE+0x008))	//  32bit dma ch14 count reg
#define DMA_CCR14	(*(volatile UINT *)(DMA_CH14_BASE+0x00C))	//  32bit dma ch14 control reg
#define DMA_RSSR14	(*(volatile UINT *)(DMA_CH14_BASE+0x010))	//  32bit dma ch14 req source sel reg
#define DMA_BLR14	(*(volatile UINT *)(DMA_CH14_BASE+0x014))	//  32bit dma ch14 burst lenght reg
#define DMA_RTOR14	(*(volatile UINT *)(DMA_CH14_BASE+0x018))	//  32bit dma ch14 req time out reg
#define DMA_BUCR14	(*(volatile UINT *)(DMA_CH14_BASE+0x018))	//  32bit dma ch14 bus utilization reg
#define DMA_CCNR14	(*(volatile UINT *)(DMA_CH14_BASE+0x01C))	//  32bit dma ch14
		
#define DMA_SAR15	(*(volatile UINT *)(DMA_CH15_BASE+0x000))	//  32bit dma ch15 source addr reg
#define DMA_DAR15	(*(volatile UINT *)(DMA_CH15_BASE+0x004))	//  32bit dma ch15 dest addr reg
#define DMA_CNTR15	(*(volatile UINT *)(DMA_CH15_BASE+0x008))	//  32bit dma ch15 count reg
#define DMA_CCR15	(*(volatile UINT *)(DMA_CH15_BASE+0x00C))	//  32bit dma ch15 control reg
#define DMA_RSSR15	(*(volatile UINT *)(DMA_CH15_BASE+0x010))	//  32bit dma ch15 req source sel reg
#define DMA_BLR15	(*(volatile UINT *)(DMA_CH15_BASE+0x014))	//  32bit dma ch15 burst lenght reg
#define DMA_RTOR15	(*(volatile UINT *)(DMA_CH15_BASE+0x018))	//  32bit dma ch15 req time out reg
#define DMA_BUCR15	(*(volatile UINT *)(DMA_CH15_BASE+0x018))	//  32bit dma ch15 bus utilization reg
#define DMA_CCNR15	(*(volatile UINT *)(DMA_CH15_BASE+0x01C))	//  32bit dma ch15
		
#define DMA_TCR	(*(volatile UINT *)(DMA_TEST_BASE+0x000))	//  32bit dma test control reg
#define DMA_TFIFOAR	(*(volatile UINT *)(DMA_TEST_BASE+0x004))	//  32bit dma test fifo A reg
#define DMA_TDRR	(*(volatile UINT *)(DMA_TEST_BASE+0x008))	//  32bit dma test request reg
#define DMA_TDIPR	(*(volatile UINT *)(DMA_TEST_BASE+0x00C))	//  32bit dma test in progress reg
#define DMA_TFIFOBR	(*(volatile UINT *)(DMA_TEST_BASE+0x010))	//  32bit dma test fifo B reg
		
//#########################################		
//# WDOG                                  #		
//# $1000_2000 to $1000_2FFF              #		
//#########################################		
#define WDOG_BASE_ADDR	0x10002000	
#define WDOG_WCR	(*(volatile UINT *)(WDOG_BASE_ADDR+0x00))	//  16bit watchdog control reg
#define WDOG_WSR	(*(volatile UINT *)(WDOG_BASE_ADDR+0x02))	//  16bit watchdog service reg
#define WDOG_WRSR	(*(volatile UINT *)(WDOG_BASE_ADDR+0x04))	//  16bit watchdog reset status reg
#define WDOG_WPR	(*(volatile UINT *)(WDOG_BASE_ADDR+0x06))	//  16bit watchdog protect reg
		
//#########################################		
//# GPT1                                  #		
//# $1000_3000 to $1000_3FFF              #		
//#########################################		
#define GPT1_BASE_ADDR	0x10003000	
#define GPT1_TCTL1	(*(volatile UINT *)(GPT1_BASE_ADDR+0x00))	//  32bit timer 1 control reg
#define GPT1_TPRER1	(*(volatile UINT *)(GPT1_BASE_ADDR+0x04))	//  32bit timer 1 prescaler reg
#define GPT1_TCMP1	(*(volatile UINT *)(GPT1_BASE_ADDR+0x08))	//  32bit timer 1 compare reg
#define GPT1_TCR1	(*(volatile UINT *)(GPT1_BASE_ADDR+0x0C))	//  32bit timer 1 capture reg
#define GPT1_TCN1	(*(volatile UINT *)(GPT1_BASE_ADDR+0x10))	//  32bit timer 1 counter reg
#define GPT1_TSTAT1	(*(volatile UINT *)(GPT1_BASE_ADDR+0x14))	//  32bit timer 1 status reg
		
//#########################################		
//# GPT2                                  #		
//# $1000_4000 to $1000_4FFF              #		
//#########################################		
#define GPT2_BASE_ADDR	0x10004000	
#define GPT2_TCTL2	(*(volatile UINT *)(GPT2_BASE_ADDR+0x00))	//  32bit timer 2 control reg
#define GPT2_TPRER2	(*(volatile UINT *)(GPT2_BASE_ADDR+0x04))	//  32bit timer 2 prescaler reg
#define GPT2_TCMP2	(*(volatile UINT *)(GPT2_BASE_ADDR+0x08))	//  32bit timer 2 compare reg
#define GPT2_TCR2	(*(volatile UINT *)(GPT2_BASE_ADDR+0x0C))	//  32bit timer 2 capture reg
#define GPT2_TCN2	(*(volatile UINT *)(GPT2_BASE_ADDR+0x10))	//  32bit timer 2 counter reg
#define GPT2_TSTAT2	(*(volatile UINT *)(GPT2_BASE_ADDR+0x14))	//  32bit timer 2 status reg
		
//#########################################		
//# GPT3                                  #		
//# $1000_5000 to $1000_5FFF              #		
//#########################################		
#define GPT3_BASE_ADDR	0x10005000	
#define GPT3_TCTL3	(*(volatile UINT *)(GPT3_BASE_ADDR+0x00))	//  32bit timer 3 control reg
#define GPT3_TPRER3	(*(volatile UINT *)(GPT3_BASE_ADDR+0x04))	//  32bit timer 3 prescaler reg
#define GPT3_TCMP3	(*(volatile UINT *)(GPT3_BASE_ADDR+0x08))	//  32bit timer 3 compare reg
#define GPT3_TCR3	(*(volatile UINT *)(GPT3_BASE_ADDR+0x0C))	//  32bit timer 3 capture reg
#define GPT3_TCN3	(*(volatile UINT *)(GPT3_BASE_ADDR+0x10))	//  32bit timer 3 counter reg
#define GPT3_TSTAT3	(*(volatile UINT *)(GPT3_BASE_ADDR+0x14))	//  32bit timer 3 status reg
		
//#########################################		
//# PWM                                   #		
//# $1000_6000 to $1000_6FFF              #		
//#########################################		
#define PWM_BASE_ADDR	0x10006000	
#define PWM_PWMC	(*(volatile UINT *)(PWM_BASE_ADDR+0x00))	//  32bit pwm control reg
#define PWM_PWMS	(*(volatile UINT *)(PWM_BASE_ADDR+0x04))	//  32bit pwm sample reg
#define PWM_PWMP	(*(volatile UINT *)(PWM_BASE_ADDR+0x08))	//  32bit pwm period reg
#define PWM_PWMCNT	(*(volatile UINT *)(PWM_BASE_ADDR+0x0C))	//  32bit pwm counter reg
#define PWM_PWMTEST1	(*(volatile UINT *)(PWM_BASE_ADDR+0x10))	//  32bit pwm test reg
		
//#########################################		
//# RTC                                   #		
//# $1000_7000 to $1000_7FFF              #		
//#########################################		
#define RTC_BASE_ADDR	0x10007000	
#define RTC_HOURMIN	(*(volatile UINT *)(RTC_BASE_ADDR+0x00))	//  32bit rtc hour/min counter reg
#define RTC_SECOND	(*(volatile UINT *)(RTC_BASE_ADDR+0x04))	//  32bit rtc seconds counter reg
#define RTC_ALRM_HM	(*(volatile UINT *)(RTC_BASE_ADDR+0x08))	//  32bit rtc alarm hour/min reg
#define RTC_ALRM_SEC	(*(volatile UINT *)(RTC_BASE_ADDR+0x0C))	//  32bit rtc alarm seconds reg
#define RTC_RTCCTL	(*(volatile UINT *)(RTC_BASE_ADDR+0x10))	//  32bit rtc control reg
#define RTC_RTCISR	(*(volatile UINT *)(RTC_BASE_ADDR+0x14))	//  32bit rtc interrupt status reg
#define RTC_RTCIENR	(*(volatile UINT *)(RTC_BASE_ADDR+0x18))	//  32bit rtc interrupt enable reg
#define RTC_STPWCH	(*(volatile UINT *)(RTC_BASE_ADDR+0x1C))	//  32bit rtc stopwatch min reg
#define RTC_DAYR	(*(volatile UINT *)(RTC_BASE_ADDR+0x20))	//  32bit rtc days counter reg
#define RTC_DAYALARM	(*(volatile UINT *)(RTC_BASE_ADDR+0x24))	//  32bit rtc day alarm reg
#define RTC_TEST1	(*(volatile UINT *)(RTC_BASE_ADDR+0x28))	//  32bit rtc test reg 1
#define RTC_TEST2	(*(volatile UINT *)(RTC_BASE_ADDR+0x2C))	//  32bit rtc test reg 2
#define RTC_TEST3	(*(volatile UINT *)(RTC_BASE_ADDR+0x30))	//  32bit rtc test reg 3
		
//#########################################		
//# KPP                                   #		
//# $1000_8000 to $1000_8FFF              #		
//#########################################		
#define KPP_BASE_ADDR	0x10008000	
#define KPP_KPCR	(*(volatile UINT *)(KPP_BASE_ADDR+0x00))	//  16bit kpp keypad control reg
#define KPP_KPSR	(*(volatile UINT *)(KPP_BASE_ADDR+0x02))	//  16bit kpp keypad status reg
#define KPP_KDDR	(*(volatile UINT *)(KPP_BASE_ADDR+0x04))	//  16bit kpp keypad data directon reg
#define KPP_KPDR	(*(volatile UINT *)(KPP_BASE_ADDR+0x06))	//  16bit kpp keypad data reg
		
//#########################################		
//# OWIRE                                 #		
//# $1000_9000 to $1000_9FFF              #		
//#########################################		
#define OWIRE_BASE_ADDR	0x10009000	
#define OWIRE_CTRL	(*(volatile UINT *)(OWIRE_BASE_ADDR+0x00))	//  16bit owire control reg
#define OWIRE_TIME_DIV	(*(volatile UINT *)(OWIRE_BASE_ADDR+0x02))	//  16bit owire time divider reg
#define OWIRE_RESET	(*(volatile UINT *)(OWIRE_BASE_ADDR+0x04))	//  16bit owire reset reg
		
//#########################################		
//# UART1                                 #		
//# $1000_A000 to $1000_AFFF              #		
//#########################################		
#define UART1_BASE_ADDR	0x1000A000	
#define UART1_URXD	(*(volatile UINT *)(UART1_BASE_ADDR+0x00))	//  32bit uart1 receiver reg
#define UART1_UTXD	(*(volatile UINT *)(UART1_BASE_ADDR+0x40))	//  32bit uart1 transmitter reg
#define UART1_UCR1	(*(volatile UINT *)(UART1_BASE_ADDR+0x80))	//  32bit uart1 control 1 reg
#define UART1_UCR2	(*(volatile UINT *)(UART1_BASE_ADDR+0x84))	//  32bit uart1 control 2 reg
#define UART1_UCR3	(*(volatile UINT *)(UART1_BASE_ADDR+0x88))	//  32bit uart1 control 3 reg
#define UART1_UCR4	(*(volatile UINT *)(UART1_BASE_ADDR+0x8C))	//  32bit uart1 control 4 reg
#define UART1_UFCR	(*(volatile UINT *)(UART1_BASE_ADDR+0x90))	//  32bit uart1 fifo control reg
#define UART1_USR1	(*(volatile UINT *)(UART1_BASE_ADDR+0x94))	//  32bit uart1 status 1 reg
#define UART1_USR2	(*(volatile UINT *)(UART1_BASE_ADDR+0x98))	//  32bit uart1 status 2 reg
#define UART1_UESC	(*(volatile UINT *)(UART1_BASE_ADDR+0x9C))	//  32bit uart1 escape char reg
#define UART1_UTIM	(*(volatile UINT *)(UART1_BASE_ADDR+0xA0))	//  32bit uart1 escape timer reg
#define UART1_UBIR	(*(volatile UINT *)(UART1_BASE_ADDR+0xA4))	//  32bit uart1 BRM incremental reg
#define UART1_UBMR	(*(volatile UINT *)(UART1_BASE_ADDR+0xA8))	//  32bit uart1 BRM modulator reg
#define UART1_UBRC	(*(volatile UINT *)(UART1_BASE_ADDR+0xAC))	//  32bit uart1 baud rate count reg
#define UART1_ONEMS	(*(volatile UINT *)(UART1_BASE_ADDR+0xB0))	//  32bit uart1 one ms reg
#define UART1_UTS	(*(volatile UINT *)(UART1_BASE_ADDR+0xB4))	//  32bit uart1 test reg
		
//#########################################		
//# UART2                                 #		
//# $1000_B000 to $1000_BFFF              #		
//#########################################		
#define UART2_BASE_ADDR	0x1000B000	
#define UART2_URXD	(*(volatile UINT *)(UART2_BASE_ADDR+0x00))	//  32bit uart2 receiver reg
#define UART2_UTXD	(*(volatile UINT *)(UART2_BASE_ADDR+0x40))	//  32bit uart2 transmitter reg
#define UART2_UCR1	(*(volatile UINT *)(UART2_BASE_ADDR+0x80))	//  32bit uart2 control 1 reg
#define UART2_UCR2	(*(volatile UINT *)(UART2_BASE_ADDR+0x84))	//  32bit uart2 control 2 reg
#define UART2_UCR3	(*(volatile UINT *)(UART2_BASE_ADDR+0x88))	//  32bit uart2 control 3 reg
#define UART2_UCR4	(*(volatile UINT *)(UART2_BASE_ADDR+0x8C))	//  32bit uart2 control 4 reg
#define UART2_UFCR	(*(volatile UINT *)(UART2_BASE_ADDR+0x90))	//  32bit uart2 fifo control reg
#define UART2_USR1	(*(volatile UINT *)(UART2_BASE_ADDR+0x94))	//  32bit uart2 status 1 reg
#define UART2_USR2	(*(volatile UINT *)(UART2_BASE_ADDR+0x98))	//  32bit uart2 status 2 reg
#define UART2_UESC	(*(volatile UINT *)(UART2_BASE_ADDR+0x9C))	//  32bit uart2 escape char reg
#define UART2_UTIM	(*(volatile UINT *)(UART2_BASE_ADDR+0xA0))	//  32bit uart2 escape timer reg
#define UART2_UBIR	(*(volatile UINT *)(UART2_BASE_ADDR+0xA4))	//  32bit uart2 BRM incremental reg
#define UART2_UBMR	(*(volatile UINT *)(UART2_BASE_ADDR+0xA8))	//  32bit uart2 BRM modulator reg
#define UART2_UBRC	(*(volatile UINT *)(UART2_BASE_ADDR+0xAC))	//  32bit uart2 baud rate count reg
#define UART2_ONEMS	(*(volatile UINT *)(UART2_BASE_ADDR+0xB0))	//  32bit uart2 one ms reg
#define UART2_UTS	(*(volatile UINT *)(UART2_BASE_ADDR+0xB4))	//  32bit uart2 test reg
		
//#########################################		
//# UART3                                 #		
//# $1000_C000 to $1000_CFFF              #		
//#########################################		
#define UART3_BASE_ADDR	0x1000C000	
#define UART3_URXD	(*(volatile UINT *)(UART3_BASE_ADDR+0x00))	//  32bit uart3 receiver reg
#define UART3_UTXD	(*(volatile UINT *)(UART3_BASE_ADDR+0x40))	//  32bit uart3 transmitter reg
#define UART3_UCR1	(*(volatile UINT *)(UART3_BASE_ADDR+0x80))	//  32bit uart3 control 1 reg
#define UART3_UCR2	(*(volatile UINT *)(UART3_BASE_ADDR+0x84))	//  32bit uart3 control 2 reg
#define UART3_UCR3	(*(volatile UINT *)(UART3_BASE_ADDR+0x88))	//  32bit uart3 control 3 reg
#define UART3_UCR4	(*(volatile UINT *)(UART3_BASE_ADDR+0x8C))	//  32bit uart3 control 4 reg
#define UART3_UFCR	(*(volatile UINT *)(UART3_BASE_ADDR+0x90))	//  32bit uart3 fifo control reg
#define UART3_USR1	(*(volatile UINT *)(UART3_BASE_ADDR+0x94))	//  32bit uart3 status 1 reg
#define UART3_USR2	(*(volatile UINT *)(UART3_BASE_ADDR+0x98))	//  32bit uart3 status 2 reg
#define UART3_UESC	(*(volatile UINT *)(UART3_BASE_ADDR+0x9C))	//  32bit uart3 escape char reg
#define UART3_UTIM	(*(volatile UINT *)(UART3_BASE_ADDR+0xA0))	//  32bit uart3 escape timer reg
#define UART3_UBIR	(*(volatile UINT *)(UART3_BASE_ADDR+0xA4))	//  32bit uart3 BRM incremental reg
#define UART3_UBMR	(*(volatile UINT *)(UART3_BASE_ADDR+0xA8))	//  32bit uart3 BRM modulator reg
#define UART3_UBRC	(*(volatile UINT *)(UART3_BASE_ADDR+0xAC))	//  32bit uart3 baud rate count reg
#define UART3_ONEMS	(*(volatile UINT *)(UART3_BASE_ADDR+0xB0))	//  32bit uart3 one ms reg
#define UART3_UTS	(*(volatile UINT *)(UART3_BASE_ADDR+0xB4))	//  32bit uart3 test reg
		
//#########################################		
//# UART4                                 #		
//# $1000_D000 to $1000_DFFF              #		
//#########################################		
#define UART4_BASE_ADDR	0x1000D000	
#define UART4_URXD	(*(volatile UINT *)(UART4_BASE_ADDR+0x00))	//  32bit uart4 receiver reg
#define UART4_UTXD	(*(volatile UINT *)(UART4_BASE_ADDR+0x40))	//  32bit uart4 transmitter reg
#define UART4_UCR1	(*(volatile UINT *)(UART4_BASE_ADDR+0x80))	//  32bit uart4 control 1 reg
#define UART4_UCR2	(*(volatile UINT *)(UART4_BASE_ADDR+0x84))	//  32bit uart4 control 2 reg
#define UART4_UCR3	(*(volatile UINT *)(UART4_BASE_ADDR+0x88))	//  32bit uart4 control 3 reg
#define UART4_UCR4	(*(volatile UINT *)(UART4_BASE_ADDR+0x8C))	//  32bit uart4 control 4 reg
#define UART4_UFCR	(*(volatile UINT *)(UART4_BASE_ADDR+0x90))	//  32bit uart4 fifo control reg
#define UART4_USR1	(*(volatile UINT *)(UART4_BASE_ADDR+0x94))	//  32bit uart4 status 1 reg
#define UART4_USR2	(*(volatile UINT *)(UART4_BASE_ADDR+0x98))	//  32bit uart4 status 2 reg
#define UART4_UESC	(*(volatile UINT *)(UART4_BASE_ADDR+0x9C))	//  32bit uart4 escape char reg
#define UART4_UTIM	(*(volatile UINT *)(UART4_BASE_ADDR+0xA0))	//  32bit uart4 escape timer reg
#define UART4_UBIR	(*(volatile UINT *)(UART4_BASE_ADDR+0xA4))	//  32bit uart4 BRM incremental reg
#define UART4_UBMR	(*(volatile UINT *)(UART4_BASE_ADDR+0xA8))	//  32bit uart4 BRM modulator reg
#define UART4_UBRC	(*(volatile UINT *)(UART4_BASE_ADDR+0xAC))	//  32bit uart4 baud rate count reg
#define UART4_ONEMS	(*(volatile UINT *)(UART4_BASE_ADDR+0xB0))	//  32bit uart4 one ms reg
#define UART4_UTS	(*(volatile UINT *)(UART4_BASE_ADDR+0xB4))	//  32bit uart4 test reg
		
//#########################################		
//# CSPI1                                 #		
//# $1000_E000 to $1000_EFFF              #		
//#########################################		
#define CSPI1_BASE_ADDR	0x1000E000	
#define CSPI1_RXDATAREG1	(*(volatile UINT *)(CSPI1_BASE_ADDR+0x00))	//  32bit cspi1 receive data reg
#define CSPI1_TXDATAREG1	(*(volatile UINT *)(CSPI1_BASE_ADDR+0x04))	//  32bit cspi1 transmit data reg
#define CSPI1_CONTROLREG1	(*(volatile UINT *)(CSPI1_BASE_ADDR+0x08))	//  32bit cspi1 control reg
#define CSPI1_INTREG1	(*(volatile UINT *)(CSPI1_BASE_ADDR+0x0C))	//  32bit cspi1 interrupt stat/ctr reg
#define CSPI1_TESTREG1	(*(volatile UINT *)(CSPI1_BASE_ADDR+0x10))	//  32bit cspi1 test reg
#define CSPI1_PERIODREG1	(*(volatile UINT *)(CSPI1_BASE_ADDR+0x14))	//  32bit cspi1 sample period ctrl reg
#define CSPI1_DMAREG1	(*(volatile UINT *)(CSPI1_BASE_ADDR+0x18))	//  32bit cspi1 dma ctrl reg
#define CSPI1_RESETREG1	(*(volatile UINT *)(CSPI1_BASE_ADDR+0x1C))	//  32bit cspi1 soft reset reg
		
//#########################################		
//# CSPI2                                 #		
//# $1000_F000 to $1000_FFFF              #		
//#########################################		
#define CSPI2_BASE_ADDR	0x1000F000	
#define CSPI2_RXDATAREG1	(*(volatile UINT *)(CSPI2_BASE_ADDR+0x00))	//  32bit cspi2 receive data reg
#define CSPI2_TXDATAREG1	(*(volatile UINT *)(CSPI2_BASE_ADDR+0x04))	//  32bit cspi2 transmit data reg
#define CSPI2_CONTROLREG1	(*(volatile UINT *)(CSPI2_BASE_ADDR+0x08))	//  32bit cspi2 control reg
#define CSPI2_INTREG1	(*(volatile UINT *)(CSPI2_BASE_ADDR+0x0C))	//  32bit cspi2 interrupt stat/ctr reg
#define CSPI2_TESTREG1	(*(volatile UINT *)(CSPI2_BASE_ADDR+0x10))	//  32bit cspi2 test reg
#define CSPI2_PERIODREG1	(*(volatile UINT *)(CSPI2_BASE_ADDR+0x14))	//  32bit cspi2 sample period ctrl reg
#define CSPI2_DMAREG1	(*(volatile UINT *)(CSPI2_BASE_ADDR+0x18))	//  32bit cspi2 dma ctrl reg
#define CSPI2_RESETREG1	(*(volatile UINT *)(CSPI2_BASE_ADDR+0x1C))	//  32bit cspi2 soft reset reg
		
		
		
		
		
		
//#########################################		
//# SSI1                                  #		
//# $1001_0000 to $1001_0FFF              #		
//#########################################		
#define SSI1_BASE_ADDR	0x10010000	
#define SSI1_STX0	(*(volatile UINT *)(SSI1_BASE_ADDR+0x00))	//  32bit ssi1 tx reg 0
#define SSI1_STX1	(*(volatile UINT *)(SSI1_BASE_ADDR+0x04))	//  32bit ssi1 tx reg 1
#define SSI1_SRX0	(*(volatile UINT *)(SSI1_BASE_ADDR+0x08))	//  32bit ssi1 rx reg 0
#define SSI1_SRX1	(*(volatile UINT *)(SSI1_BASE_ADDR+0x0C))	//  32bit ssi1 rx reg 1
#define SSI1_SCR	(*(volatile UINT *)(SSI1_BASE_ADDR+0x10))	//  32bit ssi1 control reg
#define SSI1_SISR	(*(volatile UINT *)(SSI1_BASE_ADDR+0x14))	//  32bit ssi1 intr status reg
#define SSI1_SIER	(*(volatile UINT *)(SSI1_BASE_ADDR+0x18))	//  32bit ssi1 intr enable reg
#define SSI1_STCR	(*(volatile UINT *)(SSI1_BASE_ADDR+0x1C))	//  32bit ssi1 tx config reg
#define SSI1_SRCR	(*(volatile UINT *)(SSI1_BASE_ADDR+0x20))	//  32bit ssi1 rx config reg
#define SSI1_STCCR	(*(volatile UINT *)(SSI1_BASE_ADDR+0x24))	//  32bit ssi1 tx clock control reg
#define SSI1_SRCCR	(*(volatile UINT *)(SSI1_BASE_ADDR+0x28))	//  32bit ssi1 rx clock control reg
#define SSI1_SFCSR	(*(volatile UINT *)(SSI1_BASE_ADDR+0x2C))	//  32bit ssi1 fifo control/status reg
#define SSI1_STR	(*(volatile UINT *)(SSI1_BASE_ADDR+0x30))	//  32bit ssi1 test reg
#define SSI1_SOR	(*(volatile UINT *)(SSI1_BASE_ADDR+0x34))	//  32bit ssi1 option reg
#define SSI1_SACNT	(*(volatile UINT *)(SSI1_BASE_ADDR+0x38))	//  32bit ssi1 ac97 control reg
#define SSI1_SACADD	(*(volatile UINT *)(SSI1_BASE_ADDR+0x3C))	//  32bit ssi1 ac97 cmd addr reg
#define SSI1_SACDAT	(*(volatile UINT *)(SSI1_BASE_ADDR+0x40))	//  32bit ssi1 ac97 cmd data reg
#define SSI1_SATAG	(*(volatile UINT *)(SSI1_BASE_ADDR+0x44))	//  32bit ssi1 ac97 tag reg
#define SSI1_STMSK	(*(volatile UINT *)(SSI1_BASE_ADDR+0x48))	//  32bit ssi1 tx time slot mask reg
#define SSI1_SRMSK	(*(volatile UINT *)(SSI1_BASE_ADDR+0x4C))	//  32bit ssi1 rx time slot mask reg
		
		
//#########################################		
//# SSI2                                  #		
//# $1001_1000 to $1001_1FFF              #		
//#########################################		
#define SSI2_BASE_ADDR	0x10011000	
#define SSI2_STX0	(*(volatile UINT *)(SSI2_BASE_ADDR+0x00))	//  32bit ssi2 tx reg 0
#define SSI2_STX1	(*(volatile UINT *)(SSI2_BASE_ADDR+0x04))	//  32bit ssi2 tx reg 1
#define SSI2_SRX0	(*(volatile UINT *)(SSI2_BASE_ADDR+0x08))	//  32bit ssi2 rx reg 0
#define SSI2_SRX1	(*(volatile UINT *)(SSI2_BASE_ADDR+0x0C))	//  32bit ssi2 rx reg 1
#define SSI2_SCR	(*(volatile UINT *)(SSI2_BASE_ADDR+0x10))	//  32bit ssi2 control reg
#define SSI2_SISR	(*(volatile UINT *)(SSI2_BASE_ADDR+0x14))	//  32bit ssi2 intr status reg
#define SSI2_SIER	(*(volatile UINT *)(SSI2_BASE_ADDR+0x18))	//  32bit ssi2 intr enable reg
#define SSI2_STCR	(*(volatile UINT *)(SSI2_BASE_ADDR+0x1C))	//  32bit ssi2 tx config reg
#define SSI2_SRCR	(*(volatile UINT *)(SSI2_BASE_ADDR+0x20))	//  32bit ssi2 rx config reg
#define SSI2_STCCR	(*(volatile UINT *)(SSI2_BASE_ADDR+0x24))	//  32bit ssi2 tx clock control reg
#define SSI2_SRCCR	(*(volatile UINT *)(SSI2_BASE_ADDR+0x28))	//  32bit ssi2 rx clock control reg
#define SSI2_SFCSR	(*(volatile UINT *)(SSI2_BASE_ADDR+0x2C))	//  32bit ssi2 fifo control/status reg
#define SSI2_STR	(*(volatile UINT *)(SSI2_BASE_ADDR+0x30))	//  32bit ssi2 test reg
#define SSI2_SOR	(*(volatile UINT *)(SSI2_BASE_ADDR+0x34))	//  32bit ssi2 option reg
#define SSI2_SACNT	(*(volatile UINT *)(SSI2_BASE_ADDR+0x38))	//  32bit ssi2 ac97 control reg
#define SSI2_SACADD	(*(volatile UINT *)(SSI2_BASE_ADDR+0x3C))	//  32bit ssi2 ac97 cmd addr reg
#define SSI2_SACDAT	(*(volatile UINT *)(SSI2_BASE_ADDR+0x40))	//  32bit ssi2 ac97 cmd data reg
#define SSI2_SATAG	(*(volatile UINT *)(SSI2_BASE_ADDR+0x44))	//  32bit ssi2 ac97 tag reg
#define SSI2_STMSK	(*(volatile UINT *)(SSI2_BASE_ADDR+0x48))	//  32bit ssi2 tx time slot mask reg
#define SSI2_SRMSK	(*(volatile UINT *)(SSI2_BASE_ADDR+0x4C))	//  32bit ssi2 rx time slot mask reg
		
//#########################################		
//# I2C                                   #		
//# $1001_2000 to $1001_2FFF              #		
//#########################################		
#define I2C_BASE_ADDR	0x10012000	
#define I2C_IADR	(*(volatile UINT *)(I2C_BASE_ADDR+0x00))	//  16bit i2c address reg
#define I2C_IFDR	(*(volatile UINT *)(I2C_BASE_ADDR+0x04))	//  16bit i2c frequency divider reg
#define I2C_I2CR	(*(volatile UINT *)(I2C_BASE_ADDR+0x08))	//  16bit i2c control reg
#define I2C_I2SR	(*(volatile UINT *)(I2C_BASE_ADDR+0x0C))	//  16bit i2c status reg
#define I2C_I2DR	(*(volatile UINT *)(I2C_BASE_ADDR+0x10))	//  16bit i2c data i/o reg
		
//#########################################		
//# SDHC1                                 #		
//# $1001_3000 to $1001_3FFF              #		
//#########################################		
#define SDHC1_BASE_ADDR	0x10013000	
#define SDHC1_STR_STP_CLK	(*(volatile UINT *)(SDHC1_BASE_ADDR+0x00))	//  32bit sdhc1 control reg
#define SDHC1_STATUS	(*(volatile UINT *)(SDHC1_BASE_ADDR+0x04))	//  32bit sdhc1 status reg
#define SDHC1_CLK_RATE	(*(volatile UINT *)(SDHC1_BASE_ADDR+0x08))	//  32bit sdhc1 clock rate reg
#define SDHC1_CMD_DAT_CONT	(*(volatile UINT *)(SDHC1_BASE_ADDR+0x0C))	//  32bit sdhc1 cmd/data control reg
#define SDHC1_RESPONSE_TO	(*(volatile UINT *)(SDHC1_BASE_ADDR+0x10))	//  32bit sdhc1 response time out reg
#define SDHC1_READ_TO	(*(volatile UINT *)(SDHC1_BASE_ADDR+0x14))	//  32bit sdhc1 read time out reg
#define SDHC1_BLK_LEN	(*(volatile UINT *)(SDHC1_BASE_ADDR+0x18))	//  32bit sdhc1 block length reg
#define SDHC1_NOB	(*(volatile UINT *)(SDHC1_BASE_ADDR+0x1C))	//  32bit sdhc1 number of blocks reg
#define SDHC1_REV_NO	(*(volatile UINT *)(SDHC1_BASE_ADDR+0x20))	//  32bit sdhc1 revision number reg
#define SDHC1_INT_MASK	(*(volatile UINT *)(SDHC1_BASE_ADDR+0x24))	//  32bit sdhc1 interrupt mask reg
#define SDHC1_CMD	(*(volatile UINT *)(SDHC1_BASE_ADDR+0x28))	//  32bit sdhc1 command code reg
#define SDHC1_ARGH	(*(volatile UINT *)(SDHC1_BASE_ADDR+0x2C))	//  32bit sdhc1 argument high reg
#define SDHC1_ARGL	(*(volatile UINT *)(SDHC1_BASE_ADDR+0x30))	//  32bit sdhc1 argument low reg
#define SDHC1_RES_FIFO	(*(volatile UINT *)(SDHC1_BASE_ADDR+0x34))	//  32bit sdhc1 response fifo reg
#define SDHC1_BUFFER_ACCESS	(*(volatile UINT *)(SDHC1_BASE_ADDR+0x38))	//  32bit sdhc1 buffer access reg
		
//#########################################		
//# SDHC2                                 #		
//# $1001_4000 to $1001_4FFF              #		
//#########################################		
#define SDHC2_BASE_ADDR	0x10014000	
#define SDHC2_STR_STP_CLK	(*(volatile UINT *)(SDHC2_BASE_ADDR+0x00))	//  32bit sdhc2 control reg 
#define SDHC2_STATUS	(*(volatile UINT *)(SDHC2_BASE_ADDR+0x04))	//  32bit sdhc2 status reg
#define SDHC2_CLK_RATE	(*(volatile UINT *)(SDHC2_BASE_ADDR+0x08))	//  32bit sdhc2 clock rate reg
#define SDHC2_CMD_DAT_CONT	(*(volatile UINT *)(SDHC2_BASE_ADDR+0x0C))	//  32bit sdhc2 cmd/data control reg
#define SDHC2_RESPONSE_TO	(*(volatile UINT *)(SDHC2_BASE_ADDR+0x10))	//  32bit sdhc2 response time out reg
#define SDHC2_READ_TO	(*(volatile UINT *)(SDHC2_BASE_ADDR+0x14))	//  32bit sdhc2 read time out reg
#define SDHC2_BLK_LEN	(*(volatile UINT *)(SDHC2_BASE_ADDR+0x18))	//  32bit sdhc2 block length reg
#define SDHC2_NOB	(*(volatile UINT *)(SDHC2_BASE_ADDR+0x1C))	//  32bit sdhc2 number of blocks reg
#define SDHC2_REV_NO	(*(volatile UINT *)(SDHC2_BASE_ADDR+0x20))	//  32bit sdhc2 revision number reg
#define SDHC2_INT_MASK	(*(volatile UINT *)(SDHC2_BASE_ADDR+0x24))	//  32bit sdhc2 interrupt mask reg
#define SDHC2_CMD	(*(volatile UINT *)(SDHC2_BASE_ADDR+0x28))	//  32bit sdhc2 command code reg
#define SDHC2_ARGH	(*(volatile UINT *)(SDHC2_BASE_ADDR+0x2C))	//  32bit sdhc2 argument high reg
#define SDHC2_ARGL	(*(volatile UINT *)(SDHC2_BASE_ADDR+0x30))	//  32bit sdhc2 argument low reg  
#define SDHC2_RES_FIFO	(*(volatile UINT *)(SDHC2_BASE_ADDR+0x34))	//  32bit sdhc2 response fifo reg
#define SDHC2_BUFFER_ACCESS	(*(volatile UINT *)(SDHC2_BASE_ADDR+0x38))	//  32bit sdhc2 buffer access reg
		
//#########################################		
//# GPIO                                  #		
//# $1001_5000 to $1001_5FFF              #		
//#########################################		
#define GPIOA_BASE_ADDR	0x10015000	
#define GPIOA_DDIR	(*(volatile UINT *)(GPIOA_BASE_ADDR+0x00))	//  32bit gpio pta data direction reg
#define GPIOA_OCR1	(*(volatile UINT *)(GPIOA_BASE_ADDR+0x04))	//  32bit gpio pta output config 1 reg
#define GPIOA_OCR2	(*(volatile UINT *)(GPIOA_BASE_ADDR+0x08))	//  32bit gpio pta output config 2 reg
#define GPIOA_ICONFA1	(*(volatile UINT *)(GPIOA_BASE_ADDR+0x0C))	//  32bit gpio pta input config A1 reg
#define GPIOA_ICONFA2	(*(volatile UINT *)(GPIOA_BASE_ADDR+0x10))	//  32bit gpio pta input config A2 reg
#define GPIOA_ICONFB1	(*(volatile UINT *)(GPIOA_BASE_ADDR+0x14))	//  32bit gpio pta input config B1 reg
#define GPIOA_ICONFB2	(*(volatile UINT *)(GPIOA_BASE_ADDR+0x18))	//  32bit gpio pta input config B2 reg
#define GPIOA_DR	(*(volatile UINT *)(GPIOA_BASE_ADDR+0x1C))	//  32bit gpio pta data reg
#define GPIOA_GIUS	(*(volatile UINT *)(GPIOA_BASE_ADDR+0x20))	//  32bit gpio pta in use reg
#define GPIOA_SSR	(*(volatile UINT *)(GPIOA_BASE_ADDR+0x24))	//  32bit gpio pta sample status reg
#define GPIOA_ICR1	(*(volatile UINT *)(GPIOA_BASE_ADDR+0x28))	//  32bit gpio pta interrupt ctrl 1 reg
#define GPIOA_ICR2	(*(volatile UINT *)(GPIOA_BASE_ADDR+0x2C))	//  32bit gpio pta interrupt ctrl 2 reg
#define GPIOA_IMR	(*(volatile UINT *)(GPIOA_BASE_ADDR+0x30))	//  32bit gpio pta interrupt mask reg
#define GPIOA_ISR	(*(volatile UINT *)(GPIOA_BASE_ADDR+0x34))	//  32bit gpio pta interrupt status reg
#define GPIOA_GPR	(*(volatile UINT *)(GPIOA_BASE_ADDR+0x38))	//  32bit gpio pta general purpose reg
#define GPIOA_SWR	(*(volatile UINT *)(GPIOA_BASE_ADDR+0x3C))	//  32bit gpio pta software reset reg
#define GPIOA_PUEN	(*(volatile UINT *)(GPIOA_BASE_ADDR+0x40))	//  32bit gpio pta pull up enable reg
		
#define GPIOB_BASE_ADDR	0x10015100	
#define GPIOB_DDIR	(*(volatile UINT *)(GPIOB_BASE_ADDR+0x00))	//  32bit gpio ptb data direction reg
#define GPIOB_OCR1	(*(volatile UINT *)(GPIOB_BASE_ADDR+0x04))	//  32bit gpio ptb output config 1 reg
#define GPIOB_OCR2	(*(volatile UINT *)(GPIOB_BASE_ADDR+0x08))	//  32bit gpio ptb output config 2 reg
#define GPIOB_ICONFA1	(*(volatile UINT *)(GPIOB_BASE_ADDR+0x0C))	//  32bit gpio ptb input config A1 reg
#define GPIOB_ICONFA2	(*(volatile UINT *)(GPIOB_BASE_ADDR+0x10))	//  32bit gpio ptb input config A2 reg
#define GPIOB_ICONFB1	(*(volatile UINT *)(GPIOB_BASE_ADDR+0x14))	//  32bit gpio ptb input config B1 reg
#define GPIOB_ICONFB2	(*(volatile UINT *)(GPIOB_BASE_ADDR+0x18))	//  32bit gpio ptb input config B2 reg
#define GPIOB_DR	(*(volatile UINT *)(GPIOB_BASE_ADDR+0x1C))	//  32bit gpio ptb data reg
#define GPIOB_GIUS	(*(volatile UINT *)(GPIOB_BASE_ADDR+0x20))	//  32bit gpio ptb in use reg
#define GPIOB_SSR	(*(volatile UINT *)(GPIOB_BASE_ADDR+0x24))	//  32bit gpio ptb sample status reg
#define GPIOB_ICR1	(*(volatile UINT *)(GPIOB_BASE_ADDR+0x28))	//  32bit gpio ptb interrupt ctrl 1 reg
#define GPIOB_ICR2	(*(volatile UINT *)(GPIOB_BASE_ADDR+0x2C))	//  32bit gpio ptb interrupt ctrl 2 reg
#define GPIOB_IMR	(*(volatile UINT *)(GPIOB_BASE_ADDR+0x30))	//  32bit gpio ptb interrupt mask reg
#define GPIOB_ISR	(*(volatile UINT *)(GPIOB_BASE_ADDR+0x34))	//  32bit gpio ptb interrupt status reg
#define GPIOB_GPR	(*(volatile UINT *)(GPIOB_BASE_ADDR+0x38))	//  32bit gpio ptb general purpose reg
#define GPIOB_SWR	(*(volatile UINT *)(GPIOB_BASE_ADDR+0x3C))	//  32bit gpio ptb software reset reg 
#define GPIOB_PUEN	(*(volatile UINT *)(GPIOB_BASE_ADDR+0x40))	//  32bit gpio ptb pull up enable reg
		
#define GPIOC_BASE_ADDR	0x10015200	
#define GPIOC_DDIR	(*(volatile UINT *)(GPIOC_BASE_ADDR+0x00))	//  32bit gpio ptc data direction reg
#define GPIOC_OCR1	(*(volatile UINT *)(GPIOC_BASE_ADDR+0x04))	//  32bit gpio ptc output config 1 reg
#define GPIOC_OCR2	(*(volatile UINT *)(GPIOC_BASE_ADDR+0x08))	//  32bit gpio ptc output config 2 reg
#define GPIOC_ICONFA1	(*(volatile UINT *)(GPIOC_BASE_ADDR+0x0C))	//  32bit gpio ptc input config A1 reg
#define GPIOC_ICONFA2	(*(volatile UINT *)(GPIOC_BASE_ADDR+0x10))	//  32bit gpio ptc input config A2 reg
#define GPIOC_ICONFB1	(*(volatile UINT *)(GPIOC_BASE_ADDR+0x14))	//  32bit gpio ptc input config B1 reg
#define GPIOC_ICONFB2	(*(volatile UINT *)(GPIOC_BASE_ADDR+0x18))	//  32bit gpio ptc input config B2 reg
#define GPIOC_DR	(*(volatile UINT *)(GPIOC_BASE_ADDR+0x1C))	//  32bit gpio ptc data reg
#define GPIOC_GIUS	(*(volatile UINT *)(GPIOC_BASE_ADDR+0x20))	//  32bit gpio ptc in use reg
#define GPIOC_SSR	(*(volatile UINT *)(GPIOC_BASE_ADDR+0x24))	//  32bit gpio ptc sample status reg
#define GPIOC_ICR1	(*(volatile UINT *)(GPIOC_BASE_ADDR+0x28))	//  32bit gpio ptc interrupt ctrl 1 reg
#define GPIOC_ICR2	(*(volatile UINT *)(GPIOC_BASE_ADDR+0x2C))	//  32bit gpio ptc interrupt ctrl 2 reg
#define GPIOC_IMR	(*(volatile UINT *)(GPIOC_BASE_ADDR+0x30))	//  32bit gpio ptc interrupt mask reg
#define GPIOC_ISR	(*(volatile UINT *)(GPIOC_BASE_ADDR+0x34))	//  32bit gpio ptc interrupt status reg
#define GPIOC_GPR	(*(volatile UINT *)(GPIOC_BASE_ADDR+0x38))	//  32bit gpio ptc general purpose reg
#define GPIOC_SWR	(*(volatile UINT *)(GPIOC_BASE_ADDR+0x3C))	//  32bit gpio ptc software reset reg 
#define GPIOC_PUEN	(*(volatile UINT *)(GPIOC_BASE_ADDR+0x40))	//  32bit gpio ptc pull up enable reg
		
#define GPIOD_BASE_ADDR	0x10015300	
#define GPIOD_DDIR	(*(volatile UINT *)(GPIOD_BASE_ADDR+0x00))	//  32bit gpio ptd data direction reg
#define GPIOD_OCR1	(*(volatile UINT *)(GPIOD_BASE_ADDR+0x04))	//  32bit gpio ptd output config 1 reg
#define GPIOD_OCR2	(*(volatile UINT *)(GPIOD_BASE_ADDR+0x08))	//  32bit gpio ptd output config 2 reg
#define GPIOD_ICONFA1	(*(volatile UINT *)(GPIOD_BASE_ADDR+0x0C))	//  32bit gpio ptd input config A1 reg
#define GPIOD_ICONFA2	(*(volatile UINT *)(GPIOD_BASE_ADDR+0x10))	//  32bit gpio ptd input config A2 reg
#define GPIOD_ICONFB1	(*(volatile UINT *)(GPIOD_BASE_ADDR+0x14))	//  32bit gpio ptd input config B1 reg
#define GPIOD_ICONFB2	(*(volatile UINT *)(GPIOD_BASE_ADDR+0x18))	//  32bit gpio ptd input config B2 reg
#define GPIOD_DR	(*(volatile UINT *)(GPIOD_BASE_ADDR+0x1C))	//  32bit gpio ptd data reg
#define GPIOD_GIUS	(*(volatile UINT *)(GPIOD_BASE_ADDR+0x20))	//  32bit gpio ptd in use reg
#define GPIOD_SSR	(*(volatile UINT *)(GPIOD_BASE_ADDR+0x24))	//  32bit gpio ptd sample status reg
#define GPIOD_ICR1	(*(volatile UINT *)(GPIOD_BASE_ADDR+0x28))	//  32bit gpio ptd interrupt ctrl 1 reg
#define GPIOD_ICR2	(*(volatile UINT *)(GPIOD_BASE_ADDR+0x2C))	//  32bit gpio ptd interrupt ctrl 2 reg
#define GPIOD_IMR	(*(volatile UINT *)(GPIOD_BASE_ADDR+0x30))	//  32bit gpio ptd interrupt mask reg
#define GPIOD_ISR	(*(volatile UINT *)(GPIOD_BASE_ADDR+0x34))	//  32bit gpio ptd interrupt status reg
#define GPIOD_GPR	(*(volatile UINT *)(GPIOD_BASE_ADDR+0x38))	//  32bit gpio ptd general purpose reg
#define GPIOD_SWR	(*(volatile UINT *)(GPIOD_BASE_ADDR+0x3C))	//  32bit gpio ptd software reset reg 
#define GPIOD_PUEN	(*(volatile UINT *)(GPIOD_BASE_ADDR+0x40))	//  32bit gpio ptd pull up enable reg
		
#define GPIOE_BASE_ADDR	0x10015400	
#define GPIOE_DDIR	(*(volatile UINT *)(GPIOE_BASE_ADDR+0x00))	//  32bit gpio pte data direction reg
#define GPIOE_OCR1	(*(volatile UINT *)(GPIOE_BASE_ADDR+0x04))	//  32bit gpio pte output config 1 reg
#define GPIOE_OCR2	(*(volatile UINT *)(GPIOE_BASE_ADDR+0x08))	//  32bit gpio pte output config 2 reg
#define GPIOE_ICONFA1	(*(volatile UINT *)(GPIOE_BASE_ADDR+0x0C))	//  32bit gpio pte input config A1 reg
#define GPIOE_ICONFA2	(*(volatile UINT *)(GPIOE_BASE_ADDR+0x10))	//  32bit gpio pte input config A2 reg
#define GPIOE_ICONFB1	(*(volatile UINT *)(GPIOE_BASE_ADDR+0x14))	//  32bit gpio pte input config B1 reg
#define GPIOE_ICONFB2	(*(volatile UINT *)(GPIOE_BASE_ADDR+0x18))	//  32bit gpio pte input config B2 reg
#define GPIOE_DR	(*(volatile UINT *)(GPIOE_BASE_ADDR+0x1C))	//  32bit gpio pte data reg
#define GPIOE_GIUS	(*(volatile UINT *)(GPIOE_BASE_ADDR+0x20))	//  32bit gpio pte in use reg
#define GPIOE_SSR	(*(volatile UINT *)(GPIOE_BASE_ADDR+0x24))	//  32bit gpio pte sample status reg
#define GPIOE_ICR1	(*(volatile UINT *)(GPIOE_BASE_ADDR+0x28))	//  32bit gpio pte interrupt ctrl 1 reg
#define GPIOE_ICR2	(*(volatile UINT *)(GPIOE_BASE_ADDR+0x2C))	//  32bit gpio pte interrupt ctrl 2 reg
#define GPIOE_IMR	(*(volatile UINT *)(GPIOE_BASE_ADDR+0x30))	//  32bit gpio pte interrupt mask reg
#define GPIOE_ISR	(*(volatile UINT *)(GPIOE_BASE_ADDR+0x34))	//  32bit gpio pte interrupt status reg
#define GPIOE_GPR	(*(volatile UINT *)(GPIOE_BASE_ADDR+0x38))	//  32bit gpio pte general purpose reg
#define GPIOE_SWR	(*(volatile UINT *)(GPIOE_BASE_ADDR+0x3C))	//  32bit gpio pte software reset reg 
#define GPIOE_PUEN	(*(volatile UINT *)(GPIOE_BASE_ADDR+0x40))	//  32bit gpio pte pull up enable reg
		
#define GPIOF_BASE_ADDR	0x10015500	
#define GPIOF_DDIR	(*(volatile UINT *)(GPIOF_BASE_ADDR+0x00))	//  32bit gpio ptf data direction reg
#define GPIOF_OCR1	(*(volatile UINT *)(GPIOF_BASE_ADDR+0x04))	//  32bit gpio ptf output config 1 reg
#define GPIOF_OCR2	(*(volatile UINT *)(GPIOF_BASE_ADDR+0x08))	//  32bit gpio ptf output config 2 reg
#define GPIOF_ICONFA1	(*(volatile UINT *)(GPIOF_BASE_ADDR+0x0C))	//  32bit gpio ptf input config A1 reg
#define GPIOF_ICONFA2	(*(volatile UINT *)(GPIOF_BASE_ADDR+0x10))	//  32bit gpio ptf input config A2 reg
#define GPIOF_ICONFB1	(*(volatile UINT *)(GPIOF_BASE_ADDR+0x14))	//  32bit gpio ptf input config B1 reg
#define GPIOF_ICONFB2	(*(volatile UINT *)(GPIOF_BASE_ADDR+0x18))	//  32bit gpio ptf input config B2 reg
#define GPIOF_DR	(*(volatile UINT *)(GPIOF_BASE_ADDR+0x1C))	//  32bit gpio ptf data reg
#define GPIOF_GIUS	(*(volatile UINT *)(GPIOF_BASE_ADDR+0x20))	//  32bit gpio ptf in use reg
#define GPIOF_SSR	(*(volatile UINT *)(GPIOF_BASE_ADDR+0x24))	//  32bit gpio ptf sample status reg
#define GPIOF_ICR1	(*(volatile UINT *)(GPIOF_BASE_ADDR+0x28))	//  32bit gpio ptf interrupt ctrl 1 reg
#define GPIOF_ICR2	(*(volatile UINT *)(GPIOF_BASE_ADDR+0x2C))	//  32bit gpio ptf interrupt ctrl 2 reg
#define GPIOF_IMR	(*(volatile UINT *)(GPIOF_BASE_ADDR+0x30))	//  32bit gpio ptf interrupt mask reg
#define GPIOF_ISR	(*(volatile UINT *)(GPIOF_BASE_ADDR+0x34))	//  32bit gpio ptf interrupt status reg
#define GPIOF_GPR	(*(volatile UINT *)(GPIOF_BASE_ADDR+0x38))	//  32bit gpio ptf general purpose reg
#define GPIOF_SWR	(*(volatile UINT *)(GPIOF_BASE_ADDR+0x3C))	//  32bit gpio ptf software reset reg 
#define GPIOF_PUEN	(*(volatile UINT *)(GPIOF_BASE_ADDR+0x40))	//  32bit gpio ptf pull up enable reg
		
#define GPIO_REG_BASE	0x10015600	
#define GPIO_PMASK	(*(volatile UINT *)(GPIO_REG_BASE+0x00))	//  32bit gpio interrupt mask reg
		
//#########################################		
//# AUDMUX                                #		
//# $1001_6000 to $1001_6FFF              #		
//#########################################		
#define AUDMUX_BASE_ADDR	0x10016000	
#define AUDMUX_HPCR1	(*(volatile UINT *)(AUDMUX_BASE_ADDR+0x00))	//  32bit audmux host config reg 1
#define AUDMUX_HPCR2	(*(volatile UINT *)(AUDMUX_BASE_ADDR+0x04))	//  32bit audmux host config reg 2
#define AUDMUX_HPCR3	(*(volatile UINT *)(AUDMUX_BASE_ADDR+0x08))	//  32bit audmux host config reg 3
#define AUDMUX_PPCR1	(*(volatile UINT *)(AUDMUX_BASE_ADDR+0x10))	//  32bit audmux pripheral config 1
#define AUDMUX_PPCR2	(*(volatile UINT *)(AUDMUX_BASE_ADDR+0x14))	//  32bit audmux pripheral config 2
#define AUDMUX_PPCR3	(*(volatile UINT *)(AUDMUX_BASE_ADDR+0x1C))	//  32bit audmux pripheral config 3
		
		
		
//#########################################		
//# AIPI2                                 #		
//# $1002_0000 to $1002_0FFF              #		
//#########################################		
#define AIPI2_BASE_ADDR	0x10020000	
#define AIPI2_PSR0	(*(volatile UINT *)(AIPI2_BASE_ADDR+0x00))	//  32bit Peripheral Size Reg 0
#define AIPI2_PSR1	(*(volatile UINT *)(AIPI2_BASE_ADDR+0x04))	//  32bit Peripheral Size Reg 1
#define AIPI2_PAR	(*(volatile UINT *)(AIPI2_BASE_ADDR+0x08))	//  32bit Peripheral Access Reg
		
//#########################################		
//# LCDC                                  #		
//# $1002_1000 to $1002_1FFF              #		
//#########################################		
#define LCDC_BASE_ADDR	0x10021000	
#define LCDC_LSSAR	(*(volatile UINT *)(LCDC_BASE_ADDR+0x00))	//  32bit lcdc screen start addr reg
#define LCDC_LSR	(*(volatile UINT *)(LCDC_BASE_ADDR+0x04))	//  32bit lcdc size reg
#define LCDC_LVPWR	(*(volatile UINT *)(LCDC_BASE_ADDR+0x08))	//  32bit lcdc virtual page width reg
#define LCDC_LCPR	(*(volatile UINT *)(LCDC_BASE_ADDR+0x0C))	//  32bit lcd cursor position reg
#define LCDC_LCWHBR	(*(volatile UINT *)(LCDC_BASE_ADDR+0x10))	//  32bit lcd cursor width/heigh/blink
#define LCDC_LCCMR	(*(volatile UINT *)(LCDC_BASE_ADDR+0x14))	//  32bit lcd color cursor mapping reg
#define LCDC_LPCR	(*(volatile UINT *)(LCDC_BASE_ADDR+0x18))	//  32bit lcdc panel config reg
#define LCDC_LHCR	(*(volatile UINT *)(LCDC_BASE_ADDR+0x1C))	//  32bit lcdc horizontal config reg
#define LCDC_LVCR	(*(volatile UINT *)(LCDC_BASE_ADDR+0x20))	//  32bit lcdc vertical config reg
#define LCDC_LPOR	(*(volatile UINT *)(LCDC_BASE_ADDR+0x24))	//  32bit lcdc panning offset reg
#define LCDC_LSCR	(*(volatile UINT *)(LCDC_BASE_ADDR+0x28))	//  32bit lcdc sharp config 1 reg
#define LCDC_LPCCR	(*(volatile UINT *)(LCDC_BASE_ADDR+0x2C))	//  32bit lcdc pwm contrast ctrl reg
#define LCDC_LDCR	(*(volatile UINT *)(LCDC_BASE_ADDR+0x30))	//  32bit lcdc dma control reg
#define LCDC_LRMCR	(*(volatile UINT *)(LCDC_BASE_ADDR+0x34))	//  32bit lcdc refresh mode ctrl reg
#define LCDC_LICR	(*(volatile UINT *)(LCDC_BASE_ADDR+0x38))	//  32bit lcdc interrupt config reg
#define LCDC_LIER	(*(volatile UINT *)(LCDC_BASE_ADDR+0x3C))	//  32bit lcdc interrupt enable reg
#define LCDC_LISR	(*(volatile UINT *)(LCDC_BASE_ADDR+0x40))	//  32bit lcdc interrupt status reg
#define LCDC_LGWSAR	(*(volatile UINT *)(LCDC_BASE_ADDR+0x50))	//  32bit lcdc graphic win start add
#define LCDC_LGWSR	(*(volatile UINT *)(LCDC_BASE_ADDR+0x54))	//  32bit lcdc graphic win size reg
#define LCDC_LGWVPWR	(*(volatile UINT *)(LCDC_BASE_ADDR+0x58))	//  32bit lcdc graphic win virtual pg
#define LCDC_LGWPOR	(*(volatile UINT *)(LCDC_BASE_ADDR+0x5C))	//  32bit lcdc graphic win pan offset
#define LCDC_LGWPR	(*(volatile UINT *)(LCDC_BASE_ADDR+0x60))	//  32bit lcdc graphic win positon reg
#define LCDC_LGWCR	(*(volatile UINT *)(LCDC_BASE_ADDR+0x64))	//  32bit lcdc graphic win control reg
#define LCDC_LGWDCR	(*(volatile UINT *)(LCDC_BASE_ADDR+0x68))	//  32bit lcdc graphic win DMA control reg
		
#define LCDC_BPLUT_BASE	(*(volatile UINT *)(LCDC_BASE_ADDR+0x800))	//  Background Plane LUT (*(volatile UINT *)(800 - BFC))
#define LCDC_GWLUT_BASE	(*(volatile UINT *)(LCDC_BASE_ADDR+0xC00))	//  Background Plane LUT (*(volatile UINT *)(C00 - FFC))
		
//#########################################		
//# SLCDC                                 #		
//# $1002_2000 to $1002_2FFF              #		
//#########################################		
#define SLCDC_BASE_ADDR	0x10022000	
#define SLCDC_DBADDR	(*(volatile UINT *)(SLCDC_BASE_ADDR+0x00))	//  32bit slcdc data base addr
#define SLCDC_DBUF_SIZE	(*(volatile UINT *)(SLCDC_BASE_ADDR+0x04))	//  32bit slcdc data buffer size high
#define SLCDC_CBADDR	(*(volatile UINT *)(SLCDC_BASE_ADDR+0x08))	//  32bit slcdc cmd base addr high
#define SLCDC_CBUF_SIZE	(*(volatile UINT *)(SLCDC_BASE_ADDR+0x0C))	//  32bit slcdc cmd buffer size high
#define SLCDC_CBUF_SSIZE	(*(volatile UINT *)(SLCDC_BASE_ADDR+0x10))	//  32bit slcdc cmd string size
#define SLCDC_FIFO_CONFIG	(*(volatile UINT *)(SLCDC_BASE_ADDR+0x14))	//  32bit slcdc fifo config reg
#define SLCDC_LCD_CONFIG	(*(volatile UINT *)(SLCDC_BASE_ADDR+0x18))	//  32bit slcdc lcd controller config
#define SLCDC_LCD_TXCONFIG	(*(volatile UINT *)(SLCDC_BASE_ADDR+0x1C))	//  32bit slcdc lcd transmit config reg
#define SLCDC_LCD_CTRL_STAT	(*(volatile UINT *)(SLCDC_BASE_ADDR+0x20))	//  32bit slcdc lcd control/status reg
#define SLCDC_LCD_CLKCONFIG	(*(volatile UINT *)(SLCDC_BASE_ADDR+0x24))	//  32bit slcdc lcd clock config reg
#define SLCDC_LCD_WR_DATA	(*(volatile UINT *)(SLCDC_BASE_ADDR+0x28))	//  32bit slcdc lcd write data reg
		
		
		
		
		
//#########################################		
//# SAHARA                                #		
//# $1002_3000 to $1002_3FFF              #		
//#########################################		
#define SAHARA_BASE_ADDR	0x10023000	
		
//# CHA BASE ADDRESSES		
#define SAHARA_TOP	(*(volatile UINT *)(SAHARA_BASE_ADDR+0x0000))	
#define SAHARA_AESA	(*(volatile UINT *)(SAHARA_BASE_ADDR+0x0100))	
#define SAHARA_DESA	(*(volatile UINT *)(SAHARA_BASE_ADDR+0x0101))	
#define SAHARA_MDHA	(*(volatile UINT *)(SAHARA_BASE_ADDR+0x0200))	
#define SAHARA_RNGA	(*(volatile UINT *)(SAHARA_BASE_ADDR+0x0300))	
#define SAHARA_FIDO	(*(volatile UINT *)(SAHARA_BASE_ADDR+0x0400))	
#define SAHARA_I_FIDO	(*(volatile UINT *)(SAHARA_BASE_ADDR+0x0400))	
#define SAHARA_O_FIDO	(*(volatile UINT *)(SAHARA_BASE_ADDR+0x0500))	
#define SAHARA_PKHA	(*(volatile UINT *)(SAHARA_BASE_ADDR+0x0800))	
		
//# SAHARA REGISTERS		
#define SAHARA_VER_ID	(*(volatile UINT *)(SAHARA_TOP+0x00))	
#define SAHARA_DSC_ADR	(*(volatile UINT *)(SAHARA_TOP+0x04))	
#define SAHARA_CONTROL	(*(volatile UINT *)(SAHARA_TOP+0x08))	
#define SAHARA_COMMAND	(*(volatile UINT *)(SAHARA_TOP+0x0C))	
#define SAHARA_STAT	(*(volatile UINT *)(SAHARA_TOP+0x10))	
#define SAHARA_ERR_STAT	(*(volatile UINT *)(SAHARA_TOP+0x14))	
#define SAHARA_FAULT_ADR	(*(volatile UINT *)(SAHARA_TOP+0x18))	
#define SAHARA_C_DSC_ADR	(*(volatile UINT *)(SAHARA_TOP+0x1C))	
#define SAHARA_I_DSC_ADR	(*(volatile UINT *)(SAHARA_TOP+0x20))	
#define SAHARA_BUFF_LVL	(*(volatile UINT *)(SAHARA_TOP+0x24))	
#define SAHARA_DSC_A	(*(volatile UINT *)(SAHARA_TOP+0x80))	
#define SAHARA_DSC_B	(*(volatile UINT *)(SAHARA_TOP+0x84))	
#define SAHARA_DSC_C	(*(volatile UINT *)(SAHARA_TOP+0x88))	
#define SAHARA_DSC_D	(*(volatile UINT *)(SAHARA_TOP+0x8C))	
#define SAHARA_DSC_E	(*(volatile UINT *)(SAHARA_TOP+0x90))	
#define SAHARA_DSC_F	(*(volatile UINT *)(SAHARA_TOP+0x94))	
#define SAHARA_LNK_1_A	(*(volatile UINT *)(SAHARA_TOP+0xA0))	
#define SAHARA_LNK_1_B	(*(volatile UINT *)(SAHARA_TOP+0xA4))	
#define SAHARA_LNK_1_C	(*(volatile UINT *)(SAHARA_TOP+0xA8))	
#define SAHARA_LNK_2_A	(*(volatile UINT *)(SAHARA_TOP+0xB0))	
#define SAHARA_LNK_2_B	(*(volatile UINT *)(SAHARA_TOP+0xB4))	
#define SAHARA_LNK_2_C	(*(volatile UINT *)(SAHARA_TOP+0xB8))	
#define SAHARA_FLOW_CTRL	(*(volatile UINT *)(SAHARA_TOP+0xC0))	
		
//# COMMON CHA REGISTERS		
#define SAHARA_MODE	0x00	
#define SAHARA_KEY_SIZE	0x04	
#define SAHARA_DATA_SIZE	0x08	
#define SAHARA_STATUS	0x0C	
#define SAHARA_ERROR_STATUS	0x10	
#define SAHARA_CHA_GO	0x14	
#define SAHARA_CONTEXT	0x40	
#define SAHARA_KEY	0x80	
		
//# SAHARA_AESA REGISTERS		
#define SAHARA_AESA_MODE	(*(volatile UINT *)(SAHARA_AESA+SAHARA_MODE+0x00))	
#define SAHARA_AESA_KEY_SIZE	(*(volatile UINT *)(SAHARA_AESA+SAHARA_KEY_SIZE+0x00))	
#define SAHARA_AESA_DATA_SIZE	(*(volatile UINT *)(SAHARA_AESA+SAHARA_DATA_SIZE+0x00))	
#define SAHARA_AESA_STAT	(*(volatile UINT *)(SAHARA_AESA+SAHARA_STATUS+0x00))	
#define SAHARA_AESA_ERR_STAT	(*(volatile UINT *)(SAHARA_AESA+SAHARA_ERROR_STATUS+0x00))	
#define SAHARA_AESA_CHA_GO	(*(volatile UINT *)(SAHARA_AESA+SAHARA_CHA_GO+0x00))	
#define SAHARA_AESA_CXT	(*(volatile UINT *)(SAHARA_AESA+SAHARA_CONTEXT+0x00))	
#define SAHARA_AESA_KEY_1	(*(volatile UINT *)(SAHARA_AESA+SAHARA_KEY+0x00))	
#define SAHARA_AESA_KEY_2	(*(volatile UINT *)(SAHARA_AESA+SAHARA_KEY+0x04))	
#define SAHARA_AESA_KEY_3	(*(volatile UINT *)(SAHARA_AESA+SAHARA_KEY+0x08))	
#define SAHARA_AESA_KEY_4	(*(volatile UINT *)(SAHARA_AESA+SAHARA_KEY+0x0C))	
#define SAHARA_AESA_IV_1	(*(volatile UINT *)(SAHARA_AESA+SAHARA_CONTEXT+0x00))	
#define SAHARA_AESA_IV_2	(*(volatile UINT *)(SAHARA_AESA+SAHARA_CONTEXT+0x04))	
#define SAHARA_AESA_IV_3	(*(volatile UINT *)(SAHARA_AESA+SAHARA_CONTEXT+0x08))	
#define SAHARA_AESA_IV_4	(*(volatile UINT *)(SAHARA_AESA+SAHARA_CONTEXT+0x0C))	
#define SAHARA_AESA_IV_5	(*(volatile UINT *)(SAHARA_AESA+SAHARA_CONTEXT+0x10))	
#define SAHARA_AESA_IV_6	(*(volatile UINT *)(SAHARA_AESA+SAHARA_CONTEXT+0x14))	
#define SAHARA_AESA_IV_7	(*(volatile UINT *)(SAHARA_AESA+SAHARA_CONTEXT+0x18))	
#define SAHARA_AESA_IV_8	(*(volatile UINT *)(SAHARA_AESA+SAHARA_CONTEXT+0x1C))	
#define SAHARA_AESA_IV_9	(*(volatile UINT *)(SAHARA_AESA+SAHARA_CONTEXT+0x20))	
#define SAHARA_AESA_IV_10	(*(volatile UINT *)(SAHARA_AESA+SAHARA_CONTEXT+0x24))	
#define SAHARA_AESA_IV_11	(*(volatile UINT *)(SAHARA_AESA+SAHARA_CONTEXT+0x28))	
#define SAHARA_AESA_IV_12	(*(volatile UINT *)(SAHARA_AESA+SAHARA_CONTEXT+0x2C))	
#define SAHARA_AESA_IV_13	(*(volatile UINT *)(SAHARA_AESA+SAHARA_CONTEXT+0x30))	
#define SAHARA_AESA_IV_14	(*(volatile UINT *)(SAHARA_AESA+SAHARA_CONTEXT+0x34))	
#define SAHARA_AESA_IV_15	(*(volatile UINT *)(SAHARA_AESA+SAHARA_CONTEXT+0x38))	
#define SAHARA_AESA_IV_16	(*(volatile UINT *)(SAHARA_AESA+SAHARA_CONTEXT+0x3C))	
		
//# SAHARA_DESA REGISTERS		
#define SAHARA_DESA_MODE	(*(volatile UINT *)(SAHARA_DESA+SAHARA_MODE+0x00))	
#define SAHARA_DESA_KEY_SIZE	(*(volatile UINT *)(SAHARA_DESA+SAHARA_KEY_SIZE+0x00))	
#define SAHARA_DESA_DATA_SIZE	(*(volatile UINT *)(SAHARA_DESA+SAHARA_DATA_SIZE+0x00))	
#define SAHARA_DESA_STAT	(*(volatile UINT *)(SAHARA_DESA+SAHARA_STATUS+0x00))	
#define SAHARA_DESA_ERR_STAT	(*(volatile UINT *)(SAHARA_DESA+SAHARA_ERROR_STATUS+0x00))	
#define SAHARA_DESA_CHA_GO	(*(volatile UINT *)(SAHARA_DESA+SAHARA_CHA_GO+0x00))	
#define SAHARA_DESA_KEY	(*(volatile UINT *)(SAHARA_DESA+SAHARA_KEY+0x00))	
#define SAHARA_DESA_CXT	(*(volatile UINT *)(SAHARA_DESA+SAHARA_CONTEXT+0x00))	
#define SAHARA_DESA_KEY_1	(*(volatile UINT *)(SAHARA_DESA+SAHARA_KEY+0x00))	
#define SAHARA_DESA_KEY_2	(*(volatile UINT *)(SAHARA_DESA+SAHARA_KEY+0x04))	
#define SAHARA_DESA_KEY_3	(*(volatile UINT *)(SAHARA_DESA+SAHARA_KEY+0x08))	
#define SAHARA_DESA_KEY_4	(*(volatile UINT *)(SAHARA_DESA+SAHARA_KEY+0x0C))	
#define SAHARA_DESA_KEY_5	(*(volatile UINT *)(SAHARA_DESA+SAHARA_KEY+0x10))	
#define SAHARA_DESA_KEY_6	(*(volatile UINT *)(SAHARA_DESA+SAHARA_KEY+0x14))	
#define SAHARA_DESA_IV_1	(*(volatile UINT *)(SAHARA_DESA+SAHARA_CONTEXT+0x00))	
#define SAHARA_DESA_IV_2	(*(volatile UINT *)(SAHARA_DESA+SAHARA_CONTEXT+0x04))	
		
//# SAHARA_MDHA REGISTERS		
#define SAHARA_MDHA_MODE	(*(volatile UINT *)(SAHARA_MDHA+SAHARA_MODE+0x00))	
#define SAHARA_MDHA_KEY_SIZE	(*(volatile UINT *)(SAHARA_MDHA+SAHARA_KEY_SIZE+0x00))	
#define SAHARA_MDHA_DATA_SIZE	(*(volatile UINT *)(SAHARA_MDHA+SAHARA_DATA_SIZE+0x00))	
#define SAHARA_MDHA_STAT	(*(volatile UINT *)(SAHARA_MDHA+SAHARA_STATUS+0x00))	
#define SAHARA_MDHA_ERR_STAT	(*(volatile UINT *)(SAHARA_MDHA+SAHARA_ERROR_STATUS+0x00))	
#define SAHARA_MDHA_GO	(*(volatile UINT *)(SAHARA_MDHA+SAHARA_CHA_GO+0x00))	
#define SAHARA_MDHA_KEY	(*(volatile UINT *)(SAHARA_MDHA+SAHARA_KEY+0x00))	
#define SAHARA_MDHA_CXT	(*(volatile UINT *)(SAHARA_MDHA+SAHARA_CONTEXT+0x00))	
#define SAHARA_MDHA_MD_A1	(*(volatile UINT *)(SAHARA_MDHA+SAHARA_KEY+0x00))	
#define SAHARA_MDHA_MD_B1	(*(volatile UINT *)(SAHARA_MDHA+SAHARA_KEY+0x04))	
#define SAHARA_MDHA_MD_C1	(*(volatile UINT *)(SAHARA_MDHA+SAHARA_KEY+0x08))	
#define SAHARA_MDHA_MD_D1	(*(volatile UINT *)(SAHARA_MDHA+SAHARA_KEY+0x0C))	
#define SAHARA_MDHA_MD_E1	(*(volatile UINT *)(SAHARA_MDHA+SAHARA_KEY+0x10))	
#define SAHARA_MDHA_MD_A	(*(volatile UINT *)(SAHARA_MDHA+SAHARA_CONTEXT+0x00))	
#define SAHARA_MDHA_MD_B	(*(volatile UINT *)(SAHARA_MDHA+SAHARA_CONTEXT+0x04))	
#define SAHARA_MDHA_MD_C	(*(volatile UINT *)(SAHARA_MDHA+SAHARA_CONTEXT+0x08))	
#define SAHARA_MDHA_MD_D	(*(volatile UINT *)(SAHARA_MDHA+SAHARA_CONTEXT+0x0C))	
#define SAHARA_MDHA_MD_E	(*(volatile UINT *)(SAHARA_MDHA+SAHARA_CONTEXT+0x10))	
#define SAHARA_MDHA_MD_CNT	(*(volatile UINT *)(SAHARA_MDHA+SAHARA_CONTEXT+0x14))	
		
//# SAHARA_PKHA REGISTERS		
#define SAHARA_PKHA_PGM_COUNT	(*(volatile UINT *)(SAHARA_PKHA+SAHARA_MODE+0x000))	
#define SAHARA_PKHA_KEY_SIZE	(*(volatile UINT *)(SAHARA_PKHA+SAHARA_KEY_SIZE+0x000))	
#define SAHARA_PKHA_MOD_SIZE	(*(volatile UINT *)(SAHARA_PKHA+SAHARA_DATA_SIZE+0x000))	
#define SAHARA_PKHA_STAT	(*(volatile UINT *)(SAHARA_PKHA+SAHARA_STATUS+0x000))	
#define SAHARA_PKHA_ERR_STAT	(*(volatile UINT *)(SAHARA_PKHA+SAHARA_ERROR_STATUS+0x000))	
#define SAHARA_PKHA_CHA_GO	(*(volatile UINT *)(SAHARA_PKHA+SAHARA_CHA_GO+0x000))	
#define SAHARA_PKHA_A0_BASE	(*(volatile UINT *)(SAHARA_PKHA+0x400))	
#define SAHARA_PKHA_A1_BASE	(*(volatile UINT *)(SAHARA_PKHA+0x440))	
#define SAHARA_PKHA_A2_BASE	(*(volatile UINT *)(SAHARA_PKHA+0x480))	
#define SAHARA_PKHA_A3_BASE	(*(volatile UINT *)(SAHARA_PKHA+0x4C0))	
#define SAHARA_PKHA_B0_BASE	(*(volatile UINT *)(SAHARA_PKHA+0x500))	
#define SAHARA_PKHA_B1_BASE	(*(volatile UINT *)(SAHARA_PKHA+0x540))	
#define SAHARA_PKHA_B2_BASE	(*(volatile UINT *)(SAHARA_PKHA+0x580))	
#define SAHARA_PKHA_B3_BASE	(*(volatile UINT *)(SAHARA_PKHA+0x5C0))	
#define SAHARA_PKHA_N_BASE	(*(volatile UINT *)(SAHARA_PKHA+0x600))	
#define SAHARA_PKHA_EXP_BASE	(*(volatile UINT *)(SAHARA_PKHA+0x700))	
		
//# SAHARA_RNGA REGISTERS		
#define SAHARA_RNGA_MODE	(*(volatile UINT *)(SAHARA_RNGA+SAHARA_MODE+0x00))	
#define SAHARA_RNGA_DATA_SIZE	(*(volatile UINT *)(SAHARA_RNGA+SAHARA_DATA_SIZE+0x00))	
#define SAHARA_RNGA_STAT	(*(volatile UINT *)(SAHARA_RNGA+SAHARA_STATUS+0x00))	
#define SAHARA_RNGA_ERR_STAT	(*(volatile UINT *)(SAHARA_RNGA+SAHARA_ERROR_STATUS+0x00))	
#define SAHARA_RNGA_CHA_GO	(*(volatile UINT *)(SAHARA_RNGA+SAHARA_CHA_GO+0x00))	
		
		
		
		
		
//#########################################		
//# USBOTG                                #		
//# $1002_4000 to $1002_5FFF              #		
//#########################################		
#define OTG_BASE_ADDR	0x10024000	
#define OTG_CORE_BASE	(*(volatile UINT *)(OTG_BASE_ADDR+0x000))	//  base location for core
#define OTG_FUNC_BASE	(*(volatile UINT *)(OTG_BASE_ADDR+0x040))	//  base location for function
#define OTG_HOST_BASE	(*(volatile UINT *)(OTG_BASE_ADDR+0x080))	//  base location for host
#define OTG_DMA_BASE	(*(volatile UINT *)(OTG_BASE_ADDR+0x800))	//  base location for dma
		
#define OTG_ETD_BASE	(*(volatile UINT *)(OTG_BASE_ADDR+0x200))	//  base location for etd memory
#define OTG_EP_BASE	(*(volatile UINT *)(OTG_BASE_ADDR+0x400))	//  base location for ep memory
#define OTG_SYS_BASE	(*(volatile UINT *)(OTG_BASE_ADDR+0x600))	//  base location for system
#define OTG_DATA_BASE	(*(volatile UINT *)(OTG_BASE_ADDR+0x1000))	//  base location for data memory

#define OTG_SYS_CTRL	(*(volatile UINT *)(OTG_SYS_BASE+0x000))	//  base location for system
		
#define OTG_CORE_HWMODE	(*(volatile UINT *)(OTG_CORE_BASE+0x00))	//  32bit core hardware mode reg
#define OTG_CORE_CINT_STAT	(*(volatile UINT *)(OTG_CORE_BASE+0x04))	//  32bit core int status reg
#define OTG_CORE_CINT_STEN	(*(volatile UINT *)(OTG_CORE_BASE+0x08))	//  32bit core int enable reg
#define OTG_CORE_CLK_CTRL	(*(volatile UINT *)(OTG_CORE_BASE+0x0C))	//  32bit core clock control reg
#define OTG_CORE_RST_CTRL	(*(volatile UINT *)(OTG_CORE_BASE+0x10))	//  32bit core reset control reg
#define OTG_CORE_FRM_INTVL	(*(volatile UINT *)(OTG_CORE_BASE+0x14))	//  32bit core frame interval reg
#define OTG_CORE_FRM_REMAIN	(*(volatile UINT *)(OTG_CORE_BASE+0x18))	//  32bit core frame remaining reg
#define OTG_CORE_HNP_CSTAT	(*(volatile UINT *)(OTG_CORE_BASE+0x1C))	//  32bit core HNP current state reg
#define OTG_CORE_HNP_TIMER1	(*(volatile UINT *)(OTG_CORE_BASE+0x20))	//  32bit core HNP timer 1 reg
#define OTG_CORE_HNP_TIMER2	(*(volatile UINT *)(OTG_CORE_BASE+0x24))	//  32bit core HNP timer 2 reg
#define OTG_CORE_HNP_T3PCR	(*(volatile UINT *)(OTG_CORE_BASE+0x28))	//  32bit core HNP timer 3 pulse ctrl
#define OTG_CORE_HINT_STAT	(*(volatile UINT *)(OTG_CORE_BASE+0x2C))	//  32bit core HNP int status reg
#define OTG_CORE_HINT_STEN	(*(volatile UINT *)(OTG_CORE_BASE+0x30))	//  32bit core HNP int enable reg
		
#define OTG_FUNC_CND_STAT	(*(volatile UINT *)(OTG_FUNC_BASE+0x00))	//  32bit func command status reg
#define OTG_FUNC_DEV_ADDR	(*(volatile UINT *)(OTG_FUNC_BASE+0x04))	//  32bit func device address reg
#define OTG_FUNC_SINT_STAT	(*(volatile UINT *)(OTG_FUNC_BASE+0x08))	//  32bit func system int status reg
#define OTG_FUNC_SINT_STEN	(*(volatile UINT *)(OTG_FUNC_BASE+0x0C))	//  32bit func system int enable reg
#define OTG_FUNC_XINT_STAT	(*(volatile UINT *)(OTG_FUNC_BASE+0x10))	//  32bit func X buf int status reg
#define OTG_FUNC_YINT_STAT	(*(volatile UINT *)(OTG_FUNC_BASE+0x14))	//  32bit func Y buf int status reg
#define OTG_FUNC_XYINT_STEN	(*(volatile UINT *)(OTG_FUNC_BASE+0x18))	//  32bit func XY buf int enable reg
#define OTG_FUNC_XFILL_STAT	(*(volatile UINT *)(OTG_FUNC_BASE+0x1C))	//  32bit func X filled status reg
#define OTG_FUNC_YFILL_STAT	(*(volatile UINT *)(OTG_FUNC_BASE+0x20))	//  32bit func Y filled status reg
#define OTG_FUNC_EP_EN	(*(volatile UINT *)(OTG_FUNC_BASE+0x24))	//  32bit func endpoints enable reg
#define OTG_FUNC_EP_RDY	(*(volatile UINT *)(OTG_FUNC_BASE+0x28))	//  32bit func endpoints ready reg
#define OTG_FUNC_IINT	(*(volatile UINT *)(OTG_FUNC_BASE+0x2C))	//  32bit func immediate interrupt reg
#define OTG_FUNC_EP_DSTAT	(*(volatile UINT *)(OTG_FUNC_BASE+0x30))	//  32bit func endpoints done status
#define OTG_FUNC_EP_DEN	(*(volatile UINT *)(OTG_FUNC_BASE+0x34))	//  32bit func endpoints done enable
#define OTG_FUNC_EP_TOGGLE	(*(volatile UINT *)(OTG_FUNC_BASE+0x38))	//  32bit func endpoints toggle bits
#define OTG_FUNC_FRM_NUM	(*(volatile UINT *)(OTG_FUNC_BASE+0x3C))	//  32bit func frame number reg
		
#define OTG_HOST_CTRL	(*(volatile UINT *)(OTG_HOST_BASE+0x00))	//  32bit host controller config reg
#define OTG_HOST_SINT_STAT	(*(volatile UINT *)(OTG_HOST_BASE+0x08))	//  32bit host system int status reg
#define OTG_HOST_SINT_STEN	(*(volatile UINT *)(OTG_HOST_BASE+0x0C))	//  32bit host system int enable reg
#define OTG_HOST_XINT_STAT	(*(volatile UINT *)(OTG_HOST_BASE+0x18))	//  32bit host X buf int status reg
#define OTG_HOST_YINT_STAT	(*(volatile UINT *)(OTG_HOST_BASE+0x1C))	//  32bit host Y buf int status reg
#define OTG_HOST_XYINT_STEN	(*(volatile UINT *)(OTG_HOST_BASE+0x20))	//  32bit host XY buf int enable reg
#define OTG_HOST_XFILL_STAT	(*(volatile UINT *)(OTG_HOST_BASE+0x28))	//  32bit host X filled status reg
#define OTG_HOST_YFILL_STAT	(*(volatile UINT *)(OTG_HOST_BASE+0x2C))	//  32bit host Y filled status reg
#define OTG_HOST_ETD_EN	(*(volatile UINT *)(OTG_HOST_BASE+0x40))	//  32bit host ETD enables reg
#define OTG_HOST_DIR_ROUTE	(*(volatile UINT *)(OTG_HOST_BASE+0x48))	//  32bit host direct routing reg
#define OTG_HOST_IINT	(*(volatile UINT *)(OTG_HOST_BASE+0x4C))	//  32bit host immediate interrupt reg
#define OTG_HOST_EP_DSTAT	(*(volatile UINT *)(OTG_HOST_BASE+0x50))	//  32bit host endpoints done status
#define OTG_HOST_ETD_DONE	(*(volatile UINT *)(OTG_HOST_BASE+0x54))	//  32bit host ETD done reg
#define OTG_HOST_FRM_NUM	(*(volatile UINT *)(OTG_HOST_BASE+0x60))	//  32bit host frame number reg
#define OTG_HOST_LSP_THRESH	(*(volatile UINT *)(OTG_HOST_BASE+0x64))	//  32bit host low speed threshold reg
#define OTG_HOST_HUB_DESCA	(*(volatile UINT *)(OTG_HOST_BASE+0x68))	//  32bit host root hub descriptor A
#define OTG_HOST_HUB_DESCB	(*(volatile UINT *)(OTG_HOST_BASE+0x6C))	//  32bit host root hub descriptor B
#define OTG_HOST_HUB_STAT	(*(volatile UINT *)(OTG_HOST_BASE+0x70))	//  32bit host root hub status reg
#define OTG_HOST_PORT1_STAT	(*(volatile UINT *)(OTG_HOST_BASE+0x74))	//  32bit host port 1 status bits
#define OTG_HOST_PORT2_STAT	(*(volatile UINT *)(OTG_HOST_BASE+0x78))	//  32bit host port 2 status bits
#define OTG_HOST_PORT3_STAT	(*(volatile UINT *)(OTG_HOST_BASE+0x7c))	//  32bit host port 3 status bits
		
#define OTG_DMA_REV_NUM	(*(volatile UINT *)(OTG_DMA_BASE+0x000))	//  32bit dma revision number reg
#define OTG_DMA_DINT_STAT	(*(volatile UINT *)(OTG_DMA_BASE+0x004))	//  32bit dma int status reg
#define OTG_DMA_DINT_STEN	(*(volatile UINT *)(OTG_DMA_BASE+0x008))	//  32bit dma int enable reg
#define OTG_DMA_ETD_ERR	(*(volatile UINT *)(OTG_DMA_BASE+0x00C))	//  32bit dma ETD error status reg
#define OTG_DMA_EP_ERR	(*(volatile UINT *)(OTG_DMA_BASE+0x010))	//  32bit dma EP error status reg
#define OTG_DMA_ETD_EN	(*(volatile UINT *)(OTG_DMA_BASE+0x020))	//  32bit dma ETD DMA enable reg
#define OTG_DMA_EP_EN	(*(volatile UINT *)(OTG_DMA_BASE+0x024))	//  32bit dma EP DMA enable reg
#define OTG_DMA_ETD_ENXREQ	(*(volatile UINT *)(OTG_DMA_BASE+0x028))	//  32bit dma ETD DMA enable Xtrig req
#define OTG_DMA_EP_ENXREQ	(*(volatile UINT *)(OTG_DMA_BASE+0x02C))	//  32bit dma EP DMA enable Ytrig req
#define OTG_DMA_ETD_ENXYREQ	(*(volatile UINT *)(OTG_DMA_BASE+0x030))	//  32bit dma ETD DMA enble XYtrig req
#define OTG_DMA_EP_ENXYREQ	(*(volatile UINT *)(OTG_DMA_BASE+0x034))	//  32bit dma EP DMA enable XYtrig req
#define OTG_DMA_ETD_BURST4	(*(volatile UINT *)(OTG_DMA_BASE+0x038))	//  32bit dma ETD DMA enble burst4 reg
#define OTG_DMA_EP_BURST4	(*(volatile UINT *)(OTG_DMA_BASE+0x03C))	//  32bit dma EP DMA enable burst4 reg
#define OTG_DMA_MISC_CTRL	(*(volatile UINT *)(OTG_DMA_BASE+0x040))	//  32bit dma EP misc control reg
#define OTG_DMA_ETD_CH_CLR	(*(volatile UINT *)(OTG_DMA_BASE+0x044))	//  32bit dma ETD clear channel reg
#define OTG_DMA_EP_CH_CLR	(*(volatile UINT *)(OTG_DMA_BASE+0x048))	//  32bit dma EP clear channel reg
		
#define OTG_DMA_ETD0_MSA	(*(volatile UINT *)(OTG_DMA_BASE+0x100))	//  32bit dma ETD0 mem start addr reg
#define OTG_DMA_ETD1_MSA	(*(volatile UINT *)(OTG_DMA_BASE+0x104))	//  32bit dma ETD1 mem start addr reg
#define OTG_DMA_ETD2_MSA	(*(volatile UINT *)(OTG_DMA_BASE+0x108))	//  32bit dma ETD2 mem start addr reg
#define OTG_DMA_ETD3_MSA	(*(volatile UINT *)(OTG_DMA_BASE+0x10C))	//  32bit dma ETD3 mem start addr reg
#define OTG_DMA_ETD4_MSA	(*(volatile UINT *)(OTG_DMA_BASE+0x110))	//  32bit dma ETD4 mem start addr reg
#define OTG_DMA_ETD5_MSA	(*(volatile UINT *)(OTG_DMA_BASE+0x114))	//  32bit dma ETD5 mem start addr reg
#define OTG_DMA_ETD6_MSA	(*(volatile UINT *)(OTG_DMA_BASE+0x118))	//  32bit dma ETD6 mem start addr reg
#define OTG_DMA_ETD7_MSA	(*(volatile UINT *)(OTG_DMA_BASE+0x11C))	//  32bit dma ETD7 mem start addr reg
#define OTG_DMA_ETD8_MSA	(*(volatile UINT *)(OTG_DMA_BASE+0x120))	//  32bit dma ETD8 mem start addr reg
#define OTG_DMA_ETD9_MSA	(*(volatile UINT *)(OTG_DMA_BASE+0x124))	//  32bit dma ETD9 mem start addr reg
#define OTG_DMA_ETD10_MSA	(*(volatile UINT *)(OTG_DMA_BASE+0x128))	//  32bit dma ETD10 mem start addr reg
#define OTG_DMA_ETD11_MSA	(*(volatile UINT *)(OTG_DMA_BASE+0x12C))	//  32bit dma ETD11 mem start addr reg
#define OTG_DMA_ETD12_MSA	(*(volatile UINT *)(OTG_DMA_BASE+0x130))	//  32bit dma ETD12 mem start addr reg
#define OTG_DMA_ETD13_MSA	(*(volatile UINT *)(OTG_DMA_BASE+0x134))	//  32bit dma ETD13 mem start addr reg
#define OTG_DMA_ETD14_MSA	(*(volatile UINT *)(OTG_DMA_BASE+0x138))	//  32bit dma ETD14 mem start addr reg
#define OTG_DMA_ETD15_MSA	(*(volatile UINT *)(OTG_DMA_BASE+0x13C))	//  32bit dma ETD15 mem start addr reg
#define OTG_DMA_ETD16_MSA	(*(volatile UINT *)(OTG_DMA_BASE+0x140))	//  32bit dma ETD16 mem start addr reg
#define OTG_DMA_ETD17_MSA	(*(volatile UINT *)(OTG_DMA_BASE+0x144))	//  32bit dma ETD17 mem start addr reg
#define OTG_DMA_ETD18_MSA	(*(volatile UINT *)(OTG_DMA_BASE+0x148))	//  32bit dma ETD18 mem start addr reg
#define OTG_DMA_ETD19_MSA	(*(volatile UINT *)(OTG_DMA_BASE+0x14C))	//  32bit dma ETD19 mem start addr reg
#define OTG_DMA_ETD20_MSA	(*(volatile UINT *)(OTG_DMA_BASE+0x150))	//  32bit dma ETD20 mem start addr reg
#define OTG_DMA_ETD21_MSA	(*(volatile UINT *)(OTG_DMA_BASE+0x154))	//  32bit dma ETD21 mem start addr reg
#define OTG_DMA_ETD22_MSA	(*(volatile UINT *)(OTG_DMA_BASE+0x158))	//  32bit dma ETD22 mem start addr reg
#define OTG_DMA_ETD23_MSA	(*(volatile UINT *)(OTG_DMA_BASE+0x15C))	//  32bit dma ETD23 mem start addr reg
#define OTG_DMA_ETD24_MSA	(*(volatile UINT *)(OTG_DMA_BASE+0x160))	//  32bit dma ETD24 mem start addr reg
#define OTG_DMA_ETD25_MSA	(*(volatile UINT *)(OTG_DMA_BASE+0x164))	//  32bit dma ETD25 mem start addr reg
#define OTG_DMA_ETD26_MSA	(*(volatile UINT *)(OTG_DMA_BASE+0x168))	//  32bit dma ETD26 mem start addr reg
#define OTG_DMA_ETD27_MSA	(*(volatile UINT *)(OTG_DMA_BASE+0x16C))	//  32bit dma ETD27 mem start addr reg
#define OTG_DMA_ETD28_MSA	(*(volatile UINT *)(OTG_DMA_BASE+0x170))	//  32bit dma ETD28 mem start addr reg
#define OTG_DMA_ETD29_MSA	(*(volatile UINT *)(OTG_DMA_BASE+0x174))	//  32bit dma ETD29 mem start addr reg
#define OTG_DMA_ETD30_MSA	(*(volatile UINT *)(OTG_DMA_BASE+0x178))	//  32bit dma ETD30 mem start addr reg
#define OTG_DMA_ETD31_MSA	(*(volatile UINT *)(OTG_DMA_BASE+0x17C))	//  32bit dma ETD31 mem start addr reg
		
#define OTG_DMA_EP0_O_MSA	(*(volatile UINT *)(OTG_DMA_BASE+0x180))	//  32bit dma EP0 o/p mem start addr
#define OTG_DMA_EP0_I_MSA	(*(volatile UINT *)(OTG_DMA_BASE+0x184))	//  32bit dma EP0 i/p mem start addr
#define OTG_DMA_EP1_O_MSA	(*(volatile UINT *)(OTG_DMA_BASE+0x188))	//  32bit dma EP1 o/p mem start addr
#define OTG_DMA_EP1_I_MSA	(*(volatile UINT *)(OTG_DMA_BASE+0x18C))	//  32bit dma EP1 i/p mem start addr
#define OTG_DMA_EP2_O_MSA	(*(volatile UINT *)(OTG_DMA_BASE+0x190))	//  32bit dma EP2 o/p mem start addr
#define OTG_DMA_EP2_I_MSA	(*(volatile UINT *)(OTG_DMA_BASE+0x194))	//  32bit dma EP2 i/p mem start addr
#define OTG_DMA_EP3_O_MSA	(*(volatile UINT *)(OTG_DMA_BASE+0x198))	//  32bit dma EP3 o/p mem start addr
#define OTG_DMA_EP3_I_MSA	(*(volatile UINT *)(OTG_DMA_BASE+0x19C))	//  32bit dma EP3 i/p mem start addr
#define OTG_DMA_EP4_O_MSA	(*(volatile UINT *)(OTG_DMA_BASE+0x1A0))	//  32bit dma EP4 o/p mem start addr
#define OTG_DMA_EP4_I_MSA	(*(volatile UINT *)(OTG_DMA_BASE+0x1A4))	//  32bit dma EP4 i/p mem start addr
#define OTG_DMA_EP5_O_MSA	(*(volatile UINT *)(OTG_DMA_BASE+0x1A8))	//  32bit dma EP5 o/p mem start addr
#define OTG_DMA_EP5_I_MSA	(*(volatile UINT *)(OTG_DMA_BASE+0x1AC))	//  32bit dma EP5 i/p mem start addr
#define OTG_DMA_EP6_O_MSA	(*(volatile UINT *)(OTG_DMA_BASE+0x1B0))	//  32bit dma EP6 o/p mem start addr
#define OTG_DMA_EP6_I_MSA	(*(volatile UINT *)(OTG_DMA_BASE+0x1B4))	//  32bit dma EP6 i/p mem start addr
#define OTG_DMA_EP7_O_MSA	(*(volatile UINT *)(OTG_DMA_BASE+0x1B8))	//  32bit dma EP7 o/p mem start addr
#define OTG_DMA_EP7_I_MSA	(*(volatile UINT *)(OTG_DMA_BASE+0x1BC))	//  32bit dma EP7 i/p mem start addr
#define OTG_DMA_EP8_O_MSA	(*(volatile UINT *)(OTG_DMA_BASE+0x1C0))	//  32bit dma EP8 o/p mem start addr
#define OTG_DMA_EP8_I_MSA	(*(volatile UINT *)(OTG_DMA_BASE+0x1C4))	//  32bit dma EP8 i/p mem start addr
#define OTG_DMA_EP9_O_MSA	(*(volatile UINT *)(OTG_DMA_BASE+0x1C8))	//  32bit dma EP9 o/p mem start addr
#define OTG_DMA_EP9_I_MSA	(*(volatile UINT *)(OTG_DMA_BASE+0x1CC))	//  32bit dma EP9 i/p mem start addr
#define OTG_DMA_EP10_O_MSA	(*(volatile UINT *)(OTG_DMA_BASE+0x1D0))	//  32bit dma EP10 o/p mem start addr
#define OTG_DMA_EP10_I_MSA	(*(volatile UINT *)(OTG_DMA_BASE+0x1D4))	//  32bit dma EP10 i/p mem start addr
#define OTG_DMA_EP11_O_MSA	(*(volatile UINT *)(OTG_DMA_BASE+0x1D8))	//  32bit dma EP11 o/p mem start addr
#define OTG_DMA_EP11_I_MSA	(*(volatile UINT *)(OTG_DMA_BASE+0x1DC))	//  32bit dma EP11 i/p mem start addr
#define OTG_DMA_EP12_O_MSA	(*(volatile UINT *)(OTG_DMA_BASE+0x1E0))	//  32bit dma EP12 o/p mem start addr
#define OTG_DMA_EP12_I_MSA	(*(volatile UINT *)(OTG_DMA_BASE+0x1E4))	//  32bit dma EP12 i/p mem start addr
#define OTG_DMA_EP13_O_MSA	(*(volatile UINT *)(OTG_DMA_BASE+0x1E8))	//  32bit dma EP13 o/p mem start addr
#define OTG_DMA_EP13_I_MSA	(*(volatile UINT *)(OTG_DMA_BASE+0x1EC))	//  32bit dma EP13 i/p mem start addr
#define OTG_DMA_EP14_O_MSA	(*(volatile UINT *)(OTG_DMA_BASE+0x1F0))	//  32bit dma EP14 o/p mem start addr
#define OTG_DMA_EP14_I_MSA	(*(volatile UINT *)(OTG_DMA_BASE+0x1F4))	//  32bit dma EP14 i/p mem start addr
#define OTG_DMA_EP15_O_MSA	(*(volatile UINT *)(OTG_DMA_BASE+0x1F8))	//  32bit dma EP15 o/p mem start addr
#define OTG_DMA_EP15_I_MSA	(*(volatile UINT *)(OTG_DMA_BASE+0x1FC))	//  32bit dma EP15 i/p mem start addr
		
#define OTG_DMA_ETD0_BPTR	(*(volatile UINT *)(OTG_DMA_BASE+0x200))	//  32bit dma ETD0 buf tx pointer reg
#define OTG_DMA_ETD1_BPTR	(*(volatile UINT *)(OTG_DMA_BASE+0x204))	//  32bit dma ETD1 buf tx pointer reg
#define OTG_DMA_ETD2_BPTR	(*(volatile UINT *)(OTG_DMA_BASE+0x208))	//  32bit dma ETD2 buf tx pointer reg
#define OTG_DMA_ETD3_BPTR	(*(volatile UINT *)(OTG_DMA_BASE+0x20C))	//  32bit dma ETD3 buf tx pointer reg
#define OTG_DMA_ETD4_BPTR	(*(volatile UINT *)(OTG_DMA_BASE+0x210))	//  32bit dma ETD4 buf tx pointer reg
#define OTG_DMA_ETD5_BPTR	(*(volatile UINT *)(OTG_DMA_BASE+0x214))	//  32bit dma ETD5 buf tx pointer reg
#define OTG_DMA_ETD6_BPTR	(*(volatile UINT *)(OTG_DMA_BASE+0x218))	//  32bit dma ETD6 buf tx pointer reg
#define OTG_DMA_ETD7_BPTR	(*(volatile UINT *)(OTG_DMA_BASE+0x21C))	//  32bit dma ETD7 buf tx pointer reg
#define OTG_DMA_ETD8_BPTR	(*(volatile UINT *)(OTG_DMA_BASE+0x220))	//  32bit dma ETD8 buf tx pointer reg
#define OTG_DMA_ETD9_BPTR	(*(volatile UINT *)(OTG_DMA_BASE+0x224))	//  32bit dma ETD9 buf tx pointer reg
#define OTG_DMA_ETD10_BPTR	(*(volatile UINT *)(OTG_DMA_BASE+0x228))	//  32bit dma ETD10 buf tx pointer reg
#define OTG_DMA_ETD11_BPTR	(*(volatile UINT *)(OTG_DMA_BASE+0x22C))	//  32bit dma ETD11 buf tx pointer reg
#define OTG_DMA_ETD12_BPTR	(*(volatile UINT *)(OTG_DMA_BASE+0x230))	//  32bit dma ETD12 buf tx pointer reg
#define OTG_DMA_ETD13_BPTR	(*(volatile UINT *)(OTG_DMA_BASE+0x234))	//  32bit dma ETD13 buf tx pointer reg
#define OTG_DMA_ETD14_BPTR	(*(volatile UINT *)(OTG_DMA_BASE+0x238))	//  32bit dma ETD14 buf tx pointer reg
#define OTG_DMA_ETD15_BPTR	(*(volatile UINT *)(OTG_DMA_BASE+0x23C))	//  32bit dma ETD15 buf tx pointer reg
#define OTG_DMA_ETD16_BPTR	(*(volatile UINT *)(OTG_DMA_BASE+0x240))	//  32bit dma ETD16 buf tx pointer reg
#define OTG_DMA_ETD17_BPTR	(*(volatile UINT *)(OTG_DMA_BASE+0x244))	//  32bit dma ETD17 buf tx pointer reg
#define OTG_DMA_ETD18_BPTR	(*(volatile UINT *)(OTG_DMA_BASE+0x248))	//  32bit dma ETD18 buf tx pointer reg
#define OTG_DMA_ETD19_BPTR	(*(volatile UINT *)(OTG_DMA_BASE+0x24C))	//  32bit dma ETD19 buf tx pointer reg
#define OTG_DMA_ETD20_BPTR	(*(volatile UINT *)(OTG_DMA_BASE+0x250))	//  32bit dma ETD20 buf tx pointer reg
#define OTG_DMA_ETD21_BPTR	(*(volatile UINT *)(OTG_DMA_BASE+0x254))	//  32bit dma ETD21 buf tx pointer reg
#define OTG_DMA_ETD22_BPTR	(*(volatile UINT *)(OTG_DMA_BASE+0x258))	//  32bit dma ETD22 buf tx pointer reg
#define OTG_DMA_ETD23_BPTR	(*(volatile UINT *)(OTG_DMA_BASE+0x25C))	//  32bit dma ETD23 buf tx pointer reg
#define OTG_DMA_ETD24_BPTR	(*(volatile UINT *)(OTG_DMA_BASE+0x260))	//  32bit dma ETD24 buf tx pointer reg
#define OTG_DMA_ETD25_BPTR	(*(volatile UINT *)(OTG_DMA_BASE+0x264))	//  32bit dma ETD25 buf tx pointer reg
#define OTG_DMA_ETD26_BPTR	(*(volatile UINT *)(OTG_DMA_BASE+0x268))	//  32bit dma ETD26 buf tx pointer reg
#define OTG_DMA_ETD27_BPTR	(*(volatile UINT *)(OTG_DMA_BASE+0x26C))	//  32bit dma ETD27 buf tx pointer reg
#define OTG_DMA_ETD28_BPTR	(*(volatile UINT *)(OTG_DMA_BASE+0x270))	//  32bit dma ETD28 buf tx pointer reg
#define OTG_DMA_ETD29_BPTR	(*(volatile UINT *)(OTG_DMA_BASE+0x274))	//  32bit dma ETD29 buf tx pointer reg
#define OTG_DMA_ETD30_BPTR	(*(volatile UINT *)(OTG_DMA_BASE+0x278))	//  32bit dma ETD30 buf tx pointer reg
#define OTG_DMA_ETD31_BPTR	(*(volatile UINT *)(OTG_DMA_BASE+0x27C))	//  32bit dma ETD31 buf tx pointer reg
		
#define OTG_DMA_EP0_O_BPTR	(*(volatile UINT *)(OTG_DMA_BASE+0x280))	//  32bit dma EP0 o/p buf tx pointer
#define OTG_DMA_EP0_I_BPTR	(*(volatile UINT *)(OTG_DMA_BASE+0x284))	//  32bit dma EP0 i/p buf tx pointer
#define OTG_DMA_EP1_O_BPTR	(*(volatile UINT *)(OTG_DMA_BASE+0x288))	//  32bit dma EP1 o/p buf tx pointer
#define OTG_DMA_EP1_I_BPTR	(*(volatile UINT *)(OTG_DMA_BASE+0x28C))	//  32bit dma EP1 i/p buf tx pointer
#define OTG_DMA_EP2_O_BPTR	(*(volatile UINT *)(OTG_DMA_BASE+0x290))	//  32bit dma EP2 o/p buf tx pointer
#define OTG_DMA_EP2_I_BPTR	(*(volatile UINT *)(OTG_DMA_BASE+0x294))	//  32bit dma EP2 i/p buf tx pointer
#define OTG_DMA_EP3_O_BPTR	(*(volatile UINT *)(OTG_DMA_BASE+0x298))	//  32bit dma EP3 o/p buf tx pointer
#define OTG_DMA_EP3_I_BPTR	(*(volatile UINT *)(OTG_DMA_BASE+0x29C))	//  32bit dma EP3 i/p buf tx pointer
#define OTG_DMA_EP4_O_BPTR	(*(volatile UINT *)(OTG_DMA_BASE+0x2A0))	//  32bit dma EP4 o/p buf tx pointer
#define OTG_DMA_EP4_I_BPTR	(*(volatile UINT *)(OTG_DMA_BASE+0x2A4))	//  32bit dma EP4 i/p buf tx pointer
#define OTG_DMA_EP5_O_BPTR	(*(volatile UINT *)(OTG_DMA_BASE+0x2A8))	//  32bit dma EP5 o/p buf tx pointer
#define OTG_DMA_EP5_I_BPTR	(*(volatile UINT *)(OTG_DMA_BASE+0x2AC))	//  32bit dma EP5 i/p buf tx pointer
#define OTG_DMA_EP6_O_BPTR	(*(volatile UINT *)(OTG_DMA_BASE+0x2B0))	//  32bit dma EP6 o/p buf tx pointer
#define OTG_DMA_EP6_I_BPTR	(*(volatile UINT *)(OTG_DMA_BASE+0x2B4))	//  32bit dma EP6 i/p buf tx pointer
#define OTG_DMA_EP7_O_BPTR	(*(volatile UINT *)(OTG_DMA_BASE+0x2B8))	//  32bit dma EP7 o/p buf tx pointer
#define OTG_DMA_EP7_I_BPTR	(*(volatile UINT *)(OTG_DMA_BASE+0x2BC))	//  32bit dma EP7 i/p buf tx pointer
#define OTG_DMA_EP8_O_BPTR	(*(volatile UINT *)(OTG_DMA_BASE+0x2C0))	//  32bit dma EP8 o/p buf tx pointer
#define OTG_DMA_EP8_I_BPTR	(*(volatile UINT *)(OTG_DMA_BASE+0x2C4))	//  32bit dma EP8 i/p buf tx pointer
#define OTG_DMA_EP9_O_BPTR	(*(volatile UINT *)(OTG_DMA_BASE+0x2C8))	//  32bit dma EP9 o/p buf tx pointer
#define OTG_DMA_EP9_I_BPTR	(*(volatile UINT *)(OTG_DMA_BASE+0x2CC))	//  32bit dma EP9 i/p buf tx pointer
#define OTG_DMA_EP10_O_BPTR	(*(volatile UINT *)(OTG_DMA_BASE+0x2D0))	//  32bit dma EP10 o/p buf tx pointer
#define OTG_DMA_EP10_I_BPTR	(*(volatile UINT *)(OTG_DMA_BASE+0x2D4))	//  32bit dma EP10 i/p buf tx pointer
#define OTG_DMA_EP11_O_BPTR	(*(volatile UINT *)(OTG_DMA_BASE+0x2D8))	//  32bit dma EP11 o/p buf tx pointer
#define OTG_DMA_EP11_I_BPTR	(*(volatile UINT *)(OTG_DMA_BASE+0x2DC))	//  32bit dma EP11 i/p buf tx pointer
#define OTG_DMA_EP12_O_BPTR	(*(volatile UINT *)(OTG_DMA_BASE+0x2E0))	//  32bit dma EP12 o/p buf tx pointer
#define OTG_DMA_EP12_I_BPTR	(*(volatile UINT *)(OTG_DMA_BASE+0x2E4))	//  32bit dma EP12 i/p buf tx pointer
#define OTG_DMA_EP13_O_BPTR	(*(volatile UINT *)(OTG_DMA_BASE+0x2E8))	//  32bit dma EP13 o/p buf tx pointer
#define OTG_DMA_EP13_I_BPTR	(*(volatile UINT *)(OTG_DMA_BASE+0x2EC))	//  32bit dma EP13 i/p buf tx pointer
#define OTG_DMA_EP14_O_BPTR	(*(volatile UINT *)(OTG_DMA_BASE+0x2F0))	//  32bit dma EP14 o/p buf tx pointer
#define OTG_DMA_EP14_I_BPTR	(*(volatile UINT *)(OTG_DMA_BASE+0x2F4))	//  32bit dma EP14 i/p buf tx pointer
#define OTG_DMA_EP15_O_BPTR	(*(volatile UINT *)(OTG_DMA_BASE+0x2F8))	//  32bit dma EP15 o/p buf tx pointer
#define OTG_DMA_EP15_I_BPTR	(*(volatile UINT *)(OTG_DMA_BASE+0x2FC))	//  32bit dma EP15 i/p buf tx pointer
		
//#########################################		
//# EMMA                                  #		
//# $1002_6000 to $1002_6FFF              #		
//#########################################		
#define EMMA_BASE_ADDR	0x10026000	
#define EMMA_PP_BASE	(*(volatile UINT *)(EMMA_BASE_ADDR+0x000))	//  base location for post processor
#define EMMA_PRP_BASE	(*(volatile UINT *)(EMMA_BASE_ADDR+0x400))	//  base location for pre processor
#define EMMA_DEC_BASE	(*(volatile UINT *)(EMMA_BASE_ADDR+0x800))	//  base location for decoder
#define EMMA_ENC_BASE	(*(volatile UINT *)(EMMA_BASE_ADDR+0xC00))	//  base location for encoder
		
#define EMMA_PP_CNTL	(*(volatile UINT *)(EMMA_PP_BASE+0x00))	//  32bit post processor control reg
#define EMMA_PP_INTRCTRL	(*(volatile UINT *)(EMMA_PP_BASE+0x04))	//  32bit pp interrupt enable reg
#define EMMA_PP_INTRSTATUS	(*(volatile UINT *)(EMMA_PP_BASE+0x08))	//  32bit pp interrupt status reg
#define EMMA_PP_SY_PTR	(*(volatile UINT *)(EMMA_PP_BASE+0x0C))	//  32bit pp source Y data ptr reg
#define EMMA_PP_SCB_PTR	(*(volatile UINT *)(EMMA_PP_BASE+0x10))	//  32bit pp source CB data ptr reg
#define EMMA_PP_SCR_PTR	(*(volatile UINT *)(EMMA_PP_BASE+0x14))	//  32bit pp source CR data ptr reg
#define EMMA_PP_DRGB_PTR	(*(volatile UINT *)(EMMA_PP_BASE+0x18))	//  32bit pp dest RGB data ptr reg
#define EMMA_PP_QUAN_PTR	(*(volatile UINT *)(EMMA_PP_BASE+0x1C))	//  32bit pp quantizer data ptr reg
#define EMMA_PP_PROC_PARA	(*(volatile UINT *)(EMMA_PP_BASE+0x20))	//  32bit pp process frame param reg
#define EMMA_PP_SFRM_WIDTH	(*(volatile UINT *)(EMMA_PP_BASE+0x24))	//  32bit pp source frame width reg
#define EMMA_PP_DDIS_WIDTH	(*(volatile UINT *)(EMMA_PP_BASE+0x28))	//  32bit pp destinatn display siz reg
#define EMMA_PP_DIMAGE_SIZE	(*(volatile UINT *)(EMMA_PP_BASE+0x2C))	//  32bit pp destinatn image size reg
#define EMMA_PP_DPIX_FMT	(*(volatile UINT *)(EMMA_PP_BASE+0x30))	//  32bit pp dest pixel format ctr reg
#define EMMA_PP_RSIZE_IDX	(*(volatile UINT *)(EMMA_PP_BASE+0x34))	//  32bit pp resize table index reg
#define EMMA_PP_LOCK_BIT	(*(volatile UINT *)(EMMA_PP_BASE+0x38))	//  32bit pp lock bit reg
#define EMMA_PP_RSIZE_COEF	(*(volatile UINT *)(EMMA_PP_BASE+0x100))	//  32bit pp resize coef table reg
		
#define EMMA_PRP_CNTL	(*(volatile UINT *)(EMMA_PRP_BASE+0x00))	//  32bit preprocessor control reg
#define EMMA_PRP_INTRCTRL	(*(volatile UINT *)(EMMA_PRP_BASE+0x04))	//  32bit prp interrupt enable reg
#define EMMA_PRP_INTRSTATUS	(*(volatile UINT *)(EMMA_PRP_BASE+0x08))	//  32bit prp interrupt status reg
#define EMMA_PRP_SY_PTR	(*(volatile UINT *)(EMMA_PRP_BASE+0x0C))	//  32bit prp source Y data ptr reg
#define EMMA_PRP_SCB_PTR	(*(volatile UINT *)(EMMA_PRP_BASE+0x10))	//  32bit prp source CB data ptr reg
#define EMMA_PRP_SCR_PTR	(*(volatile UINT *)(EMMA_PRP_BASE+0x14))	//  32bit prp source CR data ptr reg
#define EMMA_PRP_DRGB1_PTR	(*(volatile UINT *)(EMMA_PRP_BASE+0x18))	//  32bit prp dest RGB1 data ptr reg
#define EMMA_PRP_DRGB2_PTR	(*(volatile UINT *)(EMMA_PRP_BASE+0x1C))	//  32bit prp dest RGB2 data ptr reg
#define EMMA_PRP_DY_PTR	(*(volatile UINT *)(EMMA_PRP_BASE+0x20))	//  32bit prp dest Y data ptr reg
#define EMMA_PRP_DCB_PTR	(*(volatile UINT *)(EMMA_PRP_BASE+0x24))	//  32bit prp dest CB data ptr reg
#define EMMA_PRP_DCR_PTR	(*(volatile UINT *)(EMMA_PRP_BASE+0x28))	//  32bit prp dest CR data ptr reg
#define EMMA_PRP_SFRM_SIZE	(*(volatile UINT *)(EMMA_PRP_BASE+0x2C))	//  32bit prp source frame size reg
#define EMMA_PRP_DLST_CH1	(*(volatile UINT *)(EMMA_PRP_BASE+0x30))	//  32bit prp dest line stride reg
#define EMMA_PRP_SPIX_FMT	(*(volatile UINT *)(EMMA_PRP_BASE+0x34))	//  32bit prp source pixel format ctrl
#define EMMA_PRP_DPIX_FMT	(*(volatile UINT *)(EMMA_PRP_BASE+0x38))	//  32bit prp dest pixel format reg
#define EMMA_PRP_DISIZE_CH1	(*(volatile UINT *)(EMMA_PRP_BASE+0x3C))	//  32bit prp dest ch1 o/p image size
#define EMMA_PRP_DISIZE_CH2	(*(volatile UINT *)(EMMA_PRP_BASE+0x40))	//  32bit prp dest ch2 o/p image size
#define EMMA_PRP_DFRM_CNT	(*(volatile UINT *)(EMMA_PRP_BASE+0x44))	//  32bit prp dest frame count reg
#define EMMA_PRP_SLIN_STRID	(*(volatile UINT *)(EMMA_PRP_BASE+0x48))	//  32bit prp source line stride reg
#define EMMA_PRP_RSIZE_CTRL	(*(volatile UINT *)(EMMA_PRP_BASE+0x4C))	//  32bit prp resize control reg
#define EMMA_PRP_LOCK_BIT	(*(volatile UINT *)(EMMA_PRP_BASE+0x50))	//  32bit prp lock bit reg
		
#define EMMA_MPEG4DEC_DEREG	(*(volatile UINT *)(EMMA_DEC_BASE+0x00))
#define EMMA_MPEG4DEC_CTRLBASE	(*(volatile UINT *)(EMMA_DEC_BASE+0x04))
#define EMMA_MPEG4DEC_RLCBASE	(*(volatile UINT *)(EMMA_DEC_BASE+0x0C))
#define EMMA_MPEG4DEC_MCDOBASE	(*(volatile UINT *)(EMMA_DEC_BASE+0x14))
#define EMMA_MPEG4DEC_MCDIBASE	(*(volatile UINT *)(EMMA_DEC_BASE+0x18))
#define EMMA_MPEG4DEC_IDREG	(*(volatile UINT *)(EMMA_DEC_BASE+0x1C))
		
#define EMMA_MPEG4ENC_CONTREG0	(*(volatile UINT *)(EMMA_ENC_BASE+0x00))
#define EMMA_MPEG4ENC_CONTREG1	(*(volatile UINT *)(EMMA_ENC_BASE+0x04))
#define EMMA_MPEG4ENC_CONTREG2	(*(volatile UINT *)(EMMA_ENC_BASE+0x08))
#define EMMA_MPEG4ENC_CONTREG3	(*(volatile UINT *)(EMMA_ENC_BASE+0x0C))
#define EMMA_MPEG4ENC_CONTREG4	(*(volatile UINT *)(EMMA_ENC_BASE+0x10))
#define EMMA_MPEG4ENC_INTERRUPT	(*(volatile UINT *)(EMMA_ENC_BASE+0x14))
#define EMMA_MPEG4ENC_NBLUMBASE	(*(volatile UINT *)(EMMA_ENC_BASE+0x18))
#define EMMA_MPEG4ENC_IDREG	(*(volatile UINT *)(EMMA_ENC_BASE+0x1C))
#define EMMA_MPEG4ENC_NBCHBASE	(*(volatile UINT *)(EMMA_ENC_BASE+0x20))
#define EMMA_MPEG4ENC_MBTYPEBASE	(*(volatile UINT *)(EMMA_ENC_BASE+0x24))
#define EMMA_MPEG4ENC_VLCCTRBASE	(*(volatile UINT *)(EMMA_ENC_BASE+0x28))
#define EMMA_MPEG4ENC_VLCDABASE	(*(volatile UINT *)(EMMA_ENC_BASE+0x2C))
#define EMMA_MPEG4ENC_SARLUMBASE	(*(volatile UINT *)(EMMA_ENC_BASE+0x30))
#define EMMA_MPEG4ENC_SARCHBASE	(*(volatile UINT *)(EMMA_ENC_BASE+0x34))
#define EMMA_MPEG4ENC_SAWLUMBASE	(*(volatile UINT *)(EMMA_ENC_BASE+0x38))
#define EMMA_MPEG4ENC_SAWCHBASE	(*(volatile UINT *)(EMMA_ENC_BASE+0x3C))
		
//#########################################		
//# Clock  Reset (*(volatile UINT *)(CRM))                   #		
//# System control                        #		
//# $1002_7000 to $1002_7FFF              #		
//#########################################		
#define CRM_BASE_ADDR	0x10027000	
#define CRM_CSCR	(*(volatile UINT *)(CRM_BASE_ADDR+0x00))	//  32bit Clock Source Control Reg
#define CRM_MPCTL0	(*(volatile UINT *)(CRM_BASE_ADDR+0x04))	//  32bit MCU PLL Control Reg
#define CRM_MPCTL1	(*(volatile UINT *)(CRM_BASE_ADDR+0x08))	//  32bit MCU PLL 
#define CRM_SPCTL0	(*(volatile UINT *)(CRM_BASE_ADDR+0x0C))	//  32bit Serial Perpheral PLL Ctrl 0
#define CRM_SPCTL1	(*(volatile UINT *)(CRM_BASE_ADDR+0x10))	//  32bit Serial Perpheral PLL Ctrl 1
#define CRM_OSC26MCTL	(*(volatile UINT *)(CRM_BASE_ADDR+0x14))	//  32bit Osc 26M register
#define CRM_PCDR0	(*(volatile UINT *)(CRM_BASE_ADDR+0x18))	//  32bit Serial Perpheral Clk Div Reg
#define CRM_PCDR1	(*(volatile UINT *)(CRM_BASE_ADDR+0x1C))	//  32bit Serial Perpheral Clk Div Reg
#define CRM_PCCR0	(*(volatile UINT *)(CRM_BASE_ADDR+0x20))	//  32bit Perpheral Clk Control Reg 0
#define CRM_PCCR1	(*(volatile UINT *)(CRM_BASE_ADDR+0x24))	//  32bit Perpheral Clk Control Reg 1
#define CRM_CCSR	(*(volatile UINT *)(CRM_BASE_ADDR+0x28))	
#define CRM_PMCTL	(*(volatile UINT *)(CRM_BASE_ADDR+0x2c))	
#define CRM_PMCOUNT	(*(volatile UINT *)(CRM_BASE_ADDR+0x30))	
#define CRM_WKGDCTL	(*(volatile UINT *)(CRM_BASE_ADDR+0x34))	


		
#define CRM_RSR	(*(volatile UINT *)(CRM_BASE_ADDR+0x800))	//  32bit Reset Source Reg
		
#define SYS_BASE_ADDR	0x10027800	
#define SYS_SIDR	(*(volatile UINT *)(SYS_BASE_ADDR+0x04))	//  128bit Silicon ID Reg
#define SYS_SIDR1	(*(volatile UINT *)(SYS_BASE_ADDR+0x04))	//  128bit Silicon ID Reg word 1
#define SYS_SIDR2	(*(volatile UINT *)(SYS_BASE_ADDR+0x08))	//  128bit Silicon ID Reg word 2
#define SYS_SIDR3	(*(volatile UINT *)(SYS_BASE_ADDR+0x0C))	//  128bit Silicon ID Reg word 3
#define SYS_SIDR4	(*(volatile UINT *)(SYS_BASE_ADDR+0x10))	//  128bit Silicon ID Reg word 4
#define SYS_FMCR	(*(volatile UINT *)(SYS_BASE_ADDR+0x14))	//  Functional Muxing Control Reg
#define SYS_GPCR	(*(volatile UINT *)(SYS_BASE_ADDR+0x18))	//  Global Peripheral Control Reg
#define SYS_WBCR	(*(volatile UINT *)(SYS_BASE_ADDR+0x1C))	//  Well Bias Control Reg
#define SYS_DSCR1	(*(volatile UINT *)(SYS_BASE_ADDR+0x20))	//  Drive Strength Crtl Reg 1
#define SYS_DSCR2	(*(volatile UINT *)(SYS_BASE_ADDR+0x24))	//  Drive Strength Crtl Reg 2
#define SYS_DSCR3	(*(volatile UINT *)(SYS_BASE_ADDR+0x28))	//  Drive Strength Crtl Reg 3
#define SYS_DSCR4	(*(volatile UINT *)(SYS_BASE_ADDR+0x2C))	//  Drive Strength Crtl Reg 4
#define SYS_DSCR5	(*(volatile UINT *)(SYS_BASE_ADDR+0x30))	//  Drive Strength Crtl Reg 5
#define SYS_DSCR6	(*(volatile UINT *)(SYS_BASE_ADDR+0x34))	//  Drive Strength Crtl Reg 6
#define SYS_DSCR7	(*(volatile UINT *)(SYS_BASE_ADDR+0x38))	//  Drive Strength Crtl Reg 7
#define SYS_DSCR8	(*(volatile UINT *)(SYS_BASE_ADDR+0x3C))	//  Drive Strength Crtl Reg 8
#define SYS_DSCR9	(*(volatile UINT *)(SYS_BASE_ADDR+0x40))	//  Drive Strength Crtl Reg 9
#define SYS_DSCR10	(*(volatile UINT *)(SYS_BASE_ADDR+0x44))	//  Drive Strength Crtl Reg 10
#define SYS_DSCR11	(*(volatile UINT *)(SYS_BASE_ADDR+0x48))	//  Drive Strength Crtl Reg 11
#define SYS_DSCR12	(*(volatile UINT *)(SYS_BASE_ADDR+0x4C))	//  Drive Strength Crtl Reg 12
#define SYS_PSCR	(*(volatile UINT *)(SYS_BASE_ADDR+0x50))	//  Priority Control/select Reg
		
//#########################################		
//# FIRI                                  #		
//# $1002_8000 to $1002_8FFF              #		
//#########################################		
#define FIRI_BASE_ADDR	0x10028000	
#define FIRI_FIRITCR	(*(volatile UINT *)(FIRI_BASE_ADDR+0x00))	//  32bit firi tx control reg 
#define FIRI_FIRITCTR	(*(volatile UINT *)(FIRI_BASE_ADDR+0x04))	//  32bit firi tx count  reg
#define FIRI_FIRIRCR	(*(volatile UINT *)(FIRI_BASE_ADDR+0x08))	//  32bit firi rx control reg
#define FIRI_FIRITSR	(*(volatile UINT *)(FIRI_BASE_ADDR+0x0C))	//  32bit firi tx status reg
#define FIRI_FIRIRSR	(*(volatile UINT *)(FIRI_BASE_ADDR+0x10))	//  32bit firi rx status reg
#define FIRI_TFIFO	(*(volatile UINT *)(FIRI_BASE_ADDR+0x14))	//  32bit firi tx fifo reg
#define FIRI_RFIFO	(*(volatile UINT *)(FIRI_BASE_ADDR+0x18))	//  32bit firi rx fifo reg
#define FIRI_FIRICR	(*(volatile UINT *)(FIRI_BASE_ADDR+0x1C))	//  32bit firi control reg
		
		
		
		
//#########################################		
//# JAM                                   #		
//# $1003_E000 to $1003_EFFF              #		
//#########################################		
#define JAM_BASE_ADDR	0x1003E000	
#define JAM_ARM9P_GPR0	(*(volatile UINT *)(JAM_BASE_ADDR+0x00))	//  32bit jam debug enable
#define JAM_ARM9P_GPR4	(*(volatile UINT *)(JAM_BASE_ADDR+0x10))	//  32bit jam platform version
		
//#########################################		
//# MAX                                   #		
//# $1003_F000 to $1003_FFFF              #		
//#########################################		
#define MAX_BASE_ADDR	0x1003F000	
#define MAX_SLV0_BASE	(*(volatile UINT *)(MAX_BASE_ADDR+0x000))	//  base location for slave 0
#define MAX_SLV1_BASE	(*(volatile UINT *)(MAX_BASE_ADDR+0x100))	//  base location for slave 1
#define MAX_SLV2_BASE	(*(volatile UINT *)(MAX_BASE_ADDR+0x200))	//  base location for slave 2
#define MAX_SLV3_BASE	(*(volatile UINT *)(MAX_BASE_ADDR+0x300))	//  base location for slave 3
		
#define MAX_SLV0_MPR0	(*(volatile UINT *)(MAX_SLV0_BASE+0x00))	//  32bit max slv0 master priority reg
#define MAX_SLV0_AMPR0	(*(volatile UINT *)(MAX_SLV0_BASE+0x04))	//  32bit max slv0 alt priority reg
#define MAX_SLV0_SGPCR0	(*(volatile UINT *)(MAX_SLV0_BASE+0x10))	//  32bit max slv0 general ctrl reg
#define MAX_SLV0_ASGPCR0	(*(volatile UINT *)(MAX_SLV0_BASE+0x14))	//  32bit max slv0 alt generl ctrl reg
		
#define MAX_SLV1_MPR1	(*(volatile UINT *)(MAX_SLV1_BASE+0x00))	//  32bit max slv1 master priority reg
#define MAX_SLV1_AMPR1	(*(volatile UINT *)(MAX_SLV1_BASE+0x04))	//  32bit max slv1 alt priority reg
#define MAX_SLV1_SGPCR1	(*(volatile UINT *)(MAX_SLV1_BASE+0x10))	//  32bit max slv1 general ctrl reg
#define MAX_SLV1_ASGPCR1	(*(volatile UINT *)(MAX_SLV1_BASE+0x14))	//  32bit max slv1 alt generl ctrl reg
		
#define MAX_SLV2_MPR2	(*(volatile UINT *)(MAX_SLV2_BASE+0x00))	//  32bit max slv2 master priority reg
#define MAX_SLV2_AMPR2	(*(volatile UINT *)(MAX_SLV2_BASE+0x04))	//  32bit max slv2 alt priority reg
#define MAX_SLV2_SGPCR2	(*(volatile UINT *)(MAX_SLV2_BASE+0x10))	//  32bit max slv2 general ctrl reg
#define MAX_SLV2_ASGPCR2	(*(volatile UINT *)(MAX_SLV2_BASE+0x14))	//  32bit max slv2 alt generl ctrl reg
		
#define MAX_SLV3_MPR3	(*(volatile UINT *)(MAX_SLV3_BASE+0x00))	//  32bit max slv3 master priority reg
#define MAX_SLV3_AMPR3	(*(volatile UINT *)(MAX_SLV3_BASE+0x04))	//  32bit max slv3 alt priority reg
#define MAX_SLV3_SGPCR3	(*(volatile UINT *)(MAX_SLV3_BASE+0x10))	//  32bit max slv3 general ctrl reg
#define MAX_SLV3_ASGPCR3	(*(volatile UINT *)(MAX_SLV3_BASE+0x14))	//  32bit max slv3 alt generl ctrl reg
		
#define MAX_MST0_MGPCR0	(*(volatile UINT *)(MAX_BASE_ADDR+0x800))	//  32bit max mst0 general ctrl reg
#define MAX_MST1_MGPCR1	(*(volatile UINT *)(MAX_BASE_ADDR+0x900))	//  32bit max mst1 general ctrl reg
#define MAX_MST2_MGPCR2	(*(volatile UINT *)(MAX_BASE_ADDR+0xA00))	//  32bit max mst2 general ctrl reg
#define MAX_MST3_MGPCR3	(*(volatile UINT *)(MAX_BASE_ADDR+0xB00))	//  32bit max mst3 general ctrl reg
#define MAX_MST4_MGPCR4	(*(volatile UINT *)(MAX_BASE_ADDR+0xC00))	//  32bit max mst4 general ctrl reg
#define MAX_MST5_MGPCR5	(*(volatile UINT *)(MAX_BASE_ADDR+0xD00))	//  32bit max mst5 general ctrl reg
		
		
		
		
//#########################################		
//# AITC                                  #		
//# $1004_0000 to $1004_0FFF              #		
//#########################################		
#define AITC_BASE_ADDR	0x10040000	
#define AITC_INTCNTL	(*(volatile UINT *)(AITC_BASE_ADDR+0x00))	//  32bit aitc int control reg
#define AITC_NIMASK	(*(volatile UINT *)(AITC_BASE_ADDR+0x04))	//  32bit aitc int mask reg
#define AITC_INTENNUM	(*(volatile UINT *)(AITC_BASE_ADDR+0x08))	//  32bit aitc int enable number reg
#define AITC_INTDISNUM	(*(volatile UINT *)(AITC_BASE_ADDR+0x0C))	//  32bit aitc int disable number reg
#define AITC_INTENABLEH	(*(volatile UINT *)(AITC_BASE_ADDR+0x10))	//  32bit aitc int enable reg high
#define AITC_INTENABLEL	(*(volatile UINT *)(AITC_BASE_ADDR+0x14))	//  32bit aitc int enable reg low
#define AITC_INTTYPEH	(*(volatile UINT *)(AITC_BASE_ADDR+0x18))	//  32bit aitc int type reg high
#define AITC_INTTYPEL	(*(volatile UINT *)(AITC_BASE_ADDR+0x1C))	//  32bit aitc int type reg low
#define AITC_NIPRIORITY7	(*(volatile UINT *)(AITC_BASE_ADDR+0x20))	//  32bit aitc norm int priority lvl7
#define AITC_NIPRIORITY6	(*(volatile UINT *)(AITC_BASE_ADDR+0x24))	//  32bit aitc norm int priority lvl6
#define AITC_NIPRIORITY5	(*(volatile UINT *)(AITC_BASE_ADDR+0x28))	//  32bit aitc norm int priority lvl5
#define AITC_NIPRIORITY4	(*(volatile UINT *)(AITC_BASE_ADDR+0x2C))	//  32bit aitc norm int priority lvl4
#define AITC_NIPRIORITY3	(*(volatile UINT *)(AITC_BASE_ADDR+0x30))	//  32bit aitc norm int priority lvl3
#define AITC_NIPRIORITY2	(*(volatile UINT *)(AITC_BASE_ADDR+0x34))	//  32bit aitc norm int priority lvl2
#define AITC_NIPRIORITY1	(*(volatile UINT *)(AITC_BASE_ADDR+0x38))	//  32bit aitc norm int priority lvl1
#define AITC_NIPRIORITY0	(*(volatile UINT *)(AITC_BASE_ADDR+0x3C))	//  32bit aitc norm int priority lvl0
#define AITC_NIVECSR	(*(volatile UINT *)(AITC_BASE_ADDR+0x40))	//  32bit aitc norm int vector/status
#define AITC_FIVECSR	(*(volatile UINT *)(AITC_BASE_ADDR+0x44))	//  32bit aitc fast int vector/status
#define AITC_INTSRCH	(*(volatile UINT *)(AITC_BASE_ADDR+0x48))	//  32bit aitc int source reg high
#define AITC_INTSRCL	(*(volatile UINT *)(AITC_BASE_ADDR+0x4C))	//  32bit aitc int source reg low
#define AITC_INTFRCH	(*(volatile UINT *)(AITC_BASE_ADDR+0x50))	//  32bit aitc int force reg high
#define AITC_INTFRCL	(*(volatile UINT *)(AITC_BASE_ADDR+0x54))	//  32bit aitc int force reg low
#define AITC_NIPNDH	(*(volatile UINT *)(AITC_BASE_ADDR+0x58))	//  32bit aitc norm int pending high
#define AITC_NIPNDL	(*(volatile UINT *)(AITC_BASE_ADDR+0x5C))	//  32bit aitc norm int pending low
#define AITC_FIPNDH	(*(volatile UINT *)(AITC_BASE_ADDR+0x60))	//  32bit aitc fast int pending high
#define AITC_FIPNDL	(*(volatile UINT *)(AITC_BASE_ADDR+0x64))	//  32bit aitc fast int pending low
		
//#########################################		
//# ROMPATCH                              #		
//# $1004_1000 to $1004_1FFF              #		
//#########################################		
#define ROMPATCH_BASE_ADDR	0x10041000	
#define ROMPATCH_D15	(*(volatile UINT *)(ROMPATCH_BASE_ADDR+0x0B4))	//  32bit rompatch data reg 15
#define ROMPATCH_D14	(*(volatile UINT *)(ROMPATCH_BASE_ADDR+0x0B8))	//  32bit rompatch data reg 14
#define ROMPATCH_D13	(*(volatile UINT *)(ROMPATCH_BASE_ADDR+0x0BC))	//  32bit rompatch data reg 13
#define ROMPATCH_D12	(*(volatile UINT *)(ROMPATCH_BASE_ADDR+0x0C0))	//  32bit rompatch data reg 12
#define ROMPATCH_D11	(*(volatile UINT *)(ROMPATCH_BASE_ADDR+0x0C4))	//  32bit rompatch data reg 11
#define ROMPATCH_D10	(*(volatile UINT *)(ROMPATCH_BASE_ADDR+0x0C8))	//  32bit rompatch data reg 10
#define ROMPATCH_D9	(*(volatile UINT *)(ROMPATCH_BASE_ADDR+0x0CC))	//  32bit rompatch data reg 9
#define ROMPATCH_D8	(*(volatile UINT *)(ROMPATCH_BASE_ADDR+0x0D0))	//  32bit rompatch data reg 8
#define ROMPATCH_D7	(*(volatile UINT *)(ROMPATCH_BASE_ADDR+0x0D4))	//  32bit rompatch data reg 7
#define ROMPATCH_D6	(*(volatile UINT *)(ROMPATCH_BASE_ADDR+0x0D8))	//  32bit rompatch data reg 6
#define ROMPATCH_D5	(*(volatile UINT *)(ROMPATCH_BASE_ADDR+0x0DC))	//  32bit rompatch data reg 5
#define ROMPATCH_D4	(*(volatile UINT *)(ROMPATCH_BASE_ADDR+0x0E0))	//  32bit rompatch data reg 4
#define ROMPATCH_D3	(*(volatile UINT *)(ROMPATCH_BASE_ADDR+0x0E4))	//  32bit rompatch data reg 3
#define ROMPATCH_D2	(*(volatile UINT *)(ROMPATCH_BASE_ADDR+0x0E8))	//  32bit rompatch data reg 2
#define ROMPATCH_D1	(*(volatile UINT *)(ROMPATCH_BASE_ADDR+0x0EC))	//  32bit rompatch data reg 1
#define ROMPATCH_D0	(*(volatile UINT *)(ROMPATCH_BASE_ADDR+0x0F0))	//  32bit rompatch data reg 0
#define ROMPATCH_CNTL	(*(volatile UINT *)(ROMPATCH_BASE_ADDR+0x0F4))	//  32bit rompatch control reg
#define ROMPATCH_ENH	(*(volatile UINT *)(ROMPATCH_BASE_ADDR+0x0F8))	//  32bit rompatch enable reg high
#define ROMPATCH_ENL	(*(volatile UINT *)(ROMPATCH_BASE_ADDR+0x0FC))	//  32bit rompatch enable reg low
#define ROMPATCH_A0	(*(volatile UINT *)(ROMPATCH_BASE_ADDR+0x100))	//  32bit rompatch addr reg 0
#define ROMPATCH_A1	(*(volatile UINT *)(ROMPATCH_BASE_ADDR+0x104))	//  32bit rompatch addr reg 1
#define ROMPATCH_A2	(*(volatile UINT *)(ROMPATCH_BASE_ADDR+0x108))	//  32bit rompatch addr reg 2
#define ROMPATCH_A3	(*(volatile UINT *)(ROMPATCH_BASE_ADDR+0x10C))	//  32bit rompatch addr reg 3
#define ROMPATCH_A4	(*(volatile UINT *)(ROMPATCH_BASE_ADDR+0x110))	//  32bit rompatch addr reg 4
#define ROMPATCH_A5	(*(volatile UINT *)(ROMPATCH_BASE_ADDR+0x114))	//  32bit rompatch addr reg 5
#define ROMPATCH_A6	(*(volatile UINT *)(ROMPATCH_BASE_ADDR+0x118))	//  32bit rompatch addr reg 6
#define ROMPATCH_A7	(*(volatile UINT *)(ROMPATCH_BASE_ADDR+0x11C))	//  32bit rompatch addr reg 7
#define ROMPATCH_A8	(*(volatile UINT *)(ROMPATCH_BASE_ADDR+0x120))	//  32bit rompatch addr reg 8
#define ROMPATCH_A9	(*(volatile UINT *)(ROMPATCH_BASE_ADDR+0x124))	//  32bit rompatch addr reg 9
#define ROMPATCH_A10	(*(volatile UINT *)(ROMPATCH_BASE_ADDR+0x128))	//  32bit rompatch addr reg 10
#define ROMPATCH_A11	(*(volatile UINT *)(ROMPATCH_BASE_ADDR+0x12C))	//  32bit rompatch addr reg 11
#define ROMPATCH_A12	(*(volatile UINT *)(ROMPATCH_BASE_ADDR+0x130))	//  32bit rompatch addr reg 12
#define ROMPATCH_A13	(*(volatile UINT *)(ROMPATCH_BASE_ADDR+0x134))	//  32bit rompatch addr reg 13
#define ROMPATCH_A14	(*(volatile UINT *)(ROMPATCH_BASE_ADDR+0x138))	//  32bit rompatch addr reg 14
#define ROMPATCH_A15	(*(volatile UINT *)(ROMPATCH_BASE_ADDR+0x13C))	//  32bit rompatch addr reg 15
#define ROMPATCH_A16	(*(volatile UINT *)(ROMPATCH_BASE_ADDR+0x140))	//  32bit rompatch addr reg 16
#define ROMPATCH_A17	(*(volatile UINT *)(ROMPATCH_BASE_ADDR+0x144))	//  32bit rompatch addr reg 17
#define ROMPATCH_A18	(*(volatile UINT *)(ROMPATCH_BASE_ADDR+0x148))	//  32bit rompatch addr reg 18
#define ROMPATCH_A19	(*(volatile UINT *)(ROMPATCH_BASE_ADDR+0x14C))	//  32bit rompatch addr reg 19
#define ROMPATCH_A20	(*(volatile UINT *)(ROMPATCH_BASE_ADDR+0x150))	//  32bit rompatch addr reg 20
#define ROMPATCH_A21	(*(volatile UINT *)(ROMPATCH_BASE_ADDR+0x154))	//  32bit rompatch addr reg 21
#define ROMPATCH_A22	(*(volatile UINT *)(ROMPATCH_BASE_ADDR+0x158))	//  32bit rompatch addr reg 22
#define ROMPATCH_A23	(*(volatile UINT *)(ROMPATCH_BASE_ADDR+0x15C))	//  32bit rompatch addr reg 23
#define ROMPATCH_A24	(*(volatile UINT *)(ROMPATCH_BASE_ADDR+0x160))	//  32bit rompatch addr reg 24
#define ROMPATCH_A25	(*(volatile UINT *)(ROMPATCH_BASE_ADDR+0x164))	//  32bit rompatch addr reg 25
#define ROMPATCH_A26	(*(volatile UINT *)(ROMPATCH_BASE_ADDR+0x168))	//  32bit rompatch addr reg 26
#define ROMPATCH_A27	(*(volatile UINT *)(ROMPATCH_BASE_ADDR+0x16C))	//  32bit rompatch addr reg 27
#define ROMPATCH_A28	(*(volatile UINT *)(ROMPATCH_BASE_ADDR+0x170))	//  32bit rompatch addr reg 28
#define ROMPATCH_A29	(*(volatile UINT *)(ROMPATCH_BASE_ADDR+0x174))	//  32bit rompatch addr reg 29
#define ROMPATCH_A30	(*(volatile UINT *)(ROMPATCH_BASE_ADDR+0x178))	//  32bit rompatch addr reg 30
#define ROMPATCH_A31	(*(volatile UINT *)(ROMPATCH_BASE_ADDR+0x17C))	//  32bit rompatch addr reg 31
#define ROMPATCH_BRPT	(*(volatile UINT *)(ROMPATCH_BASE_ADDR+0x200))	//  32bit rompatch 
#define ROMPATCH_BASE	(*(volatile UINT *)(ROMPATCH_BASE_ADDR+0x204))	//  32bit rompatch base addr reg
#define ROMPATCH_SR	(*(volatile UINT *)(ROMPATCH_BASE_ADDR+0x208))	//  32bit rompatch status reg
#define ROMPATCH_ABSR	(*(volatile UINT *)(ROMPATCH_BASE_ADDR+0x20C))	//  32bit rompatch abort status reg
#define ROMPATCH_DADR	(*(volatile UINT *)(ROMPATCH_BASE_ADDR+0x210))	//  32bit rompatch d-ahb addr abort
#define ROMPATCH_IADR	(*(volatile UINT *)(ROMPATCH_BASE_ADDR+0x214))	//  32bit rompatch i-ahb addr abort
		
		
//#########################################		
//# SMN                                   #		
//# $1004_2000 to $1004_2FFF              #		
//#########################################		
#define SMN_BASE_ADDR	0x10042000	
#define SMN_STATUS	(*(volatile UINT *)(SMN_BASE_ADDR+0x00))	//  32bit SMN status reg
#define SMN_CONTROL	(*(volatile UINT *)(SMN_BASE_ADDR+0x04))	//  32bit SMN command reg
#define SMN_SEQ_START	(*(volatile UINT *)(SMN_BASE_ADDR+0x08))	//  32bit SMN sequence start reg
#define SMN_SEQ_END	(*(volatile UINT *)(SMN_BASE_ADDR+0x0C))	//  32bit SMN sequence end reg
#define SMN_SEQ_CHK	(*(volatile UINT *)(SMN_BASE_ADDR+0x10))	//  32bit SMN sequence check reg
#define SMN_BIT_CNT	(*(volatile UINT *)(SMN_BASE_ADDR+0x14))	//  32bit SMN bit count reg
#define SMN_INC_SIZE	(*(volatile UINT *)(SMN_BASE_ADDR+0x18))	//  32bit SMN increment size reg
#define SMN_BB_DEC	(*(volatile UINT *)(SMN_BASE_ADDR+0x1C))	//  32bit SMN bit bank decrement reg
#define SMN_COMP_SIZE	(*(volatile UINT *)(SMN_BASE_ADDR+0x20))	//  32bit SMN compare size reg
#define SMN_PT_CHK	(*(volatile UINT *)(SMN_BASE_ADDR+0x24))	//  32bit SMN plain text check reg
#define SMN_CT_CHK	(*(volatile UINT *)(SMN_BASE_ADDR+0x28))	//  32bit SMN cipher text check reg
#define SMN_TIMER_IV	(*(volatile UINT *)(SMN_BASE_ADDR+0x2C))	//  32bit SMN timer initial value reg
#define SMN_TIMER_CTL	(*(volatile UINT *)(SMN_BASE_ADDR+0x30))	//  32bit SMN timer control reg
#define SMN_DD_STATUS	(*(volatile UINT *)(SMN_BASE_ADDR+0x34))	//  32bit SMN debug detector reg
#define SMN_TIMER	(*(volatile UINT *)(SMN_BASE_ADDR+0x38))	//  32bit SMN timer reg
		
		
//#########################################		
//# SCM                                   #		
//# $1004_3000 to $1004_3FFF              #		
//#########################################		
#define SCM_BASE_ADDR	0x10043000	
#define SCM_RED_START	(*(volatile UINT *)(SCM_BASE_ADDR+0x00))	//  32bit SCM red memory start addr
#define SCM_BLACK_START	(*(volatile UINT *)(SCM_BASE_ADDR+0x004))	//  32bit SCM black memory start addr
#define SCM_LENGTH	(*(volatile UINT *)(SCM_BASE_ADDR+0x008))	//  32bit SCM num blks encrypted
#define SCM_CONTROL	(*(volatile UINT *)(SCM_BASE_ADDR+0x00C))	//  32bit SCM control reg
#define SCM_STATUS	(*(volatile UINT *)(SCM_BASE_ADDR+0x010))	//  32bit SCM status reg
#define SCM_ERROR	(*(volatile UINT *)(SCM_BASE_ADDR+0x014))	//  32bit SCM error status reg
#define SCM_INT_MASK	(*(volatile UINT *)(SCM_BASE_ADDR+0x018))	//  32bit SCM interrupt control reg
#define SCM_CONFIGURATION	(*(volatile UINT *)(SCM_BASE_ADDR+0x01C))	//  32bit SCM configuration
#define SCM_INIT_VECTOR_0	(*(volatile UINT *)(SCM_BASE_ADDR+0x020))	//  32bit SCM initialization vector high
#define SCM_INIT_VECTOR_1	(*(volatile UINT *)(SCM_BASE_ADDR+0x024))	//  32bit SCM initialization vector low
#define SCM_RED_MEM_BASE	(*(volatile UINT *)(SCM_BASE_ADDR+0x400))	//  32bit Red memory regs (*(volatile UINT *)(400 - 7FF))
#define SCM_BLACK_MEM_BASE	(*(volatile UINT *)(SCM_BASE_ADDR+0x800))	//  32bit Black memory regs (*(volatile UINT *)(800 - BFF)) 
		
		
		
//#########################################		
//# CSI                                   #		
//# $8000_0000 to $8000_0FFF              #		
//#########################################		
#define CSI_BASE_ADDR	0x80000000	
#define CSI_CSICR1	(*(volatile UINT *)(CSI_BASE_ADDR+0x00))	//  32bit csi control 1 reg
#define CSI_CSICR2	(*(volatile UINT *)(CSI_BASE_ADDR+0x04))	//  32bit csi control 2 reg
#define CSI_CSICR3	(*(volatile UINT *)(CSI_BASE_ADDR+0x1c))	//  32bit csi control 2 reg
#define CSI_CSISR	(*(volatile UINT *)(CSI_BASE_ADDR+0x08))	//  32bit csi status reg
#define CSI_CSISTATFIFO	(*(volatile UINT *)(CSI_BASE_ADDR+0x0C))	//  32bit csi fifo statistics reg
#define CSI_CSIRXFIFO	(*(volatile UINT *)(CSI_BASE_ADDR+0x10))	//  32bit csi receive image reg
#define CSI_CSIRXCNT	(*(volatile UINT *)(CSI_BASE_ADDR+0x14))	//  32bit csi receive count reg
		
		
		
//#########################################		
//# BMI                                   #		
//# $A000_0000 to $A000_0FFF              #		
//#########################################		
#define BMI_BASE_ADDR	0xA0000000	
#define BMI_BMICTRL1	(*(volatile UINT *)(BMI_BASE_ADDR+0x00))	//  32bit bmi control 1 reg
#define BMI_BMICTRL2	(*(volatile UINT *)(BMI_BASE_ADDR+0x04))	//  32bit bmi control 2 reg
#define BMI_BMISTR	(*(volatile UINT *)(BMI_BASE_ADDR+0x08))	//  32bit bmi status reg
#define BMI_BMIRXD	(*(volatile UINT *)(BMI_BASE_ADDR+0x0C))	//  32bit bmi Rx FIFO reg
#define BMI_BMITXD	(*(volatile UINT *)(BMI_BASE_ADDR+0x10))	//  32bit bmi Tx FIFO reg
		
		
//#########################################		
//# External Memory CSD0 CSD1    #		
//# $C000_0000 to $C3FF_FFFF (*(volatile UINT *)(CSD0))       #		
//# $C400_0000 to $C7FF_FFFF (*(volatile UINT *)(CSD1))       #		
//#########################################		
#define CSD0_BASE_ADDR	0xC0000000	//  CSD0 (*(volatile UINT *)(64Mb))
#define CSD0_END_ADDR	(*(volatile UINT *)(CSD0_BASE_ADDR+0x3FFFFFF))	
#define CSD1_BASE_ADDR	0xC4000000	//  CSD1 (*(volatile UINT *)(64Mb))
#define CSD1_END_ADDR	(*(volatile UINT *)(CSD1_BASE_ADDR+0x3FFFFFF))	
		
//#########################################		
//# External Memory CS0 - CS5             #		
//# $C800_0000 to $CBFF_FFFF (*(volatile UINT *)(CS0))        #		
//# $CC00_0000 to $CFFF_FFFF (*(volatile UINT *)(CS1))        #		
//# $D000_0000 to $D0FF_FFFF (*(volatile UINT *)(CS2))        #		
//# $D100_0000 to $D1FF_FFFF (*(volatile UINT *)(CS3))        #		
//# $D200_0000 to $D2FF_FFFF (*(volatile UINT *)(CS4))        #		
//# $D300_0000 to $D3FF_FFFF (*(volatile UINT *)(CS5))        #		
//#########################################		
#define CS0_BASE_ADDR	0xC8000000	//  CS0 (*(volatile UINT *)(64Mb))
#define CS0_END_ADDR	(*(volatile UINT *)(CS0_BASE_ADDR+0x3FFFFFF))	
#define CS1_BASE_ADDR	0xCC000000	//  CS1 (*(volatile UINT *)(64Mb))
#define CS1_END_ADDR	(*(volatile UINT *)(CS1_BASE_ADDR+0x3FFFFFF))	
#define CS2_BASE_ADDR	0xD0000000	//  CS2 (*(volatile UINT *)(16Mb))
#define CS2_END_ADDR	(*(volatile UINT *)(CS2_BASE_ADDR+0x0FFFFFF))	
#define CS3_BASE_ADDR	0xD1000000	//  CS3 (*(volatile UINT *)(16Mb))
#define CS3_END_ADDR	(*(volatile UINT *)(CS3_BASE_ADDR+0x0FFFFFF))	
#define CS4_BASE_ADDR	0xD2000000	//  CS4 (*(volatile UINT *)(16Mb))
#define CS4_END_ADDR	(*(volatile UINT *)(CS4_BASE_ADDR+0x0FFFFFF))	
#define CS5_BASE_ADDR	0xD3000000	//  CS5 (*(volatile UINT *)(16Mb))
#define CS5_END_ADDR	(*(volatile UINT *)(CS5_BASE_ADDR+0x0FFFFFF))	
		
		
		
//#########################################		
//# External Memory PCMCIA/CF             #		
//# $D400_0000 to $D7FF_FFFF              #		
//#########################################		
#define PCMCF_BASE_ADDR	0xD4000000	//  pcmcia/cf (*(volatile UINT *)(64Mb))
#define PCMCF_END_ADDR	(*(volatile UINT *)(PCMCF_BASE_ADDR+0x7FFFFFF))	
		
		
		
//#########################################		
//# SDRAMC                                #		
//# $DF00_0000 to $DF00_0FFF              #		
//#########################################		
#define SDRAMC_BASE_ADDR	0xDF000000	
#define SDRAMC_SDCTL0	(*(volatile UINT *)(SDRAMC_BASE_ADDR+0x00))	//  32bit sdram 0 control reg
#define SDRAMC_SDCTL1	(*(volatile UINT *)(SDRAMC_BASE_ADDR+0x04))	//  32bit sdram 1 control reg
#define SDRAMC_MISC	(*(volatile UINT *)(SDRAMC_BASE_ADDR+0x14))	//  32bit sdram miscellaneous reg
#define SDRAMC_SDRST	(*(volatile UINT *)(SDRAMC_BASE_ADDR+0x18))	//  32bit sdram reset reg
		
//#########################################		
//# WEIM                                  #		
//# $DF00_1000 to $DF00_1FFF              #		
//#########################################		
#define WEIM_BASE_ADDR	0xDF001000	
#define WEIM_CS0U	(*(volatile UINT *)(WEIM_BASE_ADDR+0x00))	//  32bit eim chip sel 0 upper ctr reg
#define WEIM_CS0L	(*(volatile UINT *)(WEIM_BASE_ADDR+0x04))	//  32bit eim chip sel 0 lower ctr reg
#define WEIM_CS1U	(*(volatile UINT *)(WEIM_BASE_ADDR+0x08))	//  32bit eim chip sel 1 upper ctr reg
#define WEIM_CS1L	(*(volatile UINT *)(WEIM_BASE_ADDR+0x0C))	//  32bit eim chip sel 1 lower ctr reg
#define WEIM_CS2U	(*(volatile UINT *)(WEIM_BASE_ADDR+0x10))	//  32bit eim chip sel 2 upper ctr reg
#define WEIM_CS2L	(*(volatile UINT *)(WEIM_BASE_ADDR+0x14))	//  32bit eim chip sel 2 lower ctr reg
#define WEIM_CS3U	(*(volatile UINT *)(WEIM_BASE_ADDR+0x18))	//  32bit eim chip sel 3 upper ctr reg
#define WEIM_CS3L	(*(volatile UINT *)(WEIM_BASE_ADDR+0x1C))	//  32bit eim chip sel 3 lower ctr reg
#define WEIM_CS4U	(*(volatile UINT *)(WEIM_BASE_ADDR+0x20))	//  32bit eim chip sel 4 upper ctr reg
#define WEIM_CS4L	(*(volatile UINT *)(WEIM_BASE_ADDR+0x24))	//  32bit eim chip sel 4 lower ctr reg
#define WEIM_CS5U	(*(volatile UINT *)(WEIM_BASE_ADDR+0x28))	//  32bit eim chip sel 5 upper ctr reg
#define WEIM_CS5L	(*(volatile UINT *)(WEIM_BASE_ADDR+0x2C))	//  32bit eim chip sel 5 upper ctr reg
#define WEIM_EIM	(*(volatile UINT *)(WEIM_BASE_ADDR+0x30))	//  32bit eim configuration reg
		
//#########################################		
//# PCMCIA                                #		
//# $DF00_2000 to $DF00_2FFF              #		
//#########################################		
#define PCMCIA_BASE_ADDR	0xDF002000	
#define PCMCIA_PIPR	(*(volatile UINT *)(PCMCIA_BASE_ADDR+0x00))	//  32bit pcmcia input pins reg
#define PCMCIA_PSCR	(*(volatile UINT *)(PCMCIA_BASE_ADDR+0x04))	//  32bit pcmcia status change reg
#define PCMCIA_PER	(*(volatile UINT *)(PCMCIA_BASE_ADDR+0x08))	//  32bit pcmcia enable reg
#define PCMCIA_PBR0	(*(volatile UINT *)(PCMCIA_BASE_ADDR+0x0C))	//  32bit pcmcia base reg 0
#define PCMCIA_PBR1	(*(volatile UINT *)(PCMCIA_BASE_ADDR+0x10))	//  32bit pcmcia base reg 1
#define PCMCIA_PBR2	(*(volatile UINT *)(PCMCIA_BASE_ADDR+0x14))	//  32bit pcmcia base reg 2
#define PCMCIA_PBR3	(*(volatile UINT *)(PCMCIA_BASE_ADDR+0x18))	//  32bit pcmcia base reg 3
#define PCMCIA_PBR4	(*(volatile UINT *)(PCMCIA_BASE_ADDR+0x1C))	//  32bit pcmcia base reg 4
#define PCMCIA_PBR5	(*(volatile UINT *)(PCMCIA_BASE_ADDR+0x20))	//  32bit pcmcia base reg 5
#define PCMCIA_PBR6	(*(volatile UINT *)(PCMCIA_BASE_ADDR+0x24))	//  32bit pcmcia base reg 6
#define PCMCIA_POR0	(*(volatile UINT *)(PCMCIA_BASE_ADDR+0x28))	//  32bit pcmcia option reg 0
#define PCMCIA_POR1	(*(volatile UINT *)(PCMCIA_BASE_ADDR+0x2C))	//  32bit pcmcia option reg 1
#define PCMCIA_POR2	(*(volatile UINT *)(PCMCIA_BASE_ADDR+0x30))	//  32bit pcmcia option reg 2
#define PCMCIA_POR3	(*(volatile UINT *)(PCMCIA_BASE_ADDR+0x34))	//  32bit pcmcia option reg 3
#define PCMCIA_POR4	(*(volatile UINT *)(PCMCIA_BASE_ADDR+0x38))	//  32bit pcmcia option reg 4
#define PCMCIA_POR5	(*(volatile UINT *)(PCMCIA_BASE_ADDR+0x3C))	//  32bit pcmcia option reg 5
#define PCMCIA_POR6	(*(volatile UINT *)(PCMCIA_BASE_ADDR+0x40))	//  32bit pcmcia option reg 6
#define PCMCIA_POFR0	(*(volatile UINT *)(PCMCIA_BASE_ADDR+0x44))	//  32bit pcmcia offset reg 0
#define PCMCIA_POFR1	(*(volatile UINT *)(PCMCIA_BASE_ADDR+0x48))	//  32bit pcmcia offset reg 1
#define PCMCIA_POFR2	(*(volatile UINT *)(PCMCIA_BASE_ADDR+0x4C))	//  32bit pcmcia offset reg 2
#define PCMCIA_POFR3	(*(volatile UINT *)(PCMCIA_BASE_ADDR+0x50))	//  32bit pcmcia offset reg 3
#define PCMCIA_POFR4	(*(volatile UINT *)(PCMCIA_BASE_ADDR+0x54))	//  32bit pcmcia offset reg 4
#define PCMCIA_POFR5	(*(volatile UINT *)(PCMCIA_BASE_ADDR+0x58))	//  32bit pcmcia offset reg 5
#define PCMCIA_POFR6	(*(volatile UINT *)(PCMCIA_BASE_ADDR+0x5C))	//  32bit pcmcia offset reg 6
#define PCMCIA_PGCR	(*(volatile UINT *)(PCMCIA_BASE_ADDR+0x60))	//  32bit pcmcia general control reg
#define PCMCIA_PGSR	(*(volatile UINT *)(PCMCIA_BASE_ADDR+0x64))	//  32bit pcmcia general status reg
		
//#########################################		
//# NFC                                   #		
//# $DF00_3000 to $DF00_3FFF              #		
//#########################################		
#define NFC_BASE_ADDR	0xDF003000	
		
#define NFC_MAB0_BASE	(*(volatile UINT *)(NFC_BASE_ADDR+0x000))	//  main area buffer0 (*(volatile UINT *)(3000 - 31FE))
#define NFC_MAB1_BASE	(*(volatile UINT *)(NFC_BASE_ADDR+0x200))	//  main area buffer1 (*(volatile UINT *)(3200 - 33FE))
#define NFC_MAB2_BASE	(*(volatile UINT *)(NFC_BASE_ADDR+0x400))	//  main area buffer2 (*(volatile UINT *)(3400 - 35FE))
#define NFC_MAB3_BASE	(*(volatile UINT *)(NFC_BASE_ADDR+0x600))	//  main area buffer3 (*(volatile UINT *)(3600 - 37FE))
		
#define NFC_SAB0_BASE	(*(volatile UINT *)(NFC_BASE_ADDR+0x800))	//  spare area buffer0 (*(volatile UINT *)(3800 - 380E))
#define NFC_SAB1_BASE	(*(volatile UINT *)(NFC_BASE_ADDR+0x810))	//  spare area buffer1 (*(volatile UINT *)(3810 - 381E))
#define NFC_SAB2_BASE	(*(volatile UINT *)(NFC_BASE_ADDR+0x820))	//  spare area buffer2 (*(volatile UINT *)(3820 - 382E))
#define NFC_SAB3_BASE	(*(volatile UINT *)(NFC_BASE_ADDR+0x830))	//  spare area buffer3 (*(volatile UINT *)(3830 - 383E))
		
#define NFC_SAB0_8BIT_BASE	(*(volatile UINT *)(NFC_BASE_ADDR+0x800))	//  spare area buffer0 for 8 bit(*(volatile UINT *)(3800 - 380E))
#define NFC_SAB1_8BIT_BASE	(*(volatile UINT *)(NFC_BASE_ADDR+0x810))	//  spare area buffer1 for 8 bit(*(volatile UINT *)(3810 - 381E))
#define NFC_SAB2_8BIT_BASE	(*(volatile UINT *)(NFC_BASE_ADDR+0x820))	//  spare area buffer2 for 8 bit(*(volatile UINT *)(3820 - 382E))
#define NFC_SAB3_8BIT_BASE	(*(volatile UINT *)(NFC_BASE_ADDR+0x830))	//  spare area buffer3 for 8 bit(*(volatile UINT *)(3830 - 383E))
		
#define NFC_SAB0_16BIT_BASE	(*(volatile UINT *)(NFC_BASE_ADDR+0x800))	//  spare area buffer0 for 16 bit(*(volatile UINT *)(3800 - 380E))
#define NFC_SAB1_16BIT_BASE	(*(volatile UINT *)(NFC_BASE_ADDR+0x810))	//  spare area buffer1 for 16 bit(*(volatile UINT *)(3810 - 381E))
#define NFC_SAB2_16BIT_BASE	(*(volatile UINT *)(NFC_BASE_ADDR+0x820))	//  spare area buffer2 for 16 bit(*(volatile UINT *)(3820 - 382E))
#define NFC_SAB3_16BIT_BASE	(*(volatile UINT *)(NFC_BASE_ADDR+0x830))	//  spare area buffer3 for 16 bit(*(volatile UINT *)(3830 - 383E))
		
#define NFC_REG_BASE	(*(volatile UINT *)(NFC_BASE_ADDR+0xE00))	//  register area (*(volatile UINT *)(3E00 - 3E1C))
#define NFC_BUFSIZE	(*(volatile UINT *)(NFC_REG_BASE+0x00))	//  16bit nfc internal sram size
#define NFC_BLK_ADD_LOCK	(*(volatile UINT *)(NFC_REG_BASE+0x02))	//  16bit nfc block addr for lock chk
#define NFC_RAM_BUF_ADDR	(*(volatile UINT *)(NFC_REG_BASE+0x04))	//  16bit nfc buffer number
#define NFC_NAND_FLASH_ADDR	(*(volatile UINT *)(NFC_REG_BASE+0x06))	//  16bit nfc nand flash address
#define NFC_NAND_FLASH_CMD	(*(volatile UINT *)(NFC_REG_BASE+0x08))	//  16bit nfc nand flash command
#define NFC_CONFIGURATION	(*(volatile UINT *)(NFC_REG_BASE+0x0A))	//  16bit nfc internal buf lock ctrl
#define NFC_ECC_STAT_RES	(*(volatile UINT *)(NFC_REG_BASE+0x0C))	//  16bit nfc controller status/result
#define NFC_ECC_RSLT_MA	(*(volatile UINT *)(NFC_REG_BASE+0x0E))	//  16bit nfc ecc err position in main
#define NFC_ECC_RSLT_SA	(*(volatile UINT *)(NFC_REG_BASE+0x10))	//  16bit nfc ecc err pos in spare
#define NFC_NF_WR_PROT	(*(volatile UINT *)(NFC_REG_BASE+0x12))	//  16bit nfc write protection
#define NFC_ULOCK_START_BLK	(*(volatile UINT *)(NFC_REG_BASE+0x14))	//  16bit nfc start unlock location
#define NFC_ULOCK_END_BLK	(*(volatile UINT *)(NFC_REG_BASE+0x16))	//  16bit nfc end unlock location
#define NFC_NF_WR_PROT_STAT	(*(volatile UINT *)(NFC_REG_BASE+0x18))	//  16bit nfc write protection status
#define NFC_NF_CONFIG1	(*(volatile UINT *)(NFC_REG_BASE+0x1A))	//  16bit nfc configuration 1
#define NFC_NF_CONFIG2	(*(volatile UINT *)(NFC_REG_BASE+0x1C))	//  16bit nfc configuration 2
		
//#########################################		
//# VRAM                                  #		
//# $FFFF_FF00 to $FFFF_FFFF              #		
//#########################################		
#define VRAM_BASE_ADDR	0xFFFFFF00	//  vector ram (*(volatile UINT *)(256b))
		
		
		
		
		
//######################################### 		
//# SDRAM (*(volatile UINT *)(physical))                      # 		
//# $C000_0000 to $C7FF_FFFF              # 		
//######################################### 		
#define SDRAM0_BASE	0xC0000000	//  sdram 0 bottom
#define SDRAM0_END_ADDR	0xC3FFFFFF	//  sdram 0 top
#define SDRAM1_BASE	0xC4000000	//  sdram 1 bottom
#define SDRAM1_END_ADDR	0xC7FFFFFF	//  sdram 1 top
		
//#########################################		
//# PORTHOLE                              #		
//#########################################		
#define PH_BASE_ADDR	0xD3000000	//  User Porthole
#define EVENT_PORTHOLE	(*(volatile UINT *)(PH_BASE_ADDR+0x00))	//  User Event Porthole
#define EVENT_PH	(*(volatile UINT *)(PH_BASE_ADDR+0x00))	//  User Event Porthole
#define EVENT	(*(volatile UINT *)(PH_BASE_ADDR+0x00))	//  User Event Porthole
		
#define PASS_PORTHOLE	(*(volatile UINT *)(PH_BASE_ADDR+0x04))	//  Event PASS Porthole
#define PASS_PH	(*(volatile UINT *)(PH_BASE_ADDR+0x04))	//  Event PASS Porthole
		
#define FAIL_PORTHOLE	(*(volatile UINT *)(PH_BASE_ADDR+0x08))	//  Event FAIL Porthole
#define FAIL_PH	(*(volatile UINT *)(PH_BASE_ADDR+0x08))	//  Event FAIL Porthole
		
#define STOP_PORTHOLE	(*(volatile UINT *)(PH_BASE_ADDR+0x0C))	//  Event STOP Porthole
#define STOP_PH	(*(volatile UINT *)(PH_BASE_ADDR+0x0C))	//  Event STOP Porthole
		
#define FINISH_PORTHOLE	(*(volatile UINT *)(PH_BASE_ADDR+0x10))	//  Event FINISH Porthole
#define FINISH_PH	(*(volatile UINT *)(PH_BASE_ADDR+0x10))	//  Event FINISH Porthole
		
#define EXP_DATA_PH	(*(volatile UINT *)(PH_BASE_ADDR+0x14))	//  expected data porthole
#define CODE_DATA_PH	(*(volatile UINT *)(PH_BASE_ADDR+0x18))	//  code reference porthole
#define ADDR_DATA_PH	(*(volatile UINT *)(PH_BASE_ADDR+0x1C))	//  address reference porthole
#define ACT_DATA_PH	(*(volatile UINT *)(PH_BASE_ADDR+0x20))	//  actual data porthole
		
#define ABT_DATA_PH	(*(volatile UINT *)(PH_BASE_ADDR+0x24))	//  data abort porthole
#define ABT_PREFETCH_PH	(*(volatile UINT *)(PH_BASE_ADDR+0x28))	//  prefetch abort porthole
#define ABT_UNDEF_PH	(*(volatile UINT *)(PH_BASE_ADDR+0x2C))	//  undefined instruction porthole
		
#define MSG_PORTHOLE	(*(volatile UINT *)(PH_BASE_ADDR+0x40))	//  used for comment messages
		
#define EVENT_VALUE	0x0000beef	//  Code execution complete trigger
#define USER_EVENT0_DATA	0xbadbeef0	
#define USER_EVENT1_DATA	0xbadbeef1	
#define USER_EVENT2_DATA	0xbadbeef2	
#define USER_EVENT3_DATA	0xbadbeef3	
#define USER_EVENT4_DATA	0xbadbeef4	
#define USER_EVENT5_DATA	0xbadbeef5	
#define USER_EVENT6_DATA	0xbadbeef6	
#define USER_EVENT7_DATA	0xbadbeef7	
#define USER_EVENT8_DATA	0xbadbeef8	
#define USER_EVENT9_DATA	0xbadbeef9	
#define USER_EVENT10_DATA	0xbadbeefa	
#define USER_EVENT11_DATA	0xbadbeefb	
#define USER_EVENT12_DATA	0xbadbeefc	
#define USER_EVENT13_DATA	0xbadbeefd	
#define USER_EVENT14_DATA	0xbadbeefe	
#define USER_EVENT15_DATA	0xbadbeeff	
#define MCU_JNT_HALT	0xdeaddead	
		
		
		
		
		
		
// ARM Program Status Register		
//  31  30  29  28  27  .........  7   6   5  4  3  2  1  0		
// ----------------------------------------------------------		
// | N | Z | C | V | Q |         | I | F | T |   M4 - M0    |		
// ----------------------------------------------------------		
		
//#########################################		
//# ARM MODES = {M4, M3, M2, M1, M0}      #		
//#########################################		
#define MODE_USR	0x10	
#define MODE_FIQ	0x11	
#define MODE_IRQ	0x12	
#define MODE_SVC	0x13	
#define MODE_ABT	0x17	
#define MODE_UND	0x1B	
#define MODE_SYS	0x1F	
#define MODE_BITS	0x1F	
			
		
//#########################################		
//# ARM INTERRUPTS                        #		
//#########################################		
#define INTERRUPT_BITS	0xC0	
#define ENABLE_IRQ	0x0	
#define ENABLE_FIQ	0x0	
#define DISABLE_FIQ	0x40	
#define DISABLE_IRQ	0x80	
		
//#########################################		
//# ARM CONDITION CODE FLAG = {N,Z,C,V,Q} #		
//#########################################		
#define FLAG_BITS	0xF8000000	
#define NFLAG	0x80000000	
#define ZFLAG	0x40000000	
#define CFLAG	0x20000000	
#define VFLAG	0x10000000	
#define QFLAG	0x08000000	
		
//#########################################		
//# For Stack at SDRAM0                   #		
//# SVC_STACK = C3FFFC00-C3FFFFFF (*(volatile UINT *)(1kbyte))#		
//# IRQ_STACK = C3FFF800-C3FFFBFF (*(volatile UINT *)(1kbyte))#		
//# FIQ_STACK = C3FFF400-C3FFF7FF (*(volatile UINT *)(1kbyte))#		
//# USR_STACK = C3FFF000-C3FFF3FF (*(volatile UINT *)(1kbyte))#		
//# UND_STACK = C3FFEC00-C3FFEFFF (*(volatile UINT *)(1kbyte))#		
//#########################################		
#define SDRAM0_STACK	0xC3FFE000	//  stack at bottom os SDRAM0
#define SVC_SDRAM0_STACK	SDRAM0_STACK	
#define IRQ_SDRAM0_STACK	SDRAM0_STACK-0x0400	
#define FIQ_SDRAM0_STACK	SDRAM0_STACK-0x0800	
#define USR_SDRAM0_STACK	SDRAM0_STACK-0x0C00	
#define UND_SDRAM0_STACK	SDRAM0_STACK-0x1000	
		
//#########################################		
//# For Stack at SDRAM1                   #		
//# SVC_STACK = C7FFFC00-C7FFFFFF (*(volatile UINT *)(1kbyte))#		
//# IRQ_STACK = C7FFF800-C7FFFBFF (*(volatile UINT *)(1kbyte))#		
//# FIQ_STACK = C7FFF400-C7FFF7FF (*(volatile UINT *)(1kbyte))#		
//# USR_STACK = C7FFF000-C7FFF3FF (*(volatile UINT *)(1kbyte))#		
//# UND_STACK = C7FFEC00-C7FFEFFF (*(volatile UINT *)(1kbyte))#		
//#########################################		
#define SDRAM1_STACK	0xC7FFE000	//  stack at bottom os SDRAM1
#define SVC_SDRAM1_STACK	SDRAM1_STACK	
#define IRQ_SDRAM1_STACK	SDRAM1_STACK-0x0400	
#define FIQ_SDRAM1_STACK	SDRAM1_STACK-0x0800	
#define USR_SDRAM1_STACK	SDRAM1_STACK-0x0C00	
#define UND_SDRAM1_STACK	SDRAM1_STACK-0x1000	
		
//#########################################		
//# For Stack at SRAM                     #		
//# SVC_STACK = CFFFFC00-CFFFFFFF (*(volatile UINT *)(1kbyte))#		
//# IRQ_STACK = CFFFF800-CFFFFBFF (*(volatile UINT *)(1kbyte))#		
//# FIQ_STACK = CFFFF400-CFFFF7FF (*(volatile UINT *)(1kbyte))#		
//# USR_STACK = CFFFF000-CFFFF3FF (*(volatile UINT *)(1kbyte))#		
//# UND_STACK = CFFFEC00-CFFFEFFF (*(volatile UINT *)(1kbyte))#		
//#########################################		
#define SRAM_STACK	0xCFFFE000	//  stack at bottom os SRAM
#define SVC_SRAM_STACK	SRAM_STACK	
#define IRQ_SRAM_STACK	SRAM_STACK-0x0400	
#define FIQ_SRAM_STACK	SRAM_STACK-0x0800	
#define USR_SRAM_STACK	SRAM_STACK-0x0C00	
#define UND_SRAM_STACK	SRAM_STACK-0x1000	
		
#endif
