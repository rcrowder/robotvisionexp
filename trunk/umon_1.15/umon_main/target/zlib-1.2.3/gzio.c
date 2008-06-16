/* gzio.c -- IO on .gz files
 * Copyright (C) 1995-2005 Jean-loup Gailly.
 * For conditions of distribution and use, see copyright notice in zlib.h
 *
 */

/* @(#) $Id: gzio.c,v 1.1 2007-04-27 18:08:05 umon Exp $ */

#include <stdio.h>

#include "zutil.h"

#ifndef NO_DUMMY_DECL
struct internal_state {int dummy;}; /* for buggy compilers */
#endif

#ifndef Z_BUFSIZE
#  ifdef MAXSEG_64K
#    define Z_BUFSIZE 4096 /* minimize memory usage for 16-bit DOS */
#  else
#    define Z_BUFSIZE 16384
#  endif
#endif
#ifndef Z_PRINTF_BUFSIZE
#  define Z_PRINTF_BUFSIZE 4096
#endif

#ifdef __MVS__
#  pragma map (fdopen , "\174\174FDOPEN")
   FILE *fdopen(int, const char *);
#endif

#ifndef STDC
extern voidp  malloc OF((uInt size));
extern void   free   OF((voidpf ptr));
#endif

#define ALLOC(size) malloc(size)
#define TRYFREE(p) {if (p) free(p);}

static int const gz_magic[2] = {0x1f, 0x8b}; /* gzip magic header */

/* gzip flag byte */
#define ASCII_FLAG   0x01 /* bit 0 set: file probably ascii text */
#define HEAD_CRC     0x02 /* bit 1 set: header CRC present */
#define EXTRA_FIELD  0x04 /* bit 2 set: extra field present */
#define ORIG_NAME    0x08 /* bit 3 set: original file name present */
#define COMMENT      0x10 /* bit 4 set: file comment present */
#define RESERVED     0xE0 /* bits 5..7: reserved */

typedef struct gz_stream {
    z_stream stream;
    int      z_err;   /* error code for last stream operation */
    int      z_eof;   /* set if end of input file */
    FILE     *file;   /* .gz file */
    Byte     *inbuf;  /* input buffer */
    Byte     *outbuf; /* output buffer */
    uLong    crc;     /* crc32 of uncompressed data */
    char     *msg;    /* error message */
    char     *path;   /* path name for debugging only */
    int      transparent; /* 1 if input file is not a .gz file */
    char     mode;    /* 'w' or 'r' */
    z_off_t  start;   /* start of compressed data in file (header skipped) */
    z_off_t  in;      /* bytes into deflate or inflate */
    z_off_t  out;     /* bytes out of deflate or inflate */
    int      back;    /* one character push-back */
    int      last;    /* true if push-back is last character */
} gz_stream;


local gzFile gz_open      OF((unsigned char *src, int srclen));
local int    get_byte     OF((gz_stream *s));
local void   check_header OF((gz_stream *s));
local int    destroy      OF((gz_stream *s));
local uLong  getLong      OF((gz_stream *s));

/* ===========================================================================
     Opens a gzip (.gz) file for reading or writing. The mode parameter
   is as in fopen ("rb" or "wb"). The file is given either by file descriptor
   or path name (if fd == -1).
     gz_open returns NULL if the file could not be opened or if there was
   insufficient memory to allocate the (de)compression state; errno
   can be checked to distinguish the two cases (if errno is zero, the
   zlib error is Z_MEM_ERROR).
  *** MODIFIED FOR USE WITHIN UMON ***
*/
local gzFile gz_open(unsigned char *src, int srclen)
{
	int err;
    static gz_stream s;

	s.stream.zalloc = (alloc_func)0;
	s.stream.zfree = (free_func)0;
	s.stream.opaque = (voidpf)0;
	s.stream.next_in = s.inbuf = src;
	s.stream.next_out = s.outbuf = Z_NULL;
	s.stream.avail_in = srclen;
	s.stream.avail_out = 0;
	s.file = NULL;
	s.z_err = Z_OK;
	s.z_eof = 0;
	s.in = 0;
	s.out = 0;
	s.back = EOF;
	s.crc = crc32(0L, Z_NULL, 0);
	s.msg = NULL;
	s.transparent = 0;

    s.mode = 'r';

	err = inflateInit2(&(s.stream), -MAX_WBITS);

	/* windowBits is passed < 0 to tell that there is no zlib header.
	 * Note that in this case inflate *requires* an extra "dummy" byte
	 * after the compressed stream in order to complete decompression and
	 * return Z_STREAM_END. Here the gzip CRC32 ensures that 4 bytes are
	 * present after the compressed stream.
	 */
	if (err != Z_OK)
		return(0);
	
	s.stream.avail_out = Z_BUFSIZE;

	check_header(&s); /* skip the .gz header */

    return (gzFile)&s;
}

