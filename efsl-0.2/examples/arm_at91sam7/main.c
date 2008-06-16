//*----------------------------------------------------------------------------
//*
//* AT91SAM7 efsl example (9/2006)
//* by Martin Thomas, Kaiserslautern, Germany 
//* <mthomas@rhrk.uni-kl.de>
//* http://www.siwawi.arub.uni-kl.de/avr_projects
//*
//* Inherits the efsl license.
//*
//*----------------------------------------------------------------------------

#include <string.h>
#include <stdint.h>

#include "Board.h"
#include "swi.h"
#include "systime.h"
#include "dbgu.h"

#include "efs.h"
#include "ls.h"
#include "mkfs.h"
#include "interfaces/efsl_dbg_printf_arm.h"

#define TCK  1000                           /* Timer Clock  */
#define PIV  ((MCK/TCK/16)-1)               /* Periodic Interval Value */

EmbeddedFileSystem efs;
EmbeddedFile filer, filew;
DirList list;
unsigned short e;
unsigned char buf[513];

static char LogFileName[] = "logSAM_7.txt";

static void led1(int on)
{
	AT91PS_PIO  pPIOA = AT91C_BASE_PIOA;
	
	if (on) pPIOA->PIO_CODR = LED1;
	else pPIOA->PIO_SODR = LED1;
}

static void led2(int on)
{
	AT91PS_PIO  pPIOA = AT91C_BASE_PIOA;
	
	if (on) pPIOA->PIO_CODR = LED2;
	else pPIOA->PIO_SODR = LED2;
}


static void benchmark()
{
	signed char res;
	char bmfile[] = "bm2.txt";
	unsigned long starttime, deltat, bytes;
	int error;
	unsigned short l = 100;
	
	if ( ( res = efs_init( &efs, 0 ) ) != 0 ) {
		iprintf("efs_init failed with %i\n",res);
		return;
	}
	
	rmfile( &efs.myFs, (euint8*)bmfile );
	
	if ( file_fopen( &filew, &efs.myFs , bmfile , 'w' ) != 0 ) {
		iprintf("\nfile_open for %s failed", bmfile);
		fs_umount( &efs.myFs );
		return;
	}
		
	iprintf("Write benchmark start - write to file %s (%i bytes/write)\n", 
		bmfile, l);
	
	bytes = 0;
	error = 0;
	starttime = systime_get();	// millisec.
	
	do { 
		if ( file_write( &filew, l, buf ) != l ) {
			error = 1;
		}
		else {
			bytes+=l;
		}
		deltat = (unsigned long)(systime_get()-starttime);
	} while ( ( deltat < 5000UL ) && !error );
	
	file_fclose( &filew );
	fs_flushFs( &(efs.myFs) ); // close & flushing included in time
	
	deltat = (unsigned long)(systime_get()-starttime);
	if ( error ) {
		iprintf("An error occured during write!\n");
	}
	iprintf("%lu bytes written in %lu ms (%lu KBytes/sec)\n", 
		bytes, deltat, (unsigned long)(((bytes/deltat)*1000UL)/1024UL) ) ;

	
	iprintf("Read benchmark start - from file %s (in %i bytes blocks)\n", 
		bmfile, l);
	
	if ( file_fopen( &filer, &efs.myFs , bmfile , 'r' ) != 0 ) {
		iprintf("\nfile_open for %s failed", bmfile);
		fs_umount( &efs.myFs );
		return;
	}

	bytes = 0;
	error = 0;
	starttime = systime_get();	// millisec.
		
	while ( ( e = file_read( &filer, l, buf ) ) != 0 ) {
		bytes += e;
	}

	file_fclose( &filer );

	deltat = (unsigned long)(systime_get()-starttime);
	iprintf("%lu bytes read in %lu ms (%lu KBytes/sec)\n", 
		bytes, deltat, (unsigned long)(((bytes/deltat)*1000UL)/1024UL) ) ;
		
	fs_umount( &efs.myFs ); 
}

signed char initcard(void)
{
	signed char res, retval;
	
	iprintf("CARD init...");
	if ( ( res = efs_init( &efs, 0 ) ) != 0 ) {
		iprintf("failed with %i\n",res);
		retval = -1;
	}
	else {
		led1(1);
		iprintf("ok\n");
		fs_umount( &efs.myFs ) ;
		led1(0);
		retval = 0;
	}
	return retval;
}

static void listrootdir(void)
{
	signed char res;
	
	iprintf("CARD init...");
	if ( ( res = efs_init( &efs, 0 ) ) != 0 ) {
		iprintf("failed with %i\n",res);
	}
	else {
		led1(1);
		iprintf("ok\n");
		iprintf("\nDirectory of 'root':\n");
		ls_openDir( &list, &(efs.myFs) , "/");
		while ( ls_getNext( &list ) == 0 ) {
			list.currentEntry.FileName[LIST_MAXLENFILENAME-1] = '\0';
			iprintf( "%s ( %li bytes )\n" ,
				list.currentEntry.FileName,
				list.currentEntry.FileSize ) ;
		}
		fs_umount( &efs.myFs ) ;
		led1(0);
	}
}

