/* vi: set ts=4: */
/* sd.c:
 * This code is the user interface portion of the sd (compact flash)
 * command for uMon.
 * This command is intended to be the "interface" portion of some
 * other command (for example "fatfs").  Refer to the discussion in
 * fatfs.c for more details.
 */

#include "config.h"
#if INCLUDE_SD
#include "stddefs.h"
#include "genlib.h"
#include "cli.h"
#include "tfs.h"
#include "tfsprivate.h"
#include "sd.h"


char *SdHelp[] = {
	"Secure Digital Flash Interface",
	"[options] {operation} [args]...",
#if INCLUDE_VERBOSEHELP
	"",
	"Options:",
	" -v     enable verbosity",
	"",
	"Operations:",
	" init [prefix]",
	" read {dest} {blk} {blktot}",
	" write {blk} {dest} {blktot}",
#endif
	0
};

static int sdInum;		/* Interface number: to support multiple SD interfaces.
						 * Typically this will always be zero.
						 */

int
SdCmd(int argc, char *argv[])
{
	char *cmd, *buf, *prefix, varname[16];
	int opt, verbose, sdret, blknum, blkcnt;

	verbose = 0;
	while ((opt=getopt(argc,argv,"i:r:w:v")) != -1) {
		switch(opt) {
			case 'i':
				sdInum = atoi(optarg);	/* sticky */
				break;
			case 'v':
				verbose = 1;
				break;
			default:
				return(CMD_PARAM_ERROR);
		}
	}

	if (argc < optind + 1)
		return(CMD_PARAM_ERROR);

	cmd = argv[optind];

	if (strcmp(cmd,"init") == 0) {
		if (argc != optind+2)
			return(CMD_PARAM_ERROR);

		prefix = argv[optind+1];
		if (strlen(prefix)+4 > sizeof(varname)) {
			printf("prefix %s too long\n",prefix);
			return(CMD_PARAM_ERROR);
		}

		sdret = sdInit(sdInum, verbose);
		if (sdret < 0) {
			printf("sdInit returned %d\n",sdret);
			return(CMD_FAILURE);
		}

		sprintf(varname,"%s_RD",prefix);
		shell_sprintf(varname,"0x%lx",(long)sdRead);

		sprintf(varname,"%s_WR",prefix);
		shell_sprintf(varname,"0x%lx",(long)sdWrite);

		shell_sprintf("SD_BLKSIZE","0x%lx",SD_BLKSIZE);

		if (verbose) {
			printf("read: 0x%lx, write: 0x%lx, blksiz: 0x%lx\n",
				(long)sdRead,(long)sdWrite,(long)SD_BLKSIZE);
		}
	}
	else if (strcmp(cmd,"read") == 0) {
		if (argc != optind+4)
			return(CMD_PARAM_ERROR);

		buf = (char *)strtoul(argv[optind+1],0,0);
		blknum = strtoul(argv[optind+2],0,0);
		blkcnt = strtoul(argv[optind+3],0,0);

		sdret = sdRead(sdInum,buf,blknum,blkcnt);
		if (sdret < 0) {
			printf("sdRead returned %d\n",sdret);
			return(CMD_FAILURE);
		}
	}
	else if (strcmp(cmd,"write") == 0) {
		if (argc != optind+4)
			return(CMD_PARAM_ERROR);
		buf = (char *)strtoul(argv[optind+1],0,0);
		blknum = strtoul(argv[optind+2],0,0);
		blkcnt = strtoul(argv[optind+3],0,0);

		sdret = sdWrite(sdInum,buf,blknum,blkcnt);
		if (sdret < 0) {
			printf("sdWrite returned %d\n",sdret);
			return(CMD_FAILURE);
		}
	}
	else {
		printf("sd op <%s> not found\n",cmd);
		return(CMD_FAILURE);
	}

	return(CMD_SUCCESS);
}

#ifdef INCLUDE_SD_DUMMY_FUNCS
/* This code is included here just for simulating the SD
 * interface (temporarily if a real one isn't ready.  In a real system,
 * the INCLUDE_SD_DUMMY_FUNCS definition would be off.
 */

int
sdInit(int interface)
{
	if (interface != 0)
		return(-1);

	return(0);
}

int
sdRead(int interface, char *buf, int blk, int blkcnt)
{
	char *from;
	int	size;

	if (interface != 0)
		return(-1);

	from = (char *)(blk * SD_BLKSIZE);
	size = blkcnt * SD_BLKSIZE;
	memcpy(buf,from,size);
	return(0);
}

int
sdWrite(int interface, char *buf, int blk, int blkcnt)
{
	char *to;
	int	size;

	if (interface != 0)
		return(-1);

	to = (char *)(blk * SD_BLKSIZE);
	size = blkcnt * SD_BLKSIZE;
	memcpy(to,buf,size);
	return(0);
}

#endif

#endif
