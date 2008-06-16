/******************************************************************************/
/*                                                                            */
/*  TIME.C:  Time Functions for 1000Hz Clock Tick                             */
/*                                                                            */
/******************************************************************************/
/*  ported to arm-elf-gcc / WinARM by Martin Thomas, KL, .de                  */
/*  <eversmith@heizung-thomas.de>                                             */
/*                                                                            */
/*  Based on a file that has been a part of the uVision/ARM development       */
/*  tools, Copyright KEIL ELEKTRONIK GmbH 2002-2004                           */
/******************************************************************************/

/*
  - mt: modified interrupt ISR handling, updated labels
*/

#include "AT91SAM7S64.h"
#include "Board.h"
#include "swi.h"
#include "systime.h"

#define TCK  1000                           /* Timer Clock  */
#define PIV  ((MCK/TCK/16)-1)               /* Periodic Interval Value */

volatile unsigned long systime_value;     /* Current Time Tick */            /* Current Time Tick */

/*__ramfunc*/ void systime_isr(void)         /* System Interrupt Handler */
{	
	volatile unsigned long status;
	static int cnt;
	
	// Interrupt Acknowledge
	status = AT91C_BASE_PITC->PITC_PIVR;
	
	systime_value++;                       /* Increment Time Tick */
	
	cnt++;
	if (cnt >= 500) {     /* 500ms Elapsed ? */
		*AT91C_PIOA_ODSR ^= LED2;          /* Toggle LED2 */
		cnt=0;
	}
}

void systime_init(void) {                    /* Setup PIT with Interrupt */
	volatile AT91S_AIC * pAIC = AT91C_BASE_AIC;
	
	*AT91C_PIOA_OWER = LED2;     // LED4 ODSR Write Enable

	*AT91C_PITC_PIMR = AT91C_PITC_PITIEN |    /* PIT Interrupt Enable */ 
                     AT91C_PITC_PITEN  |    /* PIT Enable */
                     PIV;                   /* Periodic Interval Value */ 

	/* Setup System Interrupt Mode and Vector with Priority 7 and Enable it */
	pAIC->AIC_SMR[AT91C_ID_SYS] = AT91C_AIC_SRCTYPE_INT_POSITIVE_EDGE | 7;
	pAIC->AIC_SVR[AT91C_ID_SYS] = (unsigned long) systime_isr;
	pAIC->AIC_IECR = (1 << AT91C_ID_SYS);
}

unsigned long systime_get(void)
{
	unsigned long state, ret;
	
	state = IntDisable();
	// iprintf("state 0x%08lx\n", state);
	ret = systime_value;
	IntRestore(state);
	
	return ret;
}
