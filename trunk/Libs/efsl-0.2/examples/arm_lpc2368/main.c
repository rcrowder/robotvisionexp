/* 
   efsl Demo-Application for NXP/Philips LPC23xx/24xx ARM7TDMI-S

   by Martin Thomas, Kaiserslautern, Germany
   http://www.siwawi.arubi.uni-kl.de/avr_projects/arm_projects

   Target:   LPC2368
   Board:    Keil MCB2300
   Compiler: GNU arm-elf or arm-eabi toolchain 
             (tested with WinARM 5/07-testing release)
 
   To be used with Juri Haberland's MCI-interface
*/

#define VERSION_STRING "0.1, mthomas, KL, .de"

#include <stdint.h>
#include <string.h>
#include <stdlib.h>

#include "LPC23xx.h"
#include "type.h"
#include "irq.h"
#include "target.h"
#include "timer.h"

#include "fio.h"
#include "uart.h"

#define BAUD 57600

#include "efs.h"
#include "ls.h"
#include "mkfs.h"
#include "interfaces/efsl_dbg_printf_arm.h"

#define rprintf efsl_debug_printf_arm

EmbeddedFileSystem efs;
EmbeddedFile filer, filew;
DirList list;
unsigned short e;
unsigned char buf[513];
static char LogFileName[] = "log23xxg.txt";

#if defined(__WINARMBOARD_KEIL_MCB2300__)
#define LED1_PORTNUM 2 /* GPIO 2 */
#define LED1_FIOPIN  FIO2PIN
#define LED1_FIOSET  FIO2SET
#define LED1_FIOCLR  FIO2CLR
#define LED1_MASK    (1UL<<0)
#define LED2_MASK    (1UL<<1)
#elif defined(__WINARMBOARD_OLIMEX_LPC_2378_STK__
// LED1 (MCIPWR on Olimex LPC-2378-STK has an indicator LED)
#define LED1_PORTNUM 0 /* GPIO0 */
#define LED1_FIOPIN  FIO0PIN
#define LED1_FIOSET  FIO0SET
#define LED1_FIOCLR  FIO0CLR
#define LED1_MASK    (1UL<<21)
#else
#error "Unkown Board"
#endif

static void time_waste( uint32_t dv )
{
	volatile uint32_t cnt;
	
	for ( cnt=0; cnt<dv ; cnt++ ) { ; }
}

static uint32_t systime_get(void)
{
	unsigned long ret;
	
	// state = IntDisable();
	ret = timer_counter;
	// IntRestore(state);
	
	return ret;
}

static void led1(int how)
{
	if (how) LED1_FIOPIN |= LED1_MASK;
	else LED1_FIOPIN &= ~LED1_MASK;
}

static signed char initcard(void)
{
	signed char res, retval;
	
	rprintf("CARD init...");
	if ( ( res = efs_init( &efs, 0 ) ) != 0 ) {
		rprintf("failed with %i\n",res);
		retval = -1;
	}
	else {
		led1(1);
		rprintf("ok\n");
		fs_umount( &efs.myFs ) ;
		led1(0);
		retval = 0;
	}
	return retval;
}

