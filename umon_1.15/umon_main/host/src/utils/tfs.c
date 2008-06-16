/* tfs.c:
 * Treat the specified file as if it was the flash in TFS.
 * Note that this has only been tested with a TFS image that comes
 * from a Big-Endian CPU.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#ifdef BUILD_WITH_VCC
#include <io.h>
#endif
#include "utils.h"

/* Note that these two files are included in the host tools directory
 * only for convenience (so that the host directory can be isolated from
 * the target directory).  The files tfs.h and tfsprivate.h in this
 * space should be identical to those in target/common.
 */
#include "tfs.h"
#include "tfsprivate.h"

#ifdef BUILD_WITH_VCC
typedef unsigned short ushort;
#endif

#ifndef O_BINARY
#define O_BINARY 0
#endif

typedef unsigned char uchar;

int debug, ecEnable, tfsMore;
unsigned long	flashBase;

struct tfsflg tfsflgtbl[] = {
	{ TFS_BRUN,			'b',	"run_at_boot",			TFS_BRUN },
	{ TFS_QRYBRUN,		'B',	"qry_run_at_boot",		TFS_QRYBRUN },
	{ TFS_EXEC,			'e',	"executable",			TFS_EXEC },
	{ TFS_SYMLINK,		'l',	"symbolic link",		TFS_SYMLINK },
	{ TFS_EBIN,			'E',	"exec-binary",			TFS_EBIN },
	{ TFS_IPMOD,		'i',	"inplace_modifiable",	TFS_IPMOD },
	{ TFS_UNREAD,		'u',	"ulvl_unreadable", 		TFS_UNREAD },
	{ TFS_ULVL1,		'1',	"ulvl_1", 				TFS_ULVLMSK },
	{ TFS_ULVL2,		'2',	"ulvl_2", 				TFS_ULVLMSK },
	{ TFS_ULVL3,		'3',	"ulvl_3", 				TFS_ULVLMSK },
	{ TFS_CPRS,			'c',	"compressed", 			TFS_CPRS },
	{ 0, 0, 0, 0 }
};

/* crc32tab[]:
 *	Used for calculating a 32-bit CRC.
 */
