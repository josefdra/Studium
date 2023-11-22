/**
 * @file
 * @brief
 * nnxt_sdcard_glue.c
 *
 *  Created on: 22.10.2018
 *      Author: Johannes Fischer
 *       Email: johannes6.fischer@oth-regensburg.de
 */



/*********************************/
/*           includes            */
#include "nnxt_config.h"
#include "stm32f4xx_gpio.h"
#include "SpiDevice.h"
#include "diskio.h"
#include "stm32f4xx_tim.h"
#include "nnxt_os_glue.h"

/*********************************/
/*           macros              */

#define FCLK_SLOW() ChangePrescaler(0x28)	/* Set SCLK = PCLK / 64 */
#define FCLK_FAST() ChangePrescaler(0x00)	/* Set SCLK = PCLK / 2 */
#define	MMC_WP		0 /* Write protected (yes:true, no:false, default:false) */

/* MMC/SD command */
#define CMD0	(0)			/* GO_IDLE_STATE */
#define CMD1	(1)			/* SEND_OP_COND (MMC) */
#define	ACMD41	(0x80+41)	/* SEND_OP_COND (SDC) */
#define CMD8	(8)			/* SEND_IF_COND */
#define CMD9	(9)			/* SEND_CSD */
#define CMD10	(10)		/* SEND_CID */
#define CMD12	(12)		/* STOP_TRANSMISSION */
#define ACMD13	(0x80+13)	/* SD_STATUS (SDC) */
#define CMD16	(16)		/* SET_BLOCKLEN */
#define CMD17	(17)		/* READ_SINGLE_BLOCK */
#define CMD18	(18)		/* READ_MULTIPLE_BLOCK */
#define CMD23	(23)		/* SET_BLOCK_COUNT (MMC) */
#define	ACMD23	(0x80+23)	/* SET_WR_BLK_ERASE_COUNT (SDC) */
#define CMD24	(24)		/* WRITE_BLOCK */
#define CMD25	(25)		/* WRITE_MULTIPLE_BLOCK */
#define CMD32	(32)		/* ERASE_ER_BLK_START */
#define CMD33	(33)		/* ERASE_ER_BLK_END */
#define CMD38	(38)		/* ERASE */
#define CMD55	(55)		/* APP_CMD */
#define CMD58	(58)		/* READ_OCR */






/*********************************/
/*      extern variables         */

/*********************************/
/*      internal variables       */

static volatile
DSTATUS Stat = STA_NOINIT;	/* Physical drive status */

static
BYTE CardType;			/* Card type flags */

static volatile
UINT Timer1, Timer2;	/* 1kHz decrement timer stopped at zero (disk_timerproc()) */

/*********************************/
/*          enums                */

/*********************************/
/*          structs              */

/*********************************/
/*        typedefs               */

/*********************************/
/*      function implementation  */


/*---------------------------------------*/
/* Init-Function - Initialize the CardSelectPin */
void SDCard_Init(void){

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

    TIM_TimeBaseInitTypeDef timerInitStructure;
    timerInitStructure.TIM_Prescaler = 65534;
    timerInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
    timerInitStructure.TIM_Period = 999;
    timerInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    timerInitStructure.TIM_RepetitionCounter = 0;
    TIM_TimeBaseInit(TIM2, &timerInitStructure);
    TIM_Cmd(TIM2, ENABLE);

    NVIC_InitTypeDef nvicStructure;
    nvicStructure.NVIC_IRQChannel = TIM2_IRQn;
    nvicStructure.NVIC_IRQChannelPreemptionPriority = 0;
    nvicStructure.NVIC_IRQChannelSubPriority = 1;
    nvicStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&nvicStructure);

    GPIO_InitTypeDef Pins_Init;
    Pins_Init.GPIO_Mode = GPIO_Mode_OUT;
	Pins_Init.GPIO_OType = GPIO_OType_PP;
	Pins_Init.GPIO_PuPd = GPIO_PuPd_UP;
	Pins_Init.GPIO_Speed = NXT_GPIO_GenSpeed;
	Pins_Init.GPIO_Pin = NXT_OnBoard_SD_CS_Pin;
	GPIO_Init(NXT_OnBoard_SD_CS_Port, &Pins_Init);
}

