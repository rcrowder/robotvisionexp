//this program is used to take pictures using the VC21CC1 and download them to a PC
#include "../../MX21.h"

#define VC21RB1_ADDRESS 0xaa

#define BUFFER_LEN 100

#define FEND	0xc0
#define FESC	0xdb
#define TFESC	0xdd
#define TFEND	0xdc

#define SER_NULL 0
#define SER_FESC 1
#define SER_ERR 2

#define I2C_TIMEOUT 100000

#define CMD_LED			4
#define CMD_MOTOR_EN	20
#define CMD_MOTOR_DUTY	21
#define CMD_MOTOR_FAULT	23
#define CMD_SERVO		11
#define CMD_ADC			30

int I2C_read(UCHAR address, UCHAR reg, UCHAR *buf, int len)
{
	UINT i;
	
	//send device  address
	I2C_I2CR=0xb0;
	I2C_I2DR=address;	
	I2C_I2SR=0x00;
	while((I2C_I2SR & 0x2)==0);
	
	//send register address
	I2C_I2DR=reg;
	I2C_I2SR=0x00;
	while((I2C_I2SR & 0x2)==0);
	
	/*//send repeat start
	I2C_I2CR=0xa4;	
	I2C_I2SR=0x00;
	i=I2C_I2DR;
	while((I2C_I2SR & 0x2)==0);
	*/
	
	//send device address with read enable
	I2C_I2CR=0xb4;
	I2C_I2DR=address | 0x01;
	I2C_I2SR=0x00;
	while((I2C_I2SR & 0x2)==0);
	
	//read the incoming word
	if(len==1)
	{
		I2C_I2CR=0xa8;
		i=I2C_I2DR;		//garbage read from last write
		I2C_I2SR=0x00;
		while((I2C_I2SR & 0x2)==0);
		
		I2C_I2CR=0x80;	//send stop
		
		buf[0]=I2C_I2DR;	//read last char
	}
	else
	{
		I2C_I2CR=0xa0;
		i=I2C_I2DR;		//garbage read from last write
	
		for(i=0;i<len;i++)
		{
			I2C_I2SR=0x00;
			while((I2C_I2SR & 0x2)==0);
			I2C_I2SR=0x00;
			
			if(i==len-2)
				I2C_I2CR=0xa8;
			if(i<len-1)
				buf[i]=I2C_I2DR;	
		}
	
		//send stop
		I2C_I2CR=0x80;	
		
		//read last char
		buf[i-1]=I2C_I2DR;
	}
	//return the data read
	return len;
}

void I2C_write(UCHAR address, UCHAR* buf, int len)
{
	int i,cnt;
	I2C_I2CR=0xb0;
	I2C_I2DR=address;	//send sensor address
	I2C_I2SR=0x00;
	
	cnt=0;
	while(((I2C_I2SR & 0x2)==0) && (++cnt<I2C_TIMEOUT));
	
	for(i=0;i<len;i++)
	{
		I2C_I2DR=buf[i];	//send sensor value
		I2C_I2SR=0x00;
		cnt=0;
		while(((I2C_I2SR & 0x2)==0) && (++cnt<I2C_TIMEOUT));
	}
	
	I2C_I2CR=0x80;	//send stop
	for(i=0;i<10000;i++);
}

void SendCh(UCHAR uc)
{
	while((UART1_USR2 & 0x4000)==0);
	UART1_UTXD=uc;
			
}

void SendSerial(UCHAR *buf, int len)
{
	int i;
	SendCh(FEND);
	for(i=0;i<len;i++)
	{
		switch(buf[i])
		{
		case FEND:
            SendCh(FESC);
			SendCh(TFEND);
			break;
		case FESC:
            SendCh(FESC);
			SendCh(TFESC);
			break;
		default:
			SendCh(buf[i]);
		}
	}
	SendCh(FEND);
}

