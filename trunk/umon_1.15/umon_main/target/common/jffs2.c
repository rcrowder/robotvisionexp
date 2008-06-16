/* vi: set ts=4: */
/* jffs2.c:
 * This command allows uMon to parse a block of flash that has been
 * formatted for JFFS2.  It has three main read-only goals:
 *
 * 1. Provide the ability to dump the node list in various ways.
 *    This is primarily used for figuring out how JFFS2 stores nodes
 *    to the flash.
 * 2. Provide the ability to list all files & directories in the JFFS2
 *    flash partition.  With this comes the ability to query for the 
 *    presence of a particular file.
 * 3. Provide the ability to copy the content of a file from JFFS2 to
 *    some other area in memory.
 */
 
#include "config.h"
#if INCLUDE_JFFS2
#include "stddefs.h"
#include "genlib.h"
#include "cli.h"
#include "tfs.h"
#include "tfsprivate.h"
#if INCLUDE_JFFS2ZLIB
#include "jz_zlib.h"
#endif

/* JFFS2_DEFAULT_BASE can be set in config.h per target; else it
 * simply defaults to zero...
 */
#ifndef JFFS2_DEFAULT_BASE
#define JFFS2_DEFAULT_BASE 0
#endif

#define IS_REG(mode)	((mode & 00170000) == 0100000)
#define IS_LNK(mode)	((mode & 00170000) == 0120000)
#define IS_BLK(mode)	((mode & 00170000) == 0060000)
#define IS_DIR(mode)	((mode & 00170000) == 0040000)
#define IS_CHR(mode)	((mode & 00170000) == 0020000)
#define IS_FIFO(mode)	((mode & 00170000) == 0010000)

#define JFFS2_MAGIC_BITMASK				0x1985

#define JFFS2_COMPAT_MASK				0xc000
#define JFFS2_FEATURE_INCOMPAT			0xc000
#define JFFS2_FEATURE_ROCOMPAT			0x8000
#define JFFS2_FEATURE_RWCOMPAT_COPY		0x4000
#define JFFS2_FEATURE_RWCOMPAT_DELETE	0x0000

#define JFFS2_NODE_ACCURATE				0x2000

#define JFFS2_NODETYPE_MASK				7
#define JFFS2_NODETYPE_DIRENT			1
#define JFFS2_NODETYPE_INODE 			2
#define JFFS2_NODETYPE_CLEANMARKER		3
#define JFFS2_NODETYPE_PADDING 			4

#define	IS_ACCURATE_INODE(nodetype) \
		(((nodetype & JFFS2_NODE_ACCURATE) == JFFS2_NODE_ACCURATE) && \
			((nodetype & JFFS2_NODETYPE_MASK) == JFFS2_NODETYPE_INODE))

#define IS_INODE(nodetype) \
		((nodetype & JFFS2_NODETYPE_MASK) == JFFS2_NODETYPE_INODE)

#define IS_PADDING(nodetype) \
		((nodetype & JFFS2_NODETYPE_MASK) == JFFS2_NODETYPE_PADDING)

#define IS_CLEANMARKER(nodetype) \
		((nodetype & JFFS2_NODETYPE_MASK) == JFFS2_NODETYPE_CLEANMARKER)

#define IS_DIRENT(nodetype) \
		((nodetype & JFFS2_NODETYPE_MASK) == JFFS2_NODETYPE_DIRENT)

#define JFFS2_COMPR_NONE		0	/* no compression */
#define JFFS2_COMPR_ZERO		1	/* data is all zeroes */
#define JFFS2_COMPR_RTIME		2
#define JFFS2_COMPR_RUBINMIPS	3
#define JFFS2_COMPR_COPY		4
#define JFFS2_COMPR_DYNRUBIN	5
#define JFFS2_COMPR_ZLIB		6

#define jint8	unsigned char
#define jint16	unsigned short
#define jint32	unsigned long

struct jffs2_unknown_node {
	jint16	magic;			/* JFFS2: 0x1985 */
	jint16	nodetype;
	jint32	totlen;
	jint32	hdr_crc;
};

struct jffs2_raw_dirent {
	jint16	magic;			/* JFFS2: 0x1985 */
	jint16	nodetype;
	jint32	totlen;			/* Size of node (including data) */
	jint32	hdr_crc;
	jint32	pino;			/* Parent inode. */
	jint32	version;		/* For each node belonging to a particular inode,
							 * this value increments by 1.
							 */
	jint32	ino;			/* Inode number.  As new inodes are added, this
							 * value is incremented by one.  An inode number
							 * is never reused.  If zero, then this node can
							 * be deleted at next garbage collection.
							 */
	jint32	mctime;
	jint8	nsize;			/* Size of the name array at the bottom of this
							 * structure.
							 */
	jint8	type;
	jint8	unused[2];
	jint32	node_crc;
	jint32	name_crc;
	jint8	name[0];
};

struct jffs2_raw_inode {
	jint16	magic;			/* JFFS2: 0x1985 */
	jint16	nodetype;
	jint32	totlen;
	jint32	hdr_crc;
	jint32	ino;
	jint32	version;
	jint32	mode;
	jint16	uid;
	jint16	gid;
	jint32	isize;
	jint32	atime;
	jint32	mtime;
	jint32	ctime;
	jint32	offset;			/* Offset into the file at which the data in 
							 * this node should appear.
							 */
	jint32	csize;
	jint32	dsize;
	jint8	compr;
	jint8	usercompr;
	jint16	flags;
	jint32	data_crc;
	jint32	node_crc;
	jint8	data[0];
};

#define UNKNOWN_SIZE	sizeof(struct jffs2_unknown_node)
#define INODE_SIZE		sizeof(struct jffs2_raw_inode)
#define DIRENT_SIZE		sizeof(struct jffs2_raw_dirent)

static int		jffs2_delidx;
static ulong	jffs2_base;
static char		jffs2_crc_enabled;
static char		*jffs2_tmp_space;
static char		*jffs2_ptr;
static char		jffs2_fullpathname[256];
static char		jffs2_matchname[sizeof(jffs2_fullpathname)];
static struct	jffs2_raw_dirent **deleted;