/*--------------------------------------------------------------------------

   Static Functions

---------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------*/
/* SPI controls (Platform dependent)                                     */
/*-----------------------------------------------------------------------*/

/* Initialize MMC interface */
static
void init_spi (void)
{
	SetChipSelect(SDCard);			/* Set CS# high */

	Delay(10);
}


/*-----------------------------------------------------------------------*/
/* Exchange packet with the SD                                           */
/*-----------------------------------------------------------------------*/
static
BYTE xchg_spi (
	BYTE dat	/* Data to send */
)
{
    SPI_ReadWrite8(&dat, sizeof(dat), SDCard);
	return dat;		/* Return received byte */
}

/*-----------------------------------------------------------------------*/
/* Wait for card ready                                                   */
/*-----------------------------------------------------------------------*/

int wait_ready (	/* 1:Ready, 0:Timeout */
	UINT wt			/* Timeout [ms] */
)
{
	BYTE d;

	Timer2 = wt;
	do {
		d = xchg_spi(0xFF);
		/* This loop takes a time. Insert rot_rdq() here for multitask envilonment. */
	} while (d != 0xFF && Timer2);	/* Wait for card goes ready or timeout */

	return (d == 0xFF) ? 1 : 0;
}

/*-----------------------------------------------------------------------*/
/* Deselect card and release SPI                                         */
/*-----------------------------------------------------------------------*/

void deselect ()
{
	SetChipSelect(SDCard);		/* Set CS# high */
	xchg_spi(0xFF);	/* Dummy clock (force DO hi-z for multiple slave SPI) */
}

/*-----------------------------------------------------------------------*/
/* Select card and wait for ready                                        */
/*-----------------------------------------------------------------------*/

int select ()	/* 1:OK, 0:Timeout */
{
	ResetChipSelect(SDCard);		/* Set CS# low */
	xchg_spi(0xFF);	/* Dummy clock (force DO enabled) */
	if (wait_ready(500)) return 1;	/* Wait for card ready */

	deselect();
	return 0;	/* Timeout */
}

/* Receive multiple byte */
static
void rcvr_spi_multi (
	BYTE *buff,		/* Pointer to data buffer */
	UINT btr		/* Number of bytes to receive (even number) */
)
{
    WORD d = 0xFFFF;

    do {
            SPI_ReadWrite16(d, 1, SDCard);                      /* Start next transaction */
            buff[1] = d; buff[0] = d >> 8;			            /* Store it */
            buff += 2;
	} while (btr -= 2);
}

/* Send multiple byte */
static
void xmit_spi_multi (
	const BYTE *buff,	/* Pointer to the data */
	UINT btx			/* Number of bytes to send (even number) */
)
{
	WORD d;

	do {
		d = buff[0] << 8 | buff[1]; buff += 2;	/* Word to send next */
		SPI_Write16(d ,sizeof(WORD) ,SDCard);						/* Start next transaction */
	} while (btx -= 2);
}


static
int xmit_datablock (	/* 1:OK, 0:Failed */
	const BYTE *buff,	/* Ponter to 512 byte data to be sent */
	BYTE token			/* Token */
)
{
	BYTE resp;


	if (!wait_ready(500)) return 0;		/* Wait for card ready */

	xchg_spi(token);					/* Send token */
	if (token != 0xFD) {				/* Send data if token is other than StopTran */
		xmit_spi_multi(buff, 512);		/* Data */
		xchg_spi(0xFF); xchg_spi(0xFF);	/* Dummy CRC */

		resp = xchg_spi(0xFF);				/* Receive data resp */
		if ((resp & 0x1F) != 0x05) return 0;	/* Function fails if the data packet was not accepted */
	}
	return 1;
}


/*-----------------------------------------------------------------------*/
/* Send a command packet to the MMC                                      */
/*-----------------------------------------------------------------------*/

