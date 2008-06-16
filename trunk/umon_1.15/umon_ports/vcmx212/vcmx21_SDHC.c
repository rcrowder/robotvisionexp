#include "MX21regs.h"
#include "vcmx21_GPIO.h"

// http://www.freescale.com/files/32bit/doc/app_note/AN2906.pdf?fpsp=1

#define	ALLOW_PERCLK2_GATING

#ifndef BOOL
	#define	BOOL		UINT
	#define	OFF			0
	#define	ON			1
#endif

#define	IO_STR_2MA	0x0 //000b
#define	IO_STR_4MA	0x1 //001b
#define	IO_STR_8MA	0x3 //011b
#define	IO_STR_12MA	0x7 //111b
#define	MASK_3_BIT	0x7 //111b

static const USHORT		scSDHC_Option_DMAEnable			= (1<<0);
static const USHORT		scSDHC_Option_AllowClkGating	= (1<<1);
static const USHORT		scSDHC_Option_SDHCEnableCheck	= (1<<2);
static const USHORT		scSDHC_Option_CardDetectCheck	= (1<<3);
static const USHORT		scSDHC_Option_ClockSettingCheck	= (1<<4);

static const USHORT		scSDHC_Flag_CardType_SD			= (1<<0);
static const USHORT		scSDHC_Flag_CardType_SDHC		= (1<<1);
static const USHORT		scSDHC_Flag_CardType_MMC		= (1<<2);
static const USHORT		scSDHC_Flag_CardType_SDIO		= (1<<3);
static const USHORT		scSDHC_Flag_CardType_Combo		= (1<<4);
static const USHORT		scSDHC_Flag_4BitBusEnable		= (1<<5);

typedef struct _TSDHCInfo
{
	UCHAR	uiOptions;
	UCHAR	uiFlags;
	USHORT	usRCA;
} TSDHCInfo;
static TSDHCInfo	gs_SDHCInfo;


static void SDHC_Enable(UINT bOn)
{
	if (bOn)
		// Enable SDHC-2
		CRM_PCCR0 |=  0x00000400;
	else
		// Disable SDHC-2 (power saving, clock gating)
		CRM_PCCR0 &= ~0x00000400;
}

static int SDHC_Port_Init(void)
{
	BOOL bCardDetected = 1;

	if ((gs_SDHCInfo.uiOptions&scSDHC_Option_CardDetectCheck) != 0)
	{
		// Card detect GPIO based switch
		GPIO_Mode(25 | GPIO_PORTA | GPIO_GPIO | GPIO_IN);       // Card Detect
		if (!(SSR(0) & (1 << 25)))
			bCardDetected = 0;
		else
			bCardDetected = 1;
	}

	// Setup GPIO lines
	GPIO_Mode(PB4_PF_SD_DAT0);
	GPIO_Mode(PB5_PF_SD_DAT1);
	GPIO_Mode(PB6_PF_SD_DAT2);
	/* Configured as GPIO with pull-up to ensure right MCC card mode */
	GPIO_Mode(  7 | GPIO_PORTB | GPIO_IN | GPIO_PUEN | GPIO_GPIO);
	GPIO_Mode(PB9_PF_SD_CLK);
	GPIO_Mode(PB8_PF_SD_CMD);	

	//set the I/O driving strength of the GPIO pad
	//set the DS_SLOW7 field (bit 21:19) in DSCR1 register. this field is exactly for Pin 254 to Pin 259
	SYS_DSCR1 = (SYS_DSCR1 & ~(MASK_3_BIT << 19)) | (IO_STR_2MA << 19);		// 2mA
//	SYS_DSCR1 = (SYS_DSCR1 & ~(MASK_3_BIT << 19)) | (IO_STR_4MA << 19);		// 4mA
//	SYS_DSCR1 = (SYS_DSCR1 & ~(MASK_3_BIT << 19)) | (IO_STR_8MA << 19);		// 8mA
//	SYS_DSCR1 = (SYS_DSCR1 & ~(MASK_3_BIT << 19)) | (IO_STR_12MA << 19);	//12mA

	//enable interrupt source 12
	AITC_INTENNUM = 12;

	//make sure CSPI modules hasn't enabled SDHC SPI mode
	CSPI1_CONTROLREG1 &= ~0x00400000;	//0x1000E008
	CSPI2_CONTROLREG1 &= ~0x00400000;	//0x1000F008
//	CSPI3_CONTROLREG1 &= ~0x00400000;	//0x10017008

	if ((gs_SDHCInfo.uiOptions&scSDHC_Option_AllowClkGating) != 0)
	{
		//check to see if CSPI modules are enabled
		if ((CRM_PCCR0&0x00000030) != 0)
		{
			// Not sure if this is needed??
			printf("Warning: CSPI module(s) is enabled and PERCLK2 gating is active\n");

			// Continuing past this point..
			// BEWARE: That changes to PERDIV2 for use with CSPI, could affect the 
			//         SDHC module and it's reliance on certain clock frequency.
			//         SDHC_SetClockRate REWRITES CRM_PCDR1 register, to get a PERDIV2 set to 8 (ie 7+1)
			//         If you require a different PERDIV2 setting, check the comments in 
			//         SDHC_SetClockRate to determine what changes a required in this code 
			//         to obtain the two clock frequencies used by the SDHC.
			//return 0;
		}
	}
	
	//check valid AIPI PSR settings
	if (!((AIPI1_PSR1&0x00100000) == 0x00100000 && (AIPI1_PSR0&0x00100000) == 0))
	{
		AIPI1_PSR1 |=  0x00100000;
		AIPI1_PSR0 &= ~0x00100000;
	}

	if (!bCardDetected)
		printf("SDHC_Port_Init: No card detected\n");

	return bCardDetected;
}

