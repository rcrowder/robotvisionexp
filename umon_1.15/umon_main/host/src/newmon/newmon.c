/* newmon.c:
	General notice:
	This code is part of a boot-monitor package developed as a generic base
	platform for embedded system designs.  As such, it is likely to be
	distributed to various projects beyond the control of the original
	author.  Please notify the author of any enhancements made or bugs found
	so that all may benefit from the changes.  In addition, notification back
	to the author will allow the new user to pick up changes that may have
	been made by other users after this version of the code was distributed.

	Author:	Ed Sutter
	email:	esutter@lucent.com		(home: lesutter@worldnet.att.net)
	phone:	908-582-2351			(home: 908-889-5161)
*/
#define WIN32_LEAN_AND_MEAN

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <signal.h>
#include <errno.h>
#ifdef BUILD_WITH_VCC
#include <winsock.h>
#else
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#endif

#include "ttftp.h"
#include "moncmd.h"


char *errmsg[] = {
	"Usage: newmon [options] {sysname} {binfile}",
	" Options:",
	"  -A#    override default use of $APPRAMBASE",
	"  -B#    override default use of $BOOTROMBASE",
	"  -b#    override default flash bank 0",
	"  -p#    override default port number of 777",
	"  -u     issue 'flash unlock' prior to 'ewrite'",
	"  -v     verbose mode (print 'Sending...' message)",
	"  -V     show version of newmon.exe.",
	"  -w#    override default timeout of 10 seconds with '#' seconds",
	" Exit status:",
	"  0 if successful",
	"  1 if timeout waiting for command completion",
	"  2 if error",
	"  3 if timeout waiting for command reception",
	0,
};

void
usage(char *error)
{
	int	i;

	if (error)
		fprintf(stderr,"ERROR: %s\n",error);
	for(i=0;errmsg[i];i++)
		fprintf(stderr,"%s\n",errmsg[i]);
	exit(EXIT_ERROR);
}

void
testTftp(int i,char *cp,int j)
{
}

int
main(int argc,char *argv[])
{
	int		opt, flashbank, ret, unlock;
	char	*AppRamBase, *BootRomBase;
	char	cmd[128];
	short	portnum;
	struct	stat	mstat;

	if (argc == 1)
		usage(0);

	AppRamBase = "$APPRAMBASE";
	BootRomBase = "$BOOTROMBASE";
	flashbank = -1;
	unlock = tftpVerbose = 0;
	moncmd_init(argv[0]);
	ttftp_init();
	portnum = IPPORT_MONCMD;

	/* newmon will always generate a timeout error message, so we
	 * stifle that by forcing quietMode.  If -w is set below, then
	 * we assume that the user wants to see the error, so quietMode
	 * is cleared in that case.
	 */
	quietMode = 1;

	while((opt=getopt(argc,argv,"A:B:b:p:uw:vV")) != EOF) {
		switch(opt) {
		case 'A':
			AppRamBase = optarg;
			break;
		case 'B':
			BootRomBase = optarg;
			break;
		case 'b':
			flashbank = atoi(optarg);
			break;
		case 'p':
			portnum = atoi(optarg);
			break;
		case 'u':
			unlock = 1;
			break;
		case 'v':
			if (verbose == 1)
				tftpVerbose = 1;
			verbose = 1;
			break;
		case 'w':
			fwaitTime = atof(optarg);
			quietMode = 0;		/* see comment above */
			break;
		case 'V':
			showVersion();
			break;
		default:
			usage("bad option");
		}
	}

	if (optind+2 != argc)
		usage(0);

	if (stat(argv[optind+1],&mstat) == -1) {
		perror(argv[optind+1]);
		exit(EXIT_ERROR);
	}

	/* Convert wait time from float to some integral number
	 * of 100ths of a second...
	 */
	fwaitTime *= 100.0;
	waitTime = (int)fwaitTime;

	/* If do_moncmd() returns, then it succeeded... */
	do_moncmd(argv[optind], "version", portnum);

	/* Issue "echo $ETHERADD" so that it can be reloaded when the
	 * target restarts (this is useful if the target's uMon was
	 * built with INCLUDE_STOREMAC).
	 */
	do_moncmd(argv[optind], "echo $ETHERADD", portnum);

	if (fcrc32(argv[optind+1],0,1) < 0)
		exit(EXIT_ERROR);

	ret = ttftp(argv[optind],"put",argv[optind+1],AppRamBase,"octet");
	if (ret != EXIT_SUCCESS) {
		fprintf(stderr,"TFTP failed\n");
		exit(EXIT_ERROR);
	}

	if (flashbank != -1) {
		sprintf(cmd,"flash bank %d",flashbank);
		do_moncmd(argv[optind], cmd, portnum);
	}

	if (unlock)
		do_moncmd(argv[optind], "flash unlock", portnum);

	do_moncmd(argv[optind], "flash opw", portnum);

	sprintf(cmd,"flash ewrite %s %s %d",
		BootRomBase,AppRamBase,mstat.st_size);

	waitTime = 0;
	do_moncmd(argv[optind], cmd, portnum);

	printf("The 'flash ewrite' command will take ~30 seconds to complete\n");
	exit(EXIT_SUCCESS);
}