static void listfilecontent(void)
{
	signed char res;
	
	iprintf("CARD init...");
	if ( ( res = efs_init( &efs, 0 ) ) != 0 ) {
		iprintf("failed with %i\n",res);
	}
	else {
		led1(1);
		iprintf("ok\n");
		if ( file_fopen( &filer, &efs.myFs , LogFileName , 'r' ) == 0 ) {
			iprintf("\nFile %s open. Content:\n", LogFileName);
			while ( ( e = file_read( &filer, 512, buf ) ) != 0 ) {
				buf[e]='\0';
				AT91F_DBGU_Printk((char*)buf);
			}
			iprintf("\n");
			file_fclose( &filer );
		}
		fs_umount( &efs.myFs ) ;
		led1(0);
	}
}

static void append(void)
{
	signed char res;
	
	iprintf("CARD init...");
	if ( ( res = efs_init( &efs, 0 ) ) != 0 ) {
		iprintf("failed with %i\n",res);
	}
	else {
		led1(1);
		iprintf("ok\n");
		if ( file_fopen( &filew, &efs.myFs , LogFileName , 'a' ) == 0 ) {
			iprintf("\nFile %s open for append. Appending...", LogFileName);
			strcpy((char*)buf, "Martin hat's angehaengt\r\n");
			if ( file_write( &filew, strlen((char*)buf), buf ) == strlen((char*)buf) ) {
				iprintf("ok\n");
			}
			else {
				iprintf("failed\n");
			}
			file_fclose( &filew );
		}
		fs_umount( &efs.myFs ) ;
		led1(0);
	}
}

#ifndef OX_SAM7_P
#warning CP and WP defined for the SAM7-P eval-board
#endif
/* CP is at PA15 on SAM7-P eval-board, on-board ext. pull-up 
   WP is at PA16 on SAM7-P eval-board, on-board ext. pull-up */
#define CP_INP           AT91C_PIO_PA15 
#define CP_HASEXTPULLUP  1
#define WP_INP           AT91C_PIO_PA16
#define WP_HASEXTPULLUP  1

static void check_wpcp(void)
{
	unsigned long reg;
	AT91PS_PIO  pPIOA = AT91C_BASE_PIOA;
	
	// disable internal Pull-Ups if needed
	if (CP_HASEXTPULLUP) {
		pPIOA->PIO_PPUDR = CP_INP;
	}
	if (WP_HASEXTPULLUP) {
		pPIOA->PIO_PPUDR = WP_INP;
	}
	// iprintf("Pull-Up-Status 0x%08x\n", pPIOA->PIO_PPUSR);
	
	// configure as inputs
	pPIOA->PIO_ODR = CP_INP | WP_INP;
	// enable control of pins by PIO
	pPIOA->PIO_PER = CP_INP | WP_INP;

	iprintf("The board my not offer connections to the WP and CP switches\n"
	        "of the card-socket.\n"
	        "EFSL will work without these switches.\n");
	
	// read status
	reg = pPIOA->PIO_PDSR;
	// iprintf("Input-Status 0x%08x\n", reg);
	iprintf("Card-presence detection by socket-switch : ");
	if ( reg & CP_INP ) {
		iprintf("not detected\n");
	}
	else {
		iprintf("detected\n");
		iprintf("Write-protection detection by socket-switch : ");
		if ( reg & WP_INP ) {
			iprintf("protected\n");
		}
		else {
			iprintf("not protected\n");
		}
	}
}

//----

static char tfname[] = "tf2.bin";
static const uint32_t testmax = 1000000;
static const uint16_t blklen = 512;

static void tf_write(void)
{
	uint32_t cnt;
	uint16_t i;
	int8_t res;
	int err;
	
	if ( ( res = efs_init( &efs, 0 ) ) != 0 ) {
		iprintf("efs_init failed with %i\n",res);
	}
	else {
		led1(1);
		if ( rmfile( &efs.myFs, (euint8*)tfname) == 0 ) {
			iprintf("File %s deleted.\n",  tfname);
		}
		if ( (res = file_fopen( &filew, &efs.myFs , tfname , 'w' ) ) == 0 ) {
			iprintf("File %s open, writing...", tfname);
			fflush(stdout);
			cnt = 0;
			err = 0;
			do {
				for ( i=0; i<blklen; i++) {
					buf[i] = (uint8_t)cnt;
					cnt++;
				}
				if ( file_write( &filew, blklen, buf ) != blklen ) {
					err = -1;
				}
			} while ( (cnt<testmax) && (err==0) );
			file_fclose( &filew );
			if ( err ) {
				iprintf("error\n");
			}
			else {
				iprintf("ok\n");
			}
		}
		else {
			iprintf("Open file %s failed with %i\n", tfname, res);
		}
		fs_umount( &efs.myFs ) ;
		led1(0);
	}
}

