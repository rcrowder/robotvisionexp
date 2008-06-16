//***************************************
//*
//* Virtual Cogs MX212 - CPU/SBC Specific Routines
//*
//* Maintained by: Tarun Tuli <tarun@virtualcogs.com>
//* Sept 18, 2006
//* Virtual Cogs - http://www.virtualcogs.com/
//*
//****************************************

#include "config.h"
#include "cpuio.h"
#include "stddefs.h"
#include "cpuio.h"
#include "genlib.h"
#include "cache.h"
#include "warmstart.h"
#include "MX21regs.h"

/* devInit():
 * As a bare minimum, initialize the console UART here using the
 * incoming 'baud' value as the baud rate.
 */
void
devInit(int baud)
{
	// Turn off LEDs to conserve power
	GPIOE_GIUS |= 0x7;			//this tells GPIO port E that the 3 LSBs are GPIO
	GPIOE_OCR1 = 0x3f;			//this tells GPIO port E that the 3 LSBs are controlled by the GPIO port E data register
	GPIOE_DDIR = 0x7;			//each GPIO pin can either be input or output - this configures the 3 LSBs as outputs
	GPIOE_DR = 0x7;				//turn off LEDs

	// Setup Perihperal clocks
	CRM_PCDR1= 0x0307070f;		// sets the input clock to the UARTs
	CRM_PCCR0 |= 0xf;			// Turn on all UART clocks			

	// UART1 Specific
	GPIOE_GIUS &= ~0x3000ul;
	GPIOE_GPR  &= ~0x3000ul;
	GPIOE_DDIR &= ~0x2000ul;
	GPIOE_DDIR |= 0x1000ul;
	
	// Initialize UART
	UART1_UCR1 = 0x0001ul;		// Enable UART
	UART1_UCR2 = 0x4027ul;		// Reset TX/RX
	UART1_UCR3 = 0x0004ul;		// Select normal RX (not IRDA)
	UART1_UCR4 = 0x0000ul;
	UART1_UFCR = 0xa80;
		
	UART1_UBIR = (baud/100) - 1;
	UART1_UBMR = 10000 - 1;		
	
	// Send debug startup string
	UART1_UTXD = 'V';
	
	// Setup CS3 for LAN91C111 based Ethernet
	WEIM_CS3U=0x04020F00;
	WEIM_CS3L=0x44442521;
	
	// Setup the LCD controller
    LCDC_LDCR=0x0004000c;	
}

/* ConsoleBaudSet():
 * Provide a means to change the baud rate of the running
 * console interface.  If the incoming value is not a valid
 * baud rate, then default to 9600.
 * In the early stages of a new port this can be left empty.
 * Return 0 if successful; else -1.
 */
int
ConsoleBaudSet(int baud)
{
	UART1_UBIR = (baud/100) - 1;
	UART1_UBMR = 10000 - 1;
	return(0);
}


/* target_putchar():
 * When buffer has space available, load the incoming character
 * into the UART.
 */
int
target_putchar(char c)
{
    /* Wait for transmit ready bit */
	while(!target_console_empty());
	UART1_UTXD = (UINT)c;

    return((int)c);
}

/* target_console_empty():
 * Target-specific portion of flush_console() in chario.c.
 * This function returns 1 if there are no characters waiting to
 * be put out on the UART; else return 0 indicating that the UART
 * is still busy outputting characters from its FIFO.
 * In the early stages of a new port this can simply return 1.
 */
int
target_console_empty(void)
{
	if (UART1_USR2 & 0x4000)
		return(1);
	else
		return(0);
}


/* target_gotachar():
 * Return 0 if no char is avaialable at UART rcv fifo; else 1.
 * Do NOT pull character out of fifo, just return status. 
 */
int
target_gotachar(void)
{
    if(UART1_USR2 & 0x1ul)
		return(1);
    return(0);
}

/* target_getchar():
 * Assume there is a character in the UART's input buffer and just
 * pull it out and return it.
 */
int 
target_getchar(void)
{
    char    c;

    c = (unsigned char)(UART1_URXD & 0xff);
    return((int)c);
}

/* intsoff():
 * Disable all system interrupts here and return a value that can
 * be used by intsrestore() (later) to restore the interrupt state.
 */
ulong
intsoff(void)
{
    ulong  psr;

    psr = getpsr();

    /*
     * Set bit 6, bit 7 to disable interrupts.
     */
    putpsr(psr | 0x000000c0);
    return(psr);
}

/* intsrestore():
 * Re-establish system interrupts here by using the status value
 * that was returned by an earlier call to intsoff().
 */
void
intsrestore(psr)
ulong     psr;
{
    putpsr(psr);
}

/* cacheInitForTarget():
 * Establish target specific function pointers and
 * enable i-cache...
 * Refer to $core/cache.c for a description of the function pointers.
 * NOTE:
 * If cache (either I or D or both) is enabled, then it is important
 * that the appropriate cacheflush/invalidate function be established.
 * This is very important because programs (i.e. cpu instructions) are
 * transferred to memory using data memory accesses and could
 * potentially result in cache coherency problems.
 */
void
cacheInitForTarget(void)
{
    asm("   MRC p15, 0, r0, c1, c0, 0");
    asm("   ORR r0, r0, #0x1000");  /* bit 12 is ICACHE enable*/
    asm("   MCR p15, 0, r0, c1, c0, 0");

    /* Flush instruction cache */
    asm("   MCR p15, 0, r0, c7, c5, 0");
}

/* target_reset():
 * The default (absolute minimum) action to be taken by this function
 * is to call monrestart(INITIALIZE).  It would be better if there was
 * some target-specific function that would really cause the target
 * to reset...
 */
void
target_reset(void)
{
	warmstart(INITIALIZE);
}

/* If any CPU IO wasn't initialized in reset.S, do it here...
 * This just provides a "C-level" IO init opportunity. 
 */
void
initCPUio(void)
{
	/* ADD_CODE_HERE */
}

