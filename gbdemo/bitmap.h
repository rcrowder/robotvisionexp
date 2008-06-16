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
#include "../mx21.h"
#include "lcd.h"

#ifndef BITMAPHEADER
#define BITMAPHEADER

typedef struct BITMAP_t
{
	int sx;
	int sy;
	COLORREF *data;
}BITMAP;

extern int Bitmap_Draw(LCD_SURFACE *pSurface, UCHAR* pBMP);
extern int Bitmap_Convert(UCHAR* pBMP, BITMAP* pB);
extern void Bitmap_BitBlt(LCD_SURFACE *pSurface,int px, int py, BITMAP* pB);
extern void Bitmap_Erase(LCD_SURFACE *pSurface,int px, int py, BITMAP* pB, COLORREF bgcol);

#endif