/* The crc32 function used in this jffs2 command is from MTD utils...
 *
 *  COPYRIGHT (C) 1986 Gary S. Brown.  You may use this program, or
 *  code or tables extracted from it, as desired without restriction.
 *
 *  First, the polynomial itself and its table of feedback terms.  The
 *  polynomial is
 *  X^32+X^26+X^23+X^22+X^16+X^12+X^11+X^10+X^8+X^7+X^5+X^4+X^2+X^1+X^0
 *
 *  Note that we take it "backwards" and put the highest-order term in
 *  the lowest-order bit.  The X^32 term is "implied"; the LSB is the
 *  X^31 term, etc.  The X^0 term (usually shown as "+1") results in
 *  the MSB being 1
 *
 *  Note that the usual hardware shift register implementation, which
 *  is what we're using (we're merely optimizing it by doing eight-bit
 *  chunks at a time) shifts bits into the lowest-order term.  In our
 *  implementation, that means shifting towards the right.  Why do we
 *  do it this way?  Because the calculated CRC must be transmitted in
 *  order from highest-order term to lowest-order term.  UARTs transmit
 *  characters in order from LSB to MSB.  By storing the CRC this way
 *  we hand it to the UART in the order low-byte to high-byte; the UART
 *  sends each low-bit to hight-bit; and the result is transmission bit
 *  by bit from highest- to lowest-order term without requiring any bit
 *  shuffling on our part.  Reception works similarly
 *
 *  The feedback terms table consists of 256, 32-bit entries.  Notes
 *
 *      The table can be generated at runtime if desired; code to do so
 *      is shown later.  It might not be obvious, but the feedback
 *      terms simply represent the results of eight shift/xor opera
 *      tions for all combinations of data and CRC register values
 *
 *      The values must be right-shifted by eight bits by the "updcrc
 *      logic; the shift must be unsigned (bring in zeroes).  On some
 *      hardware you could probably optimize the shift in assembler by
 *      using byte-swap instructions
 *      polynomial $edb88320
 */

static const jint32 jffs2_crc32_table[256] = {
	0x00000000L, 0x77073096L, 0xee0e612cL, 0x990951baL, 0x076dc419L,
	0x706af48fL, 0xe963a535L, 0x9e6495a3L, 0x0edb8832L, 0x79dcb8a4L,
	0xe0d5e91eL, 0x97d2d988L, 0x09b64c2bL, 0x7eb17cbdL, 0xe7b82d07L,
	0x90bf1d91L, 0x1db71064L, 0x6ab020f2L, 0xf3b97148L, 0x84be41deL,
	0x1adad47dL, 0x6ddde4ebL, 0xf4d4b551L, 0x83d385c7L, 0x136c9856L,
	0x646ba8c0L, 0xfd62f97aL, 0x8a65c9ecL, 0x14015c4fL, 0x63066cd9L,
	0xfa0f3d63L, 0x8d080df5L, 0x3b6e20c8L, 0x4c69105eL, 0xd56041e4L,
	0xa2677172L, 0x3c03e4d1L, 0x4b04d447L, 0xd20d85fdL, 0xa50ab56bL,
	0x35b5a8faL, 0x42b2986cL, 0xdbbbc9d6L, 0xacbcf940L, 0x32d86ce3L,
	0x45df5c75L, 0xdcd60dcfL, 0xabd13d59L, 0x26d930acL, 0x51de003aL,
	0xc8d75180L, 0xbfd06116L, 0x21b4f4b5L, 0x56b3c423L, 0xcfba9599L,
	0xb8bda50fL, 0x2802b89eL, 0x5f058808L, 0xc60cd9b2L, 0xb10be924L,
	0x2f6f7c87L, 0x58684c11L, 0xc1611dabL, 0xb6662d3dL, 0x76dc4190L,
	0x01db7106L, 0x98d220bcL, 0xefd5102aL, 0x71b18589L, 0x06b6b51fL,
	0x9fbfe4a5L, 0xe8b8d433L, 0x7807c9a2L, 0x0f00f934L, 0x9609a88eL,
	0xe10e9818L, 0x7f6a0dbbL, 0x086d3d2dL, 0x91646c97L, 0xe6635c01L,
	0x6b6b51f4L, 0x1c6c6162L, 0x856530d8L, 0xf262004eL, 0x6c0695edL,
	0x1b01a57bL, 0x8208f4c1L, 0xf50fc457L, 0x65b0d9c6L, 0x12b7e950L,
	0x8bbeb8eaL, 0xfcb9887cL, 0x62dd1ddfL, 0x15da2d49L, 0x8cd37cf3L,
	0xfbd44c65L, 0x4db26158L, 0x3ab551ceL, 0xa3bc0074L, 0xd4bb30e2L,
	0x4adfa541L, 0x3dd895d7L, 0xa4d1c46dL, 0xd3d6f4fbL, 0x4369e96aL,
	0x346ed9fcL, 0xad678846L, 0xda60b8d0L, 0x44042d73L, 0x33031de5L,
	0xaa0a4c5fL, 0xdd0d7cc9L, 0x5005713cL, 0x270241aaL, 0xbe0b1010L,
	0xc90c2086L, 0x5768b525L, 0x206f85b3L, 0xb966d409L, 0xce61e49fL,
	0x5edef90eL, 0x29d9c998L, 0xb0d09822L, 0xc7d7a8b4L, 0x59b33d17L,
	0x2eb40d81L, 0xb7bd5c3bL, 0xc0ba6cadL, 0xedb88320L, 0x9abfb3b6L,
	0x03b6e20cL, 0x74b1d29aL, 0xead54739L, 0x9dd277afL, 0x04db2615L,
	0x73dc1683L, 0xe3630b12L, 0x94643b84L, 0x0d6d6a3eL, 0x7a6a5aa8L,
	0xe40ecf0bL, 0x9309ff9dL, 0x0a00ae27L, 0x7d079eb1L, 0xf00f9344L,
	0x8708a3d2L, 0x1e01f268L, 0x6906c2feL, 0xf762575dL, 0x806567cbL,
	0x196c3671L, 0x6e6b06e7L, 0xfed41b76L, 0x89d32be0L, 0x10da7a5aL,
	0x67dd4accL, 0xf9b9df6fL, 0x8ebeeff9L, 0x17b7be43L, 0x60b08ed5L,
	0xd6d6a3e8L, 0xa1d1937eL, 0x38d8c2c4L, 0x4fdff252L, 0xd1bb67f1L,
	0xa6bc5767L, 0x3fb506ddL, 0x48b2364bL, 0xd80d2bdaL, 0xaf0a1b4cL,
	0x36034af6L, 0x41047a60L, 0xdf60efc3L, 0xa867df55L, 0x316e8eefL,
	0x4669be79L, 0xcb61b38cL, 0xbc66831aL, 0x256fd2a0L, 0x5268e236L,
	0xcc0c7795L, 0xbb0b4703L, 0x220216b9L, 0x5505262fL, 0xc5ba3bbeL,
	0xb2bd0b28L, 0x2bb45a92L, 0x5cb36a04L, 0xc2d7ffa7L, 0xb5d0cf31L,
	0x2cd99e8bL, 0x5bdeae1dL, 0x9b64c2b0L, 0xec63f226L, 0x756aa39cL,
	0x026d930aL, 0x9c0906a9L, 0xeb0e363fL, 0x72076785L, 0x05005713L,
	0x95bf4a82L, 0xe2b87a14L, 0x7bb12baeL, 0x0cb61b38L, 0x92d28e9bL,
	0xe5d5be0dL, 0x7cdcefb7L, 0x0bdbdf21L, 0x86d3d2d4L, 0xf1d4e242L,
	0x68ddb3f8L, 0x1fda836eL, 0x81be16cdL, 0xf6b9265bL, 0x6fb077e1L,
	0x18b74777L, 0x88085ae6L, 0xff0f6a70L, 0x66063bcaL, 0x11010b5cL,
	0x8f659effL, 0xf862ae69L, 0x616bffd3L, 0x166ccf45L, 0xa00ae278L,
	0xd70dd2eeL, 0x4e048354L, 0x3903b3c2L, 0xa7672661L, 0xd06016f7L,
	0x4969474dL, 0x3e6e77dbL, 0xaed16a4aL, 0xd9d65adcL, 0x40df0b66L,
	0x37d83bf0L, 0xa9bcae53L, 0xdebb9ec5L, 0x47b2cf7fL, 0x30b5ffe9L,
	0xbdbdf21cL, 0xcabac28aL, 0x53b39330L, 0x24b4a3a6L, 0xbad03605L,
	0xcdd70693L, 0x54de5729L, 0x23d967bfL, 0xb3667a2eL, 0xc4614ab8L,
	0x5d681b02L, 0x2a6f2b94L, 0xb40bbe37L, 0xc30c8ea1L, 0x5a05df1bL,
	0x2d02ef8dL
};