/* ===========================================================================
     Read a byte from a gz_stream; update next_in and avail_in. Return EOF
   for end of file.
   IN assertion: the stream s has been sucessfully opened for reading.
  *** MODIFIED FOR USE WITHIN UMON ***
*/
local int
get_byte(s)
gz_stream *s;
{
    if (s->z_eof)
        return EOF;
    if (s->stream.avail_in == 0) {
        s->z_eof = 1;
        return EOF;
    }
    s->stream.avail_in--;
    return *(s->stream.next_in)++;
}

/* ===========================================================================
      Check the gzip header of a gz_stream opened for reading. Set the stream
    mode to transparent if the gzip magic header is not present; set s->err
    to Z_DATA_ERROR if the magic header is present but the rest of the header
    is incorrect.
    IN assertion: the stream s has already been created sucessfully;
       s->stream.avail_in is zero for the first time, but may be non-zero
       for concatenated .gz files.
*/
local void check_header(s)
    gz_stream *s;
{
    int method; /* method byte */
    int flags;  /* flags byte */
    uInt len;
    int c;

#if 0
    /* Assure two bytes in the buffer so we can peek ahead -- handle case
       where first byte of header is at the end of the buffer after the last
       gzip segment */
    len = s->stream.avail_in;
    if (len < 2) {
        if (len) s->inbuf[0] = s->stream.next_in[0];
        errno = 0;
        len = (uInt)fread(s->inbuf + len, 1, Z_BUFSIZE >> len, s->file);
        if (len == 0 && ferror(s->file)) s->z_err = Z_ERRNO;
        s->stream.avail_in += len;
        s->stream.next_in = s->inbuf;
        if (s->stream.avail_in < 2) {
            s->transparent = s->stream.avail_in;
            return;
        }
    }
#endif
    /* Peek ahead to check the gzip magic header */
    if (s->stream.next_in[0] != gz_magic[0] ||
        s->stream.next_in[1] != gz_magic[1]) {
        s->transparent = 1;
        return;
    }
    s->stream.avail_in -= 2;
    s->stream.next_in += 2;

    /* Check the rest of the gzip header */
    method = get_byte(s);
    flags = get_byte(s);
    if (method != Z_DEFLATED || (flags & RESERVED) != 0) {
        s->z_err = Z_DATA_ERROR;
        return;
    }

    /* Discard time, xflags and OS code: */
    for (len = 0; len < 6; len++) (void)get_byte(s);

    if ((flags & EXTRA_FIELD) != 0) { /* skip the extra field */
        len  =  (uInt)get_byte(s);
        len += ((uInt)get_byte(s))<<8;
        /* len is garbage if EOF but the loop below will quit anyway */
        while (len-- != 0 && get_byte(s) != EOF) ;
    }
    if ((flags & ORIG_NAME) != 0) { /* skip the original file name */
        while ((c = get_byte(s)) != 0 && c != EOF) ;
    }
    if ((flags & COMMENT) != 0) {   /* skip the .gz file comment */
        while ((c = get_byte(s)) != 0 && c != EOF) ;
    }
    if ((flags & HEAD_CRC) != 0) {  /* skip the header crc */
        for (len = 0; len < 2; len++) (void)get_byte(s);
    }
    s->z_err = s->z_eof ? Z_DATA_ERROR : Z_OK;
}

 /* ===========================================================================
 * Cleanup then free the given gz_stream. Return a zlib error code.
   Try freeing in the reverse order of allocations.
 */
