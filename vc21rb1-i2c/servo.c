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

void ServoInt() __attribute__((interrupt("IRQ")));

UINT ServoVal[5];
int ServoFlag;

void ServoInt()
{
	if(ServoFlag)
	{
		T1MR0=ServoVal[4];
		ServoFlag=0;
	}
	else
	{
		T1MR0=ServoVal[0];
		T1MR1=ServoVal[1];
		T1MR2=ServoVal[2];
		T1MR3=ServoVal[3];
		T1TC=0;
		T1EMR=0x0aa0;
		ServoFlag=1;
	}
	
	T1IR=0x01;
	VICVECTADDR=0x0;	//reset VIC
}

void SetServo(int servo, int val)
{
	ServoVal[servo]=256+val;

	ServoVal[4]=10*512;
	

}

void ServoInit()
{
	//setup timer
	T1TCR=0x3;
	T1PR=234;

	ServoFlag=1;
	SetServo(0,128);
	SetServo(1,128);
	SetServo(2,128);
	SetServo(3,128);
	
	T1MCR=0x1;
	T1MR0=ServoVal[0];
	T1MR1=ServoVal[1];
	T1MR2=ServoVal[2];
	T1MR3=ServoVal[3];
	T1EMR=0x0550;
	
	T1IR=0xff;
	PINSEL0|=0x0a000000;
	PINSEL1|=0x0000010c;
	

	//init interrupts
	VICINTENABLE=0x20;
	VICVECTCNTL5=0x20 | 5;
	VICVECTADDR5=(UINT)ServoInt;
	T1TCR=0x1;	
}
