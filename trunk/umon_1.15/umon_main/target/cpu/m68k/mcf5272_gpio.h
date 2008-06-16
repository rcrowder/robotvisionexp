//==========================================================================
//
// pxa250_gpio.h
//
// Author(s):    Michael Kelly, Cogent Computer Systems, Inc.
// Contributors: 
// Date:         04/17/2002
// Description:  This file contains register and bit defines for the PXA250
//               GPIO Subsection
//
								
#define GPIO_GPLR0      0x00    	// GPIO<31:0>   status register 
#define GPIO_GPLR1      0x04    	// GPIO<63:32>  status register 
#define GPIO_GPLR2      0x08    	// GPIO<80:64>  status register 
#define GPIO_GPDR0      0x0C    	// GPIO<31:0>   direction register 
#define GPIO_GPDR1      0x10    	// GPIO<63:32>  direction register 
#define GPIO_GPDR2      0x14    	// GPIO<80:64>  direction register 
#define GPIO_GPSR0      0x18    	// GPIO<31:0>   output set register 
#define GPIO_GPSR1      0x1C    	// GPIO<63:32>  output set register 
#define GPIO_GPSR2      0x20    	// GPIO<80:64>  output set register 
#define GPIO_GPCR0      0x24    	// GPIO<31:0>   output clear register 
#define GPIO_GPCR1      0x28    	// GPIO<63:32>  output clear register 
#define GPIO_GPCR2      0x2C    	// GPIO<80:64>  output clear register 
#define GPIO_GRER0      0x30    	// GPIO<31:0>   rising-edge detect register 
#define GPIO_GRER1      0x34    	// GPIO<63:32>  rising-edge detect register 
#define GPIO_GRER2      0x38    	// GPIO<80:64>  rising-edge detect register 
#define GPIO_GFER0      0x3C    	// GPIO<31:0>   falling-edge detect register 
#define GPIO_GFER1      0x40    	// GPIO<63:32>  falling-edge detect register 
#define GPIO_GFER2      0x44    	// GPIO<80:64>  falling-edge detect register 
#define GPIO_GEDR0      0x48    	// GPIO<31:0>   edge detect status register 
#define GPIO_GEDR1      0x4C    	// GPIO<63:32>  edge detect status register 
#define GPIO_GEDR2      0x50    	// GPIO<80:64>  edge detect status register 
#define GPIO_GAFR0L    	0x54    	// GPIO<15:0>   alternate function select register 0 Lower 
#define GPIO_GAFR0U    	0x58    	// GPIO<31:16>  alternate function select register 0 Upper 
#define GPIO_GAFR1L    	0x5C    	// GPIO<47:32>  alternate function select register 1 Lower 
#define GPIO_GAFR1U    	0x60    	// GPIO<63:48>  alternate function select register 1 Upper 
#define GPIO_GAFR2L    	0x64    	// GPIO<79:64>  alternate function select register 2 Lower 
#define GPIO_GAFR2U    	0x68    	// GPIO80       alternate function select register 2 Upper 

// Bit positions for enable, direction, input status, output
// control and rising/falling edge detect registers
// GPLR0, GPDR0, GPSR0, GPCR0, GPRE0, GPFE0, GPED0
#define GPIO_0			BIT0
#define GPIO_1			BIT1
#define GPIO_2			BIT2
#define GPIO_3			BIT3
#define GPIO_4			BIT4
#define GPIO_5			BIT5
#define GPIO_6			BIT6
#define GPIO_7			BIT7
#define GPIO_8			BIT8
#define GPIO_9			BIT9
#define GPIO_10			BIT10
#define GPIO_11			BIT11
#define GPIO_12			BIT12
#define GPIO_13			BIT13
#define GPIO_14			BIT14
#define GPIO_15			BIT15
#define GPIO_16			BIT16
#define GPIO_17			BIT17
#define GPIO_18			BIT18
#define GPIO_19			BIT19
#define GPIO_20			BIT20
#define GPIO_21			BIT21
#define GPIO_22			BIT22
#define GPIO_23			BIT23
#define GPIO_24			BIT24
#define GPIO_25			BIT25
#define GPIO_26			BIT26
#define GPIO_27			BIT27
#define GPIO_28			BIT28
#define GPIO_29			BIT29
#define GPIO_30			BIT30
#define GPIO_31			BIT31