local int destroy (s)
    gz_stream *s;
{
    int err = Z_OK;

    if (!s) return Z_STREAM_ERROR;

    TRYFREE(s->msg);

    if (s->stream.state != NULL) {
        if (s->mode == 'w') {
            err = Z_STREAM_ERROR;
        } else if (s->mode == 'r') {
            err = inflateEnd(&(s->stream));
        }
    }
    if (s->z_err < 0)
		err = s->z_err;

    return err;
}

/* ===========================================================================
     Reads the given number of uncompressed bytes from the compressed file.
   gzread returns the number of bytes actually read (0 for end of file).
*/
int ZEXPORT gzread (file, buf, len)
    gzFile file;
    voidp buf;
    unsigned len;
{
    gz_stream *s = (gz_stream*)file;
    Bytef *start = (Bytef*)buf; /* starting point for crc computation */
    Byte  *next_out; /* == stream.next_out but not forced far (for MSDOS) */

    if (s == NULL || s->mode != 'r') return Z_STREAM_ERROR;

    if (s->z_err == Z_DATA_ERROR || s->z_err == Z_ERRNO) return -1;
    if (s->z_err == Z_STREAM_END) return 0;  /* EOF */

    next_out = (Byte*)buf;
    s->stream.next_out = (Bytef*)buf;
    s->stream.avail_out = len;

    if (s->stream.avail_out && s->back != EOF) {
        *next_out++ = s->back;
        s->stream.next_out++;
        s->stream.avail_out--;
        s->back = EOF;
        s->out++;
        start++;
        if (s->last) {
            s->z_err = Z_STREAM_END;
            return 1;
        }
    }

    while (s->stream.avail_out != 0) {

        if (s->transparent) {
            /* Copy first the lookahead bytes: */
            uInt n = s->stream.avail_in;
            if (n > s->stream.avail_out) n = s->stream.avail_out;
            if (n > 0) {
                zmemcpy(s->stream.next_out, s->stream.next_in, n);
                next_out += n;
                s->stream.next_out = next_out;
                s->stream.next_in   += n;
                s->stream.avail_out -= n;
                s->stream.avail_in  -= n;
            }
            if (s->stream.avail_out > 0) {
#if 0
                s->stream.avail_out -=
                    (uInt)fread(next_out, 1, s->stream.avail_out, s->file);
#else
				int i;
				char c;
				unsigned char *cp = next_out;
				for(i=0;i<s->stream.avail_out;i++) {
					c = get_byte(s);
					if (c == EOF) break;
					*cp++ = c;
				}
				s->stream.avail_out -= i;
#endif
            }
            len -= s->stream.avail_out;
            s->in  += len;
            s->out += len;
            if (len == 0) s->z_eof = 1;
            return (int)len;
        }
        if (s->stream.avail_in == 0 && !s->z_eof) {

#if 0
            errno = 0;
            s->stream.avail_in = (uInt)fread(s->inbuf, 1, Z_BUFSIZE, s->file);
#else
			int i;
			char c;
			unsigned char *cp = s->inbuf;
			for(i=0;i<Z_BUFSIZE;i++) {
				c = get_byte(s);
				if (c == EOF) break;
				*cp++ = c;
			}
			s->stream.avail_in = i;
#endif
            if (s->stream.avail_in == 0) {
                s->z_eof = 1;
                if (ferror(s->file)) {
                    s->z_err = Z_ERRNO;
                    break;
                }
            }
            s->stream.next_in = s->inbuf;
        }
        s->in += s->stream.avail_in;
        s->out += s->stream.avail_out;
        s->z_err = inflate(&(s->stream), Z_NO_FLUSH);
        s->in -= s->stream.avail_in;
        s->out -= s->stream.avail_out;

        if (s->z_err == Z_STREAM_END) {
            /* Check CRC and original size */
            s->crc = crc32(s->crc, start, (uInt)(s->stream.next_out - start));
            start = s->stream.next_out;

            if (getLong(s) != s->crc) {
                s->z_err = Z_DATA_ERROR;
            } else {
                (void)getLong(s);
                /* The uncompressed length returned by above getlong() may be
                 * different from s->out in case of concatenated .gz files.
                 * Check for such files:
                 */
                check_header(s);
                if (s->z_err == Z_OK) {
                    inflateReset(&(s->stream));
                    s->crc = crc32(0L, Z_NULL, 0);
                }
            }
        }
        if (s->z_err != Z_OK || s->z_eof) break;
    }
    s->crc = crc32(s->crc, start, (uInt)(s->stream.next_out - start));

    if (len == s->stream.avail_out &&
        (s->z_err == Z_DATA_ERROR || s->z_err == Z_ERRNO))
        return -1;
    return (int)(len - s->stream.avail_out);
}


