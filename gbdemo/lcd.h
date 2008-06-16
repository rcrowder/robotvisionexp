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

#include "../mx21.h"

#ifndef LCD
#define LCD

//defines
int LCD_SX;			//horizontal size of LCD
int LCD_SY;			//vertical size of LCD

#define IMAGE_SX LCD_SX		//horizontal size of image 
#define IMAGE_SY LCD_SY		//vertical size of image
							//these are different than LCD_SX/SY because the image in memory can be bigger
							//than the image on the LCD (helps for scrolling)

#define FRAMEBUFFER0 0xc0300000
#define FRAMEBUFFER1 0xc0500000
#define BACKBUFFER   0xc0400000

#define RGB(r,g,b) ((b)<<16 | (g)<<8 | (r))
#define SETXY(fb,x,y) *((UINT*)(fb + ((y)*IMAGE_SX + (x))))

//structures and typedefs
typedef struct LCD_SURFACE_t
{
	UCHAR *address;
}LCD_SURFACE;

typedef UINT COLORREF;

typedef struct POINT_t
{
	int x;
	int y;
}POINT;


//variables
LCD_SURFACE LcdSurfaces[3];

// Define some colors

#define LCD_BLACK (RGB(0,0,0))
#define LCD_RED (RGB(255,0,0))
#define LCD_GREEN (RGB(0,255,0))
#define LCD_BLUE (RGB(0,0,255))
#define LCD_YELLOW (RGB(255,255,0))
#define LCD_WHITE (RGB(255,255,255))

// Font sizes
#define SMALL 0
#define MEDIUM 1
#define LARGE 2

//functions
void LCD_Init(COLORREF* pClearColor);
void LCD_InitGW();
void LCD_Flip(COLORREF* pClearColor);

LCD_SURFACE *LCD_GetBackgroundBuffer();
LCD_SURFACE *LCD_GetFrontBuffer();
LCD_SURFACE *LCD_GetBackBuffer();

void LCD_Block(LCD_SURFACE *pSurface, int x0, int y0,int sx,int sy,COLORREF col);
void LCD_Line(LCD_SURFACE *pSurface,int x0, int y0, int x1, int y1,COLORREF col);
void LCD_LineW(LCD_SURFACE *pSurface,int x0, int y0, int x1, int y1,int thickness,COLORREF col);
void LCD_InitCursor(int blink,int width,int height,int x,int y,COLORREF col);
void LCD_SetCursorPos(int x, int y);

void LCD_DrawVertLine(LCD_SURFACE *pSurface, int x, int y1, int y2, UINT color);
void LCD_DrawHorLine(LCD_SURFACE *pSurface, int x1, int x2, int y, UINT color);
void LCD_DrawCircle(LCD_SURFACE *pSurface, int x0, int y0, int radius, int color);
void LCD_PutStr(LCD_SURFACE *pSurface, char *pString, int x, int y, int Size, int fColor, int bColor);
void LCD_PutChar(LCD_SURFACE *pSurface, char c, int x, int y, int size, int fColor, int bColor);

#endif