// GPLR1, GPDR1, GPSR1, GPCR1, GPRE1, GPFE1, GPED1
#define GPIO_32			BIT0
#define GPIO_33			BIT1
#define GPIO_34			BIT2
#define GPIO_35			BIT3
#define GPIO_36			BIT4
#define GPIO_37			BIT5
#define GPIO_38			BIT6
#define GPIO_39			BIT7
#define GPIO_40			BIT8
#define GPIO_41			BIT9
#define GPIO_42			BIT10
#define GPIO_43			BIT11
#define GPIO_44			BIT12
#define GPIO_45			BIT13
#define GPIO_46			BIT14
#define GPIO_47			BIT15
#define GPIO_48			BIT16
#define GPIO_49			BIT17
#define GPIO_50			BIT18
#define GPIO_51			BIT19
#define GPIO_52			BIT20
#define GPIO_53			BIT21
#define GPIO_54			BIT22
#define GPIO_55			BIT23
#define GPIO_56			BIT24
#define GPIO_57			BIT25
#define GPIO_58			BIT26
#define GPIO_59			BIT27
#define GPIO_60			BIT28
#define GPIO_61			BIT29
#define GPIO_62			BIT30
#define GPIO_63			BIT31

// GPLR2, GPDR2, GPSR2, GPCR2, GPRE2, GPFE2, GPED2
#define GPIO_64			BIT0
#define GPIO_65			BIT1
#define GPIO_66			BIT2
#define GPIO_67			BIT3
#define GPIO_68			BIT4
#define GPIO_69			BIT5
#define GPIO_70			BIT6
#define GPIO_71			BIT7
#define GPIO_72			BIT8
#define GPIO_73			BIT9
#define GPIO_74			BIT10
#define GPIO_75			BIT11
#define GPIO_76			BIT12
#define GPIO_77			BIT13
#define GPIO_78			BIT14
#define GPIO_79			BIT15
#define GPIO_80			BIT16

// Alternate Function values are written to the respective
// GAFRx_U/L register to select the alternate function for
// a GPIO pin.  Remember that you must set the direction of
// the GPIO to match the alternate function selected
#define AF0		0x0
#define AF1		0x1
#define AF2		0x2
#define AF3		0x3

// GPAFR0_L
#define GPIO_0_AF					AF0 << 0
#define GPIO_1_AF_RST				AF1 << 2	// Set GPDR0, bit GPIO_1 = 0
#define GPIO_2_AF					AF0 << 4
#define GPIO_3_AF					AF0 << 6
#define GPIO_4_AF					AF0 << 8
#define GPIO_5_AF					AF0 << 10
#define GPIO_6_AF_MMC_CLK			AF1 << 12	// Set GPDR0, bit GPIO_6 = 1
#define GPIO_7_AF_48MHZ				AF1 << 14	// Set GPDR0, bit GPIO_7 = 1
#define GPIO_8_AF_MMC_CS0			AF1 << 16	// Set GPDR0, bit GPIO_8 = 1
#define GPIO_9_AF_MMC_CS1			AF1 << 18	// Set GPDR0, bit GPIO_9 = 1
#define GPIO_10_AF_RTC_CLK			AF1 << 20	// Set GPDR0, bit GPIO_10 = 1
#define GPIO_11_AF_3_68MHZ			AF1 << 22	// Set GPDR0, bit GPIO_11 = 1
#define GPIO_12_AF_32KHZ			AF1 << 24	// Set GPDR0, bit GPIO_12 = 1
#define GPIO_13_AF_MBGNT			AF2 << 26	// Set GPDR0, bit GPIO_13 = 1
#define GPIO_14_AF_MBREQ			AF1 << 28	// Set GPDR0, bit GPIO_14 = 0
#define GPIO_15_AF_CS1				AF2 << 30	// Set GPDR0, bit GPIO_15 = 1