unsigned long crc32tab[] = {
	0x00000000, 0x77073096, 0xEE0E612C, 0x990951BA, 0x076DC419, 0x706AF48F,
	0xE963A535, 0x9E6495A3, 0x0EDB8832, 0x79DCB8A4, 0xE0D5E91E, 0x97D2D988, 
	0x09B64C2B, 0x7EB17CBD, 0xE7B82D07, 0x90BF1D91, 0x1DB71064, 0x6AB020F2,
	0xF3B97148, 0x84BE41DE, 0x1ADAD47D, 0x6DDDE4EB, 0xF4D4B551, 0x83D385C7, 
	0x136C9856, 0x646BA8C0, 0xFD62F97A, 0x8A65C9EC, 0x14015C4F, 0x63066CD9,
	0xFA0F3D63, 0x8D080DF5, 0x3B6E20C8, 0x4C69105E, 0xD56041E4, 0xA2677172, 
	0x3C03E4D1, 0x4B04D447, 0xD20D85FD, 0xA50AB56B, 0x35B5A8FA, 0x42B2986C,
	0xDBBBC9D6, 0xACBCF940, 0x32D86CE3, 0x45DF5C75, 0xDCD60DCF, 0xABD13D59, 
	0x26D930AC, 0x51DE003A, 0xC8D75180, 0xBFD06116, 0x21B4F4B5, 0x56B3C423,
	0xCFBA9599, 0xB8BDA50F, 0x2802B89E, 0x5F058808, 0xC60CD9B2, 0xB10BE924, 
	0x2F6F7C87, 0x58684C11, 0xC1611DAB, 0xB6662D3D, 0x76DC4190, 0x01DB7106,
	0x98D220BC, 0xEFD5102A, 0x71B18589, 0x06B6B51F, 0x9FBFE4A5, 0xE8B8D433, 
	0x7807C9A2, 0x0F00F934, 0x9609A88E, 0xE10E9818, 0x7F6A0DBB, 0x086D3D2D,
	0x91646C97, 0xE6635C01, 0x6B6B51F4, 0x1C6C6162, 0x856530D8, 0xF262004E, 
	0x6C0695ED, 0x1B01A57B, 0x8208F4C1, 0xF50FC457, 0x65B0D9C6, 0x12B7E950,
	0x8BBEB8EA, 0xFCB9887C, 0x62DD1DDF, 0x15DA2D49, 0x8CD37CF3, 0xFBD44C65, 
	0x4DB26158, 0x3AB551CE, 0xA3BC0074, 0xD4BB30E2, 0x4ADFA541, 0x3DD895D7,
	0xA4D1C46D, 0xD3D6F4FB, 0x4369E96A, 0x346ED9FC, 0xAD678846, 0xDA60B8D0, 
	0x44042D73, 0x33031DE5, 0xAA0A4C5F, 0xDD0D7CC9, 0x5005713C, 0x270241AA,
	0xBE0B1010, 0xC90C2086, 0x5768B525, 0x206F85B3, 0xB966D409, 0xCE61E49F, 
	0x5EDEF90E, 0x29D9C998, 0xB0D09822, 0xC7D7A8B4, 0x59B33D17, 0x2EB40D81,
	0xB7BD5C3B, 0xC0BA6CAD, 0xEDB88320, 0x9ABFB3B6, 0x03B6E20C, 0x74B1D29A, 
	0xEAD54739, 0x9DD277AF, 0x04DB2615, 0x73DC1683, 0xE3630B12, 0x94643B84,
	0x0D6D6A3E, 0x7A6A5AA8, 0xE40ECF0B, 0x9309FF9D, 0x0A00AE27, 0x7D079EB1, 
	0xF00F9344, 0x8708A3D2, 0x1E01F268, 0x6906C2FE, 0xF762575D, 0x806567CB,
	0x196C3671, 0x6E6B06E7, 0xFED41B76, 0x89D32BE0, 0x10DA7A5A, 0x67DD4ACC, 
	0xF9B9DF6F, 0x8EBEEFF9, 0x17B7BE43, 0x60B08ED5, 0xD6D6A3E8, 0xA1D1937E,
	0x38D8C2C4, 0x4FDFF252, 0xD1BB67F1, 0xA6BC5767, 0x3FB506DD, 0x48B2364B, 
	0xD80D2BDA, 0xAF0A1B4C, 0x36034AF6, 0x41047A60, 0xDF60EFC3, 0xA867DF55,
	0x316E8EEF, 0x4669BE79, 0xCB61B38C, 0xBC66831A, 0x256FD2A0, 0x5268E236, 
	0xCC0C7795, 0xBB0B4703, 0x220216B9, 0x5505262F, 0xC5BA3BBE, 0xB2BD0B28,
	0x2BB45A92, 0x5CB36A04, 0xC2D7FFA7, 0xB5D0CF31, 0x2CD99E8B, 0x5BDEAE1D, 
	0x9B64C2B0, 0xEC63F226, 0x756AA39C, 0x026D930A, 0x9C0906A9, 0xEB0E363F,
	0x72076785, 0x05005713, 0x95BF4A82, 0xE2B87A14, 0x7BB12BAE, 0x0CB61B38, 
	0x92D28E9B, 0xE5D5BE0D, 0x7CDCEFB7, 0x0BDBDF21, 0x86D3D2D4, 0xF1D4E242,
	0x68DDB3F8, 0x1FDA836E, 0x81BE16CD, 0xF6B9265B, 0x6FB077E1, 0x18B74777, 
	0x88085AE6, 0xFF0F6A70, 0x66063BCA, 0x11010B5C, 0x8F659EFF, 0xF862AE69,
	0x616BFFD3, 0x166CCF45, 0xA00AE278, 0xD70DD2EE, 0x4E048354, 0x3903B3C2, 
	0xA7672661, 0xD06016F7, 0x4969474D, 0x3E6E77DB, 0xAED16A4A, 0xD9D65ADC,
	0x40DF0B66, 0x37D83BF0, 0xA9BCAE53, 0xDEBB9EC5, 0x47B2CF7F, 0x30B5FFE9, 
	0xBDBDF21C, 0xCABAC28A, 0x53B39330, 0x24B4A3A6, 0xBAD03605, 0xCDD70693,
	0x54DE5729, 0x23D967BF, 0xB3667A2E, 0xC4614AB8, 0x5D681B02, 0x2A6F2B94, 
	0xB40BBE37, 0xC30C8EA1, 0x5A05DF1B, 0x2D02EF8D
};

