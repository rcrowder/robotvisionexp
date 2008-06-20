/******************************************************************************
 *
 * $RCSfile: $
 * $Revision: $
 *
 * This module provides interface routines to the LPC ARM UARTs.
 * Copyright 2004, R O SoftWare
 * No guarantees, warrantees, or promises, implied or otherwise.
 * May be used for hobby or commercial purposes provided copyright
 * notice remains intact.
 *
 * reduced to see what has to be done for minimum UART-support by mthomas
 *****************************************************************************/

// #warning "this is a reduced version of the R O Software code"

//// #include "LPC213x.h"
#include "uart.h"

// U0_LCR devisor latch bit 
#define UART0_LCR_DLAB  7
// U1_LCR devisor latch bit 
#define UART1_LCR_DLAB  7

/*    baudrate divisor - use UART_BAUD macro
 *    mode - see typical modes (uart.h)
 *    fmode - see typical fmodes (uart.h)
 *    NOTE: uart0Init(UART_BAUD(9600), UART_8N1, UART_FIFO_8); 
 */
void uart0Init(uint16_t baud, uint8_t mode, uint8_t fmode)
{
  // setup Pin Function Select Register (Pin Connect Block) 
  // make sure old values of Bits 0-4 are masked out and
  // set them according to UART0-Pin-Selection
  U0_TX_PINSEL_REG = ( U0_TX_PINSEL_REG & ~U0_TX_PINMASK ) | U0_TX_PINSEL;
  U0_RX_PINSEL_REG = ( U0_RX_PINSEL_REG & ~U0_RX_PINMASK ) | U0_RX_PINSEL;

  U0IER = 0x00;             // disable all interrupts
  U0IIR = 0x00;             // clear interrupt ID register
  U0LSR = 0x00;             // clear line status register

  // set the baudrate - DLAB must be set to access DLL/DLM
  U0LCR = (1<<UART0_LCR_DLAB); // set divisor latches (DLAB)
  U0DLL = (uint8_t)baud;         // set for baud low byte
  U0DLM = (uint8_t)(baud >> 8);  // set for baud high byte
  
  // set the number of characters and other
  // user specified operating parameters
  // Databits, Parity, Stopbits - Settings in Line Control Register
  U0LCR = (mode & ~(1<<UART0_LCR_DLAB)); // clear DLAB "on-the-fly"
  // setup FIFO Control Register (fifo-enabled + xx trig) 
  U0FCR = fmode;
}

int uart0Putch(int ch)
{
  while (!(U0LSR & ULSR_THRE))          // wait for TX buffer to empty
    continue;                           // also either WDOG() or swap()

  U0THR = (uint8_t)ch;  // put char to Transmit Holding Register
  return (uint8_t)ch;      // return char ("stdio-compatible"?)
}

const char *uart0Puts(const char *string)
{
	char ch;
	
	while ((ch = *string)) {
		if (uart0Putch(ch)<0) break;
		string++;
	}
	
	return string;
}

int uart0TxEmpty(void)
{
  return (U0LSR & (ULSR_THRE | ULSR_TEMT)) == (ULSR_THRE | ULSR_TEMT);
}

void uart0TxFlush(void)
{
  U0FCR |= UFCR_TX_FIFO_RESET;          // clear the TX fifo
}


/* Returns: character on success, -1 if no character is available */
int uart0Getch(void)
{
  if (U0LSR & ULSR_RDR)                 // check if character is available
    return U0RBR;                       // return character

  return -1;
}

/*    baudrate divisor - use UART_BAUD macro
 *    mode - see typical modes (uart.h)
 *    fmode - see typical fmodes (uart.h)
 *    NOTE: uart0Init(UART_BAUD(9600), UART_8N1, UART_FIFO_8); 
 */
void uart1Init(uint16_t baud, uint8_t mode, uint8_t fmode)
{
  // setup Pin Function Select Register (Pin Connect Block) 
  // make sure old values of Bits 0-4 are masked out and
  // set them according to UART0-Pin-Selection
  U1_TX_PINSEL_REG = ( U1_TX_PINSEL_REG & ~U1_TX_PINMASK ) | U1_TX_PINSEL;
  U1_RX_PINSEL_REG = ( U1_RX_PINSEL_REG & ~U1_RX_PINMASK ) | U1_RX_PINSEL;

  U1IER = 0x00;             // disable all interrupts
  U1IIR = 0x00;             // clear interrupt ID register
  U1LSR = 0x00;             // clear line status register

  // set the baudrate - DLAB must be set to access DLL/DLM
  U1LCR = (1<<UART1_LCR_DLAB); // set divisor latches (DLAB)
  U1DLL = (uint8_t)baud;         // set for baud low byte
  U1DLM = (uint8_t)(baud >> 8);  // set for baud high byte
  
  // set the number of characters and other
  // user specified operating parameters
  // Databits, Parity, Stopbits - Settings in Line Control Register
  U1LCR = (mode & ~(1<<UART1_LCR_DLAB)); // clear DLAB "on-the-fly"
  // setup FIFO Control Register (fifo-enabled + xx trig) 
  U1FCR = fmode;
}

int uart1Putch(int ch)
{
  while (!(U1LSR & ULSR_THRE))          // wait for TX buffer to empty
    continue;                           // also either WDOG() or swap()

  U1THR = (uint8_t)ch;  // put char to Transmit Holding Register
  return (uint8_t)ch;      // return char ("stdio-compatible"?)
}

const char *uart1Puts(const char *string)
{
	char ch;
	
	while ((ch = *string)) {
		if (uart1Putch(ch)<0) break;
		string++;
	}
	
	return string;
}

int uart1TxEmpty(void)
{
  return (U1LSR & (ULSR_THRE | ULSR_TEMT)) == (ULSR_THRE | ULSR_TEMT);
}

void uart1TxFlush(void)
{
  U1FCR |= UFCR_TX_FIFO_RESET;          // clear the TX fifo
}


/* Returns: character on success, -1 if no character is available */
int uart1Getch(void)
{
  if (U1LSR & ULSR_RDR)                 // check if character is available
    return U1RBR;                       // return character

  return -1;
}
