/*  VC21 series library functions
	i.MX21 functionality

    Copyright (C) 2007  Virtual Cogs Embedded Systems Inc.

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

	www.virtualcogs.com
	Virtual Cogs Embedded Systems Inc., 5694 Highway 7 East, Unit 4, Suite 311
	Markham, ON, Canada L3P 1B4
*/
#include <stdlib.h>
#include "../mx21.h"
#include "vcmx212.h"
#include "monlib.h"

static char*	s_pSDRAM0_HeapStart;
static size_t	s_iSDRAM0_HeapSize;

void VCMX212_Init(UINT clk)
{
	//TODO: init clock speed based on define CPU_SPEED
	//---------------------------------------------------------------------------------------------------------------------------------------
	//START OF HAB TOOLKIT SECTION 
	//THIS SECTION CAN BE REMOVED IF RUNNING PROGRAMS  USING JTAG OR UMON
	
	//set PLL frequency to 266MHz
	CRM_CSCR=0x1f18060f;
	CRM_MPCTL0=0x007b1c73;
	CRM_CSCR=0x1f38060f;	//MPLL restart
	while((CRM_MPCTL1 & 0x8000)==0);	//wait until PLL locked

	//enable instruction cache
	asm("   MRC p15, 0, r0, c1, c0, 0");
	asm("   ORR r0, r0, #0x1000"); //bit 12 is ICACHE enable
	asm("   MCR p15, 0, r0, c1, c0, 0");
	// Flush instruction cache 
	asm("   MCR p15, 0, r0, c7, c5, 0");
	
	//END OF HAB TOOLKIT SECTION
	//---------------------------------------------------------------------------------------------------------------------------------------	
	
	//init clocks 
	//TODO: verify that all libraries init clocks properly and delete these lines
	CRM_PCDR1=0x0708070f;	//PERCLK clock divider settings (PERCLK4=44.33MHz, PERCLK3=26.6MHz)
	CRM_PCCR0=0x64063841;	//enable I2C, DMA, LCDC, GPIO,UART1

	mon_printf("\nVCMX212_Init: Initialized MX212\n");

	//init the MicroMonitor extended heap, based on how much SDRAM0 is left

	extern char end;													// Assumes end label is setup in ldscript properly!!
	s_pSDRAM0_HeapStart	= (char*)&end;									// from ldscript
	s_iSDRAM0_HeapSize	= (32*1024*1024) - ((UINT)&end - SDRAM0_BASE);	// 32MB - (app + uMon) size

	mon_printf("VCMX212_Init: SDRAM0 start 0x%1x\n", s_pSDRAM0_HeapStart);
	mon_printf("VCMX212_Init: SDRAM0 size  0x%1x\n", s_iSDRAM0_HeapSize);
	mon_heapextend(s_pSDRAM0_HeapStart, s_iSDRAM0_HeapSize);
	
}
