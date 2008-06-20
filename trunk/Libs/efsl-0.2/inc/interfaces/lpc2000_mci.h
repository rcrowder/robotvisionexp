/*****************************************************************************\
*              efs - General purpose Embedded Filesystem library              *
*          ---------------------------------------------------------          *
*                                                                             *
* Filename :  lpc2000_mci.h                                                   *
* Description : Headerfile for lpc2000_mci.c                                  *
*                                                                             *
* This program is free software; you can redistribute it and/or               *
* modify it under the terms of the GNU General Public License                 *
* as published by the Free Software Foundation; version 2                     *
* of the License.                                                             *
*                                                                             *
* This program is distributed in the hope that it will be useful,             *
* but WITHOUT ANY WARRANTY; without even the implied warranty of              *
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the               *
* GNU General Public License for more details.                                *
*                                                                             *
* As a special exception, if other files instantiate templates or             *
* use macros or inline functions from this file, or you compile this          *
* file and link it with other works to produce a work based on this file,     *
* this file does not by itself cause the resulting work to be covered         *
* by the GNU General Public License. However the source code for this         *
* file must still be made available in accordance with section (3) of         *
* the GNU General Public License.                                             *
*                                                                             *
* This exception does not invalidate any other reasons why a work based       *
* on this file might be covered by the GNU General Public License.            *
*                                                                             *
*                                                    (c)2007 Juri Haberland   *
\*****************************************************************************/

#ifndef __INTERFACES_LPC2000_MCI_H__
#define __INTERFACES_LPC2000_MCI_H__

#include "../debug.h"
#include "config.h"

#ifndef FALSE
#define FALSE                       0
#define TRUE                        1
#endif

/* return values for functions */
#define EFSL_ERR_OK                 0
#define EFSL_ERR_FAIL               -1

/* not the same as MMC_BLOCK_SIZE, but has the same value */
#define DISK_SECTOR_SIZE            512

/* value used in several loops to either form a time out or 
   just wait a specific amount of time */
#define TIME_OUT_VAL                300

/* we need to differentiate the card types */
#define CARD_TYPE_UNKNOWN           0
#define CARD_TYPE_MMC               (1 << 0)
#define CARD_TYPE_SDSC              (1 << 1)
#define CARD_TYPE_SDHC              (1 << 2)
#define CARD_TYPE_SDV2              (1 << 3)

/**********************************************/
/* bit definitions for the MCI_POWER register */
/**********************************************/
#define MMC_POWER_OFF               0x0
#define MMC_POWER_UP                0x2
#define MMC_POWER_ON                0x3
#define MMC_BUS_MODE_OPEN_DRAIN     (1 << 6)
#define MMC_BUS_MODE_ROD            (1 << 7)

/* other constants */
#define MMC_BUS_WIDTH_1_BIT         0x0
#define MMC_BUS_WIDTH_4_BIT         0x2

/* relative card address for MMC cards */
#define MMC_CARD_ADDRESS            0x12340000

/********************************/
/* modifiers for SD/MMC command */
/********************************/
/* no response */
#define MMC_CMD_NO_RESPONSE         0
/* wait for response */
#define MMC_CMD_RESPONSE            (1 << 6)
/* wait for a long response */
#define MMC_CMD_LONGRSP             (MMC_CMD_RESPONSE | (1 << 7))
/* disable command timer, wait for interrupt */
#define MMC_CMD_INTERRUPT           (1 << 8)
/* wait for CmdPend before sending the command */
#define MMC_CMD_PENDING             (1 << 9)
/* enable the command path state machine (CPSM) */
#define MMC_CMD_ENABLE_CPSM         (1 << 10)