static int SDHC_SetClockRate(BOOL bForInit)
{
	USHORT usPrescaler, usDivider;

	// Check clock assumptions
	if ((gs_SDHCInfo.uiOptions&scSDHC_Option_ClockSettingCheck) != 0)
	{
		// FPM and MPLL enabled?
		if ((CRM_CSCR&0x00000005) == 0)
		{
			printf("SDHC_SetClockRate: CRM_CSCR settings not supported!\n");
			if ((gs_SDHCInfo.uiOptions&scSDHC_Option_AllowClkGating) != 0)
				SDHC_Enable(0);
			return 0;
		}

		// Target freq = 266MHz?
		if (CRM_MPCTL0 != 0x007b1c73)
		{
			printf("SDHC_SetClockRate: CRM_MPCTL0 settings not supported!\n");
			if ((gs_SDHCInfo.uiOptions&scSDHC_Option_AllowClkGating) != 0)
				SDHC_Enable(0);
			return 0;
		}

/*		if ((CRM_PCDR1&0x00003F00) != 0x00000700)
		{
			printf("SDHC_SetClockRate: PERDIV2 different than expected!\n");
			if ((gs_SDHCInfo.uiOptions&scSDHC_Option_AllowClkGating) != 0)
				SDHC_Enable(0);
			return 0;
		}
*/	}

	//
	// FREQref = (32.768kHz * 512 FPM) (VC21 crystal)
	// FREQpll = (2 * FREQref) * ( (MFI + (MFN / (MFD+1))) / (PD + 1) )
	// FREQpll ~= 266MHz

	// Setup PERCLK2
	// PERCLK2 = 266MHz / 8 = 33250kHz
	CRM_PCDR1 &= ~0x00003F00;
	CRM_PCDR1 |=  0x00000700;

	// CLK_DIV = PERCLK2 / CLK_usDivider
	// CLK_20M = CLK_DIV / CLK_usPrescaler
	// MMCCLK  = CLK_20M

	if (bForInit)
	{
		// Identification Mode (MMCCLK ~200kHz, Max is 400kHz)
		// CLK_DIV = 33250kHz / 10 = 3325kHz	(1001b)
		// CLK_20M =  3325kHz / 16 =  207kHz	(008h)
		usDivider	= 9;
		usPrescaler	= 8;
	}
	else
	{
		// Data Transfer Mode  (MMCCLK ~16000kHz, Max is 20MHz)
		// CLK_DIV = 33250kHz /  2 = 16625kHz	(0001b)
		// CLK_20M = 16625kHz /  1 = 16625kHz	(000h)
		usDivider	= 1;
		usPrescaler	= 0;
	}
	SDHC2_CLK_RATE = (UINT)(usPrescaler<<4) | (UINT)usDivider;

	return 1;
}

#define CMD_ERROR_NONE		0
#define	CMD_ERROR_TIMEOUT	1
#define	CMD_ERROR_CRC		2

static int SDHC_SendCommand(UCHAR ucCommand, USHORT usArgH, USHORT usArgL, USHORT usCmdDataControl, USHORT* pResponse)
{
	int bRet = CMD_ERROR_NONE;
	UINT uiStatus;// = SDHC2_STATUS;

	SDHC2_CMD			= ucCommand;
	SDHC2_ARGH			= usArgH;
	SDHC2_ARGL			= usArgL;
	SDHC2_CMD_DAT_CONT	= usCmdDataControl;

	SDHC2_STR_STP_CLK	= 0x00000002;
	do
	{
		uiStatus = SDHC2_STATUS;
	}
	while((uiStatus&0x0100) == 0); // Wait till clock is start, command submit now.

	do
	{
		uiStatus = SDHC2_STATUS;
	}
	while((uiStatus&0x2000) == 0); // Wait interrupt generated (End Command Response)

	if ((uiStatus&0x0002) != 0)	// Response Timeout?
	{
		bRet = CMD_ERROR_TIMEOUT;
	}
	if ((uiStatus&0x0020) != 0)	// Response CRC Error?
	{
		bRet = CMD_ERROR_CRC;
	}

	SDHC2_STR_STP_CLK	= 0x00000001; // Stop the clock
	do
	{
		uiStatus = SDHC2_STATUS;
	}
	while((uiStatus&0x0100) != 0); // Wait till clock is stop, command-response end.

	// Get response
	int i;
	switch (usCmdDataControl&0x0007)
	{
	default:
	case 0:		// No response
		break;

	case 1:		// Format R1/R1b/R5/R6 (48bit response with CRC7 bits)
	case 3:		// Format R3/R4 (48bit response without CRC bits)
	case 6:		// Format R6 (48bit published RCA response)
	case 7:		// Format R7 (48bit card interface condition)
		for (i = 0; i < 3; i++)
			pResponse[i] = (USHORT)(SDHC2_RES_FIFO&0xffff);
		break;

	case 2:		// Format R2 (136bit response, CSD/CID register read response)
		for (i = 0; i < 8; i++)
			pResponse[i] = (USHORT)(SDHC2_RES_FIFO&0xffff);
		break;
	}

	return bRet;
}