static jint32
jffs2crc32(jint32 val, const void *ss, int len)
{
	const unsigned char *s = ss;

	while (--len >= 0)
		val = jffs2_crc32_table[(val ^ *s++) & 0xff] ^ (val >> 8);

	return val;
}

/* jcrc32Check():
 * Determine the type of node, and then run a crc32 on the appropriate
 * portions (hdr, data, name) of the header.  Return 1 if the 
 * crc checks pass; else 0.
 */
int
jcrc32Check(struct jffs2_unknown_node *u, int verbose)
{
	ulong	crc;
	char	*errtype;
	struct jffs2_unknown_node unknown;
	struct jffs2_raw_inode inode, *ip;
	struct jffs2_raw_dirent dirent, *dp;

    /* If the crc check isn't enabled, if header is CLEANMARKER or
     * PADDING just return "pass" immediately.
     */
    if ((jffs2_crc_enabled == 0) ||
        (IS_CLEANMARKER(u->nodetype)) || (IS_PADDING(u->nodetype)))
        return(1);

	if (IS_INODE(u->nodetype)) {
		ip = (struct jffs2_raw_inode *)u;
		memcpy((char *)&inode,(char *)ip,sizeof(inode));
		inode.hdr_crc = 0;
		inode.node_crc = 0;
		inode.data_crc = 0;
		crc = jffs2crc32(0,(const void *)&inode,UNKNOWN_SIZE-4);
		if (crc != ip->hdr_crc) {
			errtype = "inode-hdr";
			goto crcerr;
		}

		inode.hdr_crc = crc;
		crc = jffs2crc32(0,(const void *)&inode,INODE_SIZE-8);
		if (crc != ip->node_crc) {
			errtype = "inode-node";
			goto crcerr;
		}

		inode.node_crc = crc;
		if (inode.compr) {
			crc = jffs2crc32(0,(const void *)ip->data,ip->csize);
			if (crc != ip->data_crc) {
				errtype = "inode-cdata";
				goto crcerr;
			}
		}
		else {
			crc = jffs2crc32(0,(const void *)ip->data,ip->dsize);
			if (crc != ip->data_crc) {
				errtype = "inode-data";
				goto crcerr;
			}
		}
	}
	else if (IS_DIRENT(u->nodetype)) {
		dp = (struct jffs2_raw_dirent *)u;
		memcpy((char *)&dirent,(char *)dp,sizeof(dirent));
		dirent.hdr_crc = 0;
		dirent.node_crc = 0;
		dirent.name_crc = 0;
		crc = jffs2crc32(0,(const void *)&dirent,UNKNOWN_SIZE-4);
		if (crc != dp->hdr_crc) {
			errtype = "dirent-hdr";
			goto crcerr;
		}

		dirent.hdr_crc = crc;
		crc = jffs2crc32(0,(const void *)&dirent,DIRENT_SIZE-8);
		if (crc != dp->node_crc) {
			errtype = "dirent-node";
			goto crcerr;
		}

		crc = jffs2crc32(0,(const void *)dp->name,dp->nsize);
		if (crc != dp->name_crc) {
			errtype = "dirent-name";
			goto crcerr;
		}
	}
	else {
		unknown = *u;
		unknown.hdr_crc = 0;
		crc = jffs2crc32(0,(const void *)&unknown,UNKNOWN_SIZE-4);
		if (crc != u->hdr_crc) {
			errtype = "unknown-hdr";
			goto crcerr;
		}
	}
	return(1);

crcerr:
	if (verbose)
		printf("JFFS2 crc error: %s @ %lx\n",errtype,(long)u);
	return(0);
}

