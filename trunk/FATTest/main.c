/* 
 * This file is a simple example of an application that could be run
 * on top of the monitor.
 *
 * Cstart():
 * The Cstart() function depends on the setting of MONCOMPTR in config.h.
 * It demonstrates the use of monConnect and the first mon_XXX function
 * typically called by an application, mon_getargv().
 *
 * main():
 * The main() function demonstrates argument processing (thanks to
 * the call to mon_getargv() in start()), environment variables and
 * a simple use of TFS to dump the content of an ASCII file.
 * Also, if the first argument is "strace_demo", then the strace_demo()
 * function is called.  Refer to strace.c for details.
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../mx21.h"
#include "monlib.h"
#include "cfg.h"
#include "vcmx212.h"

typedef unsigned long ULONG;

// Define WARM_START_UMON if this application is NOT started via uMon:TFS
// It ensures that MicroMonitor is initialised and ready to use, regardless 
// of how the application was launched (using JTAG for example).
#define WARM_START_UMON

ULONG	AppStack[APPSTACKSIZE/4];
UCHAR	Sector[512];


int main(int argc,char *argv[])
{
	//char MonCmd[256];
	int i;

	/* If argument count is greater than one, then dump out the
	 * set of CLI arguments...
	 */
	if (argc > 1) {
		for(i=0;i<argc;i++) {
			mon_printf("  arg[%d]: %s\n",i,argv[i]);
		}
	}

	VCMX212_Init(0);

//	extern int SDHC_Init(int iVerbose);
//	SDHC_Init(1);

	int EFSL_Test(void);
	EFSL_Test();

	// Assign cf to FATFS
//	mon_docommand("cf -v init FATFS", 1);

	// Use -v to check cfInit has worked and has linked with fatfs properly (lots of stdout)
//	sprintf(MonCmd, "fatfs -v -r 0x%x -w 0x%x init", (long)cfRead, (long)cfWrite);
//	mon_docommand(MonCmd, 1);

	// Try a ls fatfs command (using our custom cfRead and cfWrite)
//	sprintf(MonCmd, "fatfs -v -r 0x%x -w 0x%x ls", (long)cfRead, (long)cfWrite);
//	mon_docommand(MonCmd, 1);

	return(0);
}

void
__gccmain()
{
}

int Cstart(void)
{
	char	**argv;
	int		argc;

//#ifdef WARM_START_UMON
	extern char		__bss_start, __bss_end;
	volatile char	*ramstart;

	/* Initialise application-owned BSS space.
	 * If this application is launched by TFS, then TFS does
	 * it autmatically, however since MicroMonitor provides
	 * other alternatives for launching an application, we
	 * clear bss here anyway (just in case TFS is not launching
	 * the app) ...
	 */
	ramstart = &__bss_start;
	while(ramstart < &__bss_end)
		*ramstart++ = 0;
//#endif

	/* Connect the application to the monitor.  This must be done
	 * prior to the application making any other attempts to use the
	 * "mon_" functions provided by the monitor.
	 */
	monConnect((int(*)())(*(unsigned long *)MONCOMPTR),(void *)0,(void *)0);

#ifdef WARM_START_UMON
	mon_warmstart(WARMSTART_ALL);
#endif

	/* When the monitor starts up an application, it stores the argument
	 * list internally.  The call to mon_getargv() retrieves the arg list
	 * for use by this application...
	 */
	mon_getargv(&argc,&argv);

	/* Call main, then exit to monitor.
	 */
	main(argc,argv);

	mon_appexit(0);
	return(0);
}


//.................................................................................................
// EFSL test
//
#include <efs.h>

int EFSL_Test(void)
{
	EmbeddedFileSystem	efs;
	EmbeddedFile		file_r;
	EmbeddedFile		file_w;
	unsigned short		i, e;
	char				buf[512];

	if (efs_init(&efs, NULL) != 0)
	{
		mon_printf("Could not open filesystem.\n");
		return (-1);
	}
	
	if (file_fopen(&file_r, &efs.myFs, "test.txt", 'r') != 0)
	{
		mon_printf("Could not open test.txt\n");
		return (-2);
	}
	if (file_fopen(&file_w, &efs.myFs, "copy.txt", 'w') != 0)
	{
		mon_printf("Could not open test.txt\n");
		return (-2);
	}

	while ((e = file_read(&file_r, 512, buf)))
	{
//		for (i = 0; i < e; i++)
//			mon_printf("%c", buf[i]);
		file_write(&file_w, e, buf);
	}
//	mon_printf("\n");

	file_fclose(&file_w);
	file_fclose(&file_r);

	fs_umount(&efs.myFs);

	return (0);
}