static
BYTE send_cmd (		/* Return value: R1 resp (bit7==1:Failed to send) */
	BYTE cmd,		/* Command index */
	DWORD arg		/* Argument */
)
{
	BYTE n, res;


	if (cmd & 0x80) {	/* Send a CMD55 prior to ACMD<n> */
		cmd &= 0x7F;
		res = send_cmd(CMD55, 0);
		if (res > 1) return res;
	}

	/* Select the card and wait for ready except to stop multiple block read */
	if (cmd != CMD12) {
		deselect();
		if (!select()) return 0xFF;
	}

	/* Send command packet */
	xchg_spi(0x40 | cmd);				/* Start + command index */
	xchg_spi((BYTE)(arg >> 24));		/* Argument[31..24] */
	xchg_spi((BYTE)(arg >> 16));		/* Argument[23..16] */
	xchg_spi((BYTE)(arg >> 8));			/* Argument[15..8] */
	xchg_spi((BYTE)arg);		 		/* Argument[7..0] */
	n = 0x01;							/* Dummy CRC + Stop */
	if (cmd == CMD0) n = 0x95;			/* Valid CRC for CMD0(0) */
	if (cmd == CMD8) n = 0x87;			/* Valid CRC for CMD8(0x1AA) */
	xchg_spi(n);

	/* Receive command resp */
    if (cmd == CMD12) xchg_spi(0xFF);	/* Diacard following one byte when CMD12 */

    n = 10;
    do {
		res = xchg_spi(0xFF);
	} while ((res == 0xFF) && --n);

	return res;							/* Return received response */
}

/*-----------------------------------------------------------------------*/
/* Receive a data packet from the MMC                                    */
/*-----------------------------------------------------------------------*/

static
int rcvr_datablock (	/* 1:OK, 0:Error */
	BYTE *buff,			/* Data buffer */
	UINT btr			/* Data block length (byte) */
)
{
	BYTE token;
	Timer1 = 200;
	do {							/* Wait for DataStart token in timeout of 200ms */
		token = xchg_spi(0xFF);
		/* This loop will take a time. Insert rot_rdq() here for multitask envilonment. */
	} while ((token == 0xFF) && Timer1);
	if(token != 0xFE) return 0;		/* Function fails if invalid DataStart token or timeout */

	rcvr_spi_multi(buff, btr);		/* Store trailing data to the buffer */
	xchg_spi(0xFF);
	xchg_spi(0xFF);			/* Discard CRC */

	return 1;						/* Function succeeded */
}

/*------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* Glue-Functions between SPI and FatFS */


/*--------------------------------------------------------------------------

   Public Functions

---------------------------------------------------------------------------*/


/*-------------------------------------------------------------------------*/
/* Initialize disk drive                                                   */
/*-------------------------------------------------------------------------*/
DSTATUS disk_initialize (BYTE pdrv)
{
    BYTE pdata[4];
    BYTE n, cmd, ty;


	if (pdrv) return STA_NOINIT;			    /* Supports only drive 0 */
	init_spi();							        /* Initialize SPI */

	if (Stat & STA_NODISK) return Stat;	        /* Is card existing in the soket? */

	//FCLK_SLOW();
	for (n = 10; n; n--) xchg_spi(0xFF);	/* Send 80 dummy clocks */

	ty = 0;
	if (send_cmd(CMD0, 0) == 0x01) 			/* Put the card SPI/Idle state */
    {
        Timer1 = 1000;
		if(send_cmd(CMD8,0x1AA) == 0x01) /* Initialization timeout = 1 sec */
		{
            for (n = 0; n < 4; n++) pdata[n] = xchg_spi(0xFF);
            if(pdata[2] == 0x01 && pdata[3] == 0xAA)
            {
                while(send_cmd(ACMD41, 0x40000000) == 0x01 && Timer1);
                if(send_cmd(CMD58, 0x00)== 0x00)
                {
                    for (n = 0; n < 4; n++) pdata[n] = xchg_spi(0xFF);
                    ty = (pdata[0] & 0x40) ? CT_SD2 | CT_BLOCK : CT_SD2;	/* Card id SDv2 */
                }
            }
            else                                                            /* Not SDv2 card */
            {
                if (send_cmd(ACMD41, 0) <= 1) 	                            /* SDv1 or MMC? */
                {
                    ty = CT_SD1; cmd = ACMD41;	/* SDv1 (ACMD41(0)) */
                }
                else
                {
                    ty = CT_MMC; cmd = CMD1;	/* MMCv3 (CMD1(0)) */
                }
                while (Timer1 && send_cmd(cmd, 0));		/* Wait for end of initialization */
            }
		}
    }

    if(ty != 0 && !(ty & CT_BLOCK))
    {
        send_cmd(CMD16, 512);
    }

	CardType = ty;	/* Card type */
	deselect();

	if (ty) {			/* OK */
		//FCLK_FAST();			/* Set fast clock */
		Stat &= ~STA_NOINIT;	/* Clear STA_NOINIT flag */
	} else {			/* Failed */
		Stat = STA_NOINIT;
	}

	return Stat;
}


