/******************************************************************************
 
 efsl Demo-Application for NXP/Philips LPC2138 ARM7TDMI-S
 
 Copyright (c) 2006
 Martin Thomas, Kaiserslautern, Germany <mthomas@rhrk.uni-kl.de>
 
 Inherits the efsl's license.
 
 *****************************************************************************/

#include <string.h>

#include "LPC213x.h"
#include "lpc_config.h"
#include "sysTime.h"

#include "uart.h"

#include "efs.h"
#include "ls.h"
#include "mkfs.h"
#include "interfaces/efsl_dbg_printf_arm.h"

#define rprintf efsl_debug_printf_arm

#define BAUD 115200

#define BIT(x) ((unsigned long)1<<x)

// 1st LED on Keil MCB2130
#define LED1PIN  	17
#define LED2PIN     16
#define LED1BIT     BIT(LED1PIN)
#define LED2BIT     BIT(LED2PIN)
#define LEDDIR      IODIR1
#define LEDSET      IOSET1
#define LEDCLR      IOCLR1

static char LogFileName[] = "logdatb.txt";

static void systemInit(void)
{
	// --- enable and connect the PLL (Phase Locked Loop) ---
	// a. set multiplier and divider
	PLLCFG = MSEL | (1<<PSEL1) | (0<<PSEL0);
	// b. enable PLL
	PLLCON = (1<<PLLE);
	// c. feed sequence
	PLLFEED = PLL_FEED1;
	PLLFEED = PLL_FEED2;
	// d. wait for PLL lock (PLOCK bit is set if locked)
	while (!(PLLSTAT & (1<<PLOCK)));
	// e. connect (and enable) PLL
	PLLCON = (1<<PLLE) | (1<<PLLC);
	// f. feed sequence
	PLLFEED = PLL_FEED1;
	PLLFEED = PLL_FEED2;
	
	// --- setup and enable the MAM (Memory Accelerator Module) ---
	// a. start change by turning of the MAM (redundant)
	MAMCR = 0;	
	// b. set MAM-Fetch cycle to 3 cclk as recommended for >40MHz
	MAMTIM = MAM_FETCH;
	// c. enable MAM 
	MAMCR = MAM_MODE;
	
	// --- set VPB speed ---
	VPBDIV = VPBDIV_VAL;
	
	// --- map INT-vector ---
	#if defined(RAM_RUN)
	  MEMMAP = MEMMAP_USER_RAM_MODE;
	#elif defined(ROM_RUN)
	  MEMMAP = MEMMAP_USER_FLASH_MODE;
	#else
	#error RUN_MODE not defined!
	#endif
	
	initSysTime(); 
}

static void gpioInit(void)
{
	LEDSET  = BIT(LED1PIN)|BIT(LED2PIN);	 // set Bit = LED off (active low)
	LEDDIR |= BIT(LED1PIN)|BIT(LED2PIN); // define LED-Pin as output
}

static void ledToggle(void)
{
	static unsigned char state=0;
	
	state = !state;
	if (state) LEDCLR = BIT(LED1PIN);	// set Bit = LED on
	else LEDSET = BIT(LED1PIN);	// set Bit = LED off (active low)
}

static void led2Toggle(void)
{
	static unsigned char state=0;
	
	state = !state;
	if (state) LEDCLR = BIT(LED2PIN);	// set Bit = LED on
	else LEDSET = BIT(LED2PIN);	// set Bit = LED off (active low)
}

#if 0
static void hang(void)
{
	while(1);
}
#endif


EmbeddedFileSystem efs;
EmbeddedFile filer, filew;
DirList list;
unsigned short e;
unsigned char buf[513];

static uint32_t tic_diffMS(uint32_t dif)
{
	return dif/(sysTICSperSEC/1000);
}

