/**
 * @file
 * @brief
 * nnxt_display_glue.c
 *
 *  Created on: 22.03.2017
 *      Author: florian
 */

/*********************************/
/*           includes            */
#include "nnxt_display_glue.h"
#include "SpiDevice.h"
#include "nnxt.h"
#include "stm32f4xx_gpio.h"
#include "nnxt_config.h"
#include "em_clk.h"

/*********************************/
/*           macros              */
#define DISPLAY_CS_HI		GPIO_SetBits(NXT_OnBoard_Display_CS_Port, NXT_OnBoard_Display_CS_Pin);
#define DISPLAY_CS_LOW		GPIO_ResetBits(NXT_OnBoard_Display_CS_Port, NXT_OnBoard_Display_CS_Pin);

#define DISPLAY_DC_COMMAND	GPIO_ResetBits(NXT_OnBoard_Display_DC_Port, NXT_OnBoard_Display_DC_Pin);
#define DISPLAY_DC_DATA		GPIO_SetBits(NXT_OnBoard_Display_DC_Port, NXT_OnBoard_Display_DC_Pin);
#define DISPLAY_RESET		GPIO_ResetBits(NXT_OnBoard_Display_Reset_Port, NXT_OnBoard_Display_Reset_Pin);
#define DISPLAY_NRESET		GPIO_SetBits(NXT_OnBoard_Display_Reset_Port, NXT_OnBoard_Display_Reset_Pin);


#define WriteData(a,b)                      _Generic((a), const uint8_t* : WriteData8, uint8_t* : WriteData8, const uint16_t* : WriteData16, uint16_t* : WriteData16)(a,b)

/*********************************/
/*      extern variables         */

/*********************************/
/*      internal variables       */

/*********************************/
/*          enums                */

/*********************************/
/*          structs              */

/*********************************/
/*        typedefs               */

/*********************************/
/*      function implementation  */
static inline void WriteCommand(const uint8_t *c, uint32_t length){
	DISPLAY_DC_COMMAND;

    ResetChipSelect(Display);
	SPI_Write8(c, length, Display);
	SetChipSelect(Display);
}

static inline void WriteData8(uint8_t *d, uint32_t length){
	DISPLAY_DC_DATA;

    ResetChipSelect(Display);
	SPI_Write8(d, length, Display);
	SetChipSelect(Display);
}

static inline void WriteData16(const uint16_t *d, const uint32_t length){
    DISPLAY_DC_DATA;

    ResetChipSelect(Display);
    SPI_Write16(d, length, Display);
    SetChipSelect(Display);
}

void LCD_IO_Init(void){

	GPIO_InitTypeDef Pins_Init;
	ENABLE_CLOCK_IO(NXT_OnBoard_Display_Reset_Clock, ENABLE);
	ENABLE_CLOCK_IO(NXT_OnBoard_Display_DC_Clock, ENABLE);
	ENABLE_CLOCK_IO(NXT_OnBoard_Display_CS_Clock, ENABLE);

	Pins_Init.GPIO_Mode = GPIO_Mode_OUT;
	Pins_Init.GPIO_OType = GPIO_OType_PP;
	Pins_Init.GPIO_PuPd = GPIO_PuPd_UP;
	Pins_Init.GPIO_Speed = NXT_GPIO_GenSpeed;
	// init resetResetChipSelect
	Pins_Init.GPIO_Pin = NXT_OnBoard_Display_Reset_Pin;
	GPIO_Init(NXT_OnBoard_Display_Reset_Port, &Pins_Init);
	GPIO_SetBits(NXT_OnBoard_Display_Reset_Port, NXT_OnBoard_Display_Reset_Pin);

	// init data command line
	Pins_Init.GPIO_Pin = NXT_OnBoard_Display_DC_Pin;
	GPIO_Init(NXT_OnBoard_Display_DC_Port, &Pins_Init);
	GPIO_SetBits(NXT_OnBoard_Display_DC_Port, NXT_OnBoard_Display_DC_Pin);

	// init display chip select
	Pins_Init.GPIO_Pin = NXT_OnBoard_Display_CS_Pin;
	GPIO_Init(NXT_OnBoard_Display_CS_Port, &Pins_Init);

	// now doing a hardware display reset
	GPIO_ResetBits(NXT_OnBoard_Display_CS_Port, NXT_OnBoard_Display_CS_Pin);
	Delay(100);
	DISPLAY_RESET;
	Delay(500);
	DISPLAY_NRESET;
	Delay(500);
    GPIO_SetBits(NXT_OnBoard_Display_CS_Port, NXT_OnBoard_Display_CS_Pin);

}

void LCD_IO_WriteMultipleData8(uint8_t *pData, uint32_t Size){
	WriteData(pData, Size);
}

void LCD_IO_WriteMultipleData16(uint16_t *pData, uint32_t Size){
    WriteData(pData, Size);
}

void LCD_IO_WriteReg(uint8_t Reg){
	WriteCommand(&Reg, 1);
}

void LCD_Delay(uint32_t delay){
	Delay(delay);
}