/* ===========================================================================
      Reads one byte from the compressed file. gzgetc returns this byte
   or -1 in case of end of file or error.
*/
int ZEXPORT gzgetc(file)
    gzFile file;
{
    unsigned char c;

    return gzread(file, &c, 1) == 1 ? c : -1;
}


/* ===========================================================================
      Push one byte back onto the stream.
*/
int ZEXPORT gzungetc(c, file)
    int c;
    gzFile file;
{
    gz_stream *s = (gz_stream*)file;

    if (s == NULL || s->mode != 'r' || c == EOF || s->back != EOF) return EOF;
    s->back = c;
    s->out--;
    s->last = (s->z_err == Z_STREAM_END);
    if (s->last) s->z_err = Z_OK;
    s->z_eof = 0;
    return c;
}


/* ===========================================================================
      Reads bytes from the compressed file until len-1 characters are
   read, or a newline character is read and transferred to buf, or an
   end-of-file condition is encountered.  The string is then terminated
   with a null character.
      gzgets returns buf, or Z_NULL in case of error.

      The current implementation is not optimized at all.
*/
char * ZEXPORT gzgets(file, buf, len)
    gzFile file;
    char *buf;
    int len;
{
    char *b = buf;
    if (buf == Z_NULL || len <= 0) return Z_NULL;

    while (--len > 0 && gzread(file, buf, 1) == 1 && *buf++ != '\n') ;
    *buf = '\0';
    return b == buf && len > 0 ? Z_NULL : b;
}

/* ===========================================================================
     Returns 1 when EOF has previously been detected reading the given
   input stream, otherwise zero.
*/
int ZEXPORT gzeof (file)
    gzFile file;
{
    gz_stream *s = (gz_stream*)file;

    /* With concatenated compressed files that can have embedded
     * crc trailers, z_eof is no longer the only/best indicator of EOF
     * on a gz_stream. Handle end-of-stream error explicitly here.
     */
    if (s == NULL || s->mode != 'r') return 0;
    if (s->z_eof) return 1;
    return s->z_err == Z_STREAM_END;
}

/* ===========================================================================
     Returns 1 if reading and doing so transparently, otherwise zero.
*/
int ZEXPORT gzdirect (file)
    gzFile file;
{
    gz_stream *s = (gz_stream*)file;

    if (s == NULL || s->mode != 'r') return 0;
    return s->transparent;
}

/* ===========================================================================
   Reads a long in LSB order from the given gz_stream. Sets z_err in case
   of error.
*/
local uLong getLong (s)
    gz_stream *s;
{
    uLong x = (uLong)get_byte(s);
    int c;

    x += ((uLong)get_byte(s))<<8;
    x += ((uLong)get_byte(s))<<16;
    c = get_byte(s);
    if (c == EOF) s->z_err = Z_DATA_ERROR;
    x += ((uLong)c)<<24;
    return x;
}

/* ===========================================================================
     Flushes all pending output if necessary, closes the compressed file
   and deallocates all the (de)compression state.
*/
int ZEXPORT gzclose (file)
    gzFile file;
{
    gz_stream *s = (gz_stream*)file;

    if (s == NULL) return Z_STREAM_ERROR;

    return destroy((gz_stream*)file);
}

#ifdef STDC
#  define zstrerror(errnum) strerror(errnum)
#else
#  define zstrerror(errnum) ""
#endif

/****************************************************************************
 * 
 * From this point on, the code is pulled in to support uMon.
 *
 ****************************************************************************
 */