// GPAFR0_U
#define GPIO_16_AF_PWM0				AF2 << 0	// Set GPDR0, bit GPIO_16 = 1
#define GPIO_17_AF_PWM1				AF2 << 2	// Set GPDR0, bit GPIO_17 = 1
#define GPIO_18_AF_RDY				AF1 << 4	// Set GPDR0, bit GPIO_18 = 0
#define GPIO_19_AF_DREQ1			AF1 << 6	// Set GPDR0, bit GPIO_19 = 0
#define GPIO_20_AF_DREQ0			AF1 << 8	// Set GPDR0, bit GPIO_20 = 0
#define GPIO_21_AF					AF0 << 10
#define GPIO_22_AF					AF0 << 12
#define GPIO_23_AF_SSP_CLK			AF2 << 14	// Set GPDR0, bit GPIO_23 = 1
#define GPIO_24_AF_SSP_FRM			AF2 << 16	// Set GPDR0, bit GPIO_24 = 1
#define GPIO_25_AF_SSP_TXD			AF2 << 18	// Set GPDR0, bit GPIO_25 = 1
#define GPIO_26_AF_SSP_RXD			AF1 << 20	// Set GPDR0, bit GPIO_26 = 0
#define GPIO_27_AF_SSP_EXTCLK		AF1 << 22	// Set GPDR0, bit GPIO_27 = 0
#define GPIO_28_AF_AC97_BCLK_IN		AF1 << 24	// When GPDR0, bit GPIO_28 = 0
#define GPIO_28_AF_I2S_BCLK_IN		AF2 << 24	// When GPDR0, bit GPIO_28 = 0
#define GPIO_28_AF_I2S_BCLK_OUT		AF1 << 24	// When GPDR0, bit GPIO_28 = 1
#define GPIO_28_AF_AC97_BCLK_OUT	AF2 << 24	// When GPDR0, bit GPIO_28 = 1
#define GPIO_29_AF_AC97_SDIN0		AF1 << 26	// Set GPDR0, bit GPIO_29 = 0
#define GPIO_29_AF_I2S_SDIN			AF2 << 26	// Set GPDR0, bit GPIO_29 = 0
#define GPIO_30_AF_I2S_SDOUT		AF1 << 28	// Set GPDR0, bit GPIO_30 = 1
#define GPIO_30_AF_AC97_SDOUT		AF2 << 28	// Set GPDR0, bit GPIO_30 = 1
#define GPIO_31_AF_I2S_SYNC			AF1 << 30	// Set GPDR0, bit GPIO_31 = 1
#define GPIO_31_AF_AC97_SYNC		AF2 << 30	// Set GPDR0, bit GPIO_31 = 1

// GPAFR1_L
#define GPIO_32_AF_AC97_SDIN1		AF1 << 0	// When GPDR1, BIT GPIO_32 = 0
#define GPIO_32_AF_I2S_SYSCLK		AF1 << 0	// When GPDR1, BIT GPIO_32 = 1
#define GPIO_33_AF_CS5				AF2 << 2	// Set GPDR1, BIT GPIO_33 = 1
#define GPIO_34_AF_FF_RXD			AF1 << 4	// Set GPDR1, BIT GPIO_34 = 0
#define GPIO_34_AF_MMC_CS0			AF2 << 4	// Set GPDR1, BIT GPIO_34 = 1
#define GPIO_35_AF_FF_CTS			AF1 << 6	// Set GPDR1, BIT GPIO_35 = 0
#define GPIO_36_AF_FF_DCD			AF1 << 8	// Set GPDR1, BIT GPIO_36 = 0
#define GPIO_37_AF_FF_DSR			AF1 << 10	// Set GPDR1, BIT GPIO_37 = 0
#define GPIO_38_AF_FF_RI			AF1 << 12	// Set GPDR1, BIT GPIO_38 = 0
#define GPIO_39_AF_MM_CS1			AF1 << 14	// Set GPDR1, BIT GPIO_39 = 1
#define GPIO_39_AF_FF_TXD			AF2 << 14	// Set GPDR1, BIT GPIO_39 = 1
#define GPIO_40_AF_FF_DTR			AF2 << 16	// Set GPDR1, BIT GPIO_40 = 1
#define GPIO_41_AF_FF_RTS			AF2 << 18	// Set GPDR1, BIT GPIO_41 = 1
#define GPIO_42_AF_BT_RXD			AF1 << 20	// Set GPDR1, BIT GPIO_42 = 0
#define GPIO_43_AF_BT_TXD			AF2 << 22	// Set GPDR1, BIT GPIO_43 = 1
#define GPIO_44_AF_BT_CTS			AF1 << 24	// Set GPDR1, BIT GPIO_44 = 0
#define GPIO_45_AF_BT_RTS			AF2 << 26	// Set GPDR1, BIT GPIO_45 = 1
#define GPIO_46_AF_IR_RXD			AF1 << 28	// Set GPDR1, BIT GPIO_46 = 0
#define GPIO_46_AF_STD_RXD			AF2 << 28	// Set GPDR1, BIT GPIO_46 = 0
#define GPIO_47_AF_IR_TXD			AF1 << 30	// Set GPDR1, BIT GPIO_47 = 1
#define GPIO_47_AF_STD_TXD			AF2 << 30	// Set GPDR1, BIT GPIO_47 = 1

