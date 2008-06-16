#ifndef _GPIO_Header
#define _GPIO_Header

#define IMX_IO_PHYS		0x10000000

#define IO_ADDRESS(x)	((x) | IMX_IO_PHYS)

# define __REG(x)		(*((volatile UINT *)IO_ADDRESS(x)))
# define __REG2(x,y)	(*(volatile UINT *)((UINT)&__REG(x) + (y)))

#define IMX_GPIO_BASE              (0x15000)

/*
 *  GPIO Module and I/O Multiplexer
 *  x = 0..5 for reg_A, reg_B, reg_C, reg_D, reg_E, reg_F
 */
#define DDIR(x)    __REG2(IMX_GPIO_BASE + 0x00, ((x) & 7) << 8)
#define OCR1(x)    __REG2(IMX_GPIO_BASE + 0x04, ((x) & 7) << 8)
#define OCR2(x)    __REG2(IMX_GPIO_BASE + 0x08, ((x) & 7) << 8)
#define ICONFA1(x) __REG2(IMX_GPIO_BASE + 0x0c, ((x) & 7) << 8)
#define ICONFA2(x) __REG2(IMX_GPIO_BASE + 0x10, ((x) & 7) << 8)
#define ICONFB1(x) __REG2(IMX_GPIO_BASE + 0x14, ((x) & 7) << 8)
#define ICONFB2(x) __REG2(IMX_GPIO_BASE + 0x18, ((x) & 7) << 8)
#define DR(x)      __REG2(IMX_GPIO_BASE + 0x1c, ((x) & 7) << 8)
#define GIUS(x)    __REG2(IMX_GPIO_BASE + 0x20, ((x) & 7) << 8)
#define SSR(x)     __REG2(IMX_GPIO_BASE + 0x24, ((x) & 7) << 8)
#define ICR1(x)    __REG2(IMX_GPIO_BASE + 0x28, ((x) & 7) << 8)
#define ICR2(x)    __REG2(IMX_GPIO_BASE + 0x2c, ((x) & 7) << 8)
#define IMR(x)     __REG2(IMX_GPIO_BASE + 0x30, ((x) & 7) << 8)
#define ISR(x)     __REG2(IMX_GPIO_BASE + 0x34, ((x) & 7) << 8)
#define GPR(x)     __REG2(IMX_GPIO_BASE + 0x38, ((x) & 7) << 8)
#define SWR(x)     __REG2(IMX_GPIO_BASE + 0x3c, ((x) & 7) << 8)
#define PUEN(x)    __REG2(IMX_GPIO_BASE + 0x40, ((x) & 7) << 8)
#define PMASK      __REG(IMX_GPIO_BASE + 0x600)

/*
 *  GPIO Mode
 *
 *  The pin, port, data direction, pull-up enable, primary/alternate
 *  function, output configuration, and input configuration are encoded in a 
 *  single word as follows.
 *
 *  4:0 Pin (31-0)
 *  7:5 Port (F-A, KP)
 *  8 Direction
 *  9 PUEN
 *  10:11 Primary Function,Alternate Function
 *  13:12 OCR
 *  15:14 ICONF
 *  16    OC (KP only)
 *  19:17 IRQ configuration
 * 
 *  [ 17 18 19 | 16 | 15 14 | 13 12 | 11 10 | 9  |  8  | 7 6 5 | 4 3 2 1 0 ]
 *  [ IRQ CFG  | OC | ICONF |  OCR  | P/A   | PU | Dir | Port  |    Pin    ]
 */

#define GPIO_PIN_MASK           (0x1f<<0)

#define GPIO_PORT_POS           5
#define GPIO_PORT_MASK          (0x7 << GPIO_PORT_POS)
#define GPIO_PORTA              (0 << GPIO_PORT_POS)
#define GPIO_PORTB              (1 << GPIO_PORT_POS)
#define GPIO_PORTC              (2 << GPIO_PORT_POS)
#define GPIO_PORTD              (3 << GPIO_PORT_POS)
#define GPIO_PORTE              (4 << GPIO_PORT_POS)
#define GPIO_PORTF              (5 << GPIO_PORT_POS)
#define GPIO_PORTKP             (7 << GPIO_PORT_POS) /* For keyboard port */

#define GPIO_DIR_MASK           (1<<8)
#define GPIO_IN                 (0<<8)
#define GPIO_OUT                (1<<8)

