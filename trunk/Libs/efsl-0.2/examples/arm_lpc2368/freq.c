#include "../inc/LPC23xx.h"
#include <stdint.h>
#include "freq.h"

/*************************************************************************
 * Function Name: SYS_GetFsclk
 * Parameters: none
 * Return: uint32_t
 *
 * Description: return Sclk [Hz]
 *
 *************************************************************************/
uint32_t SYS_GetFsclk(void)
{
	uint32_t Mul = 1, Div = 1, Osc, Fsclk;
	
	if ( PLLSTAT & PLLSTAT_PLLC ) {
		// when PLL is connected
		// Mul = PLLSTAT_bit.MSEL + 1;
		// Div = PLLSTAT_bit.NSEL + 1;
		Mul = ( PLLSTAT & PLLSTAT_MSEL_MASK ) >> PLLSTAT_MSEL_OFFS;
		Div = ( PLLSTAT & PLLSTAT_NSEL_MASK ) >> PLLSTAT_NSEL_OFFS;
	}
	
	// Find clk source
	switch( CLKSRCSEL & CLKSRCSEL_CLKSRC_MASK ) {
		case 0:
			Osc = I_RC_OSC_FREQ;
			break;
		case 1:
			Osc = MAIN_OSC_FREQ;
			break;
		case 2:
			Osc = RTC_OSC_FREQ;
			break;
		default:
			Osc = 0;
	}
	
	// Calculate system frequency
	Fsclk  = Osc * Mul * 2;
	Fsclk /= Div * ( CCLKCFG + 1 );

	return(Fsclk);
}

/*************************************************************************
 * Function Name: SYS_GetFpclk
 * Parameters: Int32U Periphery
 * Return: uint32_t
 *
 * Description: return Pclk [Hz]
 *
 *************************************************************************/
uint32_t SYS_GetFpclk(uint32_t Periphery)
{
	uint32_t Fpclk;
	volatile unsigned long *pReg;
	
	if ( Periphery < 32 ) {
		pReg = &PCLKSEL0;
	}
	else {
		pReg = &PCLKSEL1;
	}

  	Periphery  &= 0x1F;   // %32
  	Fpclk = SYS_GetFsclk();
  	
	// find peripheral appropriate periphery divider
  	switch((*pReg >> Periphery) & 3) {
  		case 0:
		    Fpclk /= 4;
    		break;
  		case 1:
    		break;
  		case 2:
		    Fpclk /= 2;
		    break;
		default:
		    Fpclk /= 8;
  	}
  	
	return(Fpclk);
}