// GPAFR1_U
#define GPIO_48_AF_POE				AF2 << 0	// Set GPDR1, BIT GPIO_48 = 1
#define GPIO_49_AF_PWE				AF2 << 2	// Set GPDR1, BIT GPIO_49 = 1
#define GPIO_50_AF_PIOR				AF2 << 4	// Set GPDR1, BIT GPIO_50 = 1
#define GPIO_51_AF_PIOW				AF2 << 6	// Set GPDR1, BIT GPIO_51 = 1
#define GPIO_52_AF_PCE1				AF2 << 8	// Set GPDR1, BIT GPIO_52 = 1
#define GPIO_53_AF_MMC_CLK			AF1 << 10	// Set GPDR1, BIT GPIO_53 = 1
#define GPIO_53_AF_PCE2				AF2 << 10	// Set GPDR1, BIT GPIO_53 = 1
#define GPIO_54_AF_MMC_CLK			AF1 << 12	// Set GPDR1, BIT GPIO_54 = 1
#define GPIO_54_AF_PSKTSEL			AF2 << 12	// Set GPDR1, BIT GPIO_54 = 1
#define GPIO_55_AF_PREG				AF2 << 14	// Set GPDR1, BIT GPIO_55 = 1
#define GPIO_56_AF_PWAIT			AF1 << 16	// Set GPDR1, BIT GPIO_56 = 0
#define GPIO_57_AF_IOIS16			AF1 << 18	// Set GPDR1, BIT GPIO_57 = 0
#define GPIO_58_AF_LDD0				AF2	<< 20	// Set GPDR1, BIT GPIO_58 = 1
#define GPIO_59_AF_LDD1				AF2	<< 22	// Set GPDR1, BIT GPIO_59 = 1
#define GPIO_60_AF_LDD2				AF2	<< 24	// Set GPDR1, BIT GPIO_60 = 1
#define GPIO_61_AF_LDD3				AF2	<< 26	// Set GPDR1, BIT GPIO_61 = 1
#define GPIO_62_AF_LDD4				AF2	<< 28	// Set GPDR1, BIT GPIO_62 = 1
#define GPIO_63_AF_LDD5				AF2	<< 30	// Set GPDR1, BIT GPIO_63 = 1

// GPAFR2_L
#define GPIO_64_AF_LDD6				AF2 << 0	// Set GPDR1, BIT GPIO_64 = 1
#define GPIO_65_AF_LDD7				AF2 << 2	// Set GPDR1, BIT GPIO_65 = 1
#define GPIO_66_AF_MBREQ			AF1 << 4	// Set GPDR1, BIT GPIO_66 = 0
#define GPIO_66_AF_LDD8				AF2 << 4	// Set GPDR1, BIT GPIO_66 = 1
#define GPIO_67_AF_MMC_CS0			AF1 << 6	// Set GPDR1, BIT GPIO_67 = 1
#define GPIO_67_AF_LDD9				AF2 << 6	// Set GPDR1, BIT GPIO_67 = 1
#define GPIO_68_AF_MMC_CS1			AF1 << 8	// Set GPDR1, BIT GPIO_68 = 1
#define GPIO_68_AF_LDD10			AF2 << 8	// Set GPDR1, BIT GPIO_68 = 1
#define GPIO_69_AF_MMC_CLK			AF1 << 10	// Set GPDR1, BIT GPIO_69 = 1
#define GPIO_69_AF_LDD11			AF2 << 10	// Set GPDR1, BIT GPIO_69 = 1
#define GPIO_70_AF_RTC_CLK			AF1 << 12	// Set GPDR1, BIT GPIO_70 = 1
#define GPIO_70_AF_LDD12			AF2 << 12	// Set GPDR1, BIT GPIO_70 = 1
#define GPIO_71_AF_3_68MHZ			AF1 << 14	// Set GPDR1, BIT GPIO_71 = 1
#define GPIO_71_AF_LDD13			AF2 << 14	// Set GPDR1, BIT GPIO_71 = 1
#define GPIO_72_AF_32KHZ			AF1 << 16	// Set GPDR1, BIT GPIO_72 = 1
#define GPIO_72_AF_LDD14			AF2 << 16	// Set GPDR1, BIT GPIO_72 = 1
#define GPIO_73_AF_MBGNT			AF1 << 18	// Set GPDR1, BIT GPIO_73 = 1
#define GPIO_73_AF_LDD15			AF2 << 18	// Set GPDR1, BIT GPIO_73 = 1
#define GPIO_74_AF_LCD_FCLK			AF2 << 20	// Set GPDR1, BIT GPIO_74 = 1
#define GPIO_75_AF_LCD_LCLK			AF2 << 22	// Set GPDR1, BIT GPIO_75 = 1
#define GPIO_76_AF_LCD_PCLK			AF2 << 24	// Set GPDR1, BIT GPIO_76 = 1
#define GPIO_77_AF_LCD_BIAS			AF2 << 26	// Set GPDR1, BIT GPIO_77 = 1
#define GPIO_78_AF_CS2				AF2 << 28	// Set GPDR1, BIT GPIO_78 = 1
#define GPIO_79_AF_CS3				AF2 << 30	// Set GPDR1, BIT GPIO_79 = 1

// GPAFR2_U
#define GPIO_80_AF_CS4				AF2 << 0	// Set GPDR1, BIT GPIO_80 = 1