#define GPIO_PU_MASK            (1<<9)
#define GPIO_PUDIS              (0<<9)
#define GPIO_PUEN               (1<<9)

#define GPIO_FUNC_MASK          (0x3<<10)
#define GPIO_PF                 (0<<10)
#define GPIO_AF                 (1<<10)

#define GPIO_OCR_POS            12
#define GPIO_OCR_MASK           (0x3 << GPIO_OCR_POS)
#define GPIO_AIN                (0 << GPIO_OCR_POS)
#define GPIO_BIN                (1 << GPIO_OCR_POS)
#define GPIO_CIN                (2 << GPIO_OCR_POS)
#define GPIO_GPIO               (3 << GPIO_OCR_POS)

#define GPIO_ICONF_MASK         (0x3<<14)
#define GPIO_AOUT               (1<<14)
#define GPIO_BOUT               (2<<14)

#define GPIO_OC_MASK            (1<<16)
#define GPIO_OCDIS              (0<<16)
#define GPIO_OCEN               (1<<16)

#define GPIO_IRQ_MASK           (7 << 17)
#define GPIO_IRQ_RISING         ((0 << 18) | (1 << 17))
#define GPIO_IRQ_FALLING        ((1 << 18) | (1 << 17))
#define GPIO_IRQ_HIGH           ((2 << 18) | (1 << 17))
#define GPIO_IRQ_LOW            ((3 << 18) | (1 << 17))
#define GPIO_IRQ_CFG_MASK       (0x3 << 18)
#define GPIO_IRQ_CFG_POS        18

/*
 *  The GPIO pin naming convention was developed by the original 
 *  unknown author.  Although using defines for variables is always 
 *  a good idea for portability,  in this case the names are as specific 
 *  as the values, and thus lose their portability. Ultimately the pin 
 *  names should be changed to reflect the signal name only.  
 *
 *  The current naming convention is as follows.
 *
 *  P(port)(pin)_(function)_(signal)
 *
 *  port = (A-F)
 *  pin = (0-31)
 *  function = (PF|AF|AIN|BIN|CIN|DR|AOUT|BOUT)
 *  signal = signal name from datasheet
 *
 *  Remember that when in GPIO mode, AIN, BIN, CIN, and DR are inputs to 
 *  the GPIO peripheral module and represent outputs to the pin. 
 *  Similarly AOUT, and BOUT are outputs from the GPIO peripheral 
 *  module and represent inputs to the physical  pin in question. 
 *  Refer to the multiplexing table in the section titled "Signal 
 *  Descriptions and Pin Assignments" in the reference manual.
 */


#define PE14_PF_UART1_CTS       ( GPIO_PORTE | 14 | GPIO_PF | GPIO_OUT )
#define PE15_PF_UART1_RTS       ( GPIO_PORTE | 15 | GPIO_PF | GPIO_IN )
#define PE12_PF_UART1_TXD       ( GPIO_PORTE | 12 | GPIO_PF | GPIO_OUT )
#define PE13_PF_UART1_RXD       ( GPIO_PORTE | 13 | GPIO_PF | GPIO_IN )

#define PB29_AF_UART4_CTS       ( GPIO_PORTB | 29 | GPIO_AF | GPIO_OUT )
#define PB26_AF_UART4_RTS       ( GPIO_PORTB | 26 | GPIO_AF | GPIO_IN )
#define PB28_AF_UART4_TXD       ( GPIO_PORTB | 28 | GPIO_AF | GPIO_OUT )
#define PB31_AF_UART4_RXD       ( GPIO_PORTB | 31 | GPIO_AF | GPIO_IN )

