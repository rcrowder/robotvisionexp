/*  VC21 series library functions
	LCD functionality

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
#include <stdlib.h>
#include "../mx21.h"
#include "monlib.h"
#include "lcd.h"

extern char*	s_pSDRAM0_HeapStart;

static int LCD_Work;
static int LCD_Display;

//determine which LCD is being used and initialize it
void LCD_Init(COLORREF* pClearColor)
{
	MAX_MPR3=0x543021;		//increase the DMA priority over the processor

	CRM_PCCR0|=0x04040000;
	
	if(GPIOA_SSR & 0x80000000)	//MM COG
	{	
		LCD_SX=320;
		LCD_SY=240;
	}
	else
	{
		LCD_SX=480;
		LCD_SY=272;
	}
	LcdSurfaces[0].address=(UCHAR*)mon_malloc(LCD_SX*LCD_SY*4);
	LcdSurfaces[1].address=(UCHAR*)mon_malloc(LCD_SX*LCD_SY*4);
	LcdSurfaces[2].address=(UCHAR*)mon_malloc(LCD_SX*LCD_SY*4);

	LCD_Work	= 0;
	LCD_Display	= 1;

	if(GPIOA_SSR & 0x80000000)	//MM COG
	{	
		//init GPIO
		GPIOA_GIUS=0x4f000000;	//enable LCDC IO
		GPIOC_GIUS|=0x80000000;	//enable GPIO for backlight
		GPIOC_OCR2|=0xc0000000;	//enable GPIO output for backlight
		GPIOC_DDIR|=0x80000000;	//enable GPIO output for backlight
		GPIOC_DR|=0x80000000;	//turn on backlight

		//init LCD
		LCDC_LSSAR=(UINT)(LcdSurfaces[2].address);	//start memory location
		LCDC_LSR=LCD_SX<<16 | LCD_SY;	//screen width/16 and height
		LCDC_LVPWR=IMAGE_SX;			//virtual page width (if made bigger than the actual screen width, can implement a simple horizontal scroll by simply incrementing the FRAMEBUFFER location)
		LCDC_LPCR=0xccca0083;	//TFT, 18bpp, PIXPOL active high, VSYNC active low, HSYNC active low, data changes on pos edge of clock, OE active high, LSCLK continually enabled,
								//LSCLK always enabled, pixclk=perclk3/4
		LCDC_LHCR=0x78000144;	//set horizontal timing parameters for hsync
		LCDC_LVCR=0x0c001414;	//set vertical timing parameters for vsync
		LCDC_LDCR=0x0004000c;	//setup DMA
	}
	else
	{
		GPIOA_GIUS=0xcf000000;		//enable LCDC IO 
		GPIOA_OCR2|=0xc0000000;		//set PA31 to GPIO output controlled by DR
		GPIOA_DR&=  0x7fffffff;		//make PA31 = 0
		GPIOA_DDIR|=0x80000000;		//make PA31 an output
		
		GPIOE_GIUS|=0x00000020;		//enable GPIO for backlight
		GPIOE_OCR1|=0x00000c00;	//enable GPIO output for backlight
		GPIOE_DDIR|=0x00000020;	//enable GPIO output for backlight
		GPIOE_DR|=  0x00000020;	//turn on backlight

		LCDC_LSSAR=(UINT)(LcdSurfaces[2].address);	//start memory location
		LCDC_LSR=LCD_SX<<16 | LCD_SY;	//screen width/16 and height
		LCDC_LVPWR=IMAGE_SX;			//virtual page width (if made bigger than the actual screen width, can implement a simple horizontal scroll by simply incrementing the FRAMEBUFFER location)
		LCDC_LPCR=0xccea0083;	//TFT, 18bpp, PIXPOL active high, VSYNC active low, HSYNC active low, data changes on pos edge of clock, OE active high, LSCLK continually enabled,
								//LSCLK always enabled, pixclk=perclk3/4
		LCDC_LHCR=0xa0000100;	//set horizontal timing parameters for hsync
		LCDC_LVCR=0x28000203;	//set vertical timing parameters for vsync
		LCDC_LDCR=0x0004000c;	//setup DMA

		while(GPIOA_SSR & 0x20000000);
		while((GPIOA_SSR & 0x20000000)==0);
		GPIOA_DR|=0x80000000;	//turn on display
	}

	if (pClearColor)
	{
		volatile UINT i,j;
		COLORREF *pForeground1, *pForeground2, *pBackground;

		//erase backgrounds to specified color
		pForeground1=(COLORREF*)LcdSurfaces[0].address;
		pForeground2=(COLORREF*)LcdSurfaces[1].address;
		pBackground =(COLORREF*)LcdSurfaces[2].address;

		for(j = 0; j < LCD_SY; j++)
		{
			for(i = 0; i < LCD_SX; i++)
			{
				*pForeground1++	= *pClearColor;
				*pForeground2++	= *pClearColor;
				*pBackground++	= *pClearColor;
			}
		}
	}
	return;
}

LCD_SURFACE *LCD_GetBackgroundBuffer()
{
	return &LcdSurfaces[2];
}
LCD_SURFACE *LCD_GetFrontBuffer()
{
	return &LcdSurfaces[LCD_Display];
}
LCD_SURFACE *LCD_GetBackBuffer()
{
	return &LcdSurfaces[LCD_Work];
}

//initialize the graphics window
void LCD_InitGW()
{
	UINT i;
	LCDC_LGWSAR=(UINT)(LcdSurfaces[0].address);	//start memory location
	LCDC_LGWSR=LCD_SX<<16 | LCD_SY;
	LCDC_LGWVPWR=LCD_SX;
	LCDC_LGWDCR=0x0004000c;
	
	i=CRM_PCCR0;
	CRM_PCCR0=0x0;			//have to turn off clocks to get graphic window on
	LCDC_LGWCR=0xffc20000;
	CRM_PCCR0=i;			//clocks on
}

void LCD_Flip(COLORREF* pClearColor)
{
	int i, j;

	//swap the buffers
	i=LCD_Work;
	LCD_Work=LCD_Display;
	LCD_Display=i;

	while((GPIOA_SSR & 0x20000000));
	LCDC_LGWSAR=(UINT)(LcdSurfaces[LCD_Display].address);	//start memory location
	while((GPIOA_SSR & 0x20000000)==0);
	while((GPIOA_SSR & 0x20000000));

	if (pClearColor)
	{
		//erase backgrounds to specified color
		COLORREF *pFB = (COLORREF*)LcdSurfaces[LCD_Work].address;

		for(j = 0; j < LCD_SY; j++)
		{
			for(i = 0; i < LCD_SX; i++)
			{
				*pFB++ = *pClearColor;
			}
		}
	}
}

#define PT_IN_IMAGE(x,y) (x>=0 && x<IMAGE_SX && y>=0 && y<(IMAGE_SX * IMAGE_SY))	//y is actually memory address of row
void LCD_Line(LCD_SURFACE *pSurface,int x0, int y0, int x1, int y1,COLORREF col)
{
	
	int dy = y1 - y0;
    int dx = x1 - x0;
    int stepx, stepy;
	volatile UINT *mem=(UINT*)(pSurface->address);
//	int index;

    if (dy < 0)
	{
		dy = -dy;
		stepy = -IMAGE_SX;
	}
	else
	{
		stepy = IMAGE_SX;
	}
    if (dx < 0) 
	{ 
		dx = -dx;
		stepx = -1;
	} 
	else 
	{ 
		stepx = 1; 
	}
    dy <<= 1;                                                  // dy is now 2*dy
    dx <<= 1;                                                  // dx is now 2*dx

	y0*=IMAGE_SX;
	y1*=IMAGE_SX;
    
	//first point
	
	if(PT_IN_IMAGE(x0,y0) )
	{
		mem[y0+x0]=col;
	}

    if (dx > dy) 
	{
        int fraction = dy - (dx >> 1);                         // same as 2*dy - dx
        while (x0 != x1) 
		{
            if (fraction >= 0) 
			{
                y0 += stepy;
				fraction -= dx;                                // same as fraction -= 2*dx
			}
            x0 += stepx;
		
			fraction += dy;                                    // same as fraction -= 2*dy
            
			if(PT_IN_IMAGE(x0,y0))
			{
				mem[x0+y0]=col;
			}
			
        }
    }
	else
	{
        int fraction = dx - (dy >> 1);
        while (y0 != y1) 
		{
            if (fraction >= 0) 
			{
                x0 += stepx;
                fraction -= dy;
            }
            y0 += stepy;
            fraction += dx;
            
			if(PT_IN_IMAGE(x0,y0))
			{
				mem[x0+y0]=col;
			}
        }
    }
}

void LCD_LineW(LCD_SURFACE *pSurface,int x0, int y0, int x1, int y1,int thickness, COLORREF col)
{
	int dy = y1 - y0;
    int dx = x1 - x0;
    int stepx, stepy;
	volatile UINT *mem=(UINT*)(pSurface->address);
//	int index;
	int x,y;

    if (dy < 0)
	{
		dy = -dy;
		stepy = -IMAGE_SX;
	}
	else
	{
		stepy = IMAGE_SX;
	}
    if (dx < 0) 
	{ 
		dx = -dx;
		stepx = -1;
	} 
	else 
	{ 
		stepx = 1; 
	}
    dy <<= 1;                                                  // dy is now 2*dy
    dx <<= 1;                                                  // dx is now 2*dx

	y0*=IMAGE_SX;
	y1*=IMAGE_SX;
    
	if (dx > dy) 
	{
        int fraction = dy - (dx >> 1);                         // same as 2*dy - dx
       

		while (x0 != x1) 
		{
            if (fraction >= 0) 
			{
                y0 += stepy;
				fraction -= dx;                                // same as fraction -= 2*dx
			}
            x0 += stepx;
		
			fraction += dy;                                    // same as fraction -= 2*dy
            
			for(y=y0-(thickness*IMAGE_SX);y<y0+(thickness*IMAGE_SX);y+=IMAGE_SX)
			{
				if(PT_IN_IMAGE(x0,y))
				{
					mem[y+x0]=col;
				}
			}

        }
    }
	else
	{
        int fraction = dx - (dy >> 1);
        while (y0 != y1) 
		{
            if (fraction >= 0) 
			{
                x0 += stepx;
                fraction -= dy;
            }
            y0 += stepy;
            fraction += dx;
            
			for(x=x0-thickness;x<x0+thickness;x++)
			{
				if(PT_IN_IMAGE(x,y0))
				{
					mem[x+y0]=col;
				}
			}
        }
    }
}


void LCD_Block(LCD_SURFACE *pSurface, int x0, int y0, int sx, int sy, COLORREF col)
{
	volatile UINT *mem=(UINT*)(pSurface->address);
	int x,y;
	if(x0>=0 && x0<IMAGE_SX && y0>=0 && y0<IMAGE_SY)
	{
		for(y=y0;y<y0+sy;y++)
		{
			if(y>=IMAGE_SY)
				break;
			if(y>=0)
			{
				int yp=y*IMAGE_SX;
				for(x=x0;x<x0+sx;x++)
				{
					if(x>=IMAGE_SX)
						break;
					if(x>=0)
					{
						
						mem[yp + x]=col;
					}
				}
			}
		}
	}
}

void LCD_InitCursor(int blink,int width,int height,int x,int y,COLORREF col)
{
	LCDC_LCPR=0x90000000 | (x<<16) | y;
	LCDC_LCWHBR=(blink << 31) | (width<<24) | (height<<16) ;
	int red=(col>>16) & 0xff;
	int green=(col>>8) & 0xff;
	int blue=(col) & 0xff;
	LCDC_LCCMR= (red<<12) | (green<<6) | blue;
}

void LCD_SetCursorPos(int x, int y)
{
	LCDC_LCPR=0x90000000 | (x<<16) | y;
}

void LCD_DrawVertLine(LCD_SURFACE *pSurface, int x, int y1, int y2, UINT color)
{
	volatile UINT *mem=(UINT*)(pSurface->address);
	int i;
	
	mem += x;
	mem += (y1 * LCD_SX);
	for (i=y1; i<=y2; i++)
	{
		*mem=color;	
		mem += LCD_SX;
	}
}

// Draw hor line
// Assumes one 32-bit word per pixel

void LCD_DrawHorLine(LCD_SURFACE *pSurface, int x1, int x2, int y, UINT color)
{
	volatile UINT *mem=(UINT*)(pSurface->address);
	int i;
	
	mem += x1;
	mem += (y * LCD_SX);
	
	for (i=x1; i<=x2; i++)
	{
		*mem++=color;	
	}
}

#define LCDSetPixel(x, y, c) *((UINT*)mem+((y)*LCD_SX)+(x)) = (c);
//
// Draws a circle in the specified color at center (x0,y0) with radius
//
// Inputs: x0 = row address (0 .. 131)
// y0 = column address (0 .. 131)
// radius = radius in pixels
// color = 12-bit color value rrrrggggbbbb
//
// Returns: nothing
//
// Author: Jack Bresenham IBM, Winthrop University (Father of this algorithm, 1962)
//
// Note: taken verbatim Wikipedia article on Bresenham's line algorithm
// http://www.wikipedia.org
//
// *************************************************************************************

void LCD_DrawCircle(LCD_SURFACE *pSurface, int x0, int y0, int radius, int color) 
{
	volatile UINT *mem=(UINT*)(pSurface->address);
	int f = 1 - radius;
	int ddF_x = 0;
	int ddF_y = -2 * radius;
	int x = 0;
	int y = radius;
	
	LCDSetPixel(x0, y0 + radius, color);
	LCDSetPixel(x0, y0 - radius, color);
	LCDSetPixel(x0 + radius, y0, color);
	LCDSetPixel(x0 - radius, y0, color);
	
	while (x < y) 
	{
		if (f >= 0) 
		{
			y--;
			ddF_y += 2;
			f += ddF_y;
		}
		
		x++;
		ddF_x += 2;
		f += ddF_x + 1;
		
		LCDSetPixel(x0 + x, y0 + y, color);
		LCDSetPixel(x0 - x, y0 + y, color);
		LCDSetPixel(x0 + x, y0 - y, color);
		LCDSetPixel(x0 - x, y0 - y, color);
		LCDSetPixel(x0 + y, y0 + x, color);
		LCDSetPixel(x0 - y, y0 + x, color);
		LCDSetPixel(x0 + y, y0 - x, color);
		LCDSetPixel(x0 - y, y0 - x, color);
	}
}

// *********************************************************************************
// Font tables
//
// FONT6x8 - SMALL font (mostly 5x7)
// FONT8x8 - MEDIUM font (8x8 characters, a bit thicker)
// FONT8x16 - LARGE font (8x16 characters, thicker)
//
// Note: ASCII characters 0x00 through 0x1F are not included in these fonts.
// First row of each font contains the number of columns, the
// number of rows and the number of bytes per character.
//
// Author: Jim Parise, James P Lynch July 7, 2007
// *********************************************************************************
const unsigned char FONT6x8[97][8] = {
0x06,0x08,0x08,0x00,0x00,0x00,0x00,0x00, // columns, rows, num_bytes_per_char
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // space 0x20
0x20,0x20,0x20,0x20,0x20,0x00,0x20,0x00, // !
0x50,0x50,0x50,0x00,0x00,0x00,0x00,0x00, // "
0x50,0x50,0xF8,0x50,0xF8,0x50,0x50,0x00, // #
0x20,0x78,0xA0,0x70,0x28,0xF0,0x20,0x00, // $
0xC0,0xC8,0x10,0x20,0x40,0x98,0x18,0x00, // %
0x40,0xA0,0xA0,0x40,0xA8,0x90,0x68,0x00, // &
0x30,0x30,0x20,0x40,0x00,0x00,0x00,0x00, // '
0x10,0x20,0x40,0x40,0x40,0x20,0x10,0x00, // (
0x40,0x20,0x10,0x10,0x10,0x20,0x40,0x00, // )
0x00,0x20,0xA8,0x70,0x70,0xA8,0x20,0x00, // *
0x00,0x20,0x20,0xF8,0x20,0x20,0x00,0x00, // +
0x00,0x00,0x00,0x00,0x30,0x30,0x20,0x40, // ,
0x00,0x00,0x00,0xF8,0x00,0x00,0x00,0x00, // -
0x00,0x00,0x00,0x00,0x00,0x30,0x30,0x00, // .
0x00,0x08,0x10,0x20,0x40,0x80,0x00,0x00, // / (forward slash)
0x70,0x88,0x88,0xA8,0x88,0x88,0x70,0x00, // 0 0x30
0x20,0x60,0x20,0x20,0x20,0x20,0x70,0x00, // 1
0x70,0x88,0x08,0x70,0x80,0x80,0xF8,0x00, // 2
0xF8,0x08,0x10,0x30,0x08,0x88,0x70,0x00, // 3
0x10,0x30,0x50,0x90,0xF8,0x10,0x10,0x00, // 4
0xF8,0x80,0xF0,0x08,0x08,0x88,0x70,0x00, // 5
0x38,0x40,0x80,0xF0,0x88,0x88,0x70,0x00, // 6
0xF8,0x08,0x08,0x10,0x20,0x40,0x80,0x00, // 7
0x70,0x88,0x88,0x70,0x88,0x88,0x70,0x00, // 8
0x70,0x88,0x88,0x78,0x08,0x10,0xE0,0x00, // 9
0x00,0x00,0x20,0x00,0x20,0x00,0x00,0x00, // :
0x00,0x00,0x20,0x00,0x20,0x20,0x40,0x00, // ;
0x08,0x10,0x20,0x40,0x20,0x10,0x08,0x00, // <
0x00,0x00,0xF8,0x00,0xF8,0x00,0x00,0x00, // =
0x40,0x20,0x10,0x08,0x10,0x20,0x40,0x00, // >
0x70,0x88,0x08,0x30,0x20,0x00,0x20,0x00, // ?
0x70,0x88,0xA8,0xB8,0xB0,0x80,0x78,0x00, // @ 0x40
0x20,0x50,0x88,0x88,0xF8,0x88,0x88,0x00, // A
0xF0,0x88,0x88,0xF0,0x88,0x88,0xF0,0x00, // B
0x70,0x88,0x80,0x80,0x80,0x88,0x70,0x00, // C
0xF0,0x88,0x88,0x88,0x88,0x88,0xF0,0x00, // D
0xF8,0x80,0x80,0xF0,0x80,0x80,0xF8,0x00, // E
0xF8,0x80,0x80,0xF0,0x80,0x80,0x80,0x00, // F
0x78,0x88,0x80,0x80,0x98,0x88,0x78,0x00, // G
0x88,0x88,0x88,0xF8,0x88,0x88,0x88,0x00, // H
0x70,0x20,0x20,0x20,0x20,0x20,0x70,0x00, // I
0x38,0x10,0x10,0x10,0x10,0x90,0x60,0x00, // J
0x88,0x90,0xA0,0xC0,0xA0,0x90,0x88,0x00, // K
0x80,0x80,0x80,0x80,0x80,0x80,0xF8,0x00, // L
0x88,0xD8,0xA8,0xA8,0xA8,0x88,0x88,0x00, // M
0x88,0x88,0xC8,0xA8,0x98,0x88,0x88,0x00, // N
0x70,0x88,0x88,0x88,0x88,0x88,0x70,0x00, // O
0xF0,0x88,0x88,0xF0,0x80,0x80,0x80,0x00, // P 0x50
0x70,0x88,0x88,0x88,0xA8,0x90,0x68,0x00, // Q
0xF0,0x88,0x88,0xF0,0xA0,0x90,0x88,0x00, // R
0x70,0x88,0x80,0x70,0x08,0x88,0x70,0x00, // S
0xF8,0xA8,0x20,0x20,0x20,0x20,0x20,0x00, // T
0x88,0x88,0x88,0x88,0x88,0x88,0x70,0x00, // U
0x88,0x88,0x88,0x88,0x88,0x50,0x20,0x00, // V
0x88,0x88,0x88,0xA8,0xA8,0xA8,0x50,0x00, // W
0x88,0x88,0x50,0x20,0x50,0x88,0x88,0x00, // X
0x88,0x88,0x50,0x20,0x20,0x20,0x20,0x00, // Y
0xF8,0x08,0x10,0x70,0x40,0x80,0xF8,0x00, // Z
0x78,0x40,0x40,0x40,0x40,0x40,0x78,0x00, // [
0x00,0x80,0x40,0x20,0x10,0x08,0x00,0x00, // \ (back slash)
0x78,0x08,0x08,0x08,0x08,0x08,0x78,0x00, // ]
0x20,0x50,0x88,0x00,0x00,0x00,0x00,0x00, // ^
0x00,0x00,0x00,0x00,0x00,0x00,0xF8,0x00, // _
0x60,0x60,0x20,0x10,0x00,0x00,0x00,0x00, // ` 0x60
0x00,0x00,0x60,0x10,0x70,0x90,0x78,0x00, // a
0x80,0x80,0xB0,0xC8,0x88,0xC8,0xB0,0x00, // b
0x00,0x00,0x70,0x88,0x80,0x88,0x70,0x00, // c
0x08,0x08,0x68,0x98,0x88,0x98,0x68,0x00, // d
0x00,0x00,0x70,0x88,0xF8,0x80,0x70,0x00, // e
0x10,0x28,0x20,0x70,0x20,0x20,0x20,0x00, // f
0x00,0x00,0x70,0x98,0x98,0x68,0x08,0x70, // g
0x80,0x80,0xB0,0xC8,0x88,0x88,0x88,0x00, // h
0x20,0x00,0x60,0x20,0x20,0x20,0x70,0x00, // i
0x10,0x00,0x10,0x10,0x10,0x90,0x60,0x00, // j
0x80,0x80,0x90,0xA0,0xC0,0xA0,0x90,0x00, // k
0x60,0x20,0x20,0x20,0x20,0x20,0x70,0x00, // l
0x00,0x00,0xD0,0xA8,0xA8,0xA8,0xA8,0x00, // m
0x00,0x00,0xB0,0xC8,0x88,0x88,0x88,0x00, // n
0x00,0x00,0x70,0x88,0x88,0x88,0x70,0x00, // o
0x00,0x00,0xB0,0xC8,0xC8,0xB0,0x80,0x80, // p 0x70
0x00,0x00,0x68,0x98,0x98,0x68,0x08,0x08, // q
0x00,0x00,0xB0,0xC8,0x80,0x80,0x80,0x00, // r
0x00,0x00,0x78,0x80,0x70,0x08,0xF0,0x00, // s
0x20,0x20,0xF8,0x20,0x20,0x28,0x10,0x00, // t
0x00,0x00,0x88,0x88,0x88,0x98,0x68,0x00, // u
0x00,0x00,0x88,0x88,0x88,0x50,0x20,0x00, // v
0x00,0x00,0x88,0x88,0xA8,0xA8,0x50,0x00, // w
0x00,0x00,0x88,0x50,0x20,0x50,0x88,0x00, // x
0x00,0x00,0x88,0x88,0x78,0x08,0x88,0x70, // y
0x00,0x00,0xF8,0x10,0x20,0x40,0xF8,0x00, // z
0x10,0x20,0x20,0x40,0x20,0x20,0x10,0x00, // {
0x20,0x20,0x20,0x00,0x20,0x20,0x20,0x00, // |
0x40,0x20,0x20,0x10,0x20,0x20,0x40,0x00, // }
0x40,0xA8,0x10,0x00,0x00,0x00,0x00,0x00, // ~
0x70,0xD8,0xD8,0x70,0x00,0x00,0x00,0x00}; // DEL

const unsigned char FONT8x8[97][8] = {
0x08,0x08,0x08,0x00,0x00,0x00,0x00,0x00, // columns, rows, num_bytes_per_char
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // space 0x20
0x30,0x78,0x78,0x30,0x30,0x00,0x30,0x00, // !
0x6C,0x6C,0x6C,0x00,0x00,0x00,0x00,0x00, // "
0x6C,0x6C,0xFE,0x6C,0xFE,0x6C,0x6C,0x00, // #
0x18,0x3E,0x60,0x3C,0x06,0x7C,0x18,0x00, // $
0x00,0x63,0x66,0x0C,0x18,0x33,0x63,0x00, // %
0x1C,0x36,0x1C,0x3B,0x6E,0x66,0x3B,0x00, // &
0x30,0x30,0x60,0x00,0x00,0x00,0x00,0x00, // '
0x0C,0x18,0x30,0x30,0x30,0x18,0x0C,0x00, // (
0x30,0x18,0x0C,0x0C,0x0C,0x18,0x30,0x00, // )
0x00,0x66,0x3C,0xFF,0x3C,0x66,0x00,0x00, // *
0x00,0x30,0x30,0xFC,0x30,0x30,0x00,0x00, // +
0x00,0x00,0x00,0x00,0x00,0x18,0x18,0x30, // ,
0x00,0x00,0x00,0x7E,0x00,0x00,0x00,0x00, // -
0x00,0x00,0x00,0x00,0x00,0x18,0x18,0x00, // .
0x03,0x06,0x0C,0x18,0x30,0x60,0x40,0x00, // / (forward slash)
0x3E,0x63,0x63,0x6B,0x63,0x63,0x3E,0x00, // 0 0x30
0x18,0x38,0x58,0x18,0x18,0x18,0x7E,0x00, // 1
0x3C,0x66,0x06,0x1C,0x30,0x66,0x7E,0x00, // 2
0x3C,0x66,0x06,0x1C,0x06,0x66,0x3C,0x00, // 3
0x0E,0x1E,0x36,0x66,0x7F,0x06,0x0F,0x00, // 4
0x7E,0x60,0x7C,0x06,0x06,0x66,0x3C,0x00, // 5
0x1C,0x30,0x60,0x7C,0x66,0x66,0x3C,0x00, // 6
0x7E,0x66,0x06,0x0C,0x18,0x18,0x18,0x00, // 7
0x3C,0x66,0x66,0x3C,0x66,0x66,0x3C,0x00, // 8
0x3C,0x66,0x66,0x3E,0x06,0x0C,0x38,0x00, // 9
0x00,0x18,0x18,0x00,0x00,0x18,0x18,0x00, // :
0x00,0x18,0x18,0x00,0x00,0x18,0x18,0x30, // ;
0x0C,0x18,0x30,0x60,0x30,0x18,0x0C,0x00, // <
0x00,0x00,0x7E,0x00,0x00,0x7E,0x00,0x00, // =
0x30,0x18,0x0C,0x06,0x0C,0x18,0x30,0x00, // >
0x3C,0x66,0x06,0x0C,0x18,0x00,0x18,0x00, // ?
0x3E,0x63,0x6F,0x69,0x6F,0x60,0x3E,0x00, // @ 0x40
0x18,0x3C,0x66,0x66,0x7E,0x66,0x66,0x00, // A
0x7E,0x33,0x33,0x3E,0x33,0x33,0x7E,0x00, // B
0x1E,0x33,0x60,0x60,0x60,0x33,0x1E,0x00, // C
0x7C,0x36,0x33,0x33,0x33,0x36,0x7C,0x00, // D
0x7F,0x31,0x34,0x3C,0x34,0x31,0x7F,0x00, // E
0x7F,0x31,0x34,0x3C,0x34,0x30,0x78,0x00, // F
0x1E,0x33,0x60,0x60,0x67,0x33,0x1F,0x00, // G
0x66,0x66,0x66,0x7E,0x66,0x66,0x66,0x00, // H
0x3C,0x18,0x18,0x18,0x18,0x18,0x3C,0x00, // I
0x0F,0x06,0x06,0x06,0x66,0x66,0x3C,0x00, // J
0x73,0x33,0x36,0x3C,0x36,0x33,0x73,0x00, // K
0x78,0x30,0x30,0x30,0x31,0x33,0x7F,0x00, // L
0x63,0x77,0x7F,0x7F,0x6B,0x63,0x63,0x00, // M
0x63,0x73,0x7B,0x6F,0x67,0x63,0x63,0x00, // N
0x3E,0x63,0x63,0x63,0x63,0x63,0x3E,0x00, // O
0x7E,0x33,0x33,0x3E,0x30,0x30,0x78,0x00, // P 0x50
0x3C,0x66,0x66,0x66,0x6E,0x3C,0x0E,0x00, // Q
0x7E,0x33,0x33,0x3E,0x36,0x33,0x73,0x00, // R
0x3C,0x66,0x30,0x18,0x0C,0x66,0x3C,0x00, // S
0x7E,0x5A,0x18,0x18,0x18,0x18,0x3C,0x00, // T
0x66,0x66,0x66,0x66,0x66,0x66,0x7E,0x00, // U
0x66,0x66,0x66,0x66,0x66,0x3C,0x18,0x00, // V
0x63,0x63,0x63,0x6B,0x7F,0x77,0x63,0x00, // W
0x63,0x63,0x36,0x1C,0x1C,0x36,0x63,0x00, // X
0x66,0x66,0x66,0x3C,0x18,0x18,0x3C,0x00, // Y
0x7F,0x63,0x46,0x0C,0x19,0x33,0x7F,0x00, // Z
0x3C,0x30,0x30,0x30,0x30,0x30,0x3C,0x00, // [
0x60,0x30,0x18,0x0C,0x06,0x03,0x01,0x00, // \ (back slash)
0x3C,0x0C,0x0C,0x0C,0x0C,0x0C,0x3C,0x00, // ]
0x08,0x1C,0x36,0x63,0x00,0x00,0x00,0x00, // ^
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF, // _
0x18,0x18,0x0C,0x00,0x00,0x00,0x00,0x00, // ` 0x60
0x00,0x00,0x3C,0x06,0x3E,0x66,0x3B,0x00, // a
0x70,0x30,0x3E,0x33,0x33,0x33,0x6E,0x00, // b
0x00,0x00,0x3C,0x66,0x60,0x66,0x3C,0x00, // c
0x0E,0x06,0x3E,0x66,0x66,0x66,0x3B,0x00, // d
0x00,0x00,0x3C,0x66,0x7E,0x60,0x3C,0x00, // e
0x1C,0x36,0x30,0x78,0x30,0x30,0x78,0x00, // f
0x00,0x00,0x3B,0x66,0x66,0x3E,0x06,0x7C, // g
0x70,0x30,0x36,0x3B,0x33,0x33,0x73,0x00, // h
0x18,0x00,0x38,0x18,0x18,0x18,0x3C,0x00, // i
0x06,0x00,0x06,0x06,0x06,0x66,0x66,0x3C, // j
0x70,0x30,0x33,0x36,0x3C,0x36,0x73,0x00, // k
0x38,0x18,0x18,0x18,0x18,0x18,0x3C,0x00, // l
0x00,0x00,0x66,0x7F,0x7F,0x6B,0x63,0x00, // m
0x00,0x00,0x7C,0x66,0x66,0x66,0x66,0x00, // n
0x00,0x00,0x3C,0x66,0x66,0x66,0x3C,0x00, // o
0x00,0x00,0x6E,0x33,0x33,0x3E,0x30,0x78, // p 0x70
0x00,0x00,0x3B,0x66,0x66,0x3E,0x06,0x0F, // q
0x00,0x00,0x6E,0x3B,0x33,0x30,0x78,0x00, // r
0x00,0x00,0x3E,0x60,0x3C,0x06,0x7C,0x00, // s
0x08,0x18,0x3E,0x18,0x18,0x1A,0x0C,0x00, // t
0x00,0x00,0x66,0x66,0x66,0x66,0x3B,0x00, // u
0x00,0x00,0x66,0x66,0x66,0x3C,0x18,0x00, // v
0x00,0x00,0x63,0x6B,0x7F,0x7F,0x36,0x00, // w
0x00,0x00,0x63,0x36,0x1C,0x36,0x63,0x00, // x
0x00,0x00,0x66,0x66,0x66,0x3E,0x06,0x7C, // y
0x00,0x00,0x7E,0x4C,0x18,0x32,0x7E,0x00, // z
0x0E,0x18,0x18,0x70,0x18,0x18,0x0E,0x00, // {
0x0C,0x0C,0x0C,0x00,0x0C,0x0C,0x0C,0x00, // |
0x70,0x18,0x18,0x0E,0x18,0x18,0x70,0x00, // }
0x3B,0x6E,0x00,0x00,0x00,0x00,0x00,0x00, // ~
0x1C,0x36,0x36,0x1C,0x00,0x00,0x00,0x00}; // DEL

const unsigned char FONT8x16[97][16] = {
0x08,0x10,0x10,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // columns, rows, nbytes 
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // space 0x20
0x00,0x00,0x18,0x3C,0x3C,0x3C,0x18,0x18,0x18,0x00,0x18,0x18,0x00,0x00,0x00,0x00, // !
0x00,0x63,0x63,0x63,0x22,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // "
0x00,0x00,0x00,0x36,0x36,0x7F,0x36,0x36,0x36,0x7F,0x36,0x36,0x00,0x00,0x00,0x00, // #
0x0C,0x0C,0x3E,0x63,0x61,0x60,0x3E,0x03,0x03,0x43,0x63,0x3E,0x0C,0x0C,0x00,0x00, // $
0x00,0x00,0x00,0x00,0x00,0x61,0x63,0x06,0x0C,0x18,0x33,0x63,0x00,0x00,0x00,0x00, // %
0x00,0x00,0x00,0x1C,0x36,0x36,0x1C,0x3B,0x6E,0x66,0x66,0x3B,0x00,0x00,0x00,0x00, // &
0x00,0x30,0x30,0x30,0x60,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // '
0x00,0x00,0x0C,0x18,0x18,0x30,0x30,0x30,0x30,0x18,0x18,0x0C,0x00,0x00,0x00,0x00, // (
0x00,0x00,0x18,0x0C,0x0C,0x06,0x06,0x06,0x06,0x0C,0x0C,0x18,0x00,0x00,0x00,0x00, // )
0x00,0x00,0x00,0x00,0x42,0x66,0x3C,0xFF,0x3C,0x66,0x42,0x00,0x00,0x00,0x00,0x00, // *
0x00,0x00,0x00,0x00,0x18,0x18,0x18,0xFF,0x18,0x18,0x18,0x00,0x00,0x00,0x00,0x00, // +
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x18,0x18,0x18,0x30,0x00,0x00, // ,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // -
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x18,0x18,0x00,0x00,0x00,0x00, // .
0x00,0x00,0x01,0x03,0x07,0x0E,0x1C,0x38,0x70,0xE0,0xC0,0x80,0x00,0x00,0x00,0x00, // / (forward slash)
0x00,0x00,0x3E,0x63,0x63,0x63,0x6B,0x6B,0x63,0x63,0x63,0x3E,0x00,0x00,0x00,0x00, // 0 0x30
0x00,0x00,0x0C,0x1C,0x3C,0x0C,0x0C,0x0C,0x0C,0x0C,0x0C,0x3F,0x00,0x00,0x00,0x00, // 1
0x00,0x00,0x3E,0x63,0x03,0x06,0x0C,0x18,0x30,0x61,0x63,0x7F,0x00,0x00,0x00,0x00, // 2
0x00,0x00,0x3E,0x63,0x03,0x03,0x1E,0x03,0x03,0x03,0x63,0x3E,0x00,0x00,0x00,0x00, // 3
0x00,0x00,0x06,0x0E,0x1E,0x36,0x66,0x66,0x7F,0x06,0x06,0x0F,0x00,0x00,0x00,0x00, // 4
0x00,0x00,0x7F,0x60,0x60,0x60,0x7E,0x03,0x03,0x63,0x73,0x3E,0x00,0x00,0x00,0x00, // 5
0x00,0x00,0x1C,0x30,0x60,0x60,0x7E,0x63,0x63,0x63,0x63,0x3E,0x00,0x00,0x00,0x00, // 6
0x00,0x00,0x7F,0x63,0x03,0x06,0x06,0x0C,0x0C,0x18,0x18,0x18,0x00,0x00,0x00,0x00, // 7
0x00,0x00,0x3E,0x63,0x63,0x63,0x3E,0x63,0x63,0x63,0x63,0x3E,0x00,0x00,0x00,0x00, // 8
0x00,0x00,0x3E,0x63,0x63,0x63,0x63,0x3F,0x03,0x03,0x06,0x3C,0x00,0x00,0x00,0x00, // 9
0x00,0x00,0x00,0x00,0x00,0x18,0x18,0x00,0x00,0x00,0x18,0x18,0x00,0x00,0x00,0x00, // :
0x00,0x00,0x00,0x00,0x00,0x18,0x18,0x00,0x00,0x00,0x18,0x18,0x18,0x30,0x00,0x00, // ;
0x00,0x00,0x00,0x06,0x0C,0x18,0x30,0x60,0x30,0x18,0x0C,0x06,0x00,0x00,0x00,0x00, // <
0x00,0x00,0x00,0x00,0x00,0x00,0x7E,0x00,0x00,0x7E,0x00,0x00,0x00,0x00,0x00,0x00, // =
0x00,0x00,0x00,0x60,0x30,0x18,0x0C,0x06,0x0C,0x18,0x30,0x60,0x00,0x00,0x00,0x00, // >
0x00,0x00,0x3E,0x63,0x63,0x06,0x0C,0x0C,0x0C,0x00,0x0C,0x0C,0x00,0x00,0x00,0x00, // ?
0x00,0x00,0x3E,0x63,0x63,0x6F,0x6B,0x6B,0x6E,0x60,0x60,0x3E,0x00,0x00,0x00,0x00, // @ 0x40
0x00,0x00,0x08,0x1C,0x36,0x63,0x63,0x63,0x7F,0x63,0x63,0x63,0x00,0x00,0x00,0x00, // A
0x00,0x00,0x7E,0x33,0x33,0x33,0x3E,0x33,0x33,0x33,0x33,0x7E,0x00,0x00,0x00,0x00, // B
0x00,0x00,0x1E,0x33,0x61,0x60,0x60,0x60,0x60,0x61,0x33,0x1E,0x00,0x00,0x00,0x00, // C
0x00,0x00,0x7C,0x36,0x33,0x33,0x33,0x33,0x33,0x33,0x36,0x7C,0x00,0x00,0x00,0x00, // D
0x00,0x00,0x7F,0x33,0x31,0x34,0x3C,0x34,0x30,0x31,0x33,0x7F,0x00,0x00,0x00,0x00, // E
0x00,0x00,0x7F,0x33,0x31,0x34,0x3C,0x34,0x30,0x30,0x30,0x78,0x00,0x00,0x00,0x00, // F
0x00,0x00,0x1E,0x33,0x61,0x60,0x60,0x6F,0x63,0x63,0x37,0x1D,0x00,0x00,0x00,0x00, // G
0x00,0x00,0x63,0x63,0x63,0x63,0x7F,0x63,0x63,0x63,0x63,0x63,0x00,0x00,0x00,0x00, // H
0x00,0x00,0x3C,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x3C,0x00,0x00,0x00,0x00, // I
0x00,0x00,0x0F,0x06,0x06,0x06,0x06,0x06,0x06,0x66,0x66,0x3C,0x00,0x00,0x00,0x00, // J
0x00,0x00,0x73,0x33,0x36,0x36,0x3C,0x36,0x36,0x33,0x33,0x73,0x00,0x00,0x00,0x00, // K
0x00,0x00,0x78,0x30,0x30,0x30,0x30,0x30,0x30,0x31,0x33,0x7F,0x00,0x00,0x00,0x00, // L
0x00,0x00,0x63,0x77,0x7F,0x6B,0x63,0x63,0x63,0x63,0x63,0x63,0x00,0x00,0x00,0x00, // M
0x00,0x00,0x63,0x63,0x73,0x7B,0x7F,0x6F,0x67,0x63,0x63,0x63,0x00,0x00,0x00,0x00, // N
0x00,0x00,0x1C,0x36,0x63,0x63,0x63,0x63,0x63,0x63,0x36,0x1C,0x00,0x00,0x00,0x00, // O
0x00,0x00,0x7E,0x33,0x33,0x33,0x3E,0x30,0x30,0x30,0x30,0x78,0x00,0x00,0x00,0x00, // P 0x50
0x00,0x00,0x3E,0x63,0x63,0x63,0x63,0x63,0x63,0x6B,0x6F,0x3E,0x06,0x07,0x00,0x00, // Q
0x00,0x00,0x7E,0x33,0x33,0x33,0x3E,0x36,0x36,0x33,0x33,0x73,0x00,0x00,0x00,0x00, // R
0x00,0x00,0x3E,0x63,0x63,0x30,0x1C,0x06,0x03,0x63,0x63,0x3E,0x00,0x00,0x00,0x00, // S
0x00,0x00,0xFF,0xDB,0x99,0x18,0x18,0x18,0x18,0x18,0x18,0x3C,0x00,0x00,0x00,0x00, // T
0x00,0x00,0x63,0x63,0x63,0x63,0x63,0x63,0x63,0x63,0x63,0x3E,0x00,0x00,0x00,0x00, // U
0x00,0x00,0x63,0x63,0x63,0x63,0x63,0x63,0x63,0x36,0x1C,0x08,0x00,0x00,0x00,0x00, // V
0x00,0x00,0x63,0x63,0x63,0x63,0x63,0x6B,0x6B,0x7F,0x36,0x36,0x00,0x00,0x00,0x00, // W
0x00,0x00,0xC3,0xC3,0x66,0x3C,0x18,0x18,0x3C,0x66,0xC3,0xC3,0x00,0x00,0x00,0x00, // X
0x00,0x00,0xC3,0xC3,0xC3,0x66,0x3C,0x18,0x18,0x18,0x18,0x3C,0x00,0x00,0x00,0x00, // Y
0x00,0x00,0x7F,0x63,0x43,0x06,0x0C,0x18,0x30,0x61,0x63,0x7F,0x00,0x00,0x00,0x00, // Z
0x00,0x00,0x3C,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x3C,0x00,0x00,0x00,0x00, // [
0x00,0x00,0x80,0xC0,0xE0,0x70,0x38,0x1C,0x0E,0x07,0x03,0x01,0x00,0x00,0x00,0x00, // \ (back slash)
0x00,0x00,0x3C,0x0C,0x0C,0x0C,0x0C,0x0C,0x0C,0x0C,0x0C,0x3C,0x00,0x00,0x00,0x00, // ]
0x08,0x1C,0x36,0x63,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // ^
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0x00,0x00,0x00, // _
0x18,0x18,0x0C,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // ` 0x60
0x00,0x00,0x00,0x00,0x00,0x3C,0x46,0x06,0x3E,0x66,0x66,0x3B,0x00,0x00,0x00,0x00, // a
0x00,0x00,0x70,0x30,0x30,0x3C,0x36,0x33,0x33,0x33,0x33,0x6E,0x00,0x00,0x00,0x00, // b
0x00,0x00,0x00,0x00,0x00,0x3E,0x63,0x60,0x60,0x60,0x63,0x3E,0x00,0x00,0x00,0x00, // c
0x00,0x00,0x0E,0x06,0x06,0x1E,0x36,0x66,0x66,0x66,0x66,0x3B,0x00,0x00,0x00,0x00, // d
0x00,0x00,0x00,0x00,0x00,0x3E,0x63,0x63,0x7E,0x60,0x63,0x3E,0x00,0x00,0x00,0x00, // e
0x00,0x00,0x1C,0x36,0x32,0x30,0x7C,0x30,0x30,0x30,0x30,0x78,0x00,0x00,0x00,0x00, // f
0x00,0x00,0x00,0x00,0x00,0x3B,0x66,0x66,0x66,0x66,0x3E,0x06,0x66,0x3C,0x00,0x00, // g
0x00,0x00,0x70,0x30,0x30,0x36,0x3B,0x33,0x33,0x33,0x33,0x73,0x00,0x00,0x00,0x00, // h
0x00,0x00,0x0C,0x0C,0x00,0x1C,0x0C,0x0C,0x0C,0x0C,0x0C,0x1E,0x00,0x00,0x00,0x00, // i
0x00,0x00,0x06,0x06,0x00,0x0E,0x06,0x06,0x06,0x06,0x06,0x66,0x66,0x3C,0x00,0x00, // j
0x00,0x00,0x70,0x30,0x30,0x33,0x33,0x36,0x3C,0x36,0x33,0x73,0x00,0x00,0x00,0x00, // k
0x00,0x00,0x1C,0x0C,0x0C,0x0C,0x0C,0x0C,0x0C,0x0C,0x0C,0x1E,0x00,0x00,0x00,0x00, // l
0x00,0x00,0x00,0x00,0x00,0x6E,0x7F,0x6B,0x6B,0x6B,0x6B,0x6B,0x00,0x00,0x00,0x00, // m
0x00,0x00,0x00,0x00,0x00,0x6E,0x33,0x33,0x33,0x33,0x33,0x33,0x00,0x00,0x00,0x00, // n
0x00,0x00,0x00,0x00,0x00,0x3E,0x63,0x63,0x63,0x63,0x63,0x3E,0x00,0x00,0x00,0x00, // o
0x00,0x00,0x00,0x00,0x00,0x6E,0x33,0x33,0x33,0x33,0x3E,0x30,0x30,0x78,0x00,0x00, // p 0x70
0x00,0x00,0x00,0x00,0x00,0x3B,0x66,0x66,0x66,0x66,0x3E,0x06,0x06,0x0F,0x00,0x00, // q
0x00,0x00,0x00,0x00,0x00,0x6E,0x3B,0x33,0x30,0x30,0x30,0x78,0x00,0x00,0x00,0x00, // r
0x00,0x00,0x00,0x00,0x00,0x3E,0x63,0x38,0x0E,0x03,0x63,0x3E,0x00,0x00,0x00,0x00, // s
0x00,0x00,0x08,0x18,0x18,0x7E,0x18,0x18,0x18,0x18,0x1B,0x0E,0x00,0x00,0x00,0x00, // t
0x00,0x00,0x00,0x00,0x00,0x66,0x66,0x66,0x66,0x66,0x66,0x3B,0x00,0x00,0x00,0x00, // u
0x00,0x00,0x00,0x00,0x00,0x63,0x63,0x36,0x36,0x1C,0x1C,0x08,0x00,0x00,0x00,0x00, // v
0x00,0x00,0x00,0x00,0x00,0x63,0x63,0x63,0x6B,0x6B,0x7F,0x36,0x00,0x00,0x00,0x00, // w
0x00,0x00,0x00,0x00,0x00,0x63,0x36,0x1C,0x1C,0x1C,0x36,0x63,0x00,0x00,0x00,0x00, // x
0x00,0x00,0x00,0x00,0x00,0x63,0x63,0x63,0x63,0x63,0x3F,0x03,0x06,0x3C,0x00,0x00, // y
0x00,0x00,0x00,0x00,0x00,0x7F,0x66,0x0C,0x18,0x30,0x63,0x7F,0x00,0x00,0x00,0x00, // z
0x00,0x00,0x0E,0x18,0x18,0x18,0x70,0x18,0x18,0x18,0x18,0x0E,0x00,0x00,0x00,0x00, // {
0x00,0x00,0x18,0x18,0x18,0x18,0x18,0x00,0x18,0x18,0x18,0x18,0x18,0x00,0x00,0x00, // |
0x00,0x00,0x70,0x18,0x18,0x18,0x0E,0x18,0x18,0x18,0x18,0x70,0x00,0x00,0x00,0x00, // }
0x00,0x00,0x3B,0x6E,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // ~
0x00,0x70,0xD8,0xD8,0x70,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}; // DEL



// Notes: Here's an example to display "E" at address (20,20)
//
// LCDPutChar('E', 20, 20, MEDIUM, WHITE, BLACK);
//
//  (20,20)     (27,20)
//   |             |
//   |             |
// : V             V
// : _ # # # # # # # 0x7F
// : _ _ # # _ _ _ # 0x31
// : _ _ # # _ # _ _ 0x34
// y _ _ # # # # _ _ 0x3C
// : _ _ # # _ # _ _ 0x34
// : _ _ # # _ _ _ # 0x31
// : _ # # # # # # # 0x7F
// V _ _ _ _ _ _ _ _ 0x00
//
// ------x------->
//  ^              ^
//  |              |
//  |              |
//  (20,27)      (27,27)

// LCDPutStr
//
// Draws a null-terminates character string at the specified (x,y) address, size and color
//
// Inputs: pString = pointer to character string to be displayed
// x = column address (0 .. LCD_SX-1)
// y = row address (0 .. LCD_SY-1)
// Size = font pitch (SMALL, MEDIUM, LARGE)
// fColor = 32-bit foreground color value 0x00RRGGBB
// bColor = 32-bit background color value 0x00RRGGBB
//
//
// Returns: nothing
//
// Notes: Here's an example to display "Hello World!" at address (20,20)
//
// LCDPutChar("Hello World!", 20, 20, LARGE, WHITE, BLACK);
//
//
// Author: James P Lynch July 7, 2007
// *************************************************************************************************
void LCD_PutStr(LCD_SURFACE *pSurface, char *pString, int x, int y, int Size, int fColor, int bColor) 
{
	// loop until null-terminator is seen
	while (*pString) 
	{
		// draw the character
		LCD_PutChar(pSurface, *pString++, x, y, Size, fColor, bColor);
		// advance the x position
		if (Size == SMALL)
			x += 6;
		else if (Size == MEDIUM)
			x += 8;
		else
			x += 8;
		
		// bail out if x exceeds or is equal to LCD_SX
		if (x >= LCD_SX) 
			break;
	}
}

// LCDPutChar
// Draws a character (ASCII 32-96) at the specified (x,y) address, size and color
//
// Inputs: pString = pointer to character string to be displayed
// x = column address (0 .. LCD_SX-1)
// y = row address (0 .. LCD_SY-1)
// Size = font pitch (SMALL, MEDIUM, LARGE)
// fColor = 32-bit foreground color value 0x00RRGGBB
// bColor = 32-bit background color value 0x00RRGGBB

void LCD_PutChar(LCD_SURFACE *pSurface, char c, int x, int y, int size, int fColor, int bColor) 
{
	volatile UINT *mem=(UINT*)(pSurface->address);
	volatile UINT *pFB;
	volatile UINT *pFBi;
	int i,j;
	unsigned int nCols;
	unsigned int nRows;
	unsigned int nBytes;
	unsigned char PixelRow;
	unsigned char Mask;
	const unsigned char *pChar;
									
	// get pointer to the first byte of the desired character
	// get the nColumns, nRows and nBytes

	c -= 0x1F;	// Calculate offset; ' ' would give 1, skipping the font info structure in the first element

	switch (size)
	{
		case LARGE:
			nCols  = FONT8x16[0][0];
			nRows  = FONT8x16[0][1];
			nBytes = FONT8x16[0][2];
			pChar  = FONT8x16[(int)c];
			break;
			
		case MEDIUM:
			nCols  = FONT8x8[0][0];
			nRows  = FONT8x8[0][1];
			nBytes = FONT8x8[0][2];
			pChar  = FONT8x8[(int)c];
			break;

		default:
		case SMALL:
			nCols  = FONT6x8[0][0];
			nRows  = FONT6x8[0][1];
			nBytes = FONT6x8[0][2];
			pChar  = FONT6x8[(int)c];
			break;
	}
	
	
	// Set initial row (x)
	
	pFBi = mem + x;
	
	// Set initial column (y)
	
	pFBi += (y * LCD_SX);
	pFB = pFBi;
	
	// Loop on each row, going from top to bottom

	for (i = 0; i < nRows; i++) 
	{
		// copy pixel row from font table and then increment row
		
		PixelRow = pChar[i];
		
		// loop on each pixel in the row (left to right)
		
		Mask = 0x80;
		for (j = 0; j < nCols; j++) 
		{
			// if pixel bit set, use foreground color; else use the background color
			// now get the pixel color
			if ((PixelRow & Mask) == 0)
				*pFB = bColor;
			else
				*pFB = fColor;
				
			pFB++;
			Mask >>= 1;
		}
		
		// Next row, back to initial column
		
		pFBi += LCD_SX;
		pFB = pFBi;
	}
}