int SDHC_Init(int iVerbose)
{
	int		bRet = 0, bCardFound = 0;
	UINT	i, Count = 0;
	USHORT	usResponse[8] = { 0,0,0,0,0,0,0,0 };

	gs_SDHCInfo.uiOptions	= 0;
	gs_SDHCInfo.uiFlags		= 0;
	gs_SDHCInfo.usRCA		= 0;

	// Setup some options
	gs_SDHCInfo.uiOptions |= scSDHC_Option_AllowClkGating;
	gs_SDHCInfo.uiOptions |= scSDHC_Option_ClockSettingCheck;

	SDHC_Enable(1);

	// Setup the SDHC2
	if (!SDHC_Port_Init())
	{
		if ((gs_SDHCInfo.uiOptions&scSDHC_Option_AllowClkGating) != 0)
			SDHC_Enable(0);

		return 0;
	}

	// Card Identification Mode....................................................................
	//
	//if (iVerbose)
		printf("SDHC_Init: Card Identification Mode\n");

	// Set up the clock for identification mode (MMCCLK @ ~200kHz)
	if (!SDHC_SetClockRate(1))
		return 0;

	// SD Software reset
	SDHC2_STR_STP_CLK = 0x00000008;
	SDHC2_STR_STP_CLK = 0x00000009;
	for (i = 0; i < 8; i++)
		SDHC2_STR_STP_CLK = 0x00000001;

	// Setup defaults
	SDHC2_RESPONSE_TO	= 0xff;
	SDHC2_READ_TO		= 0x2DB4;
	SDHC2_BLK_LEN		= 512;
	SDHC2_NOB			= 1;

	// Send CMD0 (GO_IDLE_STATE)
	SDHC_SendCommand(0, 0x0000, 0x0000, 0x0080, usResponse);

	// Send CMD8 (SEND_IF_COND)
	if (SDHC_SendCommand(8, 0x0000, 0x01AA, 0x0007, usResponse) == CMD_ERROR_NONE)
	{
		// Cards might respond to CMD8, but not support SDC Ver2.0+. So..

		// Valid voltage range and echo-back check pattern?
		if ((usResponse[1]&0x000f) == 0x0001 && (usResponse[2]&0xAA00) == 0xAA00)	// Is this check ok??
		{
			Count = 1000;
			do
			{
				// Set voltage for card (with HCS = 1)
				bRet = SDHC_SendCommand(55, 0x0000, 0x0000, 0x0001, usResponse);
				bRet = SDHC_SendCommand(41, 0x40ff, 0x8000, 0x0003, usResponse);
				Count--;
			}
			while (Count > 0 && (usResponse[0]&0x0080) == 0);

			if (Count == 0)
			{
				printf("SDHC_Init: No response from card (ACMD41)\n");
				if ((gs_SDHCInfo.uiOptions&scSDHC_Option_AllowClkGating) != 0)
					SDHC_Enable(0);
				return 0;
			}

			bCardFound = 1;
			//if (iVerbose)
			{
				if (usResponse[0]&0x0040)
				{
					printf( "SDHC_Init: Ver2.00+ High Capacity SD Memory Card detected\n");
					gs_SDHCInfo.uiOptions |= scSDHC_Flag_CardType_SDHC;
				}
				else
				{
					printf( "SDHC_Init: Ver2.00+ Standard Capacity SD Memory Card detected\n");
					gs_SDHCInfo.uiOptions |= scSDHC_Flag_CardType_SD;
				}
			}
		}
	}

	if (!bCardFound)
	{
		// Send ACMD41 (SD_SEND_OP_COND) - Query voltage supported from card
		bRet = SDHC_SendCommand(55, 0x0000, 0x0000, 0x0001, usResponse);
		bRet = SDHC_SendCommand(41, 0x0000, 0x0000, 0x0003, usResponse);

		// MMC?
		if (bRet == CMD_ERROR_TIMEOUT)
		{
			// MMC
			Count = 1000;
			do
			{
				// Set voltage for card
				bRet = SDHC_SendCommand(1, 0x00ff, 0x8000, 0x0003, usResponse);
				Count--;
			}
			while (Count > 0 && (usResponse[0]&0x0080) == 0);
			if (Count == 0)
			{
				printf("SDHC_Init: No response from card (CMD1)\n");
				if ((gs_SDHCInfo.uiOptions&scSDHC_Option_AllowClkGating) != 0)
					SDHC_Enable(0);
				return 0;
			}
			gs_SDHCInfo.uiOptions |= scSDHC_Flag_CardType_MMC;
			//if (iVerbose)
				printf( "SDHC_Init: MMC Memory Card detected\n");
		}
		else
		{
			// SD
			Count = 1000;
			do
			{
				// Set voltage for card
				bRet = SDHC_SendCommand(55, 0x0000, 0x0000, 0x0001, usResponse);
				bRet = SDHC_SendCommand(41, 0x00ff, 0x8000, 0x0003, usResponse);
				Count--;
			}
			while (Count > 0 && (usResponse[0]&0x0080) == 0);
			if (Count == 0)
			{
				printf("SDHC_Init: No response from card (ACMD41)\n");
				if ((gs_SDHCInfo.uiOptions&scSDHC_Option_AllowClkGating) != 0)
					SDHC_Enable(0);
				return 0;
			}
			gs_SDHCInfo.uiOptions |= scSDHC_Flag_CardType_SD;
			//if (iVerbose)
				printf( "SDHC_Init: Ver1.X Standard Capacity SD Memory Card detected\n");
		}

		bCardFound = 1;
	}

	if (!bCardFound)
	{
		printf("SDHC_Init: No card detected\n");
		if ((gs_SDHCInfo.uiOptions&scSDHC_Option_AllowClkGating) != 0)
			SDHC_Enable(0);
		return 0;
	}

	// Send CMD2 (ALL_SEND_CID)
	Count = 1000;
	do
	{
		bRet = SDHC_SendCommand(2, 0x0000, 0x0000, 0x0002, usResponse);
		Count--;
	}
	while (Count > 0 && bRet != CMD_ERROR_NONE);
	if (Count == 0)
	{
		printf("SDHC_Init: No response from card (CMD2)\n");
		if ((gs_SDHCInfo.uiOptions&scSDHC_Option_AllowClkGating) != 0)
			SDHC_Enable(0);
		return 0;
	}

	if (iVerbose)
	{
		printf("Manufacturer ID:	%02d\n",		(usResponse[0]&0xff00)>>8);
		printf("OEM/Application ID:	%c%c\n",		(usResponse[0]&0x00ff), (usResponse[1]&0xff00)>>8);
		printf("Product name:		%c%c%c%c%c\n",	(usResponse[1]&0x00ff), (usResponse[2]&0xff00)>>8, (usResponse[2]&0x00ff), (usResponse[3]&0xff00)>>8, (usResponse[3]&0x00ff));
		printf("Product revision:	%d.%d\n",		(usResponse[4]&0xf000)>>12, (usResponse[4]&0x0f00)>>8);
		printf("Product serial num:	%d\n",			((UINT)(usResponse[4]&0x00ff))<<24 | ((UINT)usResponse[5])<<8 | (usResponse[6]&0xff00)>>8);
		printf("Manufacturing date:	%d/%d\n",		(usResponse[7]&0x0f00)>>8, 2000+((usResponse[6]&0x00ff)<<8 | (usResponse[7]&0xf000)>>12));
	}

	// Send CMD3 (SEND_RELATIVE_ADDR)
	Count = 1000;
	do
	{
		bRet = SDHC_SendCommand(3, 0x0000, 0x0000, 0x0006, usResponse);
		Count--;
	}
	while (Count > 0 && bRet != CMD_ERROR_NONE);
	if (Count == 0)
	{
		printf("SDHC_Init: No response from card (CMD3)\n");
		if ((gs_SDHCInfo.uiOptions&scSDHC_Option_AllowClkGating) != 0)
			SDHC_Enable(0);
		return 0;
	}

	gs_SDHCInfo.usRCA = (usResponse[0]&0x00ff)<<8 | (usResponse[1]&0xff00)>>8;
	if (iVerbose)
		printf("RCA: 0x%04X\n",	gs_SDHCInfo.usRCA);


	// Data Transfer Mode..........................................................................
	//
	//if (iVerbose)
		printf("SDHC_Init: Data Transfer Mode\n");

	if (!SDHC_SetClockRate(0))
		return 0;

	// Send CMD9 (SEND_CSD)
	Count = 1000;
	do
	{
		bRet = SDHC_SendCommand(9, gs_SDHCInfo.usRCA, 0x0000, 0x0002, usResponse);
		Count--;
	}
	while (Count > 0 && bRet != CMD_ERROR_NONE);
	if (Count == 0)
	{
		printf("SDHC_Init: No response from card (CMD9)\n");
		if ((gs_SDHCInfo.uiOptions&scSDHC_Option_AllowClkGating) != 0)
			SDHC_Enable(0);
		return 0;
	}

	// NOT CSD Version 1.0 or 2.0?
	if ((usResponse[0]&0xc000)>>14 > 1)
	{
		printf("SDHC_Init: Invalid CSD version (Ver %d.0)\n", (usResponse[0]&0xc000)>>14);
		if ((gs_SDHCInfo.uiOptions&scSDHC_Option_AllowClkGating) != 0)
			SDHC_Enable(0);
		return 0;
	}

	UINT C_SIZE = 0;
	UINT C_SIZE_MULT = 0;
	UINT READ_BL_LEN = 0;
	UINT WRITE_BL_LEN = 0;
	UINT uiCardSize = 0;

	// CSD Version 1.0?
	if ((usResponse[0]&0xc000)>>14 == 0)
	{
		C_SIZE			= (usResponse[3]&0x03ff)<<2 | (usResponse[4]&0xC000)>>14;
		C_SIZE_MULT		= 0;
		READ_BL_LEN		= 0;
		WRITE_BL_LEN	= 0;

		switch (usResponse[2]&0x000f)
		{
		case 9:		READ_BL_LEN = 512;  	break;
		case 10:	READ_BL_LEN = 1024;		break;
		case 11:	READ_BL_LEN = 2048;		break;
		}
		switch ((usResponse[6]&0x07c0)>>6)
		{
		case 9:		WRITE_BL_LEN = 512;  	break;
		case 10:	WRITE_BL_LEN = 1024;	break;
		case 11:	WRITE_BL_LEN = 2048;	break;
		}
		switch ((usResponse[4]&0x0003)<<1 | (usResponse[5]&0x8000)>>15)
		{
		case 0:		C_SIZE_MULT	= 4;		break;
		case 1:		C_SIZE_MULT	= 8;		break;
		case 2:		C_SIZE_MULT	= 16;		break;
		case 3:		C_SIZE_MULT	= 32;		break;
		case 4:		C_SIZE_MULT	= 64;		break;
		case 5:		C_SIZE_MULT	= 128;		break;
		case 6:		C_SIZE_MULT	= 256;		break;
		case 7:		C_SIZE_MULT	= 512;		break;
		}

		uiCardSize = (C_SIZE+1) * C_SIZE_MULT * READ_BL_LEN;
	}
	else
	// CSD Version 2.0?
	if ((usResponse[0]&0xc000)>>14 == 1)
	{
		C_SIZE			= ((UINT)(usResponse[3]&0x003f))<<16 | usResponse[4];
		C_SIZE_MULT		= 1;
		READ_BL_LEN		= 512;
		WRITE_BL_LEN	= 512;

		uiCardSize = (C_SIZE+1) * READ_BL_LEN * 1024;
	}
	mon_printf("SDHC_Init: Card capacity %dMB (%d bytes)\n", (uiCardSize/1024/1024), uiCardSize);

/*	if (iVerbose)
	{
		// CSD Version 1.0?
		if ((usResponse[0]&0xc000)>>14 == 0)
		{
			mon_printf("CSD_STRUCTURE:		%d\n", (usResponse[0]&0xc000)>>14);
			mon_printf("TAAC:				%d\n", (usResponse[0]&0x00ff));
			mon_printf("NSAC:				%d\n", (usResponse[1]&0xff00)>>8);
			mon_printf("TRAN_SPEED:			%dMHz\n", (usResponse[1]&0x00ff)==0x32 ? 25 : 50);
			mon_printf("CCC:				%d\n", (usResponse[2]&0xfff0)>>4);
			mon_printf("READ_BL_LEN:		%d bytes (%d)\n", READ_BL_LEN, usResponse[2]&0x000f);
			mon_printf("READ_BL_PARTIAL:	%s\n", (usResponse[3]&0x8000)>>15 ? "YES" : "NO");
			mon_printf("WRITE_BLK_MISALIGN:	%s\n", (usResponse[3]&0x4000)>>14 ? "YES" : "NO");
			mon_printf("READ_BLK_MISALIGN:	%s\n", (usResponse[3]&0x2000)>>13 ? "YES" : "NO");
			mon_printf("DSR_IMP:			%s\n", (usResponse[3]&0x1000)>>12 ? "YES" : "NO");
			mon_printf("C_SIZE:				%d\n", (usResponse[3]&0x03ff)<<2 | (usResponse[4]&0xC000)>>14);
			mon_printf("VDD_R_CURR_MIN:		%d\n", (usResponse[4]&0x3800)>>11);
			mon_printf("VDD_R_CURR_MAX:		%d\n", (usResponse[4]&0x0700)>>8);
			mon_printf("VDD_W_CURR_MIN:		%d\n", (usResponse[4]&0x00e0)>>5);
			mon_printf("VDD_W_CURR_MAX:		%d\n", (usResponse[4]&0x001c)>>2);
			mon_printf("C_SIZE_MULT:		%d\n", (usResponse[4]&0x0003)<<1 | (usResponse[5]&0x8000)>>15);
			mon_printf("ERASE_BLK_EN:		%d\n", (usResponse[5]&0x4000)>>14);
			mon_printf("SECTOR_SIZE:		%d\n", (usResponse[5]&0x3f80)>>7);
			mon_printf("WP_GRP_SIZE:		%d\n", (usResponse[5]&0x007f));
			mon_printf("WP_GRP_ENABLE:		%d\n", (usResponse[6]&0x8000)>>15);
			mon_printf("R2W_FACTOR:			%d\n", (usResponse[6]&0x1c00)>>10);
			mon_printf("WRITE_BL_LEN:		%d bytes (%d)\n", WRITE_BL_LEN, (usResponse[6]&0x07c0)>>6);
			mon_printf("WRITE_BL_PARTIAL:	%d\n", (usResponse[6]&0x0020)>>5);
			mon_printf("FILE_FORMAT_GRP:	%d\n", (usResponse[7]&0x8000)>>15);
			mon_printf("COPY:				%d\n", (usResponse[7]&0x4000)>>14);
			mon_printf("PERM_WRITE_PROTECT:	%d\n", (usResponse[7]&0x2000)>>13);
			mon_printf("TMP_WRITE_PROTECT:	%d\n", (usResponse[7]&0x1000)>>12);
			mon_printf("FILE_FORMAT:		%d\n", (usResponse[7]&0x0c00)>>10);
			mon_printf("\n");
		}
		else
		// CSD Version 2.0?
		if ((usResponse[0]&0xc000)>>14 == 1)
		{
			mon_printf("CSD_STRUCTURE:		%d\n", (usResponse[0]&0xc000)>>14);
			mon_printf("TAAC:				%d\n", (usResponse[0]&0x00ff));
			mon_printf("NSAC:				%d\n", (usResponse[1]&0xff00)>>8);
			mon_printf("TRAN_SPEED:			%dMHz\n", (usResponse[1]&0x00ff)==0x32 ? 25 : 50);
			mon_printf("CCC:				%d\n", (usResponse[2]&0xfff0)>>4);
			mon_printf("READ_BL_LEN:		%d bytes (%d)\n", READ_BL_LEN, usResponse[2]&0x000f);
			mon_printf("READ_BL_PARTIAL:	%s\n", (usResponse[3]&0x8000)>>15 ? "YES" : "NO");
			mon_printf("WRITE_BLK_MISALIGN:	%s\n", (usResponse[3]&0x4000)>>14 ? "YES" : "NO");
			mon_printf("READ_BLK_MISALIGN:	%s\n", (usResponse[3]&0x2000)>>13 ? "YES" : "NO");
			mon_printf("DSR_IMP:			%s\n", (usResponse[3]&0x1000)>>12 ? "YES" : "NO");
			mon_printf("C_SIZE:				%d\n", ((UINT)(usResponse[3]&0x003f))<<16 | usResponse[4]);
			mon_printf("ERASE_BLK_EN:		%d\n", (usResponse[5]&0x4000)>>14);
			mon_printf("SECTOR_SIZE:		%d\n", (usResponse[5]&0x3f80)>>7);
			mon_printf("WP_GRP_SIZE:		%d\n", (usResponse[5]&0x007f));
			mon_printf("WP_GRP_ENABLE:		%d\n", (usResponse[6]&0x8000)>>15);
			mon_printf("R2W_FACTOR:			%d\n", (usResponse[6]&0x1c00)>>10);
			mon_printf("WRITE_BL_LEN:		%d bytes (%d)\n", WRITE_BL_LEN, (usResponse[6]&0x07c0)>>6);
			mon_printf("WRITE_BL_PARTIAL:	%d\n", (usResponse[6]&0x0020)>>5);
			mon_printf("FILE_FORMAT_GRP:	%d\n", (usResponse[7]&0x8000)>>15);
			mon_printf("COPY:				%d\n", (usResponse[7]&0x4000)>>14);
			mon_printf("PERM_WRITE_PROTECT:	%d\n", (usResponse[7]&0x2000)>>13);
			mon_printf("TMP_WRITE_PROTECT:	%d\n", (usResponse[7]&0x1000)>>12);
			mon_printf("FILE_FORMAT:		%d\n", (usResponse[7]&0x0c00)>>10);
			mon_printf("\n");
		}
	}
*/
	if ((gs_SDHCInfo.uiOptions&scSDHC_Option_AllowClkGating) != 0)
		SDHC_Enable(0);

	return 1;
}