static char *
compr2str(jint8 compr)
{
	switch(compr) {
		case JFFS2_COMPR_NONE:
			return("none");
		case JFFS2_COMPR_ZERO:
			return("zero");
		case JFFS2_COMPR_RTIME:
			return("rtime");
		case JFFS2_COMPR_RUBINMIPS:
			return("rubinmips");
		case JFFS2_COMPR_COPY:
			return("copy");
		case JFFS2_COMPR_DYNRUBIN:
			return("dynrubin");
		case JFFS2_COMPR_ZLIB:
			return("zlib");
		default:
			return("???");

	}
}



static void
showUnknown(int nodenum, struct jffs2_unknown_node *u)
{
	printf("Node %3d @ (UNKNOWN, size=%ld) 0x%08lx...\n",
		nodenum,u->totlen,(long)u);
	printf("  type:     0x%04x\n",u->nodetype);
}

static void
showPadding(int nodenum, struct jffs2_raw_inode *i)
{
	printf("Node %3d (PADDING, size=%ld) @ 0x%08lx...\n",
		nodenum,i->totlen,(long)i);
}

static void
showCleanmarker(int nodenum,struct jffs2_raw_inode *i)
{
	printf("Node %3d (CLEANMARKER, size=%ld) @ 0x%08lx...\n",
		nodenum,i->totlen,(long)i);
}

static void
showInode(int nodenum,struct jffs2_raw_inode *i)
{
	printf("Node %3d (%04x=INODE, size=%ld) @ 0x%08lx",
		nodenum,i->nodetype,i->totlen,(long)i);
	if (i->ino == 0)
		printf(" (deleted)");
	else if (i->dsize)
		printf(" (data at 0x%08lx)",&i->data);
	printf("...\n");
	printf("    ino: 0x%06lx,   mode: 0x%06lx, version: 0x%06lx\n",
		i->ino,i->mode,i->version);
	printf("  isize: 0x%06lx,  csize: 0x%06lx,   dsize: 0x%06lx\n",
		i->isize, i->csize, i->dsize);
	printf(" offset: 0x%06lx,  compr: %8s,  ucompr: %s\n",
		i->offset,compr2str(i->compr),compr2str(i->usercompr));
}

static void
showDirent(int nodenum,struct jffs2_raw_dirent *d)
{
	jint8 i;

	printf("Node %3d (%04x=DIRENT <",nodenum,d->nodetype);
	for(i=0;i<d->nsize;i++)
		putchar(d->name[i]);
	printf(">, size=%ld)",d->totlen);
	printf(" @ 0x%06lx",(long)d);
	if (d->ino == 0)
		printf(" (deleted)");
	printf("...\n");
	printf("    ino: 0x%06lx,   pino: 0x%06lx, version: 0x%06lx\n",
		d->ino, d->pino, d->version);
	printf("  nsize: 0x%06lx,   type: 0x%06lx\n",
		d->nsize,d->version);
}


/* getDirent():
 * Take the incoming inode number and return a pointer to the
 * corresponding jffs2_raw_dirent structure.
 */
static struct jffs2_raw_dirent *
getDirent(jint32 ino)
{
	ulong	jaddr;
	struct	jffs2_unknown_node *u;
	
	jaddr = jffs2_base;
	u = (struct jffs2_unknown_node *)jffs2_base;

	while(u->magic == JFFS2_MAGIC_BITMASK) {
		if (IS_DIRENT(u->nodetype)) {
			if (((struct jffs2_raw_dirent *)u)->ino == ino) {
				return((struct jffs2_raw_dirent *)u);
			}
		}
		jaddr += u->totlen;
		while(jaddr & 0x3) jaddr++;
		while(*(ulong *)jaddr == 0xffffffff) jaddr += 4;
		u = (struct jffs2_unknown_node *)jaddr;
	}
	return(0);
}

/* getInode():
 * Take the incoming inode number and return a pointer to the
 * corresponding jffs2_raw_inode structure.
 */
static struct jffs2_raw_inode *
getInode(jint32 ino)
{
	ulong	jaddr;
	struct	jffs2_unknown_node *u;
	
	jaddr = jffs2_base;
	u = (struct jffs2_unknown_node *)jffs2_base;

	while(u->magic == JFFS2_MAGIC_BITMASK) {
		if (IS_INODE(u->nodetype)) {
			if (((struct jffs2_raw_inode *)u)->ino == ino) {
				return((struct jffs2_raw_inode *)u);
			}
		}
		jaddr += u->totlen;
		while(jaddr & 0x3) jaddr++;
		while(*(ulong *)jaddr == 0xffffffff) jaddr += 4;
		u = (struct jffs2_unknown_node *)jaddr;
	}
	return(0);
}

/* printDirent():
 * This recursive function is used to build a string that represents
 * the full path name of the file whose inode is specifed by 
 * the incoming argument 'ino'.
 */
static int
printDirent(int ino)
{
	int i;
	struct jffs2_raw_dirent *d;
	
	if ((d = getDirent(ino)) == 0)
		return(-1);

	if (d->pino == 1) {
		int i;

		for(i=0;i<d->nsize;i++)
			*jffs2_ptr++ = d->name[i];
		*jffs2_ptr++ = '/';
	}
	else {
		printDirent(d->pino);
		for(i=0;i<d->nsize;i++)
			*jffs2_ptr++ = d->name[i];
		*jffs2_ptr++ = '/';
	}
	return(0);
}

/* direntIsDeleted():
 * Return 1 if the specified directory entry is deleted; else
 * return 0.
 */
