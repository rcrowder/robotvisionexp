/*****************************************************************************
 *   timer.c:  Timer C file for NXP LPC23xx/24xx Family Microprocessors
 *
 *   Copyright(C) 2006, NXP Semiconductor
 *   All rights reserved.
 *
 *   History
 *   2006.09.01  ver 1.00    Prelimnary version, first Release
 *
******************************************************************************/
#include "LPC23xx.h"		/* LPC23xx/24xx Peripheral Registers	*/
#include "type.h"
#include "irq.h"
#include "timer.h"

volatile DWORD timer_counter = 0;

/******************************************************************************
** Function name:		Timer0Handler
**
** Descriptions:		Timer/Counter 0 interrupt handler
**				executes each 10ms @ 60 MHz CPU Clock
**
** parameters:			None
** Returned value:		None
** 
******************************************************************************/
#ifdef __GNUC__
void Timer0Handler (void); // avoid missing proto warning - mthomas
#endif
void Timer0Handler (void) __irq 
{  
    T0IR = 1;			/* clear interrupt flag */
    IENABLE;			/* handles nested interrupt */

    timer_counter++;

    IDISABLE;
    VICVectAddr = 0;		/* Acknowledge Interrupt */
}

/******************************************************************************
** Function name:		enable_timer
**
** Descriptions:		Enable timer
**
** parameters:			timer number: 0 or 1
** Returned value:		None
** 
******************************************************************************/
void enable_timer( BYTE timer_num )
{
    if ( timer_num == 0 )
    {
		T0TCR = 1;
    }
    else
    {
		T1TCR = 1;
    }
    return;
}

/******************************************************************************
** Function name:		disable_timer
**
** Descriptions:		Disable timer
**
** parameters:			timer number: 0 or 1
** Returned value:		None
** 
******************************************************************************/
void disable_timer( BYTE timer_num )
{
    if ( timer_num == 0 )
    {
		T0TCR = 0;
    }
    else
    {
		T1TCR = 0;
    }
    return;
}

/******************************************************************************
** Function name:		reset_timer
**
** Descriptions:		Reset timer
**
** parameters:			timer number: 0 or 1
** Returned value:		None
** 
******************************************************************************/
void reset_timer( BYTE timer_num )
{
    DWORD regVal;

    if ( timer_num == 0 )
    {
		regVal = T0TCR;
		regVal |= 0x02;
		T0TCR = regVal;
    }
    else
    {
		regVal = T1TCR;
		regVal |= 0x02;
		T1TCR = regVal;
    }
    return;
}

/******************************************************************************
** Function name:		init_timer
**
** Descriptions:		Initialize timer, set timer interval, reset timer,
**						install timer interrupt handler
**
** parameters:			None
** Returned value:		true or false, if the interrupt handler can't be
**						installed, return false.
** 
******************************************************************************/
DWORD init_timer ( DWORD TimerInterval ) 
{
    timer_counter = 0;
    T0MR0 = TimerInterval;
    T0MCR = 3;				/* Interrupt and Reset on MR0 */
    if ( install_irq( TIMER0_INT, (void *)Timer0Handler, HIGHEST_PRIORITY ) == FALSE )
    {
		return (FALSE);
    }
    else
    {
		return (TRUE);
    }
}

/******************************************************************************
**                            End Of File
******************************************************************************/