int SDHC_Read(char *pBuffer, int iBlockNum, int iBlockCnt, int iVerbose)
{
	int bRet = 0, Count;
	USHORT	usResponse[8] = { 0,0,0,0,0,0,0,0 };

	if ((gs_SDHCInfo.uiOptions&scSDHC_Option_AllowClkGating) != 0)
		SDHC_Enable(1);

	// Send CMD13 (SEND_STATUS)
	bRet = SDHC_SendCommand(13, gs_SDHCInfo.usRCA, 0x0000, 0x0001, usResponse);

	Count = 1000;
	while ((bRet != CMD_ERROR_NONE || (usResponse[1]&0x0001) == 0) && Count > 0)
	{
		bRet = SDHC_SendCommand(13, gs_SDHCInfo.usRCA, 0x0000, 0x0001, usResponse);
		Count--;
	}
	if (Count == 0)
	{
		printf("SDHC_Read: No response from card (CMD13)\n");
		if ((gs_SDHCInfo.uiOptions&scSDHC_Option_AllowClkGating) != 0)
			SDHC_Enable(0);
		return 0;
	}

	// Send CMD7 (SELECT_CARD)
	Count = 1000;
	do
	{
		bRet = SDHC_SendCommand(7, gs_SDHCInfo.usRCA, 0x0000, 0x0001, usResponse);
		Count--;
	}
	while (Count > 0 && bRet != CMD_ERROR_NONE);
	if (Count == 0)
	{
		printf("SDHC_Read: No response from card 0x%4X (CMD7)\n", gs_SDHCInfo.usRCA);
		if ((gs_SDHCInfo.uiOptions&scSDHC_Option_AllowClkGating) != 0)
			SDHC_Enable(0);
		return 0;
	}


	// Send CMD16 (SET_BLOCKLEN)
	Count = 1000;
	do
	{
		bRet = SDHC_SendCommand(16, gs_SDHCInfo.usRCA, 0x0200, 0x0001, usResponse);
		Count--;
	}
	while (Count > 0 && bRet != CMD_ERROR_NONE);
	if (Count == 0)
	{
		printf("SDHC_Read: No response from card (CMD16)\n");
		if ((gs_SDHCInfo.uiOptions&scSDHC_Option_AllowClkGating) != 0)
			SDHC_Enable(0);
		return 0;
	}

	if ((gs_SDHCInfo.uiFlags&scSDHC_Flag_4BitBusEnable) != 0)
	{
		GPIO_Mode(PB7_PF_SD_DAT3); 

		// Send ACMD6 (SET_BUS_WIDTH)
		Count = 1000;
		do
		{
			bRet = SDHC_SendCommand(55, gs_SDHCInfo.usRCA, 0x0000, 0x0001, usResponse);
			bRet = SDHC_SendCommand( 6, gs_SDHCInfo.usRCA, 0x0002, 0x0001, usResponse);
			Count--;
		}
		while (Count > 0 && bRet != CMD_ERROR_NONE);
		if (Count == 0)
		{
			printf("SDHC_Read: No response from card (ACMD6)\n");
			if ((gs_SDHCInfo.uiOptions&scSDHC_Option_AllowClkGating) != 0)
				SDHC_Enable(0);
			return 0;
		}
	}

	// Setup number of blocks to transfer
	SDHC2_NOB = 1;//iBlockCnt;


	// Read single block(s)

	UINT i, j, iCurrBlockCnt;
	UINT uiStatus, uiDataBuffer;
	UINT uiAddr, uiTotalRecv;

	if ((gs_SDHCInfo.uiFlags&scSDHC_Flag_CardType_SDHC) != 0)
		uiAddr = iBlockNum;
	else
		uiAddr = iBlockNum * 512;

	for (iCurrBlockCnt = 0; iCurrBlockCnt < iBlockCnt; iCurrBlockCnt++)
	{
		SDHC2_CMD			= 17;
		SDHC2_ARGH			= uiAddr>>16;
		SDHC2_ARGL			= uiAddr&0xffff;
		SDHC2_CMD_DAT_CONT	= 0x0009;

		if ((gs_SDHCInfo.uiFlags&scSDHC_Flag_4BitBusEnable) != 0)
			SDHC2_CMD_DAT_CONT |= 0x0200;

		SDHC2_STR_STP_CLK	= 0x00000002;
		do
		{
			uiStatus = SDHC2_STATUS;
		}
		while((uiStatus&0x0100) == 0); // Wait till clock is start, command submit now.

		uiTotalRecv = 0;

		Count = 1000;
		while (Count > 0)
		{
			uiStatus = SDHC2_STATUS;

			// Continue if not set;
			// Read Operation Done (READ_OP_DONE),
			// Application FIFO Full (APPL_BUFF_FF),
			// CRC Read Error (CRC_READ_ERR),
			// TimeOut Response (TIME_OUT_RESP),
			// TimeOut Read (TIME_OUT_READ)
			if ((uiStatus&0x088B) == 0)
				continue;

			j = 0xFF;
			while(j--);

			Count--;

			if(uiStatus&0x000B)
			{
				printf("SDHC_Read: Read Error (Status = 0x%lx)\n",uiStatus);
				bRet = 0;
				goto StopReadClock;
			}
			if((uiStatus&0x0080)!=0)		// Buffer is FULL
			{
				// 1 bit bus?
				if ((gs_SDHCInfo.uiFlags&scSDHC_Flag_4BitBusEnable) == 0)
				{
					for(i = 0; i < 8; i++)
					{
						uiDataBuffer = SDHC2_BUFFER_ACCESS & 0x0000ffff;
						*pBuffer++ = uiDataBuffer&0xff;
						*pBuffer++ = uiDataBuffer>>8;
					}
					uiTotalRecv += 16;
				}
				else
				// 4 bit bus
				{
					for(i = 0; i < 32; i++)
					{
						uiDataBuffer = SDHC2_BUFFER_ACCESS & 0x0000ffff;
						*pBuffer++ = uiDataBuffer&0xff;
						*pBuffer++ = uiDataBuffer>>8;
						*pBuffer++ = uiDataBuffer&0xff;
						*pBuffer++ = uiDataBuffer>>8;
					}
					uiTotalRecv += 64;
				}

				Count = 1000;
			}
			if ((uiStatus&0x0800) != 0)	// TRANS Done
			{
				bRet = 1;
				goto SingleReadEnd;
			}
		}

SingleReadEnd:
		uiAddr += 512;

		if(!bRet)
		{
			Count = 1000;
			while(Count > 0)
			{
				uiStatus = SDHC2_STATUS;

				if (uiStatus&0x000B)
				{
					printf("SDHC_Read: Read Error (Status = 0x%lx)\n",uiStatus);
					bRet = 0;
					break;
				}
				else
				if((uiStatus&0x0800)!=0)
				{
					// READ_OP_DONE
					bRet = 1;
					goto StopReadClock;
				}
				Count--;
			}
		}

StopReadClock:
		SDHC2_STR_STP_CLK	= 0x00000001; // Stop the clock
		do
		{
			uiStatus = SDHC2_STATUS;
		}
		while((uiStatus&0x0100) != 0); // Wait till clock is stop, command-response end.
	}

	// Send CMD7 (DESELECT_CARD)
	SDHC_SendCommand(7, 0x0000, 0x0000, 0x0001, usResponse);

//	printf("Received %d bytes\n", uiTotalRecv);
	if ((gs_SDHCInfo.uiOptions&scSDHC_Option_AllowClkGating) != 0)
		SDHC_Enable(0);

	return bRet;
}