/*-----------------------------------------------------------------------*/
/* Get disk status                                                       */
/*-----------------------------------------------------------------------*/
DSTATUS disk_status (BYTE pdrv)
{
    if (pdrv) return STA_NOINIT;		/* Supports only drive 0 */

	return Stat;	/* Return disk status */
}

/*-----------------------------------------------------------------------*/
/* Read sector(s)                                                        */
/*-----------------------------------------------------------------------*/
DRESULT disk_read (BYTE pdrv, BYTE* buff, DWORD sector, UINT count)
{
    if (pdrv || !count) return RES_PARERR;		/* Check parameter */
	if (Stat & STA_NOINIT) return RES_NOTRDY;	/* Check if drive is ready */

	if (!(CardType & CT_BLOCK)) sector *= 512;	/* LBA ot BA conversion (byte addressing cards) */

	if (count == 1)
    {	/* Single sector read */
		if ((send_cmd(CMD17, sector) == 0)	/* READ_SINGLE_BLOCK */
			&& rcvr_datablock(buff, 512)) {
			count = 0;
		}
	}
	else {				/* Multiple sector read */
		if (send_cmd(CMD18, sector) == 0) {	/* READ_MULTIPLE_BLOCK */
			do {
				if (!rcvr_datablock(buff, 512)) break;
				buff += 512;
			} while (--count);
			send_cmd(CMD12, 0);				/* STOP_TRANSMISSION */
		}
	}
	deselect();

	return count ? RES_ERROR : RES_OK;	/* Return result */
}

/*-----------------------------------------------------------------------*/
/* Write sector(s)                                                       */
/*-----------------------------------------------------------------------*/
DRESULT disk_write (BYTE pdrv, const BYTE* buff, DWORD sector, UINT count)
{
    if (pdrv || !count) return RES_PARERR;		/* Check parameter */
	if (Stat & STA_NOINIT) return RES_NOTRDY;	/* Check drive status */
	if (Stat & STA_PROTECT) return RES_WRPRT;	/* Check write protect */

	if (!(CardType & CT_BLOCK)) sector *= 512;	/* LBA ==> BA conversion (byte addressing cards) */

	if (count == 1) {	/* Single sector write */
		if ((send_cmd(CMD24, sector) == 0)	/* WRITE_BLOCK */
			&& xmit_datablock(buff, 0xFE)) {
			count = 0;
		}
	}
	else {				/* Multiple sector write */
		if (CardType & CT_SDC) send_cmd(ACMD23, count);	/* Predefine number of sectors */
		if (send_cmd(CMD25, sector) == 0) {	/* WRITE_MULTIPLE_BLOCK */
			do {
				if (!xmit_datablock(buff, 0xFC)) break;
				buff += 512;
			} while (--count);
			if (!xmit_datablock(0, 0xFD)) count = 1;	/* STOP_TRAN token */
		}
	}
	deselect();

	return count ? RES_ERROR : RES_OK;	/* Return result */
}