static int
direntIsDeleted(struct jffs2_raw_dirent *d)
{
	int j;
	for (j=0;j<jffs2_delidx;j++) {
		if ((d->pino == deleted[j]->pino) &&
			(memcmp((char *)d->name,(char *)deleted[j]->name,d->nsize) == 0) &&
			(d->version <= deleted[j]->version))
					return (1);
	}
	return(0);
}

/* buildDeletedList():
 * Walk through the node table and build a list of the directory
 * entries that are deleted.
 */
static int
buildDeletedList(void)
{
	ulong	jaddr;
	struct	jffs2_unknown_node *u;

	jffs2_delidx = 0;
	jaddr = jffs2_base;
	u = (struct jffs2_unknown_node *)jaddr;
	deleted = (struct jffs2_raw_dirent **)jffs2_tmp_space;

	while(u->magic == JFFS2_MAGIC_BITMASK) {
		if (IS_DIRENT(u->nodetype)) {
			if (((struct jffs2_raw_dirent *)u)->ino == 0)
				deleted[jffs2_delidx++] = (struct jffs2_raw_dirent *)u;
		}
		jaddr += u->totlen;
		while(jaddr & 0x3) jaddr++;
		while(*(ulong *)jaddr == 0xffffffff) jaddr += 4;
		u = (struct jffs2_unknown_node *)jaddr;
	}
	return(jffs2_delidx);
}

/* findInode():
 * Scan through the node table looking for the specified file.
 * If found, return that inode value; else return -1.
 */
static jint32
findInode(char *fname)
{
	ulong	jaddr;
	jint32	inode;
	struct	jffs2_raw_inode *i;
	struct	jffs2_unknown_node *u;

	inode = -1;

	/* Initialize pointers...
	 */
	jaddr = jffs2_base;
	u = (struct jffs2_unknown_node *)jaddr;

	/* Determine which files are deleted...
	 */
	buildDeletedList();

	/* Scan through nodes sequentially, looking for specified filename...
	 */
	while(u->magic == JFFS2_MAGIC_BITMASK) {
		if (IS_DIRENT(u->nodetype)) {
			int j;
			struct jffs2_raw_dirent *d;

			d = (struct jffs2_raw_dirent *)u;
			jffs2_ptr = jffs2_fullpathname;

			if (d->ino != 0) {
				if (direntIsDeleted(d)) {
					jaddr += u->totlen;
					while(jaddr & 0x3) jaddr++;
					u = (struct jffs2_unknown_node *)jaddr;
					continue;
				}
				if (d->pino > 1)
					printDirent(d->pino);
				for(j=0;j<d->nsize;j++)
					*jffs2_ptr++ = d->name[j];
				i = getInode(d->ino);
				if ((i) && (IS_DIR(i->mode)))
					*jffs2_ptr++ = '/';

				*jffs2_ptr = 0;
				if (strcmp(jffs2_fullpathname,fname) == 0) 
					return(d->ino);
			}
		}
		jaddr += u->totlen;
		while(jaddr & 0x3) jaddr++;
		while(*(ulong *)jaddr == 0xffffffff) jaddr += 4;
		u = (struct jffs2_unknown_node *)jaddr;
	}
	return(-1);
}

/* jzlibcpy():
 * A variation on "memcpy", but using JFFS2's zlib decompression...
 */
int
jzlibcpy(char *src, char *dest, ulong srclen, ulong destlen)
{
#if INCLUDE_JFFS2ZLIB
	z_stream strm;
	int ret;

	if ((strm.workspace = malloc(zlib_inflate_workspacesize())) == 0)
		return(-1);

	if (Z_OK != zlib_inflateInit(&strm)) {
		free(strm.workspace);
		return -1;
	}
	
	strm.next_in = (unsigned char *)src;
	strm.avail_in = srclen;
	strm.total_in = 0;

	strm.next_out = (unsigned char *)dest;
	strm.avail_out = destlen;
	strm.total_out = 0;

	while((ret = zlib_inflate(&strm, Z_FINISH)) == Z_OK);

	zlib_inflateEnd(&strm);
	free(strm.workspace);
	return(strm.total_out);
#else
	printf("INCLUDE_JFFS2ZLIB not set in config.h, can't decompress\n");
	return -1;
#endif
}

/* constructInodeData():
 * This function takes an inode and reconstructs the complete file
 * data starting at the address specified by base.
 * The return value is the size of the reconstruction.
 * Return -1 if the construction fails.
 */
static int
constructInodeData(jint32 inode, char *base)
{
	int		filesize = 0;
	ulong	jaddr;
	struct	jffs2_unknown_node *u;

	/* For each node that has the same inode number as that of the
	 * incoming argument, construct the file data ...
	 */
	jaddr = jffs2_base;
	u = (struct jffs2_unknown_node *)jaddr;

	while(u->magic == JFFS2_MAGIC_BITMASK) {
		if (IS_ACCURATE_INODE(u->nodetype)) {
			struct jffs2_raw_inode *i = (struct jffs2_raw_inode *)u;

			if (i->ino == inode) {
				if (i->compr == JFFS2_COMPR_NONE) {
					memcpy((char *)base+i->offset,(char *)i->data,i->dsize);
				}
				else if (i->compr == JFFS2_COMPR_ZERO) {
					memset(base+i->offset,0,i->dsize);
				}
				else if (i->compr == JFFS2_COMPR_ZLIB) {
					if (jzlibcpy((char *)i->data, (char *)base+i->offset,i->csize,i->dsize) < 0)
						return(-1);
				}
				else {
					printf("jffs2 %s-type decompression not supported\n",
						compr2str(i->compr));
					return(-1);
				}
				if (filesize < (i->offset+i->dsize))
					filesize = (i->offset+i->dsize);
			}
		}
		jaddr += u->totlen;
		while(jaddr & 0x3) jaddr++;
		while(*(ulong *)jaddr == 0xffffffff) jaddr += 4;
		u = (struct jffs2_unknown_node *)jaddr;
	}
	return(filesize);
}

