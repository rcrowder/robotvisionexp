/* u2d:
 * Unix-to-Dos converter.
 * Simply scans through the file and replaces all 0x0a's with 0x0d 0x0a.
 */
#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "utils.h"
#ifdef BUILD_WITH_VCC
#include <io.h>
#else
#include <unistd.h>
#endif
#include "version.h"

#ifndef O_BINARY
#define O_BINARY 0
#endif

int debug;

char *bin_ext[] = {
	".pdf",
	".doc",
	".exe",
	".bin",
	".bmp",
	".gif",
	".jpg",
	".jpeg",
	0
};

int
file_is_binary(char *name)
{
	char **ext;
	int namelen = strlen(name);
	int extlen;
	
	ext = bin_ext;
	while(*ext) {
		extlen = strlen(*ext);
		if (namelen > extlen) {
			if (strcmp(&name[namelen-extlen],*ext) == 0)
				return(1);
		}
		ext++;
	}
	return(0);
}

int
main(int argc,char *argv[])
{
	struct	stat	mstat;
	int		ifd, ofd, opt, i, arg;
	char	lastc, c, cr, tmpfile[16], *infile, cmd[128];

	cr	= 0x0d;
	while((opt=getopt(argc,argv,"V")) != EOF) {
		switch(opt) {
		case 'V':
			showVersion();
			break;
		default:
			usage(0);
		}
	}

	if (argc < (optind+1))
		usage(0);

	for(arg = optind;arg < argc;arg++) {

		infile = argv[arg];
		printf("%s...\n",infile);

		if (file_is_binary(infile)) {
			fprintf(stderr,"'%s' may be binary, skip it? (y or n)", infile);
			if (getchar() == 'y') {
				fprintf(stderr,"skipping '%s'\n",infile);
				continue;
			}
		}

		ifd = open(infile,O_RDONLY | O_BINARY);
		if (ifd < 0) {
			perror(infile);
			usage(0);
		}
		strcpy(tmpfile,"u2dXXXXXX");
#ifdef BUILD_WITH_VCC
		mktemp(tmpfile);
#else
		mkstemp(tmpfile);
#endif
		ofd = open(tmpfile,O_WRONLY | O_BINARY | O_CREAT | O_TRUNC,0666);
		if (ofd < 0) {
			perror(tmpfile);
			usage(0);
		}
		lastc = 0;
		while(read(ifd,&c,1) == 1) {
			if ((c == 0x0a) && (lastc != 0x0d))
				write(ofd,&cr,1);	
			write(ofd,&c,1);	
			lastc = c;
		}
		close(ifd);
		close(ofd);
		sprintf(cmd,"mv %s %s",tmpfile,infile);
		system(cmd);
	}
	exit(0);
}

char *usage_txt[] = {
	"Usage: u2d [options] {ifile} [ifile] [ifile] ...",
	" Replace all !0x0d 0x0a sequences from input file with 0x0d 0x0a",
	" Options:",
	"  -V          show version of tool",
	(char *)0,
};
