//==========================================================================
//
// mcf5272_gpio.c
//
//
// Author(s):    Michael Kelly, Cogent Computer Systems, Inc.
// Contributors: 
// Date:         03/06/2003
// Description:  This file contains code to set, clear and test the GPIO 
//				 bits on the MCF5272
//
//--------------------------------------------------------------------------

#include "config.h"
#include "cpuio.h"
#include "stddefs.h"
#include "genlib.h"
#include "mcf5272.h"
#include "csb360_gpio.h"

// Function Prototypes
int GPIO_set(int);
int GPIO_clr(int);
int GPIO_tst(int);
int GPIO_in(int);
int GPIO_out(int);

//#define GPIO_DEBUG

//--------------------------------------------------------
// GPIO_set()
//
// This function sets the desired bit passed in.

int GPIO_set(int gpio_bit)
{
    MCF5272_IMM *imm;

    imm = mcf5272_get_immp();

	// quick sanity test
	if (gpio_bit > 32) return -1;

	// First find out which port the bit belongs to
	if (gpio_bit < 16)	// Port A
	{
		MCF5272_WR_GPIO_PADAT(imm, (MCF5272_RD_GPIO_PADAT(imm) | (1 << gpio_bit)));
	}
	else // Port B
	{
		MCF5272_WR_GPIO_PBDAT(imm, (MCF5272_RD_GPIO_PBDAT(imm) | (1 << (gpio_bit - 16))));
	}
	return 0;
}

//--------------------------------------------------------
// GPIO_clr()
//
// This function clears the desired bit passed in.  NOTE: We do not
// test to see if clearing the bit would screw up any alternate
// functions.  Use this function with caution!
//

int GPIO_clr(int gpio_bit)
{
    MCF5272_IMM *imm;

    imm = mcf5272_get_immp();

	// quick sanity test
	if (gpio_bit > 32) return -1;

	// First find out which port the bit belongs to
	if (gpio_bit < 16)	// Port A
	{
		MCF5272_WR_GPIO_PADAT(imm, (MCF5272_RD_GPIO_PADAT(imm) & ~(1 << gpio_bit)));
	}
	else // Port B
	{
		MCF5272_WR_GPIO_PBDAT(imm, (MCF5272_RD_GPIO_PBDAT(imm) & ~(1 << (gpio_bit - 16))));
	}
	return 0;

}

//--------------------------------------------------------
// GPIO_tst()
//
// This function returns the state of desired bit passed in.
// It does not test to see if it's an input or output and thus
// can be used to verify if an output set/clr has taken place
// as well as for testing an input state.
//

int GPIO_tst(int gpio_bit)
{
    MCF5272_IMM *imm;

    imm = mcf5272_get_immp();

	// First find out which port the bit belongs to
	if (gpio_bit < 16)	// Port A
	{
		// Get the state
		if (MCF5272_RD_GPIO_PADAT(imm) & (1 << gpio_bit))
			return 1;
	}
	else // Port B
	{
		// Get the state
		if (MCF5272_RD_GPIO_PBDAT(imm) & (1 << (gpio_bit - 16)))
			return 1;
	}
	return 0; // bit was not set
}

//--------------------------------------------------------
// GPIO_in()
//
// This function changes the direction of the desired bit 
// to input.  NOTE: We do not test to see if changing the
// direction of the bit would screw up anything.  Use this
// function with caution!
int GPIO_in(int gpio_bit)
{
    MCF5272_IMM *imm;

    imm = mcf5272_get_immp();

	// First find out which port the bit belongs to
	if (gpio_bit < 16)	// Port A
	{
		// Make it an output
		MCF5272_WR_GPIO_PADDR(imm, (MCF5272_RD_GPIO_PADDR(imm) & ~(1 << gpio_bit)));
	}
	else // Port B
	{
		// Make it an output
		MCF5272_WR_GPIO_PBDDR(imm, (MCF5272_RD_GPIO_PBDDR(imm) & ~(1 << (gpio_bit - 16))));
	}
	return 0;
}

//--------------------------------------------------------
// GPIO_out()
//
// This function changes the direction of the desired bit 
// to output.  NOTE: We do not test to see if changing the
// direction of the bit would screw up anything.  Use this
// function with caution!
int GPIO_out(int gpio_bit)
{
    MCF5272_IMM *imm;

    imm = mcf5272_get_immp();

	// First find out which port the bit belongs to
	if (gpio_bit < 16)	// Port A
	{
		// Make it an output
		MCF5272_WR_GPIO_PADDR(imm, (MCF5272_RD_GPIO_PADDR(imm) | (1 << gpio_bit)));
	}
	else // Port B
	{
		// Make it an output
		MCF5272_WR_GPIO_PBDDR(imm, (MCF5272_RD_GPIO_PBDDR(imm) | (1 << (gpio_bit - 16))));
	}
	return 0;
}