static int
getInodeDataSize(jint32 inode)
{
	int		filesize = 0;
	ulong	jaddr;
	struct	jffs2_unknown_node *u;

	/* Retrieve the dsize and offset values for each node that has
	 * the same inode number as that of the incoming argument.  Use
	 * that information to determine the size of the file.
 	 */
	jaddr = jffs2_base;
	u = (struct jffs2_unknown_node *)jaddr;

	while(u->magic == JFFS2_MAGIC_BITMASK) {
		if (IS_ACCURATE_INODE(u->nodetype)) {
			struct jffs2_raw_inode *i = (struct jffs2_raw_inode *)u;

			if (i->ino == inode) {
				if (filesize < (i->offset+i->dsize))
					filesize = (i->offset+i->dsize);
			}
		}
		jaddr += u->totlen;
		while(jaddr & 0x3) jaddr++;
		while(*(ulong *)jaddr == 0xffffffff) jaddr += 4;
		u = (struct jffs2_unknown_node *)jaddr;
	}
	return(filesize);
}


char *Jffs2Help[] = {
	"Read JFFS2-formatted flash space",
	"[b:cs:] {operation} [args]...",
#if INCLUDE_VERBOSEHELP
	"",
	"Options:",
	" -b {addr}   base address of JFFS2 space (see note below)",
	" -c          enable CRC checks for header/name/data",
	" -s {addr}   base of ram scratch space used by jffs2",
	"             (default is APPRAMBASE)",
	"",
	"Operations:",
	" cat {fname}  dump content of ASCII file to console",
	"              - copies content of jffs2 file to TFS or memory",
	" dirent       list all dirent-type nodes sequentially",
	" dump         list all nodes sequentially",
	" get {jffs_file} {tfs_file | addr}",
	"              (get from jfffs2)",
	" ino  [rng]   list nodes associated with specified inode",
	" ls  [fltr]   list files and/or dirs heirarchically",
	"              - if 'fltr' is specified, then list only those that match",
	"              - fltr format: sss, *sss, sss*, *sss* ('sss' is any string)",
	"              - loads JFFS2NAME & JFFS2SIZE based on most recent match",
	"              - loads JFFS2TOT with number of files listed",
	" qry [fltr]   quiet version of \"ls\"",
	" node [rng]   list range of nodes",
	" ntot         list total number of nodes",
	" pino #       list nodes with specified parent inode",
	" zinf {src} {dest} {srclen} {destlen}",
	"              inflate using JFFS2's zlib",
	"Notes:",
	" - Base of JFFS2 partition defaults to content of JFFS2BASE (if set).",
	"   The '-b' option overrides this.",
#endif
	0
};