static void listrootdir(void)
{
	signed char res;
	
	rprintf("CARD init...");
	if ( ( res = efs_init( &efs, 0 ) ) != 0 ) {
		rprintf("failed with %i\n",res);
	}
	else {
		led1(1);
		rprintf("ok\n");
		rprintf("\nDirectory of 'root':\n");
		ls_openDir( &list, &(efs.myFs) , "/");
		while ( ls_getNext( &list ) == 0 ) {
			list.currentEntry.FileName[LIST_MAXLENFILENAME-1] = '\0';
			rprintf( "%s ( %li bytes )\n" ,
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
	
	rprintf("CARD init...");
	if ( ( res = efs_init( &efs, 0 ) ) != 0 ) {
		rprintf("failed with %i\n",res);
	}
	else {
		led1(1);
		rprintf("ok\n");
		if ( file_fopen( &filer, &efs.myFs , LogFileName , 'r' ) == 0 ) {
			rprintf("\nFile %s open. Content:\n", LogFileName);
			while ( ( e = file_read( &filer, 512, buf ) ) != 0 ) {
				buf[e]='\0';
				uart1Puts((char*)buf);
			}
			rprintf("\n");
			file_fclose( &filer );
		}
		fs_umount( &efs.myFs ) ;
		led1(0);
	}
}

static void append(void)
{
	signed char res;
	
	rprintf("CARD init...");
	if ( ( res = efs_init( &efs, 0 ) ) != 0 ) {
		rprintf("failed with %i\n",res);
	}
	else {
		led1(1);
		rprintf("ok\n");
		if ( file_fopen( &filew, &efs.myFs , LogFileName , 'a' ) == 0 ) {
			rprintf("\nFile %s open for append. Appending...", LogFileName);
			strcpy((char*)buf, "Martin hat's angehaengt\r\n");
			if ( file_write( &filew, strlen((char*)buf), buf ) == strlen((char*)buf) ) {
				rprintf("ok\n");
			}
			else {
				rprintf("failed\n");
			}
			file_fclose( &filew );
		}
		fs_umount( &efs.myFs ) ;
		led1(0);
	}
}


static void benchmark(void)
{
	signed char res;
	char bmfile[] = "bm4.txt";
	unsigned long starttime, deltat, bytes;
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
		rprintf("An error occured during write!\n");
	}
	rprintf("%lu bytes written in %lu ms (%lu KBytes/sec)\n", 
		bytes, deltat, (unsigned long)(((bytes/deltat)*1000UL)/1024UL) ) ;

	
	rprintf("Read benchmark start - from file %s (in %i bytes blocks)\n", 
		bmfile, l);
	
	if ( file_fopen( &filer, &efs.myFs , bmfile , 'r' ) != 0 ) {
		rprintf("\nfile_open for %s failed", bmfile);
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
	rprintf("%lu bytes read in %lu ms (%lu KBytes/sec)\n", 
		bytes, deltat, (unsigned long)(((bytes/deltat)*1000UL)/1024UL) ) ;
		
	fs_umount( &efs.myFs ); 
}


static char tfname[] = "tf4.bin";
static const uint32_t testmax = 1000000;
//static const uint32_t testmax = 4000;
static const uint16_t blklen = 512;

static void tf_write(void)
{
	uint32_t cnt;
	uint16_t i;
	int8_t res;
	int err;
	
	if ( ( res = efs_init( &efs, 0 ) ) != 0 ) {
		rprintf("efs_init failed with %i\n",res);
	}
	else {
		led1(1);
		if ( rmfile( &efs.myFs, (euint8*)tfname) == 0 ) {
			rprintf("File %s deleted.\n",  tfname);
		}
		else {
			rprintf("Failed to delete %s\n", tfname );
		}
		if ( (res = file_fopen( &filew, &efs.myFs , tfname , 'w' ) ) == 0 ) {
			rprintf("File %s open, writing...", tfname);
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
				rprintf("error\n");
			}
			else {
				rprintf("ok\n");
			}
		}
		else {
			rprintf("Open file %s failed with %i\n", tfname, res);
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
		rprintf("efs_init failed with %i\n",res);
	}
	else {
		led1(1);
		if ( file_fopen( &filer, &efs.myFs , tfname , 'r' ) == 0 ) {
			rprintf("File %s open. Verify...", tfname);
			cnt = 0;
			err = 0;
			while ( ( e = file_read( &filer, 512, buf ) ) != 0 ) {
				for ( i=0; i<e; i++ ) {
					if ( buf[i] != (uint8_t)(cnt) ) {
						rprintf("failed at position %lu\n", cnt);
						err = -1;
					}
					cnt++;
				}
			}
			file_fclose( &filer );
			if ( err ) {
				rprintf("FAILED.\n");
			}
			else {
				rprintf("o.k.\n");
			}
		}
		else {
			rprintf("Can not open testfile %s\n", tfname );
		}
		fs_umount( &efs.myFs ) ;
		led1(0);
	}
}



static int getline( char* inp, const int len, int *cnt )
{
	int c, mycnt, ret;
	
	ret = 0;
	mycnt = *cnt;
	
	if ( ( c = uart1Getch() ) > 0 ) {
		if ( c == '\r' ) {
			inp[mycnt] = '\0';
			ret = 1;
		}
		else if ( c == '\b' ) {
			if ( mycnt>0 ) {
				mycnt--;
				inp[mycnt] = '\0';
			}
		}
		else if ( mycnt < len-1 ) {
			inp[mycnt] = (char)c;
			mycnt++;
		}
	}
	
	*cnt = mycnt;
	return ret;
}

int main (void)
{
	typedef enum {
		ST_MENU_SHORT, ST_MENU_LONG, ST_GET_INP,
		ST_PROCESS_INP } main_state;
	main_state state;
	uint32_t mytime;
	// int8_t ret;
	int i, res, cnt, choice;
	
	char inp[10];
	
	
	// called in startup: TargetResetInit();
	
	GPIOInit( LED1_PORTNUM, FAST_PORT, DIR_OUT, LED1_MASK|LED2_MASK );
	
	// do some "alive" blinks
	for ( i=0; i<4; i++ ) {
		LED1_FIOPIN ^= LED1_MASK;
		time_waste( 0x00090000 );
	}
	led1(0);
	
	init_timer( TIME_INTERVAL );
	enable_timer( 0 );
	
	uart1Init( UART_BAUD(BAUD), UART_8N1, UART_FIFO_8 );

	uart1Puts( "\r\nMMC/SD-Card Filesystem Test (P:LPC2368 L:EFSL/MCI)\r\n" );
	uart1Puts( "Version " VERSION_STRING "\r\n" );
	uart1Puts( "done by Martin Thomas, Kaiserslautern, Germany\r\n" );
	
	/* init efsl debug-output */
	efsl_debug_devopen_arm(uart1Putch);
		
#if 0
	rprintf( "CARD init..." );
	if ( ( res = efs_init( &efs, 0 ) ) != 0 ) {
		rprintf( "failed with %i\n", res );
	}
	else {
		rprintf("ok\n");
		rprintf("Listing of root-directory:\n");
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
			strcpy( (char*)buf, "Martin hat's angehaengt\r\n" );
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

	state = ST_MENU_LONG;
	mytime = systime_get();
	inp[0] = '\0';
	cnt = 0;
	
	while ( 1 ) {
		switch (state) {
		case ST_MENU_LONG :
			rprintf("** Test Menu ** (testfile: %s)\n", LogFileName);
			rprintf("(1) Card-init\n");
			rprintf("(2) List root directory\n");
			rprintf("(3) List content of testfile\n");
			rprintf("(4) Append line to testfile\n");
			rprintf("(5) Benchmark (a simple one...)\n");
			rprintf("(6) Create file for verify-test\n");
			rprintf("(7) Verify-test\n");
			rprintf("Select > ");
			state = ST_GET_INP;
			break;
		case ST_MENU_SHORT :
			rprintf("Select (0 shows menu) > ");
			state = ST_GET_INP;
			break;
		case ST_GET_INP :
			res = getline( inp, 10, &cnt );
			if ( res == 1 ) {
				state = ST_PROCESS_INP;
			}
			break;
		case ST_PROCESS_INP :
			choice = atoi(inp);
			rprintf("Selected %i\n", choice);
			cnt = 0;
			inp[0] = '\0';

			switch (choice) {
			case 0 :
				state = ST_MENU_LONG;
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
				tf_write();
				break;
			case 7:
				tf_verify();
				break;
			default:
				rprintf("Invalid Choice\n");
				break;
			} /* switch choice */
			if ( choice != 0 ) {
				state = ST_MENU_SHORT;
			}
		} /* switch state */

		if ( (uint32_t)(systime_get()-mytime) >= 500 ) {
			LED1_FIOPIN ^= LED2_MASK;
			mytime = systime_get();
		}

	} // main-loop

	return 0;
}
