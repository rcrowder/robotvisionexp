#include <stdio.h>

int	cfInit(int iInterface, int iVerbose)
{
	if (iInterface != 0) {
		if (iVerbose)
			printf("cfInit bad interface %d\n",iInterface);
		return(-1);
	}

	extern int SDHC_Init(int iVerbose);
	return SDHC_Init(iVerbose);
}

int	cfRead(int iInterface, char *buf, int bknum, int bkcnt, int iVerbose)
{
//	if (iVerbose)
//		printf( "Entering: cfRead(%d,0x%08X,%d,%d,%d)\n", iInterface,buf,bknum,bkcnt,iVerbose );
	if (iInterface != 0) {
		if (iVerbose)
			printf("cfRead bad interface %d\n",iInterface);
		return(-1);
	}

	extern int SDHC_Read(char *buf, int bknum, int bkcnt, int iVerbose);
	if (SDHC_Read(buf,bknum,bkcnt,iVerbose) != 1)
		return(-1);

//	if (iVerbose)
//		printf("Leaving: cfRead\n");
	return(0);
}

int	cfWrite(int iInterface, char *buf, int bknum, int bkcnt, int iVerbose)
{
//	if (iVerbose)
//		printf( "Entering: cfWrite(%d,0x%08X,%d,%d,%d)\n", iInterface,buf,bknum,bkcnt,iVerbose );
	if (iInterface != 0) {
		if (iVerbose)
			printf("cfWrite bad interface %d\n",iInterface);
		return(-1);
	}

	extern int SDHC_Write(char *buf, int bknum, int bkcnt, int iVerbose);
	if (SDHC_Write(buf,bknum,bkcnt,iVerbose) != 1)
		return(-1);

//	if (iVerbose)
//		printf("Leaving: cfWrite\n");
	return(0);
}