unsigned long
crc32(uchar *buffer,unsigned long nbytes)
{
	unsigned long crc_rslt, temp;

	crc_rslt = 0xffffffff;
	while(nbytes) {
		temp = (crc_rslt ^ *buffer++) & 0x000000FFL;
		crc_rslt = ((crc_rslt >> 8) & 0x00FFFFFFL) ^ crc32tab[temp];
		nbytes--;
	}
	return(~crc_rslt);
}

char *
tfsflagsbtoa(long flags,char *fstr)
{
	int	i;
	struct	tfsflg	*tfp;

	if ((!flags) || (!fstr))
		return((char *)0);

	i = 0;
	tfp = tfsflgtbl;
	*fstr = 0;
	while(tfp->sdesc) {
		if ((flags & tfp->mask) == tfp->flag)
			fstr[i++] = tfp->sdesc;
		tfp++;
	}
	fstr[i] = 0;
	return(fstr);
}

/* tfshdrcrc():
 * This function takes a pointer to a tfs header that has
 * NOT YET BEEN endian-swapped.  It is important that the crc
 * be done on the header prior to endian-swap simply because that
 * will goof up the crc.
 */
unsigned long
tfshdrcrc(TFILE *hdr)
{
	unsigned long	crc;
	TFILE hdrcpy;

	hdrcpy = *hdr;
	hdrcpy.next = 0;
	hdrcpy.hdrcrc = 0;
	hdrcpy.flags |= otherEnd32(ecEnable,(TFS_NSTALE | TFS_ACTIVE));
	crc = crc32((uchar *)&hdrcpy,TFSHDRSIZ);
	return(otherEnd32(ecEnable,crc));
}

/* getHdr():
 * Copy the header pointed to by "flash" to the header structure
 * pointed to by "hdr".  The "flash" pointer is assumed to be pointing
 * into the binary file that represents the content of some TFS image.
 * Do endian-swapping if enabled.
 */
struct tfshdr *
getHdr(char *flash, TFILE *hdr)
{
	TFILE	*thp;
	unsigned long	nexthdr;

	thp = (struct tfshdr *)flash;
	hdr->hdrsize = otherEnd16(ecEnable,thp->hdrsize);
	hdr->hdrvrsn = otherEnd16(ecEnable,thp->hdrvrsn);
	hdr->filsize = otherEnd32(ecEnable,thp->filsize);
	hdr->flags = otherEnd32(ecEnable,thp->flags);
	hdr->filcrc = otherEnd32(ecEnable,thp->filcrc);
	hdr->hdrcrc = otherEnd32(ecEnable,thp->hdrcrc);
	hdr->modtime = otherEnd32(ecEnable,thp->modtime);
	hdr->next = (TFILE *)otherEnd32(ecEnable,(unsigned long)(thp->next));
	memcpy(hdr->name,thp->name,TFSNAMESIZE+1);
	memcpy(hdr->info,thp->info,TFSNAMESIZE+1);
	hdr->rsvd[0] = otherEnd32(ecEnable,thp->rsvd[0]);
	hdr->rsvd[1] = otherEnd32(ecEnable,thp->rsvd[1]);
	hdr->rsvd[2] = otherEnd32(ecEnable,thp->rsvd[2]);
	hdr->rsvd[3] = otherEnd32(ecEnable,thp->rsvd[3]);

	nexthdr = (unsigned long)(flash + sizeof(struct tfshdr));
	nexthdr += hdr->filsize;
	if (nexthdr & 0xf)
		nexthdr = (nexthdr | 0xf) + 1;
	return((struct tfshdr *)nexthdr);
}

/* showHdr():
 * This function dumps the content of a header that has been extracted
 * from the TFS binary and converted to the same endian-ness of the PC.
 */