int SDHC_Write(char *pBuffer, int iBlockNum, int iBlockCnt, int iVerbose)
{
	int bRet = 0, Count;
	USHORT	usResponse[8] = { 0,0,0,0,0,0,0,0 };

	if ((gs_SDHCInfo.uiOptions&scSDHC_Option_AllowClkGating) != 0)
		SDHC_Enable(1);

	// Send CMD13 (SEND_STATUS)
	bRet = SDHC_SendCommand(13, gs_SDHCInfo.usRCA, 0x0000, 0x0001, usResponse);

	Count = 1000;
	while ((bRet != CMD_ERROR_NONE || (usResponse[1]&0x0001) == 0) && Count > 0)
	{
		bRet = SDHC_SendCommand(13, gs_SDHCInfo.usRCA, 0x0000, 0x0001, usResponse);
		Count--;
	}
	if (Count == 0)
	{
		printf("SDHC_Write: No response from card (CMD13)\n");
		if ((gs_SDHCInfo.uiOptions&scSDHC_Option_AllowClkGating) != 0)
			SDHC_Enable(0);
		return 0;
	}

	// Send CMD7 (SELECT_CARD)
	Count = 1000;
	do
	{
		bRet = SDHC_SendCommand(7, gs_SDHCInfo.usRCA, 0x0000, 0x0001, usResponse);
		Count--;
	}
	while (Count > 0 && bRet != CMD_ERROR_NONE);
	if (Count == 0)
	{
		printf("SDHC_Write: No response from card 0x%4X (CMD7)\n", gs_SDHCInfo.usRCA);
		if ((gs_SDHCInfo.uiOptions&scSDHC_Option_AllowClkGating) != 0)
			SDHC_Enable(0);
		return 0;
	}


	// Send CMD16 (SET_BLOCKLEN)
	Count = 1000;
	do
	{
		bRet = SDHC_SendCommand(16, gs_SDHCInfo.usRCA, 0x0200, 0x0001, usResponse);
		Count--;
	}
	while (Count > 0 && bRet != CMD_ERROR_NONE);
	if (Count == 0)
	{
		printf("SDHC_Write: No response from card (CMD16)\n");
		if ((gs_SDHCInfo.uiOptions&scSDHC_Option_AllowClkGating) != 0)
			SDHC_Enable(0);
		return 0;
	}

	if ((gs_SDHCInfo.uiFlags&scSDHC_Flag_4BitBusEnable) != 0)
	{
		GPIO_Mode(PB7_PF_SD_DAT3); 

		// Send ACMD6 (SET_BUS_WIDTH)
		Count = 1000;
		do
		{
			bRet = SDHC_SendCommand(55, gs_SDHCInfo.usRCA, 0x0000, 0x0001, usResponse);
			bRet = SDHC_SendCommand( 6, gs_SDHCInfo.usRCA, 0x0002, 0x0001, usResponse);
			Count--;
		}
		while (Count > 0 && bRet != CMD_ERROR_NONE);
		if (Count == 0)
		{
			printf("SDHC_Write: No response from card (ACMD6)\n");
			if ((gs_SDHCInfo.uiOptions&scSDHC_Option_AllowClkGating) != 0)
				SDHC_Enable(0);
			return 0;
		}
	}

	// Setup number of blocks to transfer
	SDHC2_NOB = 1;//iBlockCnt;


	// Write single block(s)

	UINT i, j, iCurrBlockCnt;
	UINT uiStatus, uiAddr;
	UINT uiTotalSent = 0;
	UINT uiBurstLen = 8;
	USHORT	*pData = (USHORT*)pBuffer;

	if ((gs_SDHCInfo.uiFlags&scSDHC_Flag_CardType_SDHC) != 0)
		uiAddr = iBlockNum;
	else
		uiAddr = iBlockNum * 512;

	// 4 bit bus?
	if ((gs_SDHCInfo.uiFlags&scSDHC_Flag_4BitBusEnable) != 0)
		uiBurstLen = 32;

	for (iCurrBlockCnt = 0; iCurrBlockCnt < iBlockCnt; iCurrBlockCnt++)
	{
		SDHC2_CMD			= 24;
		SDHC2_ARGH			= uiAddr>>16;
		SDHC2_ARGL			= uiAddr&0xffff;
		SDHC2_CMD_DAT_CONT	= 0x0019;

		if ((gs_SDHCInfo.uiFlags&scSDHC_Flag_4BitBusEnable) != 0)
			SDHC2_CMD_DAT_CONT |= 0x0200;

		SDHC2_STR_STP_CLK	= 0x00000002;
		do
		{
			uiStatus = SDHC2_STATUS;
		}
		while((uiStatus&0x0100) == 0); // Wait till clock is start, command submit now.

		uiTotalSent = 0;

		Count = 1000;
		BOOL bWriteDone = 0;
		while (!bWriteDone && Count > 0)
		{
			uiStatus = SDHC2_STATUS;

			// Continue if not set;
			// WRITE_OP_DONE
			// APPL_BUFF_FE
			// TIME_OUT_RESP
			// TIME_OUT_READ
			if ((uiStatus&0x1043) == 0)
				continue;

			j = 0xFF;
			while(j--);

			// Timeouts?
			if (uiStatus&0x0003)
			{
				printf("SDHC_Write: Write Error (Status = 0x%lx)\n",uiStatus);
				bRet = 0;
				goto SingleWriteEnd;
			}
			else
			if ((uiStatus&0x0040) != 0) // Buffer is empty
			{
				for(i = 0; i < uiBurstLen; i++)
				{
					SDHC2_BUFFER_ACCESS = (UINT)(*pData++);
				}
				uiTotalSent += uiBurstLen*2;
				Count = 1000;
			}
			if (uiTotalSent == 512)
			{
				bRet = 1;
				bWriteDone = 1;
			}

			Count--;
		}
		if (Count == 0)
		{
			printf("SDHC_Write: Timeout during data write\n");
			if ((gs_SDHCInfo.uiOptions&scSDHC_Option_AllowClkGating) != 0)
				SDHC_Enable(0);
			return 0;
		}

SingleWriteEnd:
		uiAddr += 512;

		if(!bRet)
		{
			Count = 1000;
			while(Count > 0)
			{
				uiStatus = SDHC2_STATUS;

				if (uiStatus&0x0007)
				{
					printf("SDHC_Write: Write Error (Status = 0x%lx)\n",uiStatus);
					bRet = 0;
					break;
				}
				else
				if((uiStatus&0x1000)!=0)
				{
					// WRITE_OP_DONE
					bRet = 1;
					Count = 1;
					goto StopWriteClock;
				}
				Count--;
			}
		}

StopWriteClock:
		SDHC2_STR_STP_CLK	= 0x00000001; // Stop the clock
		do
		{
			uiStatus = SDHC2_STATUS;
		}
		while((uiStatus&0x0100) != 0); // Wait till clock is stop, command-response end.
	}

	// Send CMD7 (DESELECT_CARD)
	SDHC_SendCommand(7, 0x0000, 0x0000, 0x0001, usResponse);

//	printf("Sent %d bytes\n", uiTotalSent);
	if ((gs_SDHCInfo.uiOptions&scSDHC_Option_AllowClkGating) != 0)
		SDHC_Enable(0);
	return bRet;

}


