/*    Copyright (C) 2007  Virtual Cogs Embedded Systems Inc.

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

#include "../LPC2136.h"


void MotorInit()
{
	PINSEL0|=0x00008000;
	PINSEL1|=0x00000400;

	//disable motors 
	IO1SET=0x00030000;
	IO1DIR=0x000f0000;
		
	//setup PWM
	PWMPCR=0x2400;
	PWMPR=24;
	PWMMCR=0x2;
	PWMMR0=256;
	PWMMR2=255;
	PWMMR5=255;
	PWMLER=0x24;
	PWMTCR=0x9;
}

void MotorEnable(UCHAR flags)
{
	if(flags & 0x01)
		IO1CLR=0x00010000;
	else
		IO1SET=0x00010000;

	if(flags & 0x02)
		IO1CLR=0x00020000;
	else
		IO1SET=0x00020000;
}

void MotorDuty(UCHAR spd0, UCHAR spd1)
{
	UCHAR spd0a,spd1a;

	if(spd0<128)
	{
		spd0a=255-2*(127-spd0);
		IO1CLR=0x00040000;
	}
	else
	{
		spd0a=255-2*(spd0-128);
		IO1SET=0x00040000;
	}

	if(spd1<128)
	{
		spd1a=255-2*(127-spd1);
		IO1CLR=0x00080000;
	}
	else
	{
		spd1a=255-2*(spd1-128);
		IO1SET=0x00080000;
	}
		
	PWMMR2=spd0a;
	PWMMR5=spd1a;
	PWMLER=0x24;
}

UCHAR MotorFault()
{
	return (IO1PIN>>21) & 0x03;
}