static void tf_verify(void)
{
	signed char res;
	uint32_t cnt;
	uint16_t i;
	int err;

	if ( ( res = efs_init( &efs, 0 ) ) != 0 ) {
		iprintf("efs_init failed with %i\n",res);
	}
	else {
		led1(1);
		if ( file_fopen( &filer, &efs.myFs , tfname , 'r' ) == 0 ) {
			iprintf("File %s open. Verify...", tfname);
			fflush(stdout);
			cnt = 0;
			err = 0;
			while ( ( e = file_read( &filer, 512, buf ) ) != 0 ) {
				for ( i=0; i<e; i++ ) {
					if ( buf[i] != (uint8_t)(cnt) ) {
						iprintf("failed at position %lu\n", cnt);
						err = -1;
					}
					cnt++;
				}
			}
			file_fclose( &filer );
			if ( err ) {
				iprintf("FAILED.\n");
			}
			else {
				iprintf("o.k.\n");
			}
		}
		fs_umount( &efs.myFs ) ;
		led1(0);
	}
}


static void dump_interrupt_state(void)
{
	unsigned long cpsr;
	const unsigned long I_Bit = 0x80;
	const unsigned long F_Bit = 0x40;
	
	cpsr = IntGetCPSR();
	
	iprintf("State : stat-reg 0x%08lx -> ", cpsr);
	
	if ( cpsr & I_Bit ) {
		iprintf("IRQ disabled, ");
	}
	else {
		iprintf("IRQ enabled, ");
	}
	
	if ( cpsr & F_Bit ) {
		iprintf("FIQ disabled\n");
	}
	else {
		iprintf("FIQ enabled\n");
	}
}

int main(void)
{
	int drawmenu;
	unsigned int choice;
	
	AT91PS_PMC  pPMC  = AT91C_BASE_PMC;
	AT91PS_PIO  pPIOA = AT91C_BASE_PIOA;
	AT91PS_RSTC pRSTC = AT91C_BASE_RSTC;
	
	// Enable the clock for PIO and UART0
	pPMC->PMC_PCER = ( ( 1 << AT91C_ID_PIOA ) | ( 1 << AT91C_ID_US0 ) ); // n.b. IDs are just bit-numbers
	
	// Configure the PIO Lines corresponding to LED1 to LED4
	pPIOA->PIO_PER = LED_MASK; // pins controlled by PIO (GPIO)
	pPIOA->PIO_OER = LED_MASK; // pins outputs
	
	// Turn off the LEDs. Low Active: set bits to turn off 
	pPIOA->PIO_SODR = LED_MASK;
	
	// enable reset-key on demo-board 
	pRSTC->RSTC_RMR = (0xA5000000 | AT91C_RSTC_URSTEN);

	systime_init();

	AT91F_DBGU_Init();
	
	IntEnable(); // enable INT-exceptions
	
	iprintf("\n\nAT91SAM7 Filesystem-Demo (P:AT91SAM7S L:efsl)\n"
	        "efsl AT91-Interface and this Demo-Application\n"
	        "done by Martin Thomas, Kaiserslautern, Germany\n\n");
	
	/* init efsl debug-output */
	efsl_debug_devopen_arm(AT91F_DBGU_putc);
	
	drawmenu = 1;

	// main-loop
	while (1) {
		
		if ( drawmenu ) {
			iprintf("** Test Menu ** (testfile: %s)\n", LogFileName);
			iprintf("(1) Card-init\n");
			iprintf("(2) List root directory\n");
			iprintf("(3) List content of testfile\n");
			iprintf("(4) Append line to testfile\n");
			iprintf("(5) Benchmark (a simple one...)\n");
			iprintf("(6) Check socket switches (CP/WP)\n");
			iprintf("(7) LED1 on\n");
			iprintf("(8) LED1 off\n");
			iprintf("(9) Create file for verify-test\n");
			iprintf("(10) Verify-test\n");
#if 0			
			iprintf("(11) INT-State\n");
			iprintf("(12) LED2 on\n");
			iprintf("(13) LED2 off\n");
#endif
			iprintf("Select > ");
			drawmenu = 0;
		}
		else {
			iprintf("Select (0 shows menu) > ");
		}
		fflush(stdout);
		AT91F_DBGU_scanf("%i", &choice);
		iprintf("Selected %i\n", choice);

		switch (choice) {
		case 0 :
			drawmenu = 1;
			break;
		case 1 :
			initcard();
			break;
		case 2 :
			listrootdir();
			break;
		case 3 :
			listfilecontent();
			break;
		case 4 :
			append();
			break;
		case 5 :
			benchmark();
			break;
		case 6 :
			check_wpcp();
			break;
		case 7 : 
			led1(1);
			break;
		case 8 :
			led1(0);
			break;
		case 9 :
			tf_write();
			break;
		case 10:
			tf_verify();
			break;
		case 11:
			dump_interrupt_state();
			break;
		case 12: 
			led2(1);
			break;
		case 13:
			led2(0);
			break;
		default:
			iprintf("Invalid Choice\n");
			break;
		}
	}
	
	return 0; /* never reached */
}
