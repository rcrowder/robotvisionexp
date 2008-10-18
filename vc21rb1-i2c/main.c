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

//to compile for RAM execution (rather than FLASH), follow comments after RAM comments and adjust ldscript file
#include "../LPC2136.h"

#define CMD_LED			4
#define CMD_MOTOR_EN	20
#define CMD_MOTOR_DUTY	21
#define CMD_MOTOR_FAULT	23
#define CMD_SERVO		11
#define CMD_ADC			30

#define BUFFER_LEN 100

#define TIMEOUT 1000000

#define MOTOR_OFF 0x80
#define MOTOR_UPDATE_RATE 10000

void IRQ_Routine (void)   __attribute__ ((interrupt("IRQ")));
void FIQ_Routine (void)   __attribute__ ((interrupt("FIQ")));
void SWI_Routine (void)   __attribute__ ((interrupt("SWI")));
void UNDEF_Routine (void) __attribute__ ((interrupt("UNDEF")));

extern int SerialFlag;
extern UINT SerialBufLen;
extern UCHAR SerialBuf[BUFFER_LEN];

void InterruptInit()
{
	//RAM
	//uncoment following line when running from RAM (moves exception vector table to RAM)
	//MEMMAP=0x2;
	
}

void LEDInit()
{
	IO1DIR|=0x00100000;

}


int main()
{
	UCHAR buf[20];
	int len;
	int RxLen;
	int TimeoutCnt;
	char Motor1Speed,Motor1Desired;
	char Motor2Speed,Motor2Desired;
	int MotorFlag;
	int MotorCnt;
	
	PINSEL0=0x00008000;
	PINSEL1=0x00000400;
	
	
	//set internal PLL to 60MHz
	PLLCFG=0x24;

	PLLCON=0x1;
	PLLFEED=0xAA;
	PLLFEED=0x55;

	while(!(PLLSTAT & 0x400));

	PLLCON=0x3;
	PLLFEED=0xAA;
	PLLFEED=0x55;

	//set pclk to 60MHz
	VPBDIV=0x01;
	
	//RAM
	//comment the following 2 lines when running from RAM
	//set up memory accelerator
	MAMTIM=0x04;
	MAMCR=0x2;
	
	InterruptInit();
	I2Cinit();
	MotorInit();
	LEDInit();
	ServoInit();
	ADCInit();
	
	//enable interrupts
	asm("MRS r1, CPSR;"
		"BIC r1, r1, #0x80;"
		"MSR CPSR_c, r1;");
		
	TimeoutCnt=0;
	Motor1Desired=Motor1Speed=MOTOR_OFF;
	Motor2Desired=Motor2Speed=MOTOR_OFF;
	MotorCnt=0;
	while(1)
	{
		if(SerialFlag)
		{
			SerialFlag=0;
			TimeoutCnt=0;
			RxLen=SerialBufLen;
			SerialBufLen=0;

			switch(SerialBuf[0])
			{
			case CMD_LED:
				if(SerialBuf[1])
					IO1CLR=0x00100000;
				else
					IO1SET=0x00100000;
				break;
			case CMD_MOTOR_EN:
				MotorEnable(SerialBuf[1]);
				break;
			case CMD_MOTOR_DUTY:
				Motor1Desired=SerialBuf[1];
				Motor2Desired=SerialBuf[2];
				//MotorDuty(SerialBuf[1],SerialBuf[2]);
				break;
			case CMD_MOTOR_FAULT:
				buf[0]=MotorFault();
				I2CSend(buf,1);
				break;
			case CMD_SERVO:
				SetServo(SerialBuf[1],SerialBuf[2]);
				break;
			case CMD_ADC:
				len=ADCFillValues(buf);
				I2CSend(buf,len);
				break;
			}
			
		}
		
		//slowly ramp up speed - could be placed in a timer interrupt
		MotorCnt++;
		if(MotorCnt==MOTOR_UPDATE_RATE)
		{
			MotorCnt=0;
			MotorFlag=0;
			if(Motor1Desired<Motor1Speed)
			{
				Motor1Speed--;
				MotorFlag=1;
			}
			if(Motor1Desired>Motor1Speed)
			{	
				Motor1Speed++;
				MotorFlag=1;
			}
			if(Motor2Desired<Motor2Speed)
			{
				Motor2Speed--;
				MotorFlag=1;
			}
			if(Motor2Desired>Motor2Speed)
			{	
				Motor2Speed++;
				MotorFlag=1;
			}
			if(MotorFlag)
				MotorDuty(Motor1Speed,Motor2Speed);
		}
		
		//ensure the VCMX212 still transmitting messages, otherwise shut down motors
		if(TimeoutCnt==TIMEOUT)
		{
			MotorDuty(MOTOR_OFF,MOTOR_OFF);
			Motor1Desired=Motor1Speed=MOTOR_OFF;
			Motor2Desired=Motor2Speed=MOTOR_OFF;
			TimeoutCnt++;
		}
		else if(TimeoutCnt<TIMEOUT)
		{
			TimeoutCnt++;
		}
		
	}

	return 0;
}

/*  Stubs for various interrupts (may be replaced later)  */
/*  ----------------------------------------------------  */

void IRQ_Routine (void) {
	while (1) ;	
}

void FIQ_Routine (void)  {
	while (1) ;	
}


void SWI_Routine (void)  {
	while (1) ;	
}


void UNDEF_Routine (void) {
	while (1) ;	
}
