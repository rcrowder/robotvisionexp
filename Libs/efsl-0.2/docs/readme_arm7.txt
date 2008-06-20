*
*  efsl SD-Card SPI-Interface for 
*  - Philips LPC2000 ARM7TDMI-S (SPI, SSP, MCI)
*  - Atmel AT91SAM7S ARM7TDMI   
* 
*  The LPC2000 and AT91SAM7 interfaces are
*  Copyright (c) 2005, 2006, 2007
*  - Martin Thomas, Kaiserslautern, Germany
*    <mthomas@rhrk.uni-kl.de>
*    http://www.siwawi.arubi.uni-kl.de/avr_projects
*  - Mike Anton (LPC2000 SPI/SSP improvements)
*  - Juri Haberland (LPC2000 MCI)
*
*

All interfaces inherit the license from the 
main efsl-source.

See changelog.txt for latest modifications/additions.

-------------------------------------------------

Common "ARM7" Interface Information:

- A GNU arm-elf toolchain/cross-compiler
  must be available to build the examples
  and the library.
  Ready-made toolchains: i.e. WinARM or 
  GNUARM, Codesourcery gnu-Packages.
  Other ARM-compilers may work too but
  have not been tested.

- #define BYTE_ALIGNMENT is disabled
  for the ARM-interfaces in config*.h 
  It didn't work with alignment enabled.

- The hardware connection for SPI-mode is similar 
  to the connection described
  in the efsl-manual for Atmel AVRs.
  Pullup-Resistors should be optional.
  Tests did work without pullups.

-------------------------------------------------

For each interface an example application
is provided in the examples-directory. The 
examples can be build with the library 
libefsl.a or with the efsl source-code.

(1) To use the efsl-source "directly"

- Verfiy that conf/config.h is present
  and the settings match the used 
  interfaces (see templates).
- Modify the line in the example's 
  Makefile to: EFSL_AS_LIB = 0
- "make all" in the example's directory.

(2) To use the efsl-library (libefsl.a)

- Verfiy that conf/config.h is present
  and the settings match the used 
  interfaces (see templates).
- Build the library libefsl.a with
  the Makefile from the efsl root-
  directory for the used interface.
  (i.e. "make --makefile=Makefile-AT91 lib",
  "make --makefile=Makefile-LPC2000_MCI lib" 
  or "make --makefile=Makefile-LPC2000 lib" )
- Modify the line in the example's 
  Makefile to: EFSL_AS_LIB = 1
- "make all" in the example's directory


The example-applications demonstrate:

- Init of the efsl ARM debug-system
- Init of the efsl
- List contents of the SD-Card's
  root directory
- Open a file and type the content
- Append a line of text to a file
- "cat/type" the file's content again

The applications print status- and
debug-messages to the UART1 on
LPC2000 or DBGU on AT91SAM7
(115200,8,N,1,no FC). The LPC2368 example
uses 97600,8,N,1.

-------------------------------------------------

Additional Information for the LPC2000 Interface

- The efsl-interface supports SD-Cards
  connected to the LPC2000 SPI ("SPI0") 
  and SSP ("SPI1") interface. The used
  SPI-interface is hardcoded by the value 
  defined in config.h and can not be 
  changed during runtime.

- Only some parts of the LPC2000-family
  have the fast SSP-Interface (i.e. LPC213x,
  LPC214x) others only provide the slower 
  SPI interface. On some reviewed hardware-
  versions the SSP functionality has been added
  (refer to the latest datasheets).
  
- The MCI-interface of LPC2000 is supported.
  Only some members of the LPC2000-famiy offer 
  such an interface.

- To build the library for SPI/SSP create 
  the file conf/config.h based on the template 
  config-sample-lpc2000.h and use the 
  makefile Makefile-LPC2000 in the
  efsl root-directory.

