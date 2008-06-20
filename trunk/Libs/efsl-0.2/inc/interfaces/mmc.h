/*****************************************************************
 * FILE:        mmc.h
 * REVISION:    $Id: mmc.h,v 1.4 2007/07/17 15:13:04 juri Exp $
 * DESCRIPTION: definitions of the MMC and SD card commands
 * AUTHOR:      Juri Haberland <juri@sapienti-sat.org>
 *****************************************************************
 * CONFIGURABLE OPTIONS: none
 *****************************************************************/

#ifndef __INTERFACES_MMC_H__
#define __INTERFACES_MMC_H__


#define MMC_BLOCK_SIZE              512

/**************************************/
/* SD and MMC card status definitions */
/**************************************/
#define MMC_C_S_READY_FOR_DATA      0x100
#define MMC_C_S_STATE_IDLE          0x000
#define MMC_C_S_STATE_READY         0x200
#define MMC_C_S_STATE_IDENT         0x400
#define MMC_C_S_STATE_STBY          0x600
#define MMC_C_S_STATE_TRAN          0x800
#define MMC_C_S_STATE_DATA          0xA00
#define MMC_C_S_STATE_RCV           0xC00
#define MMC_C_S_STATE_PRG           0xE00
#define MMC_C_S_STATE_DIS           0x1000
#define MMC_C_S_

/*
 * card register definitions
 */

/********************************/
/* OCR register bit definitions */
/********************************/
#define MMC_OCR_LOW_VOLTAGE_RANGE   0x00000080      /* low voltage range: 1.65 - 1.95V */ 
#define MMC_OCR_HIGH_VOLTAGE_RANGE  0x00FF8000      /* high voltage range: 2.7 - 3.6V */
#define MMC_OCR_CARD_HIGH_CAPACITY  0x40000000      /* Card high capacity status bit */
#define MMC_OCR_CARD_NOT_BUSY       0x80000000      /* Card Power up status bit */


/****************/
/* MMC commands */
/****************/
#define MMC_GO_IDLE_STATE           0
#define MMC_SEND_OP_COND            1
#define MMC_ALL_SEND_CID            2
#define MMC_SET_RELATIVE_ADDR       3
#define MMC_SET_DSR                 4

#define MMC_SELECT_CARD             7
#define MMC_SEND_EXT_CSD            8
#define MMC_SEND_CSD                9

#define MMC_STOP_TRANSMISSION       12
#define MMC_SEND_STATUS             13

#define MMC_READ_SINGLE_BLOCK       17

#define MMC_WRITE_BLOCK             24

#define MMC_APP_CMD                 55


/***************/
/* SD commands */
/***************/
#define SD_SEND_RELATIVE_ADDR       3

#define SD_APP_SET_BUS_WIDTH        6

#define SD_SEND_IF_COND             8

#define SD_APP_OP_COND              41


#endif /* __INTERFACES_MMC_H__ */

/***** end of file mmc.h *****/