/*-----------------------------------------------------------------------*/
/* Miscellaneous drive controls other than data read/write               */
/*-----------------------------------------------------------------------*/
DRESULT disk_ioctl (BYTE pdrv, BYTE cmd, void* buff)
{
    DRESULT res;
	BYTE n, csd[16];
	DWORD *dp, st, ed, csize;


	if (pdrv) return RES_PARERR;					/* Check parameter */
	if (Stat & STA_NOINIT) return RES_NOTRDY;	/* Check if drive is ready */

	res = RES_ERROR;

	switch (cmd) {
	case CTRL_SYNC :		/* Wait for end of internal write process of the drive */
		if (select()) res = RES_OK;
		break;

	case GET_SECTOR_COUNT :	/* Get drive capacity in unit of sector (DWORD) */
		if ((send_cmd(CMD9, 0) == 0) && rcvr_datablock(csd, 16)) {
			if ((csd[0] >> 6) == 1) {	/* SDC ver 2.00 */
				csize = csd[9] + ((WORD)csd[8] << 8) + ((DWORD)(csd[7] & 63) << 16) + 1;
				*(DWORD*)buff = csize << 10;
			} else {					/* SDC ver 1.XX or MMC ver 3 */
				n = (csd[5] & 15) + ((csd[10] & 128) >> 7) + ((csd[9] & 3) << 1) + 2;
				csize = (csd[8] >> 6) + ((WORD)csd[7] << 2) + ((WORD)(csd[6] & 3) << 10) + 1;
				*(DWORD*)buff = csize << (n - 9);
			}
			res = RES_OK;
		}
		break;

	case GET_BLOCK_SIZE :	/* Get erase block size in unit of sector (DWORD) */
		if (CardType & CT_SD2) {	/* SDC ver 2.00 */
			if (send_cmd(ACMD13, 0) == 0) {	/* Read SD status */
				xchg_spi(0xFF);
				if (rcvr_datablock(csd, 16)) {				/* Read partial block */
					for (n = 64 - 16; n; n--) xchg_spi(0xFF);	/* Purge trailing data */
					*(DWORD*)buff = 16UL << (csd[10] >> 4);
					res = RES_OK;
				}
			}
		} else {					/* SDC ver 1.XX or MMC */
			if ((send_cmd(CMD9, 0) == 0) && rcvr_datablock(csd, 16)) {	/* Read CSD */
				if (CardType & CT_SD1) {	/* SDC ver 1.XX */
					*(DWORD*)buff = (((csd[10] & 63) << 1) + ((WORD)(csd[11] & 128) >> 7) + 1) << ((csd[13] >> 6) - 1);
				} else {					/* MMC */
					*(DWORD*)buff = ((WORD)((csd[10] & 124) >> 2) + 1) * (((csd[11] & 3) << 3) + ((csd[11] & 224) >> 5) + 1);
				}
				res = RES_OK;
			}
		}
		break;

	case CTRL_TRIM :	/* Erase a block of sectors (used when _USE_ERASE == 1) */
		if (!(CardType & CT_SDC)) break;				/* Check if the card is SDC */
		if (disk_ioctl(pdrv, MMC_GET_CSD, csd)) break;	/* Get CSD */
		if (!(csd[0] >> 6) && !(csd[10] & 0x40)) break;	/* Check if sector erase can be applied to the card */
		dp = buff; st = dp[0]; ed = dp[1];				/* Load sector block */
		if (!(CardType & CT_BLOCK)) {
			st *= 512; ed *= 512;
		}
		if (send_cmd(CMD32, st) == 0 && send_cmd(CMD33, ed) == 0 && send_cmd(CMD38, 0) == 0 && wait_ready(30000)) {	/* Erase sector block */
			res = RES_OK;	/* FatFs does not check result of this command */
		}
		break;

	default:
		res = RES_PARERR;
	}

	deselect();

	return res;
}


/*-----------------------------------------------------------------------*/
/* Device timer function                                                 */
/*-----------------------------------------------------------------------*/
/* This function must be called from timer interrupt routine in period
/  of 1 ms to generate card control timing.
*/

void TIM2_IRQHandler(void){
	WORD n;
	BYTE s;

    if (TIM_GetITStatus(TIM2, TIM_IT_Update))
    {
        n = Timer1;						/* 1kHz decrement timer stopped at 0 */
        if (n) Timer1 = --n;
        n = Timer2;
        if (n) Timer2 = --n;

        s = Stat;
        if (MMC_WP) {	/* Write protected */
            s |= STA_PROTECT;
        } else {		/* Write enabled */
            s &= ~STA_PROTECT;
        }
        Stat = s;

        TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
    }
}
