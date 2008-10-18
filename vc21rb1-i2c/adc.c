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

#define ADC_NUM_CHANNELS 2

USHORT ADCVal[8];

void ADCInt() __attribute__((interrupt("IRQ")));

void ADCInt()
{
	UINT val=AD0DR;
	int channel=(val>>24)&0x7;
	ADCVal[channel]=val & 0xffff;

	VICVECTADDR=0x0;	//reset VIC
}

void ADCInit()
{
	PINSEL1|=0x01400000;	//connect AD0.0 AD0.1 to ADC
	AD0CR=0x00210d03;

	//init interrupts
	VICINTENABLE=0x40000;
	VICVECTCNTL9=0x20 | 18;
	VICVECTADDR9=(UINT)ADCInt;

}

int ADCFillValues(UCHAR *buf)
{
	int len=0;
	int i;

	for(i=0;i<ADC_NUM_CHANNELS;i++)
	{
		buf[len++]=ADCVal[i] & 0xff;
		buf[len++]=(ADCVal[i]>>8) & 0xff;
	}
	return len;
}
