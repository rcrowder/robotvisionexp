/* sd.h:
 * Header file supporting compact flash command.
 */

#ifndef SD_BLKSIZE
#define SD_BLKSIZE	512
#endif


/* These two functions must be supplied by the port-specific code.
 */
extern int	sdInit(int interface, int verbosity);
extern int	sdRead(int interface, char *buf, int blknum, int blkcount);
extern int	sdWrite(int interface, char *buf, int blknum, int blkcount);