static void benchmark(void)
{
	signed char res;
	char bmfile[] = "bm2ka.txt";
	unsigned long starttic, deltat, bytes;
	int error;
	unsigned short l = 100;
	
	if ( ( res = efs_init( &efs, 0 ) ) != 0 ) {
		rprintf("efs_init failed with %i\n",res);
		return;
	}
	
	rmfile( &efs.myFs, (euint8*)bmfile );
	
	if ( file_fopen( &filew, &efs.myFs , bmfile , 'w' ) != 0 ) {
		rprintf("\nfile_open for %s failed", bmfile);
		fs_umount( &efs.myFs );
		return;
	}
	
	rprintf("Write benchmark start - write to file %s (%i bytes/write)\n", 
		bmfile, l);
	
	bytes = 0;
	error = 0;
	starttic = getSysTICs();
	
	do { 
		if ( file_write( &filew, l, buf ) != l ) {
			error = 1;
		}
		else {
			bytes+=l;
		}
	} while ( ( getElapsedSysTICs(starttic) < FIVE_SEC ) && !error );
	
	file_fclose( &filew );
	fs_flushFs( &(efs.myFs) ); // close & flushing included in time
	
	deltat = tic_diffMS(getElapsedSysTICs(starttic));

	if ( error ) {
		rprintf("An error occured during write!\n");
	}
	rprintf("%lu bytes written in %lu ms (%lu KBytes/sec)\n", 
		bytes, 
		deltat,
		bytes*1000/deltat/1024
	);

	
	rprintf("Read benchmark start - from file %s (in %i bytes blocks)\n", 
		bmfile, l);
	
	if ( file_fopen( &filer, &efs.myFs , bmfile , 'r' ) != 0 ) {
		rprintf("\nfile_open for %s failed", bmfile);
		fs_umount( &efs.myFs );
		return;
	}

	bytes = 0;
	error = 0;
	starttic = getSysTICs();
		
	while ( ( e = file_read( &filer, l, buf ) ) != 0 ) {
		bytes += e;
	}

	file_fclose( &filer );

	deltat = tic_diffMS(getElapsedSysTICs(starttic));
	rprintf("%lu bytes read in %lu ms (%lu KBytes/sec)\n", 
		bytes, 
		deltat, 
		bytes*1000/deltat/1024 ) ;
		
	fs_umount( &efs.myFs ); 
}

int main(void)
{
	int ch;
	int8_t res;
	uint32_t startTime, t;
	
	systemInit();
	gpioInit();
	
	uart1Init(UART_BAUD(BAUD), UART_8N1, UART_FIFO_8); // setup the UART
		
	uart1Puts("\r\nMMC/SD Card Filesystem Test (P:LPC2138 L:EFSL)(##)\r\n");
	uart1Puts("efsl LPC2000-port and this Demo-Application\r\n");
	uart1Puts("done by Martin Thomas, Kaiserslautern, Germany\r\n");
	
	/* init efsl debug-output */
	efsl_debug_devopen_arm(uart1Putch);
		
	ledToggle();
	
#if 1
	rprintf("CARD init...");
	if ( ( res = efs_init( &efs, 0 ) ) != 0 ) {
		rprintf("failed with %i\n",res);
	}
	else {
		rprintf("ok\n");
		rprintf("Directory of 'root':\n");
		ls_openDir( &list, &(efs.myFs) , "/");
		while ( ls_getNext( &list ) == 0 ) {
			list.currentEntry.FileName[LIST_MAXLENFILENAME-1] = '\0';
			rprintf( "%s ( %li bytes )\n" ,
				list.currentEntry.FileName,
				list.currentEntry.FileSize ) ;
		}
		
		if ( file_fopen( &filer, &efs.myFs , LogFileName , 'r' ) == 0 ) {
			rprintf("File %s open. Content:\n", LogFileName);
			while ( ( e = file_read( &filer, 512, buf ) ) != 0 ) {
				buf[e]='\0';
				uart1Puts((char*)buf);
			}
			rprintf("\n");
			file_fclose( &filer );
		}
		
		if ( file_fopen( &filew, &efs.myFs , LogFileName , 'a' ) == 0 ) {
			rprintf("File %s open for append. Appending...", LogFileName);
			strcpy((char*)buf, "Martin hat's angehaengt\r\n");
			if ( file_write( &filew, strlen((char*)buf), buf ) == strlen((char*)buf) ) {
				rprintf("ok\n\n");
			}
			else {
				rprintf("fail\n\n");
			}
			file_fclose( &filew );
		}
		
		if ( file_fopen( &filer, &efs.myFs , LogFileName , 'r' ) == 0 ) {
			rprintf("File %s open. Content:\n", LogFileName);
			while ( ( e = file_read( &filer, 512, buf ) ) != 0 ) {
				buf[e]='\0';
				uart1Puts((char*)buf);
			}
			rprintf("\n");
			file_fclose( &filer );
		}
		
		fs_umount( &efs.myFs ) ;
	}
#endif
	
	startTime = getSysTICs();
	
	while(1) {
		if ((ch = uart1Getch()) >= 0) {
			uart1Puts("You pressed : ");
			uart1Putch(ch);
			uart1Puts("\r\n");
			if ( ch == 'M' ) {
				rprintf("Creating FS\n");
				rprintf("Disabled\n");
				// mkfs_makevfat(&efs.myPart);
			}
			if ( ch == 'b' ) {
				benchmark();
			}
			ledToggle();
		}
		t = getElapsedSysTICs(startTime);
		if ( t > HALF_SEC ) {
			// rprintf("%lu %lu\n", t, tic_diffMS(t));
			startTime += HALF_SEC;
			led2Toggle();
		}
	}
	
	return 0; /* never reached */
}