- I have tested the interface with a 
  LPC2138-controller and the SPI(0) 
  and SSP/SPI1-Interface. Other 
  LPC2000-ARM7 controllers should be 
  supported too. (I have got positive
  feedback from LPC2148-users). 
  Verify that the register-adresses in 
  inc/interfaces/LPC2000_regs.h
  match the used controller and the
  maximum speed defined in lpc2000_sd.c
  is available for the part. Verify the 
  pin-connections in lpc2000_sd.c.
  Adjust the memory-setting in the linker-script.

- To build the library for MCI create 
  a file conf/config.h based on the template 
  config-sample-lpc2000\_MCI.h and use the 
  makefile Makefile-LPC2000\_MCI in the
  efsl root-directory.

- So far the LPC2000 MCI interface has only been
  tested with LPC2368 controllers. Other LPC2000-ARM7
  controllers with MCI should be supported too. Verify
  that the register-adresses in inc/interfaces/LPC2000_regs.h
  match the used controller and adjust the value of 
  TIME_OUT_VAL in lpc2000\_mci.h if needed.


-------------------------------------------------

Additional Information for the AT91 Interface

- The interface supports SD-Cards connected 
  to the AT91SAM7 SPI interface. 

- To build the library create a file 
  conf/config.h based on the template 
  config-sample-at91.h and use the 
  makefile Makefile-AT91 in the
  efsl-root-directory for AT91 (ARM7TDMI).

- DMA-mode can be enabled optionaly 
  (define DMA in at91_sd.c)

- The AT91SAM7 offers 4 chip-select
  signals which can be mapped to
  different pins. Chip-select is done
  by the hardware. See at91_sd.c
  for configuration options.
  Chip-Select can be fixed or variable
  (Fixed or variable peripheral select - see 
  option in AT91_sd.c, further information 
  in the datasheet).

- I have tested the interface with  AT91SAM7S256 
  and SAM7S64 controllers. Other AT91-ARM7 controllers 
  should be supported too.
  To adapt the code for another AT91 with SPI:
  - check if the register-defintions in AT91SAM7S_regs.h 
    are valid for the target. The file can be replaced
    by header file provided by Atmel for the used part 
	(see at91.com)
  - verify the maximum speed in at91_sd.c
  - verfiy the pin-connections in at91_sd.c
  - Check the memory-settings in the linker-script of
    the example.

- AT91 TODO: 16bit-transfer, SSC-support
  
-------------------------------------------------

Test Hard- and Software:

- Keil (keil.com) MCB2130 board with LPC2138
  with additional SD/MMC-connector (from Buerklin.de)
- Atmel AT91SAM7S-EK board with AT91SAM7S64
  with additional SD/MMC-connector
- SAM7-P board with AT91SAM7S256
  with on-board SD/MMC-connector
- Keil MCB2300 board with LPC2368 with on-board
  SD/MMC-connector connected to the MCI-pins

- SanDisk "standard" SD-Card 256MB
  (not the "highspeed"-version)
- extrememory SD-Card 1GB

- WinARM (arm-elf GNU toolchain)
- Bray++ Terminal

WinARM is available at:
http://www.siwawi.arubi.uni-kl.de/avr_projects/arm_projects

-------------------------------------------------

Credits:

- The code in efsl_debug_printf_arm.c is based 
  on code from Holger Klabunde and has been 
  modified to support a "fdevopen"-function 
  (efsl_debug_devopen_arm). 
  The original code has no copyright-notice and
  is derived from an free code by Volker Oth.

- Register-definitions for LPC213x are 
  based on a header-files from Keil/ARM 
  and NXP.
  
- Register-definitions for AT91SAM7 are from 
  at91.com (provided by Atmel).
  
- The basic method to handle the SPI interface
  on AT91 has been found in a free example from 
  Olimex.com.
  
- Mike Anton has contributed improvements for the
  LPC2000 interface (FastIO on LPC214x and SPI-FIFO code)

- Juri Haberland has contributed a LPC2000
  MCI interface

   
-------------------------------------------------


I hope the ARM-interfaces are useful for you.
Martin Thomas 8/2007
