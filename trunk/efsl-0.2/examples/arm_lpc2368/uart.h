/******************************************************************************
 * based on software from:
 * Copyright 2004, R O SoftWare
 * No guarantees, warrantees, or promises, implied or otherwise.
 * May be used for hobby or commercial purposes provided copyright
 * notice remains intact.
 * 
 * reduced to learn what has to be done to enable and use UART0
 *****************************************************************************/
#ifndef INC_UART_H
#define INC_UART_H


#include <stdint.h>

#if defined(__WINARMBOARD_KEIL_MCB2300__)
#include "LPC23xx.h"
#include "target.h"
#define UART_BAUD(baud) (uint16_t)(((Fpclk) / ((baud) * 16.0)) + 0.5)
// KEIL MCP2300
// UART0 TX P0.2 RX P0.3
#define U0_TX_PINSEL_REG  PINSEL0
#define U0_TX_PINSEL     (1UL<<4)  /* PINSEL0 Value for UART0 TX */
#define U0_TX_PINMASK    (3UL<<4)  /* PINSEL0 Mask  for UART0 RX */
#define U0_RX_PINSEL_REG  PINSEL0
#define U0_RX_PINSEL     (1UL<<6)  /* PINSEL0 Value for UART0 TX */
#define U0_RX_PINMASK    (3UL<<6)  /* PINSEL0 Mask  for UART0 RX */
// UART1 TX P0.15 RX P0.16
#define U1_TX_PINSEL_REG  PINSEL0
#define U1_TX_PINSEL     (1UL<<30)  /* PINSEL0 Value for UART0 TX */
#define U1_TX_PINMASK    (3UL<<30)  /* PINSEL0 Mask  for UART0 RX */
#define U1_RX_PINSEL_REG  PINSEL1
#define U1_RX_PINSEL     (1UL<<0)  /* PINSEL0 Value for UART0 TX */
#define U1_RX_PINMASK    (3UL<<0)  /* PINSEL0 Mask  for UART0 RX */

#else
#warning "unkown board/setup - fallback back to old LPC213x/4x code"
#include "LPC213x.h"
#include "lpc_config.h"
#define UART_BAUD(baud) (uint16_t)(((FOSC*PLL_M/VPBDIV_VAL) / ((baud) * 16.0)) + 0.5)

// P0.0 P0.1
#define U0_TX_PINSEL_REG  PINSEL0
#define U0_TX_PINSEL     (1UL<<0)  /* PINSEL0 Value for UART0 TX */
#define U0_TX_PINMASK    (3UL<<0)  /* PINSEL0 Mask  for UART0 RX */
#define U0_RX_PINSEL_REG  PINSEL0
#define U0_RX_PINSEL     (1UL<<2)  /* PINSEL0 Value for UART0 TX */
#define U0_RX_PINMASK    (3UL<<2)  /* PINSEL0 Mask  for UART0 RX */

// P0.8 P0.9
#define U1_TX_PINSEL_REG  PINSEL0
#define U1_TX_PINSEL     (1UL<<16)  /* PINSEL0 Value for UART0 TX */
#define U1_TX_PINMASK    (3UL<<16)  /* PINSEL0 Mask  for UART0 RX */
#define U1_RX_PINSEL_REG  PINSEL0
#define U1_RX_PINSEL     (1UL<<18)  /* PINSEL0 Value for UART0 TX */
#define U1_RX_PINMASK    (3UL<<18)  /* PINSEL0 Mask  for UART0 RX */

#endif

#include "lpcUART.h"

///////////////////////////////////////////////////////////////////////////////
// use the following macros to determine the 'baud' parameter values
// for uart0Init() and uart1Init()
// CAUTION - 'baud' SHOULD ALWAYS BE A CONSTANT or
// a lot of code will be generated.
// Baud-Rate is calculated based on pclk (VPB-clock)
// the devisor must be 16 times the desired baudrate
// see above #define UART_BAUD(baud) (uint16_t)(((FOSC*PLL_M/VPBDIV_VAL) / ((baud) * 16.0)) + 0.5)

///////////////////////////////////////////////////////////////////////////////
// Definitions for typical UART 'baud' settings
#define B1200         UART_BAUD(1200)
#define B9600         UART_BAUD(9600)
#define B19200        UART_BAUD(19200)
#define B38400        UART_BAUD(38400)
#define B57600        UART_BAUD(57600)
#define B115200       UART_BAUD(115200)

///////////////////////////////////////////////////////////////////////////////
// Definitions for typical UART 'mode' settings
#define UART_8N1      (uint8_t)(ULCR_CHAR_8 + ULCR_PAR_NO   + ULCR_STOP_1)
#define UART_7N1      (uint8_t)(ULCR_CHAR_7 + ULCR_PAR_NO   + ULCR_STOP_1)
#define UART_8N2      (uint8_t)(ULCR_CHAR_8 + ULCR_PAR_NO   + ULCR_STOP_2)
#define UART_7N2      (uint8_t)(ULCR_CHAR_7 + ULCR_PAR_NO   + ULCR_STOP_2)
#define UART_8E1      (uint8_t)(ULCR_CHAR_8 + ULCR_PAR_EVEN + ULCR_STOP_1)
#define UART_7E1      (uint8_t)(ULCR_CHAR_7 + ULCR_PAR_EVEN + ULCR_STOP_1)
#define UART_8E2      (uint8_t)(ULCR_CHAR_8 + ULCR_PAR_EVEN + ULCR_STOP_2)
#define UART_7E2      (uint8_t)(ULCR_CHAR_7 + ULCR_PAR_EVEN + ULCR_STOP_2)
#define UART_8O1      (uint8_t)(ULCR_CHAR_8 + ULCR_PAR_ODD  + ULCR_STOP_1)
#define UART_7O1      (uint8_t)(ULCR_CHAR_7 + ULCR_PAR_ODD  + ULCR_STOP_1)
#define UART_8O2      (uint8_t)(ULCR_CHAR_8 + ULCR_PAR_ODD  + ULCR_STOP_2)
#define UART_7O2      (uint8_t)(ULCR_CHAR_7 + ULCR_PAR_ODD  + ULCR_STOP_2)

///////////////////////////////////////////////////////////////////////////////
// Definitions for typical UART 'fmode' settings
#define UART_FIFO_OFF (0x00)
#define UART_FIFO_1   (uint8_t)(UFCR_FIFO_ENABLE + UFCR_FIFO_TRIG1)
#define UART_FIFO_4   (uint8_t)(UFCR_FIFO_ENABLE + UFCR_FIFO_TRIG4)
#define UART_FIFO_8   (uint8_t)(UFCR_FIFO_ENABLE + UFCR_FIFO_TRIG8)
#define UART_FIFO_14  (uint8_t)(UFCR_FIFO_ENABLE + UFCR_FIFO_TRIG14)

void uart0Init(uint16_t baud, uint8_t mode, uint8_t fmode);
int uart0Putch(int ch);
uint16_t uart0Space(void);
const char *uart0Puts(const char *string);
int uart0TxEmpty(void);
void uart0TxFlush(void);
int uart0Getch(void);

void uart1Init(uint16_t baud, uint8_t mode, uint8_t fmode);
int uart1Putch(int ch);
uint16_t uart1Space(void);
const char *uart1Puts(const char *string);
int uart1TxEmpty(void);
void uart1TxFlush(void);
int uart1Getch(void);


#endif