void
showHdr(struct tfshdr *hdr)
{
	char buf[128], *state;

	buf[0] = 0;
	printf("   hdrsize: %d%s\n", hdr->hdrsize,
		hdr->hdrsize == TFSHDRSIZ ? "" : " (bad hdr size)");

	printf("   hdrvrsn: 0x%04x\n", hdr->hdrvrsn);
	printf("   filsize: %d\n",hdr->filsize);
	tfsflagsbtoa(hdr->flags,buf);
	if (TFS_DELETED(hdr))
		state = "(deleted)";
	else if (TFS_STALE(hdr))
		state = "(stale)";
	else
		state = "";
	printf("   flags:   0x%08x %s %s\n",hdr->flags,buf,state);
	printf("   hdrcrc:  0x%08x\n",hdr->hdrcrc);
	printf("   filcrc:  0x%08x\n",hdr->filcrc);
	printf("   modtime: 0x%08x\n",hdr->modtime);
	printf("   next:    0x%08x\n",hdr->next);
	printf("   name:    <%s>\n",hdr->name);
	printf("   info:    <%s>\n",hdr->info);
	printf("   rsvd[0]: 0x%08x\n",hdr->rsvd[0]);
	printf("   rsvd[1]: 0x%08x\n",hdr->rsvd[1]);
	printf("   rsvd[2]: 0x%08x\n",hdr->rsvd[2]);
	printf("   rsvd[3]: 0x%08x\n",hdr->rsvd[3]);
}

#define NOT_FF	1
#define IS_FF	2

/* postTFSTest():
 * This function attempts to analyze the state of the flash after
 * what has been previously determined to be the last file stored
 * in TFS.
 */
void
postTFSTest(uchar *base, uchar *curpos, int size)
{
	int		notFF, state;
	uchar	*flashend, *flashp;
	int		err_tot, err_block_tot, err_block_size, err_block_maxsize;

	flashp = curpos;
	flashend = base + size;

	notFF = 0;
	state = NOT_FF;
	err_tot = 0;
	err_block_tot = 0;
	err_block_size = 0;
	err_block_maxsize = 0;

	while(flashp < flashend) {
		switch(state) {
		case NOT_FF:
			if (*flashp == 0xFF) {
				state = IS_FF;
				if (err_block_size > err_block_maxsize)
					err_block_maxsize = err_block_size;
			}
			else {
				err_tot++;
				err_block_size++;
			}
			break;
		case IS_FF:
			if (*flashp != 0xFF) {
				state = NOT_FF;
				err_tot++;
				err_block_tot++;
				err_block_size = 1;
			}
			break;
		}
		flashp++;
	}
	if (err_tot) {
		printf("\n\nPOST TFS SPACE Analysis:\n");
		printf("Size of space analyzed: %d bytes\n",size - (curpos - base));
		printf("Total blocks of non-FF space in post-TFS area: %d (%d bytes)\n",
			err_block_tot,err_tot);
		printf("Largest errored block size: %d\n",err_block_maxsize);
	}
	else
		printf("Space after end of TFS is clear.\n");
}

