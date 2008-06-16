/*  VC21 series library functions
	Bitmap file functionality

    Copyright (C) 2007  Virtual Cogs Embedded Systems Inc.

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

	www.virtualcogs.com
	Virtual Cogs Embedded Systems Inc., 5694 Highway 7 East, Unit 4, Suite 311
	Markham, ON, Canada L3P 1B4
*/
#include "monlib.h"
#include "bitmap.h"
#include "lcd.h"

#define DMADELAY 100

//draws a bitmap file on a frame buffer
//centers the image on the background
int Bitmap_Draw(LCD_SURFACE *pSurface, UCHAR* pBMP)
{
	volatile UINT *pFB=(UINT*)(pSurface->address);
	volatile UINT *pFB1;
	UINT FileSize;
	UINT BmpOffset;
	UINT width;
	UINT height;
	int adjust;
	int i;
	int j;
	

	if(pBMP[0]!='B' && pBMP[1]!='M')
		return -1;
		
	FileSize=pBMP[2] + (pBMP[3]<<8) + (pBMP[4]<<16) + (pBMP[5]<<24);		//this is the actual file size
	BmpOffset=pBMP[10] + (pBMP[11]<<8) + (pBMP[12]<<16) + (pBMP[13]<<24);	//this is the location of the actual data
	width=pBMP[18] + (pBMP[19]<<8) + (pBMP[20]<<16) + (pBMP[21]<<24);	//bitmap width
	height=pBMP[22] + (pBMP[23]<<8) + (pBMP[24]<<16) + (pBMP[25]<<24);	//bitmap height
	
	pBMP+=BmpOffset;
	
	pFB+=IMAGE_SX *(IMAGE_SY-1);
	pFB-=(IMAGE_SX * ((IMAGE_SY-height)/2));
	pFB1=pFB;
	pFB+=(IMAGE_SX - width)/2;
	j=0;
	adjust=0;
	if((width*3)%4)
		adjust+=4-((width*3)%4);
	for(i=BmpOffset;i<FileSize;i+=3)
	{
		*pFB++=(COLORREF)RGB(pBMP[2],pBMP[1],pBMP[0]);
		pBMP+=3;
		j++;
		if(j==width)
		{
			pFB1-=IMAGE_SX;
			pFB=pFB1+(IMAGE_SX - width)/2;
			j=0;
			pBMP+=adjust;
		}
		
	}
	
	return 0;
	
}

//converts a BMP file to a 32 bit memory image suitable to be copied to framebuffer
int Bitmap_Convert(UCHAR* pBMP, BITMAP* pB)
{
	COLORREF *pFB;
	UINT FileSize;
	UINT BmpOffset;
	int i;
	int j;
	int adjust;
	
	if(pBMP[0]!='B' && pBMP[1]!='M')
		return -1;
		
	FileSize=pBMP[2] + (pBMP[3]<<8) + (pBMP[4]<<16) + (pBMP[5]<<24);		//this is the actual file size
	BmpOffset=pBMP[10] + (pBMP[11]<<8) + (pBMP[12]<<16) + (pBMP[13]<<24);	//this is the location of the actual data
	pB->sx=pBMP[18] + (pBMP[19]<<8) + (pBMP[20]<<16) + (pBMP[21]<<24);	//bitmap width
	pB->sy=pBMP[22] + (pBMP[23]<<8) + (pBMP[24]<<16) + (pBMP[25]<<24);	//bitmap height
	
	pBMP+=BmpOffset;
	
	pB->data = (COLORREF*)mon_malloc(pB->sx*pB->sy*sizeof(COLORREF));
	pFB=pB->data;

	pFB+=(pB->sx) *((pB->sy)-1);
	
	j=0;
	adjust=0;
	if((pB->sx*3)%4)
		adjust=4-((pB->sx*3)%4);
	for(i=BmpOffset;i<FileSize;i+=3)
	{
		*pFB++=(COLORREF)RGB(pBMP[2],pBMP[1],pBMP[0]);
		pBMP+=3;
		j++;
		if(j==pB->sx)
		{
			pFB-=2*(pB->sx);
			j=0;
			pBMP+=adjust;
		}
		if (pFB < pB->data)
		{
			// TODO: Work out why exit is via here
			//mon_printf("Convert failed\n");
			return -1;
		}
	}
	
	return 0;
	
}

//copies bitmap to LCD surface
void Bitmap_BitBlt(LCD_SURFACE *pSurface,int px, int py, BITMAP* pB)
{
	volatile UINT *mem=(UINT*)(pSurface->address);
	volatile COLORREF *pFB=pB->data;
	DMA_DCR=0x2;			//reset DMA
	DMA_DCR=0x1;			//enable DMA and user mode
	DMA_DBOSR=0xffff;		//clear DMA overflow status errors
	DMA_DSESR=0xffff;		//clear DMA overflow status errors
	DMA_DISR=0xffff;		//clear DMA interrupts
	DMA_WSRA=IMAGE_SX * 4;	//LCD  width
	DMA_XSRA=pB->sx*4;	//image width
	DMA_YSRA=pB->sy;	//image height
	DMA_SAR0=(UINT)pFB;	//set DMA source register to audio buffer register
	DMA_DAR0=(UINT)mem+((py*IMAGE_SX) + px)*4;	//set destination to SSI1_STX0
	DMA_BLR0=64;				//set burst read length
	DMA_BUCR0=DMADELAY;
	DMA_CCR0=0x1003;		//enable DMA
	while((DMA_DISR & 0x0001)==0);
}

void Bitmap_Erase(LCD_SURFACE *pSurface,int px, int py, BITMAP* pB, COLORREF bgcol)
{
	volatile UINT *mem=(UINT*)(pSurface->address);
//	volatile COLORREF *pFB=pB->data;
	DMA_DCR=0x2;			//reset DMA
	DMA_DCR=0x1;			//enable DMA and user mode
	DMA_DBOSR=0xffff;		//clear DMA overflow status errors
	DMA_DSESR=0xffff;		//clear DMA overflow status errors
	DMA_DISR=0xffff;		//clear DMA interrupts
	DMA_WSRA=IMAGE_SX * 4;	//LCD  width
	DMA_XSRA=pB->sx*4;	//image width
	DMA_YSRA=pB->sy;	//image height
	DMA_SAR0=(UINT)&bgcol;	//set DMA source register to audio buffer register
	DMA_DAR0=(UINT)mem+((py*IMAGE_SX) + px)*4;	//set destination to SSI1_STX0
	DMA_BLR0=64;				//set burst read length
	DMA_BUCR0=DMADELAY;
	DMA_CCR0=0x1803;		//enable DMA
	while((DMA_DISR & 0x0001)==0);

}

