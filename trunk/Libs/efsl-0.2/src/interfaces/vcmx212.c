#include "types.h"

#ifndef NULL
#define NULL	0
#endif

struct hwInterface
{
	/* Obligatory fields */
	euint32 sectorCount;
};
typedef struct hwInterface hwInterface;

esint16 if_initInterface(hwInterface* hw, euint8* opts)
{
	if (hw == NULL)
		return (-1);
	extern int SDHC_Init(int iVerbose);
	SDHC_Init(0);

	return (0);
}

esint8 if_readBuf(hwInterface* hw, euint32 address, euint8* buf)
{
	extern int SDHC_Read(char *buf, int bknum, int bkcnt, int iVerbose);
	if (SDHC_Read(buf,address,1, 0) != 1)
		return (-1);

	return (0);
}

esint8 if_writeBuf(hwInterface* hw, euint32 address, euint8* buf)
{
	extern int SDHC_Write(char *buf, int bknum, int bkcnt, int iVerbose);
	if (SDHC_Write(buf,address,1, 0) != 1)
		return (-1);

	return (0);
}