/* UNTESTED (Copied code)
	// DMAC init for SDHC channel setup
	*(P_U32)DMA_DCR = 0x0001;				// Enable the DMA (DEN)
	*(P_U32)DMA_IMR = 0x07FF;				// Disable all I/O Channel IRQ
	if (direction == 1)//write
	{
		*(P_U32)DMA_SAR1 = Memory_Addr;		// Source Address
		*(P_U32)DMA_DAR1 = 0x00214038;		// Destination Address
	}
	else //read
	{
		*(P_U32)DMA_DAR1 = Memory_Addr;		// Destination Address
		*(P_U32)DMA_SAR1 = 0x00214038;		// Source Address
	}
	*(P_U32)DMA_CNTR1 = Size;				// Set No of Byte transfer
	*(P_U32)DMA_CCR1 = dir;					// Ch1: FIFO as the target, Linear Mem source,
											// Mem inc, 16-bit target, 32-bit source,
											// Request Enable, DMA Disable
	*(P_U32)DMA_RSSR1 = 0x000D;				// Ch1: DMA request select; SDHC is bit[13]
	if(SD_4bit_enable)
		*(P_U32)DMA_BLR1= 0x0000;			// Ch1: No. of FIFO to be read, burst length x32
	else
		*(P_U32)DMA_BLR1= 0x0010;			// Ch1: No. of FIFO to be read, burst length x8
	// Start DMA and Poll end of DMA transfer done
	*(P_U32)DMA_ISR = 0x0002;				// Clear DMA ISR for MMC
	*(P_U32)DMA_CCR1 = dir_en;				// Ch1: FIFO as the target, Linear Mem source,
											// Mem inc, 16-bit target, 32-bit source,
											// Request Enable, DMA Enable
	while ( ((*(P_U32)DMA_ISR) & 0x2) ==0 )	//wait for the data transfer complete
		;
	*(P_U32)DMA_ISR = 0x0002;				// Clear DMA ISR for MMC
	*(P_U32)DMA_CCR1 = dir;					// Ch1: FIFO as the target, Linear Mem source,
											// Mem inc, 16-bit target, 32-bit source,
											// Request Enable, DMA Disable
	// End of DMA usage
*/
