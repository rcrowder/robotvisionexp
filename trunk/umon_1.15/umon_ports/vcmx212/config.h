/* MicroMonitor configuration file.
 *
 * Virtual Cogs VCMX212
 *
 * Maintained By: Tarun Tuli  tarun@virtualcogs.com
 * September 18, 2006
 */


// Ethernet setup
#define     DEFAULT_ETHERADD "00:01:02:03:04:00"  	
#define     DEFAULT_IPADD    "192.168.0.55"		

// Device is little endian
#define CPU_LE

/* XBUFCNT & RBUFCNT:
 *  Number of transmit and receive buffers allocated to ethernet.
 *  The total of XBUFCNT+RBUFCNT should not exceed MAXEBDS
 */
#define XBUFCNT 	8
#define RBUFCNT 	8
#define XBUFSIZE	2048
#define RBUFSIZE	4096

// override the app ram base to set it to 2Mbyte.  This reserves space
// for umon and the LCD controller buffer
#define APPRAMBASE_OVERRIDE 0xC0200000

/* PLATFORM_NAME:
 * Some string that briefly describes your target.
 */
#define PLATFORM_NAME   "Virtual Cogs VCMX212"
#define PLATFORM_TYPE   VCMX212

/* CPU_NAME:
 * The name of your CPU (i.e. "PowerPC 405", "Coldfire 5272" ,etc...). 
 */
#define CPU_NAME    "MC9328MX21 ARM926EJS"
#define CPU_TYPE    MC9328MX21

/* LOOPS_PER_SECOND:
 * Approximately the size of a loop that will cause a 1-second delay.
 * This value can be adjusted by using the "sleep -c" command.
 */
#define LOOPS_PER_SECOND	100000

/* PRE_COMMANDLOOP_HOOK():
 * This is a port-specific function that will be called just before
 * MicroMonitor turns over control to the endless command processing
 * loop in start() (start.c).
#define PRE_COMMANDLOOP_HOOK()	func()
 */

/* Flash bank configuration:
 * Basic information needed to configure the flash driver.
 * Fill in port specific values here.
 */
#define SINGLE_FLASH_DEVICE 	1
#define FLASH_COPY_TO_RAM 		1
#define FLASH_BANK0_BASE_ADDR  	0xc8000000			//ncs0
#define FLASH_PROTECT_RANGE  	"0-16"
#define FLASH_BANK0_WIDTH       2
#define FLASH_LARGEST_SECTOR    0x10000
#define FLASH_BANK0_SIZE		0xFFFFFF			// 16MB
#define FLASH_TIMEOUT			100000

/* If there is a need to have the range of protected sectors locked (and
 * the flash device driver supports it), then enable this macro...
#define LOCK_FLASH_PROTECT_RANGE
 */

/* TFS definitions:
 * Values that configure the flash space that is allocated to TFS.
 * Fill in port specific values here.
 */
#define TFSSPARESIZE    		FLASH_LARGEST_SECTOR
#define TFSSTART        		(FLASH_BANK0_BASE_ADDR+(FLASH_LARGEST_SECTOR*17))
#define TFSEND          		(TFSSTART+0x003fffff)
//(FLASH_BANK0_BASE_ADDR+(FLASH_SIZE-(FLASH_LARGEST_SECTOR*17)))
#define TFSSPARE        		(TFSEND+1)
#define TFSSECTORCOUNT			((TFSSPARE-TFSSTART)/FLASH_LARGEST_SECTOR)
#define TFS_EBIN_ELF    		1
#define TFS_VERBOSE_STARTUP		1
//#define TFS_ALTDEVTBL_BASE		&alt_tfsdevtbl_base
# define FLASHBANKS				1


/* ALLOCSIZE:
 * Specify the size of the memory block (in monitor space) that
 * is to be allocated to malloc in the monitor.  Note that this
 * size can be dynamically increased using the heap command at
 * runtime.
 */
#define ALLOCSIZE 		(64*1024)

/* MONSTACKSIZE:
 * The amount of space allocated to the monitor's stack.
 */
#define MONSTACKSIZE	(32*1024)

/* INCLUDE_XXX Macros:
 * Set/clear the appropriate macros depending on what you want
 * to configure in your system.  The sanity of this list is tested
 * through the inclusion of "inc_check.h" at the bottom of this list...
 * When starting a port, set all but INCLUDE_MALLOC, INCLUDE_SHELLVARS,
 * INCLUDE_MEMCMDS and INCLUDE_XMODEM to zero.  Then in small steps
 * enable the following major features in the following order:
 *
 *	INCLUDE_FLASH to test the flash device drivers.
 *	INCLUDE_TFS* to overlay TFS onto the flash. 
 *  INCLUDE_ETHERNET, INCLUDE_TFTP to turn the ethernet interface.
 *
 * All other INCLUDE_XXX macros can be enabled as needed and for the
 * most part, they're hardware independent.
 * Note that for building a very small footprint, INCLUDE_MALLOC and
 * INCLUDE_SHELLVARS can be disabled.
 */


#define INCLUDE_MALLOC			1
#define INCLUDE_MEMCMDS         1
#define INCLUDE_SHELLVARS		1
#define INCLUDE_XMODEM          1
#define INCLUDE_EDIT            1
#define INCLUDE_DISASSEMBLER    0
#define INCLUDE_UNZIP           1
#define INCLUDE_ETHERNET        0
#define INCLUDE_ICMP			0
#define INCLUDE_TFTP            0
#define INCLUDE_TFS             1
#define INCLUDE_FLASH           1
#define INCLUDE_LINEEDIT        1
#define INCLUDE_DHCPBOOT        0
#define INCLUDE_TFSAPI          1
#define INCLUDE_TFSSYMTBL       1
#define INCLUDE_TFSSCRIPT       1
#define INCLUDE_TFSCLI          1
#define INCLUDE_EE              0
#define INCLUDE_GDB             0
#define INCLUDE_STRACE          0
#define INCLUDE_CAST            0
#define INCLUDE_STRUCT          0
#define INCLUDE_REDIRECT        0
#define INCLUDE_QUICKMEMCPY     0
#define INCLUDE_PROFILER        0
#define INCLUDE_BBC             0
#define INCLUDE_MEMTRACE        1
#define INCLUDE_STOREMAC        1
#define INCLUDE_VERBOSEHELP     1
#define INCLUDE_HWTMR	 	    0
#define INCLUDE_PORTCMD	 	    0
#define INCLUDE_USRLVL	 	    1

/* Some fine tuning (if needed)...
 * If these #defines are not in config.h, they default to '1' in
 * various other include files within uMon source; hence, they're
 * really only useful if you need to turn off ('0') the specific
 * facility or block of code.
 */
#define INCLUDE_TFTPSRVR		0
#define INCLUDE_ETHERVERBOSE	0
#define INCLUDE_MONCMD			0

#define INCLUDE_JFFS2			1
#define INCLUDE_JFFS2_ZLIB		1
#define TFS_DISABLE_AUTODEFRAG  0

/* RJC: Additions for CF and FATFS
		- Dont forget to add cf.c dosfs.c and fatfs.c into makefile COMCSRC list
 */
#define INCLUDE_CF				1
#define INCLUDE_FATFS			1

#include "inc_check.h"
