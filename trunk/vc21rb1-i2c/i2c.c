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

#define BUFFER_LEN 100

#define FEND	0xc0
#define FESC	0xdb
#define TFESC	0xdd
#define TFEND	0xdc

#define SER_NULL 0
#define SER_FESC 1
#define SER_ERR 2




UCHAR SerialBuf[BUFFER_LEN];
UINT SerialBufLen;

int SerialFlag;

int TxIndex;
static UCHAR TxSerialBuf[BUFFER_LEN];
UINT TxSerialBufLen;

void I2Cint() __attribute__((interrupt("IRQ")));
void I2Cint()
{
	switch(I2C0STAT)
	{
	case 0x60:	//address+write
		I2C0CONSET=0x04;
		I2C0CONCLR=0x08;
		break;
	case 0x80:	//receive data
		SerialBuf[SerialBufLen++]=I2C0DAT;
		I2C0CONCLR=0x08;
		break;
	case 0xa0:	//receive stop
		I2C0CONSET=0x04;
		I2C0CONCLR=0x08;
		SerialFlag=1;
		break;
	case 0xa8:	//slave trasmitter address=read
		I2C0DAT=TxSerialBuf[TxIndex++];
		I2C0CONSET=0x04;
		I2C0CONCLR=0x08;
		break;
	case 0xb8:	//ACK received on transmit
		I2C0DAT=TxSerialBuf[TxIndex++];
		I2C0CONSET=0x04;
		I2C0CONCLR=0x08;
		break;
	case 0xc0:	//NAK received on transmit
	case 0xc8:	//last byte transmitted
		I2C0CONSET=0x04;
		I2C0CONCLR=0x08;
		break;
	
	}
	VICVECTADDR=0x0;	//reset VIC
}

void I2Cinit()
{
	UINT i;
	PINSEL0|=0x00000050;

	//init i2c
	I2C0CONSET=0x44;
	I2C0CONCLR=0x38;
	I2C0ADR=0xaa;
	
	//init interrupts
	VICINTENABLE=0x200;
	VICVECTCNTL7=0x20 | 9;
	VICVECTADDR7=(UINT)I2Cint;

	SerialBufLen=0;
	SerialFlag=0;
}

void I2CSend(UCHAR *buf, int len)
{
	int i;
	TxSerialBufLen=len;
	for(i=0;i<len;i++)
		TxSerialBuf[i]=buf[i];
	TxIndex=0;
}