int main ()
{
	volatile UINT i;
	UCHAR cmd,address,value;
	UCHAR *pMem;
	
	//---------------------------------------------------------------------------------------------------------------------------------------
	//START OF HAB TOOLKIT SECTION 
	//THIS SECTION CAN BE REMOVED IF RUNNING PROGRAMS  USING JTAG OR UMON
	
	//set PLL frequency to 266MHz
	CRM_CSCR=0x1f18060f;
	CRM_MPCTL0=0x007b1c73;
	CRM_CSCR=0x1f38060f;	//MPLL restart
	while((CRM_MPCTL1 & 0x8000)==0);	//wait until PLL locked

	//enable instruction cache
	asm("   MRC p15, 0, r0, c1, c0, 0");
    asm("   ORR r0, r0, #0x1000"); //bit 12 is ICACHE enable
    asm("   MCR p15, 0, r0, c1, c0, 0");
    // Flush instruction cache 
    asm("   MCR p15, 0, r0, c7, c5, 0");
	
	//END OF HAB TOOLKIT SECTION
	//---------------------------------------------------------------------------------------------------------------------------------------
	
	CRM_PCDR1=0x05070705;	//PERCLK clock divider settings (44.33MHz)
	CRM_PCCR0=0xc0403801;	//enable CSI, DMA, UART1, I2C, GPIO clk
		
	GPIOB_GIUS=0xff0003f0;	//enable CSI IO
	GPIOD_GIUS=0xfff80000;	//enable I2C IO
		
	//init UART1
	GPIOE_DDIR=0x1000;
	UART1_UCR1=0x0001;
	UART1_UCR2=0x4027;
	UART1_UCR3=0x0004;
	UART1_UCR4=0x0000;
	UART1_UFCR=0x0a80;
	UART1_UBIR=2702;
	UART1_UBMR=64999;
	
	//init I2C
	I2C_IFDR=0x15;
	I2C_I2CR=0x80;	//do a complete reset of I2C
	I2C_I2CR=0x00;
	I2C_I2CR=0x80;
	
	
	while(1)
	{
		//wait for commands from PC
		UINT SerMode=SER_NULL;
		UINT SerialFlag=0;
		UINT SerialBufLen=0;
		UCHAR SerialBuf[BUFFER_LEN];
		while(1)
		{
			while((UART1_USR2 & 0x0001)==0);
			
			UCHAR uc=UART1_URXD & 0xff;
		
			switch(SerMode)
			{
			case SER_NULL:
				switch(uc)
				{
				case FEND:
					if(SerialBufLen>0)
						SerialFlag=1;
					break;
				case FESC:
					SerMode=SER_FESC;
					break;
				default:
					SerialBuf[SerialBufLen++]=uc;
					break;
				}
				break;
			case SER_FESC:
				SerMode=SER_NULL;
				switch(uc)
				{
				case TFESC:
					SerialBuf[SerialBufLen++]=FESC;
					break;
				case TFEND:
					SerialBuf[SerialBufLen++]=FEND;
					break;
				default:
					SerMode=SER_ERR;
					break;
				}
				break;
			case SER_ERR:
				if(uc==FEND)
				{
					SerialBufLen=0;
					SerMode=SER_NULL;
				}
				break;
			}
			if(SerialFlag)
				break;
		}
		
		//process commands
		switch(SerialBuf[0])
		{
		case CMD_LED:
		case CMD_SERVO:
		case CMD_MOTOR_DUTY:
		case CMD_MOTOR_EN:
			I2C_write(VC21RB1_ADDRESS, SerialBuf, SerialBufLen);
			break;
		case CMD_MOTOR_FAULT:
			I2C_read(VC21RB1_ADDRESS,SerialBuf[0],SerialBuf,1);
			SendSerial(SerialBuf,1);
			break;
		case CMD_ADC:
			I2C_read(VC21RB1_ADDRESS,SerialBuf[0],SerialBuf,4);
			SendSerial(SerialBuf,4);
			break;
		}
		
	}
	return 0;
}

