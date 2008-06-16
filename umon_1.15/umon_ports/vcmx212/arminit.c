//this program inits SDRAM and processor


#include "MX21regs.h"
void ARMinit(void)  __attribute__((naked));

#define i (*(volatile UINT *)(0xdf003400))
#define pSrc (*(volatile UINT *)(0xdf003404))
#define pDst (*(volatile UINT *)(0xdf003408))


void ARMinit(void)
{
	register int *memptr;

	/*setmem 0x10000000 0x00040304 32 AIPI1 PSR0
	setmem 0x10020000 0x00000000 32 AIPI2 PSR0
	setmem 0x10000004 0xFFFBFCFB 32 AIPI1 PSR1
	setmem 0x10020004 0xFFFFFFFF 32 AIPI2 PSR1
	*/
	AIPI1_PSR0=0x00040304;
	AIPI2_PSR0=0x3ffc0000;
	AIPI1_PSR1=0xFFFBFCFB;
	AIPI2_PSR1=0xFFFFFFFF;


	CRM_MPCTL0 = 0x007b1c73;		// Do PLL clock configuration 
	for(i=0;i<10000;i++);
  
	CRM_CSCR=0x33180607;			//increase speed to 254MHz
	for(i=0;i<10000;i++);

 
	WEIM_CS0U = 0x00000a00;			// Set wait states on flash memory (WS=10)
	WEIM_CS0L = 0x20000d01; 		// Set flash bus to 16bit mode
	for(i=0;i<10000;i++);

	//enable SDRAM
	SDRAMC_SDCTL0=0x8212c339;
	
	//wait 100us
	for(i=0;i<1000000;i++);
	
	//precharge all
	memptr=(UINT*)0xc0200000;
	SDRAMC_SDCTL0=0x9212c339;
	*memptr=0x00;
	for(i=0;i<1000;i++);

	//refresh
	SDRAMC_SDCTL0=0xa212c339;
	*memptr=0x00;
	for(i=0;i<1000;i++);

	SDRAMC_SDCTL0=0xa212c339;
	*memptr=0x00;
	for(i=0;i<1000;i++);

	//mode sel
	//memptr=(UINT*)0xc0118000;	//no burst
	memptr=(UINT*)0xc0119800; //w/burst
	SDRAMC_SDCTL0=0xb212c339;
	*memptr=0x00;
	for(i=0;i<1000;i++);

	//enable SDRAM in normal mode
	SDRAMC_SDCTL0=0x8212c339;

	//asm ("mrc p15, 0, r0, c1, c0, 0;" :  : );
	
/*	
	asm("ldr r0, =0x0;" 
		"mcr p15, 0, r0, c7, c5, 0"
		: : );
		
	asm("ldr r0, =0x00051078;" 
		"mcr p15, 0, r0, c1, c0, 0"
		: : );
*/

   	// Set wait states on flash memory (WS=15)
/*
	ldr r2, =0x00000f00
	ldr r1, =0xdf001000
	str r2, [r1]

	// Set flash bus to 16bit
	ldr r2, =0x20000d01
	ldr r1, =0xdf001004
	str r2, [r1]
*/


	// Copy flash to SDRAM
	pSrc = (UINT)0xc8001000;
	pDst = (UINT)0xc0000000;

	for(i=0; i < 200000; i=i+4)
		*(UINT *)pDst++ = *(UINT *)pSrc++;

	// Jump to 0xc0000000
	asm volatile ("mov pc, #-1073741824;" : :);
}