#define PA5_PF_LSCLK            ( GPIO_PORTA |  5 | GPIO_PF | GPIO_OUT )
#define PA6_PF_LD0              ( GPIO_PORTA |  6 | GPIO_PF | GPIO_OUT )
#define PA7_PF_LD1              ( GPIO_PORTA |  7 | GPIO_PF | GPIO_OUT )
#define PA8_PF_LD2              ( GPIO_PORTA |  8 | GPIO_PF | GPIO_OUT )
#define PA9_PF_LD3              ( GPIO_PORTA |  9 | GPIO_PF | GPIO_OUT )
#define PA10_PF_LD4             ( GPIO_PORTA | 10 | GPIO_PF | GPIO_OUT )
#define PA11_PF_LD5             ( GPIO_PORTA | 11 | GPIO_PF | GPIO_OUT )
#define PA12_PF_LD6             ( GPIO_PORTA | 12 | GPIO_PF | GPIO_OUT )
#define PA13_PF_LD7             ( GPIO_PORTA | 13 | GPIO_PF | GPIO_OUT )
#define PA14_PF_LD8             ( GPIO_PORTA | 14 | GPIO_PF | GPIO_OUT )
#define PA15_PF_LD9             ( GPIO_PORTA | 15 | GPIO_PF | GPIO_OUT )
#define PA16_PF_LD10            ( GPIO_PORTA | 16 | GPIO_PF | GPIO_OUT )
#define PA17_PF_LD11            ( GPIO_PORTA | 17 | GPIO_PF | GPIO_OUT )
#define PA18_PF_LD12            ( GPIO_PORTA | 18 | GPIO_PF | GPIO_OUT )
#define PA19_PF_LD13            ( GPIO_PORTA | 19 | GPIO_PF | GPIO_OUT )
#define PA20_PF_LD14            ( GPIO_PORTA | 20 | GPIO_PF | GPIO_OUT )
#define PA21_PF_LD15            ( GPIO_PORTA | 21 | GPIO_PF | GPIO_OUT )
#define PA22_PF_LD16            ( GPIO_PORTA | 22 | GPIO_PF | GPIO_OUT )
#define PA23_PF_LD17            ( GPIO_PORTA | 23 | GPIO_PF | GPIO_OUT )
#define PA24_PF_REV             ( GPIO_PORTA | 24 | GPIO_PF | GPIO_OUT )
#define PA25_PF_CLS             ( GPIO_PORTA | 25 | GPIO_PF | GPIO_OUT )
#define PA26_PF_PS              ( GPIO_PORTA | 26 | GPIO_PF | GPIO_OUT )
#define PA27_PF_SPL_SPR         ( GPIO_PORTA | 27 | GPIO_PF | GPIO_OUT )
#define PA28_PF_LP_HSYNC        ( GPIO_PORTA | 28 | GPIO_PF | GPIO_OUT )
#define PA29_PF_FLM_VSYNC       ( GPIO_PORTA | 29 | GPIO_PF | GPIO_OUT )
#define PA30_PF_CONTRAST        ( GPIO_PORTA | 30 | GPIO_PF | GPIO_OUT )
#define PA31_PF_ACD_OE          ( GPIO_PORTA | 31 | GPIO_PF | GPIO_OUT )


#define PE18_PF_SD_DAT0			( GPIO_PORTE | GPIO_OUT | GPIO_PF | GPIO_PUEN | 18 )
#define PE19_PF_SD_DAT1			( GPIO_PORTE | GPIO_OUT | GPIO_PF | GPIO_PUEN | 19 )
#define PE20_PF_SD_DAT2			( GPIO_PORTE | GPIO_OUT | GPIO_PF | GPIO_PUEN | 20 )
#define PE21_PF_SD_DAT3			( GPIO_PORTE | GPIO_OUT | GPIO_PF | GPIO_PUEN | 21 )
#define PE23_PF_SD_CLK			( GPIO_PORTE | GPIO_OUT | GPIO_PF | 23 )
#define PE22_PF_SD_CMD			( GPIO_PORTE | GPIO_OUT | GPIO_PF | GPIO_PUEN | 22 )

#define PB4_PF_SD_DAT0			( GPIO_PORTB | GPIO_OUT | GPIO_PF | GPIO_PUEN |  4 )
#define PB5_PF_SD_DAT1			( GPIO_PORTB | GPIO_OUT | GPIO_PF | GPIO_PUEN |  5 )
#define PB6_PF_SD_DAT2			( GPIO_PORTB | GPIO_OUT | GPIO_PF | GPIO_PUEN |  6 )
#define PB7_PF_SD_DAT3			( GPIO_PORTB | GPIO_OUT | GPIO_PF |  7 )
#define PB9_PF_SD_CLK			( GPIO_PORTB | GPIO_OUT | GPIO_PF |  9 )
#define PB8_PF_SD_CMD			( GPIO_PORTB | GPIO_OUT | GPIO_PF | GPIO_PUEN |  8 )


extern void GPIO_DisableIRQ(int mode);
extern void GPIO_Mode(int gpio_mode);

#endif //_GPIO_Header
