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

#include <string.h>
#include <stdlib.h>
#include "../mx21.h"
#include "monlib.h"
#include "tfs.h"
#include "cfg.h"
#include "vcmx212.h"
#include "lcd.h"
#include "bitmap.h"

// Define WARM_START_UMON if this application is NOT started via uMon:TFS
// It ensures that MicroMonitor is initialised and ready to use, regardless 
// of how the application was launched (using JTAG for example).
//#define WARM_START_UMON

unsigned long	AppStack[APPSTACKSIZE/4];

#define BGCOL RGB(128,0,0)
#define TEXT_COL RGB(128,128,128)
#define TEXT_BKGD RGB(32,0,0)

#define NUM_IMAGES 5

UCHAR *TFS_ReadFile(char* filename, UINT* pFileSize)
{
	UCHAR* pData = NULL;
	struct tfshdr bg_hdr;

	if (pFileSize)	*pFileSize = 0;

	// Does the file exists?
	if (mon_tfsfstat(filename,&bg_hdr) == 0)
	{
		// Grab the TFS file descriptor
		int tfd = mon_tfsopen(filename, TFS_RDONLY, NULL);
		if (tfd >= 0)
		{
			if (pFileSize)	*pFileSize = bg_hdr.filsize;

			pData = (UCHAR*)mon_malloc(bg_hdr.filsize);
			mon_tfsread(tfd, pData, bg_hdr.filsize);
			mon_tfsclose(tfd, NULL);
			return pData;
		}

		// Hmmm.. Find out what happened
		mon_printf("%s : %s\n", filename, (char*)mon_tfsctrl(TFS_ERRMSG,tfd,0));
		return NULL;
	}

	mon_printf("%s : file not found\n");
	return NULL;
}

int
main(int argc,char *argv[])
{
	COLORREF	ClearColor = LCD_BLUE;
	UINT		uiFileSize;
	UCHAR*		pBitmap;
	int			i, j, index;
	BITMAP		images[NUM_IMAGES];
	int			wx[2][NUM_IMAGES];
	int			wy[2][NUM_IMAGES];
	int			x[NUM_IMAGES];
	int			y[NUM_IMAGES];
	int			xs[NUM_IMAGES];
	int			ys[NUM_IMAGES];

	/* If argument count is greater than one, then dump out the
	 * set of CLI arguments...
	 */
	if (argc > 1) {
		for(i=0;i<argc;i++) {
			mon_printf("  arg[%d]: %s\n",i,argv[i]);
		}
	}

	VCMX212_Init(0);
	LCD_Init(&ClearColor);
	
	//load and LCD_Display background image
	LCD_Block(LCD_GetBackgroundBuffer(),0,0,IMAGE_SX,IMAGE_SY,RGB(0,0,80));
	Bitmap_Draw(LCD_GetBackgroundBuffer(),TFS_ReadFile("vcogs.bmp", NULL));

	LCD_PutStr(LCD_GetBackgroundBuffer(),"Bounce (uMon) Demo - D.Foisy, S.Bragg, R.Crowder", 2, 2, SMALL, LCD_WHITE, LCD_BLACK);

	//draw the transparent background color BGCOL onto the two fore frames
	LCD_Block(LCD_GetFrontBuffer(),0,0,IMAGE_SX,IMAGE_SY,BGCOL);
	LCD_Block(LCD_GetBackBuffer(),0,0,IMAGE_SX,IMAGE_SY,BGCOL);
	LCD_InitGW();	//init the fore frame window

	//load other images
	pBitmap=TFS_ReadFile("vcmx212.bmp", &uiFileSize);
	Bitmap_Convert(pBitmap,&images[0]);
	mon_free(pBitmap);
	
	pBitmap=TFS_ReadFile("vc21pc1.bmp", &uiFileSize);
	Bitmap_Convert(pBitmap,&images[1]);
	mon_free(pBitmap);
	
	pBitmap=TFS_ReadFile("vc21mm1.bmp", &uiFileSize);
	Bitmap_Convert(pBitmap,&images[2]);
	mon_free(pBitmap);
	
	pBitmap=TFS_ReadFile("vc21en1.bmp", &uiFileSize);
	Bitmap_Convert(pBitmap,&images[3]);
	mon_free(pBitmap);
	
	pBitmap=TFS_ReadFile("vc21br1.bmp", &uiFileSize);
	Bitmap_Convert(pBitmap,&images[4]);
	mon_free(pBitmap);

	//set the initial position and speed of movement
	x[0]=10;
	y[0]=10;
	xs[0]=5;
	ys[0]=-3;
	
	x[1]=15;
	y[1]=15;
	xs[1]=-3;
	ys[1]=1;
	
	x[2]=25;
	y[2]=25;
	xs[2]=-3;
	ys[2]=6;
	
	x[3]=35;
	y[3]=35;
	xs[3]=1;
	ys[3]=-3;
	
	x[4]=45;
	y[4]=45;
	xs[4]=2;
	ys[4]=1;
	
	
	for(j=0;j<2;j++)
	{
		for(i=0;i<NUM_IMAGES;i++)
		{
			wx[j][i]=x[i];
			wy[j][i]=y[i];
		}
	}
	
	//copy the images to the two fore frame page flipping buffers
	for(i=0;i<NUM_IMAGES;i++)
	{
		Bitmap_BitBlt(LCD_GetFrontBuffer(),x[i],y[i],&images[i]);
		Bitmap_BitBlt(LCD_GetBackBuffer(),x[i],y[i],&images[i]);
	}
	
	index = 0;

	while(1)
	{
		//erase the odl images
		for(i=0;i<NUM_IMAGES;i++)
			Bitmap_Erase(LCD_GetBackBuffer(),wx[index][i],wy[index][i],&images[i],BGCOL);
		
		//draw the new image
		for(i=0;i<NUM_IMAGES;i++)
		{
			x[i]+=xs[i];
			if((x[i]+images[i].sx)>=IMAGE_SX)
			{
				xs[i]=-xs[i];
				x[i]+=xs[i]*2;
			}
			else if(x[i]<0)
			{
				xs[i]=-xs[i];
				x[i]+=xs[i]*2;
			}
			y[i]+=ys[i];
			if((y[i]+images[i].sy)>=IMAGE_SY)
			{
				ys[i]=-ys[i];
				y[i]+=ys[i]*2;
			}
			else if(y[i]<0)
			{
				ys[i]=-ys[i];
				y[i]+=ys[i]*2;
			}
			Bitmap_BitBlt(LCD_GetBackBuffer(),x[i],y[i],&images[i]);
			wx[index][i]=x[i];
			wy[index][i]=y[i];
			
		}
		index ^= 1;
		
		LCD_Flip(NULL);
	}
	return(0);
}

void
__gccmain()
{
}

int
Cstart(void)
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

	/* Won't get here. */
	return(0);
}