int
main(int argc,char *argv[])
{
	unsigned long	offset, hdrstart;
	int		ofd, ifd, opt, ftot, size;
	uchar	*flash, *end;
	char	*filename, *command, *outfile;
	int		hdrcrcfail, filcrcfail;
	struct	stat	mstat;
	struct	tfshdr	fhdr, *next, *this;
	
	tfsMore = 0;
	ecEnable = 0;
	flashBase = 0;
	while((opt=getopt(argc,argv,"b:dehmV")) != EOF) {
		switch(opt) {
		case 'b':
			flashBase = strtoul(optarg,0,0);
			break;
		case 'd':
			debug = 1;
			break;
		case 'e':
			ecEnable = 1;
			break;
		case 'h':
			usage(0);
			break;
		case 'm':
			tfsMore = 1;
			break;
		case 'V':
			showVersion();
			break;
		default:
			usage(0);
			break;
		}
	}

	if (argc < (optind + 2)) 
		usage("Bad argcnt");

	filename = argv[optind];
	command = argv[optind+1];

	/* Open binary input file: */
	ifd = open(filename,O_RDONLY | O_BINARY);
	if (ifd == -1) {
		perror(filename);
		usage(0);
	}
	fstat(ifd,&mstat);
	fprintf(stderr,"Binary file size=%d\n",mstat.st_size);

	/* Allocate space for local storage of the file, but make sure that
	 * the base address of the data is 16-byte aligned...
	 */
	flash = Malloc(mstat.st_size+64);
	end = flash + mstat.st_size+64;
	flash += 32;
#if 0
	(long)flash &= ~0xf;
#else
	flash = (uchar *)((long)flash & ~0xf);
#endif
	if (flash == 0) {
		perror("malloc");
		exit(1);
	}
	if (read(ifd,flash,mstat.st_size) != mstat.st_size) {
		perror("read");
		exit(1);
	}
	close(ifd);

	if (strcmp(command,"ls") == 0) {
		ftot = 0;
		next = this = (struct tfshdr *)flash;
		while(1) {
			if (debug)
				fprintf(stderr,"hdr offset: 0x%lx\n",(long)this-(long)flash);
			next = getHdr((char *)this, &fhdr);

			if (fhdr.hdrsize == 0xffff) {
				if (debug)
					showHdr(&fhdr);
				break;
			}
			if ((char *)next >= (char *)end) {
				fprintf(stderr,"\nFile passes end of block\n");
				break;
			}

			hdrcrcfail = filcrcfail = 0;
			if (tfshdrcrc(this) != this->hdrcrc)
				hdrcrcfail = 1;
			
			if ((fhdr.filsize > mstat.st_size) || (fhdr.filsize < 0) ||
				(crc32((uchar *)(this+1),fhdr.filsize) != fhdr.filcrc))
				filcrcfail = 1;

			hdrstart = flashBase + ((char *)this - (char *)flash);
			printf("\nFile%3d:\n Hdr at: 0x%08x %s %s\n",++ftot,hdrstart,
				hdrcrcfail ? "hdr-crc-failure" : "",
				filcrcfail ? "file-crc-failure" : "");

			if (hdrcrcfail) {
				fprintf(stderr,"\n\nTerminating at file offset 0x%lx\n",
					(unsigned long)this);
				break;
			}

			showHdr(&fhdr);
			if (tfsMore) {
				putchar('?');
				if (getchar()  == 'q')
					break;
			}
			this = next;
			if (((char *)next + TFSHDRSIZ) >= (char *)end) {
				fprintf(stderr,"\nHeader passes end of block\n");
				break;
			}
		}
		if (!hdrcrcfail) {
			printf("\n\n");
			printf("Total files: %d\n",ftot);
			printf("Last file ends at file offset 0x%x\n",(char *)this-(char *)flash);
			postTFSTest(flash,(uchar *)this,mstat.st_size);
		}
	}
	else if (strcmp(command,"get") == 0) {
		if (argc != (optind + 4)) {
			fprintf(stderr,"get needs {offset} and {ofile} args\n");
			return(1);
		}
		offset = strtoul(argv[optind+2],0,0);
		outfile = argv[optind+3];
		next = this = (struct tfshdr *)flash;
		while(1) {
			next = getHdr((char *)this, &fhdr);
			if (fhdr.hdrsize == 0xffff) {
				fprintf(stderr,"file not found at offset 0x%x\n",offset);
				break;
			}
			if ((flashBase + ((char *)this - (char *)flash)) == offset) {
				ofd = open(outfile,O_RDWR | O_BINARY | O_CREAT | O_TRUNC,0777);
				if (ofd < 0) {
					perror("open");
					break;
				}
				size = write(ofd,(char *)(this+1),fhdr.filsize);
				if (size != fhdr.filsize) {
					perror("write");
					break;
				}
				close(ofd);
				break;
			}
			this = next;
		}
	}
	else
		fprintf(stderr,"Cmd: %s not recognized\n",command);

	free(flash);
	return(0);
}

char	*usage_txt[] = {
	"Usage: tfs [options] {infile} {command} [command args]",
	"Options:",
	"   -b {hex_addr}   base of TFS flash in real system",
	"   -e              do an endianness conversion on the headers",
	"   -h              generate this help message",
	"   -m              throttle output with 'more'",
	"   -V              display version (build date) of tool",
	"Commands:",
	"   ls              list headers",
	"   get {offset} {ofile}",
	"                   extract file at specified offset",
	0,
};