/***************************************/
/* status codes in MCI_STATUS register */
/***************************************/
#define MCI_STAT_CMD_CRC_FAIL       (1 << 0)
#define MCI_STAT_DATA_CRC_FAIL      (1 << 1)
#define MCI_STAT_CMD_TIMEOUT        (1 << 2)
#define MCI_STAT_DATA_TIMEOUT       (1 << 3)
#define MCI_STAT_TX_UNDERRUN        (1 << 4)
#define MCI_STAT_RX_OVERRUN         (1 << 5)
#define MCI_STAT_CMD_RESP_END       (1 << 6)
#define MCI_STAT_CMD_SENT           (1 << 7)
#define MCI_STAT_DATA_END           (1 << 8)
#define MCI_STAT_START_BIT_ERR      (1 << 9)
#define MCI_STAT_DATA_BLOCK_END     (1 << 10)
#define MCI_STAT_CMD_ACTIVE         (1 << 11)
#define MCI_STAT_TX_ACTIVE          (1 << 12)
#define MCI_STAT_RX_ACTIVE          (1 << 13)
#define MCI_STAT_TX_FIFO_HALF_FULL  (1 << 14)
#define MCI_STAT_RX_FIFO_HALF_FULL  (1 << 15)
#define MCI_STAT_TX_FIFO_FULL       (1 << 16)
#define MCI_STAT_RX_FIFO_FULL       (1 << 17)
#define MCI_STAT_TX_FIFO_EMPTY      (1 << 18)
#define MCI_STAT_RX_FIFO_EMPTY      (1 << 19)
#define MCI_STAT_TX_DATA_AVLBL      (1 << 20)
#define MCI_STAT_RX_DATA_AVLBL      (1 << 21)

/* bit mask used to clear the status bits of the MCI_STATUS register */
#define MCI_STATUS_CLEAR_MASK       0x000007FF


/******************************************/
/* MCI_DATA_CTRL register bit definitions */
/******************************************/
#define MCI_DATA_TRANS_EN           0x01
#define MCI_DATA_TRANS_DIS          0x00
#define MCI_DATA_TO_HOST            0x02
#define MCI_DATA_TO_CARD            0x00
#define MCI_DATA_STREAM_MODE        0x04
#define MCI_DATA_BLOCK_MODE         0x00
#define MCI_DATA_DMA_EN             0x08
#define MCI_DATA_DMA_DIS            0x00
#define MCI_DATA_BLOCK_SIZE_1       0x00
#define MCI_DATA_BLOCK_SIZE_2       0x10
#define MCI_DATA_BLOCK_SIZE_4       0x20
#define MCI_DATA_BLOCK_SIZE_8       0x30
#define MCI_DATA_BLOCK_SIZE_16      0x40
#define MCI_DATA_BLOCK_SIZE_32      0x50
#define MCI_DATA_BLOCK_SIZE_64      0x60
#define MCI_DATA_BLOCK_SIZE_128     0x70
#define MCI_DATA_BLOCK_SIZE_256     0x80
#define MCI_DATA_BLOCK_SIZE_512     0x90
#define MCI_DATA_BLOCK_SIZE_1024    0xA0
#define MCI_DATA_BLOCK_SIZE_2048    0xB0


/**************************************/
/* MCI_CLOCK register bit definitions */
/**************************************/
/* clock divisor is -1 encoded; formula: MCICLK = MCLK/[2x(ClkDiv+1)],
   where MCLK is PCLK/2 = 28.8MHz => high speed: 14.4MHz, low speed: 400kHz */
// mthomas #define MCI_CLK_DIV_HIGH_SPEED      0
#define MCI_CLK_DIV_HIGH_SPEED      3
#define MCI_CLK_DIV_LOW_SPEED       35
#define MCI_CLK_ENABLE              (1 << 8)
#define MCI_CLK_PWRSAVE             (1 << 9)
#define MCI_CLK_BYPASS_DIV          (1 << 10)
#define MCI_CLK_WIDE_BUS            (1 << 11)


/*************************************************************\
*             hwInterface
*             -----------
* unsigned long   sectorCount    Number of sectors on the card
* unsigned long   address        relative card address
* unsigned short  type           card type (MMC/SDSC/SDHC...)
\*************************************************************/
struct hwInterface {
    euint32     sectorCount;
    euint32     address;
    euint8      type;
};
typedef struct hwInterface hwInterface;

esint8 if_initInterface(hwInterface * card, eint8 * opts);
esint8 if_readBuf(hwInterface* card, euint32 block_no, euint8* buf);
esint8 if_writeBuf(hwInterface* card, euint32 block_no, euint8* buf);
esint8 if_setPos(hwInterface* card, euint32 block_no);

void    if_mci_init(void);
esint8  mci_card_init(hwInterface * card);
euint8  mci_get_disk_size(hwInterface * card, euint32 * card_size);
esint8  mci_read_block(hwInterface * card, euint32 block_no, euint8 * buf);
esint8  mci_write_block(hwInterface * card, euint32 block_no, euint8 * buf);

#endif /* __INTERFACES_LPC23X8_H__ */