int
Jffs2Cmd(int argc, char *argv[])
{
	ulong	jaddr;
	jint32	inode;
	jint16	nodetype;
	struct	jffs2_raw_inode *i;
	struct	jffs2_raw_dirent *d;
	struct	jffs2_unknown_node *u;
	int		opt, nodetot, j, filesize;
	char	*env, *cmd, *fname, *range, *filedata;

	jffs2_crc_enabled = 0;
	jffs2_ptr = jffs2_fullpathname;
	nodetot = 0;

	if ((env = getenv("JFFS2BASE")))
		jffs2_base = (jint32)strtoul(env,0,0);
	else
		jffs2_base = (jint32)JFFS2_DEFAULT_BASE;
	
	jffs2_tmp_space = (char *)0xffffffff;
	while ((opt=getopt(argc,argv,"b:cs:")) != -1) {
		switch(opt) {
			case 'b':
				jffs2_base = (jint32)strtoul(optarg,0,0);
				break;
			case 'c':
				jffs2_crc_enabled = 1;
				break;
			case 's':
				jffs2_tmp_space = (char *)strtoul(optarg,0,0);
				break;
			default:
				return(CMD_PARAM_ERROR);
		}
	}

	if (argc < optind + 1)
		return(CMD_PARAM_ERROR);

	if (jffs2_tmp_space == (char *)0xffffffff)
		jffs2_tmp_space = (char *)getAppRamStart();

	cmd = argv[optind];

	jaddr = jffs2_base;

	u = (struct jffs2_unknown_node *)jaddr;

	if (strcmp(cmd,"dump") == 0) {
		printf("JFFS2 base: 0x%lx\n",jffs2_base);

		while((u->magic == JFFS2_MAGIC_BITMASK) && !gotachar()) {
			if (!jcrc32Check(u,1))
				break;

			nodetype = u->nodetype;

			if IS_DIRENT(nodetype) {
				showDirent(nodetot,(struct jffs2_raw_dirent *)u);
			}
			else if IS_INODE(nodetype) {
				showInode(nodetot,(struct jffs2_raw_inode *)u);
			}
			else if IS_CLEANMARKER(nodetype) {
				showCleanmarker(nodetot,(struct jffs2_raw_inode *)u);
			}
			else if IS_PADDING(nodetype) {
				showPadding(nodetot,(struct jffs2_raw_inode *)u);
			}
			else {
				showUnknown(nodetot,(struct jffs2_unknown_node *)u);
			}
	
			jaddr += u->totlen;
			while(jaddr & 0x3) jaddr++;
			while(*(ulong *)jaddr == 0xffffffff) jaddr += 4;
			u = (struct jffs2_unknown_node *)jaddr;
	
			nodetot++;
		}
		printf("%d nodes\n",nodetot);
	}
	else if (strcmp(cmd,"dirent") == 0) {
		while((u->magic == JFFS2_MAGIC_BITMASK) && !gotachar()) {
			if (!jcrc32Check(u,1))
				break;

			nodetype = u->nodetype;
			if IS_DIRENT(nodetype) {
				showDirent(nodetot,(struct jffs2_raw_dirent *)u);
			}
			jaddr += u->totlen;
			while(jaddr & 0x3) jaddr++;
			while(*(ulong *)jaddr == 0xffffffff) jaddr += 4;
			u = (struct jffs2_unknown_node *)jaddr;
	
			nodetot++;
		}
	}
	else if (strcmp(cmd,"pino") == 0) {
		jint32 pino;

		if (argc != optind+2)
			return(CMD_PARAM_ERROR);

		pino = strtol(argv[optind+1],0,0);

		while((u->magic == JFFS2_MAGIC_BITMASK) && !gotachar()) {
			if (!jcrc32Check(u,1))
				break;

			nodetype = u->nodetype;
			if IS_DIRENT(nodetype) {
				d = (struct jffs2_raw_dirent *)u;
				if (pino == d->pino)
					showDirent(nodetot,(struct jffs2_raw_dirent *)u);
			}
			jaddr += u->totlen;
			while(jaddr & 0x3) jaddr++;
			while(*(ulong *)jaddr == 0xffffffff) jaddr += 4;
			u = (struct jffs2_unknown_node *)jaddr;
	
			nodetot++;
		}
	}
	else if (strcmp(cmd,"ino") == 0) {
		if (argc == optind+1)
			range = "all";
		else
			range = argv[optind+1];
		nodetot = 0;
		jaddr = jffs2_base;
		u = (struct jffs2_unknown_node *)jffs2_base;

		while((u->magic == JFFS2_MAGIC_BITMASK) && !gotachar()) {
			if (!jcrc32Check(u,1))
				break;

			if IS_DIRENT(u->nodetype) {
				d = (struct jffs2_raw_dirent *)u;
				if(inRange(range,d->ino))
					showDirent(nodetot,(struct jffs2_raw_dirent *)u);
			}
			else if IS_INODE(u->nodetype) {
				i = (struct jffs2_raw_inode *)u;
				if(inRange(range,i->ino))
					showInode(nodetot,(struct jffs2_raw_inode *)u);
			}
			nodetot++;
			jaddr += u->totlen;
			while(jaddr & 0x3) jaddr++;
			while(*(ulong *)jaddr == 0xffffffff) jaddr += 4;
			u = (struct jffs2_unknown_node *)jaddr;
		}
	}
	else if (strcmp(cmd,"node") == 0) {
		if (argc == optind+1)
			range = "all";
		else
			range = argv[optind+1];
		nodetot = 0;
		jaddr = jffs2_base;
		u = (struct jffs2_unknown_node *)jffs2_base;

		while((u->magic == JFFS2_MAGIC_BITMASK) && !gotachar()) {
			if (!jcrc32Check(u,1))
				break;

			if IS_DIRENT(u->nodetype) {
				if(inRange(range,nodetot))
					showDirent(nodetot,(struct jffs2_raw_dirent *)u);
			}
			else if IS_INODE(u->nodetype) {
				if(inRange(range,nodetot))
					showInode(nodetot,(struct jffs2_raw_inode *)u);
			}
			else if IS_CLEANMARKER(u->nodetype) {
				if(inRange(range,nodetot))
					showCleanmarker(nodetot,(struct jffs2_raw_inode *)u);
			}
			else if IS_PADDING(u->nodetype) {
				if(inRange(range,nodetot))
					showPadding(nodetot,(struct jffs2_raw_inode *)u);
			}
			else {
				if(inRange(range,nodetot))
					showUnknown(nodetot,(struct jffs2_unknown_node *)u);
			}
			nodetot++;
			jaddr += u->totlen;
			while(jaddr & 0x3) jaddr++;
			while(*(ulong *)jaddr == 0xffffffff) jaddr += 4;
			u = (struct jffs2_unknown_node *)jaddr;
		}
	}
	else if (strcmp(cmd,"ntot") == 0) {
		jint32 lo, hi;

		lo = hi = 0;
		while((u->magic == JFFS2_MAGIC_BITMASK) && !gotachar()) {
			if (!jcrc32Check(u,1))
				break;

			if (IS_DIRENT(u->nodetype)) {
				struct jffs2_raw_dirent *d = (struct jffs2_raw_dirent *)u;

				if (d->ino > hi)
					hi = d->ino;
				if (d->ino < lo)
					lo = d->ino;
			}
			else if (IS_INODE(u->nodetype)) {
				struct jffs2_raw_inode *i = (struct jffs2_raw_inode *)u;

				if (i->ino > hi)
					hi = i->ino;
				if (i->ino < lo)
					lo = i->ino;
			}
			else if (IS_CLEANMARKER(u->nodetype)) {
			}
			else {
				printf("Unknown nodetype at 0x%lx\n",(long)u);
				break;
			}

			jaddr += u->totlen;
			while(jaddr & 0x3) jaddr++;
			while(*(ulong *)jaddr == 0xffffffff) jaddr += 4;
			u = (struct jffs2_unknown_node *)jaddr;
		}
		printf("inodes range from %ld thru %ld\n",lo,hi);
	}
	else if (strcmp(cmd,"zinf") == 0) {
		int		size;
		char	*src, *dst;
		ulong	srclen, dstlen;

		if (argc != optind+5)
			return(CMD_PARAM_ERROR);

		src = (char *)strtoul(argv[optind+1],0,0);
		dst = (char *)strtoul(argv[optind+2],0,0);
		srclen = strtoul(argv[optind+3],0,0);
		dstlen = strtoul(argv[optind+4],0,0);
		size = jzlibcpy(src,dst,srclen,dstlen);
		if (size > 0)
			printf("Decompressed to %d bytes\n",size);
	}
	else if ((strcmp(cmd,"ls") == 0) || (strcmp(cmd,"qry") == 0)) {
		int match, matchtot, matchsize, ftot, quiet;

		/* If the sub-command is "qry", then we do essentially the same
		 * thing as "ls" except quietly (just populate the shell variables).
		 */
		if (cmd[0] == 'q')
			quiet = 1;
		else
			quiet = 0;

		/* Prior to each 'ls', clear the content of the name and
		 * size shell variables...
		 */
		setenv("JFFS2NAME",0);
		setenv("JFFS2SIZE",0);

		if (argc == optind+2)
			fname = argv[optind+1];
		else
			fname = 0;

		match = matchsize = matchtot = filesize = ftot = 0;

		/* First record all deleted dirents...
		 */
		buildDeletedList();

		/* Now scan through the nodes recursively, printing those
		 * dirents that are not deleted.
		 */
		while((u->magic == JFFS2_MAGIC_BITMASK) && !gotachar()) {
			if (!jcrc32Check(u,1))
				break;

			if (IS_DIRENT(u->nodetype)) {
				int j, len, len1;
				struct jffs2_raw_inode *i;
				struct jffs2_raw_dirent *d = (struct jffs2_raw_dirent *)u;

				match = len = 0;
				jffs2_ptr = jffs2_fullpathname;

				if (d->ino != 0) {
					if (direntIsDeleted(d)) {
						jaddr += u->totlen;
						while(jaddr & 0x3) jaddr++;
						u = (struct jffs2_unknown_node *)jaddr;
						continue;
					}
					if (d->pino > 1)
						printDirent(d->pino);
					for(j=0;j<d->nsize;j++)
						*jffs2_ptr++ = d->name[j];
					i = getInode(d->ino);
					if ((i) && (IS_DIR(i->mode)))
						*jffs2_ptr++ = '/';

					*jffs2_ptr = 0;
					if (fname) {
						len = strlen(fname);
						len1 = strlen(jffs2_fullpathname);

						if ((fname[0] == '*') && (fname[len-1] == '*')) {
							fname[len-1] = 0;
							fname++;
							if (strstr(jffs2_fullpathname,fname))
								match = 1;
							fname--;
							fname[len-1] = '*';
						}
						else if (fname[0] == '*') {
							fname++; len--;
							if (!strcmp(jffs2_fullpathname+(len1-len),fname))
								match = 1;
							fname--;
						}
						else if (fname[len-1] == '*') {
							fname[len-1] = 0; len--;
							if (!strncmp(jffs2_fullpathname,fname,len))
								match = 1;
							fname[len] = '*';
						}
						else if (!strcmp(jffs2_fullpathname,fname)) {
							match = 1;
						}
						if (match) {
							ftot++;
							matchtot++;
							matchsize = filesize = getInodeDataSize(d->ino);
							strcpy(jffs2_matchname,jffs2_fullpathname);
							if (!quiet) {
								if (IS_DIR(i->mode)) {
									printf(" %-40s (dir)\n",
										jffs2_fullpathname);
								}
								else if (IS_REG(i->mode)) {
									printf(" %-40s (size: %d bytes)\n",
										jffs2_fullpathname,filesize);
								}
								else if (IS_LNK(i->mode)) {
									printf(" %-40s (lnk)\n",
										jffs2_fullpathname);
								}
								else if (IS_BLK(i->mode)) {
									printf(" %-40s (blk)\n",
										jffs2_fullpathname);
								}
								else if (IS_CHR(i->mode)) {
									printf(" %-40s (chr)\n",
										jffs2_fullpathname);
								}
								else if (IS_FIFO(i->mode)) {
									printf(" %-40s (fifo)\n",
										jffs2_fullpathname);
								}
								else {
									printf(" %s\n",jffs2_fullpathname);
								}
							}
						}
					}
					else {
						ftot++;
						printf(" %s\n",jffs2_fullpathname);
					}
				}
			}
			jaddr += u->totlen;
			while(jaddr & 0x3) jaddr++;
			while(*(ulong *)jaddr == 0xffffffff) jaddr += 4;
			u = (struct jffs2_unknown_node *)jaddr;
		}
		if ((ftot > 0) && (!quiet))
			printf(" Total: %d\n",ftot);

		shell_sprintf("JFFS2TOT","%d",ftot);
		if (matchtot > 0) {
			shell_sprintf("JFFS2SIZE","%d",matchsize);
			setenv("JFFS2NAME",jffs2_matchname);
		}
	}
	else if (strcmp(cmd,"cat") == 0) {
		if (argc != optind+2)
			return(CMD_PARAM_ERROR);

		fname = argv[optind+1];

		if ((inode = findInode(fname)) == -1) {
			printf("Can't find file '%s'\n",fname);
			return(CMD_FAILURE);
		}

		filedata = jffs2_tmp_space;
		filesize = constructInodeData(inode,filedata);

		if (filesize < 0) {
			printf("Can't construct file data\n");
			return(CMD_FAILURE);
		}
			
		/* dump the content of the file to the console:
		 */
		for(j=0;j<filesize;j++)
			putchar(filedata[j]);
	}
	else if (strcmp(cmd,"get") == 0) {
		char *dest;
		int	totfs = 0;

		if (argc != optind+3)
			return(CMD_PARAM_ERROR);

		fname = argv[optind+1];
		dest = argv[optind+2];
		if ((dest[0] == '0') && (dest[1] == 'x')) 
			filedata = (char *)strtoul(dest,0,0);
		else {
#if INCLUDE_TFS
			totfs = 1;
			filedata = jffs2_tmp_space;
#else
			printf("Can't copy to TFS\n");
			return(CMD_FAILURE);
#endif
		}

		if ((inode = findInode(fname)) == -1) {
			printf("Can't find file '%s'\n",fname);
			return(CMD_FAILURE);
		}

		filesize = constructInodeData(inode,filedata);

		if (filesize < 0) {
			printf("Can't construct file data\n");
			return(CMD_FAILURE);
		}

#if INCLUDE_TFS
		if (totfs) {
			int	tfserr;
			char *flags, *info;

			flags = info = (char *)0;
			if ((flags = strchr(dest,','))) {
				*flags = 0; flags++;
				if ((info = strchr(flags,',')))
					*info = 0; info++;
			}
			tfserr = tfsadd(dest,info,flags,(uchar *)filedata,filesize);
			if (tfserr != TFS_OKAY) {
				printf("TFS error: %s\n",tfserrmsg(tfserr));
				return(CMD_FAILURE);
			}
		}
		else
#endif
		{
			printf("Copied %d bytes to 0x%lx\n",filesize,(long)filedata);
		}
	}
	else {
		printf("jffs2 cmd <%s> not found\n",cmd);
		return(CMD_FAILURE);
	}

	return(CMD_SUCCESS);
}

#endif
