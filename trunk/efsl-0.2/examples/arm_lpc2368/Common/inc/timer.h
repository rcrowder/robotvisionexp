/*****************************************************************************
 *   timer.h:  Header file for NXP LPC23xx/24xx Family Microprocessors
 *
 *   Copyright(C) 2006, NXP Semiconductor
 *   All rights reserved.
 *
 *   History
 *   2006.07.13  ver 1.00    Prelimnary version, first Release
 *
******************************************************************************/
#ifndef __TIMER_H 
#define __TIMER_H

#define TIME_INTERVAL	Fpclk/1000 - 1
/* depending on the CCLK and PCLK setting, CCLK = 60Mhz, PCLK = 1/4 CCLK
10mSec = 150.000-1 counts */
// #define TIME_INTERVAL	149999		

extern DWORD init_timer( DWORD timerInterval );
extern void enable_timer( BYTE timer_num );
extern void disable_timer( BYTE timer_num );
extern void reset_timer( BYTE timer_num );

extern volatile DWORD timer_counter;

#endif /* end __TIMER_H */
/*****************************************************************************
**                            End Of File
******************************************************************************/