#include "cli.h"
#include "tfs.h"
#include "tfsprivate.h"
#include "cache.h"
extern int shell_sprintf(char *,char *,...);
extern unsigned long getAppRamStart(void);
extern int getopt(int,char **,char *);
extern char *optarg;
extern int optind;

/* ===========================================================================
 * Uncompress input to output then close both files
 * (this function was originally in minigzip.c from the original zlib code)
 */
void gz_uncompress(in, out)
    gzFile in;
    FILE   *out;
{
    local char buf[256];
    int len;

    for (;;) {
        len = gzread(in, buf, sizeof(buf));
        if (len < 0) {
			printf("gzread return: %d\n",len);
			break;
		}
        if (len == 0)
			break;

//		if ((int)fwrite(buf, 1, (unsigned)len, out) != len) {
//			error("failed fwrite");
//		}
    }
//	if (fclose(out)) error("failed fclose");

//	if (gzclose(in) != Z_OK) error("failed gzclose");
}


/* ===========================================================================
	unZip():
	This is the front end to the whole zlib decompressor.
	It is a chop-up of the original minigzip.c code that came with
	the zlib source.
*/
int
unZip(char *src, int srclen, char *dest, int destlen)
{
	int	len;
	gz_stream 	*s;

	if ((s = gz_open((unsigned char *)src,srclen)) == Z_NULL) {
		printf("gzInit(0x%lx,%d) failed!\n",(unsigned long)src,srclen);
		return(-1);
	}

	len = gzread(s,dest,destlen);
	if (len < 0)
		printf("gzRead() returned %d\n",len);

    destroy(s);

	if (len > 0) {
		flushDcache(dest,len);
		invalidateIcache(dest,len);
	}

	return(len);
}

char *UnzipHelp[] = {
	"Decompress memory (or file) to some other block of memory.",
	"-[v:] {src} [dest]",
	"  src:  addr,len | filename",
	"  dest: addr[,len]",
	"Options:",
	" -v{varname} place decompress size into shellvar",
	0,
};

/* Unzip():
 * Access ZLIB decompressor from monitor command line.
 */
int
Unzip(int argc,char *argv[])
{
	int		opt, tot;
	unsigned long	srclen, destlen;
	char	*varname, *asc_src, *asc_dst, *comma, *src, *dest;

	destlen = 99999999;
	varname = asc_dst = (char *)0;
	dest = (char *)getAppRamStart();

	while((opt=getopt(argc,argv,"v:")) != -1) {
		switch(opt) {
		case 'v':
			varname = optarg;
			break;
		default:
			return(CMD_PARAM_ERROR);
		}
	}

	if (argc == optind+1) {
		asc_src = argv[optind];
	}
	else if (argc == optind+2) {
		asc_src = argv[optind];
		asc_dst = argv[optind+1];
	}
	else {
		return(CMD_PARAM_ERROR);
	}

	comma = strchr(asc_src,',');
	if (comma) {
		*comma = 0;
		src = (char *)strtoul(asc_src,(char **)0,0);
		srclen = strtoul(comma+1,(char **)0,0);
	}
	else {
		TFILE *tfp;
		tfp = tfsstat(asc_src);
		if (!tfp) {
			printf("%s: file not found\n",asc_src); 
			return(CMD_FAILURE);
		}
		src = TFS_BASE(tfp);
		srclen = TFS_SIZE(tfp);
	}

	if (asc_dst) {
		comma = strchr(asc_dst,',');
		if (comma) {
			*comma = 0;
			destlen = strtoul(comma+1,(char **)0,0);
		}
		dest = (char *)strtoul(asc_dst,(char **)0,0);
	}

	tot = unZip(src,srclen,dest,destlen);
	printf("Decompressed %ld bytes from 0x%lx to %d bytes at 0x%lx.\n",
		srclen,(unsigned long)src,tot,(unsigned long)dest);

	if (varname)
		shell_sprintf(varname,"%d",tot);

	return(0);
}

/* Front end to the rest of the unZip() stuff...
	Return the size of the decompressed data or -1 if failure.
	The same front API end is available if unpack is used instead of unzip.
*/
int
decompress(char *src,int srclen, char *dest)
{
	return(unZip(src,srclen,dest,99999999));
}
