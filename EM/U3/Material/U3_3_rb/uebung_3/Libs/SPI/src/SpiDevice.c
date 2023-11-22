/**
 * @file
 * @brief
 * SpiDevice.c
 *
 *  Created on: 22.10.2018
 *      Author: Johannes Fischer
         Email: johannes6.fischer@oth-regensburg.de
 */

/*********************************/
/*           includes            */
#include "stm32f4xx_spi.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include "SpiDevice.h"
#include "nnxt_os_glue.h"

/*********************************/
/*           macros              */

#define SPI_FLUSH_INIT	10

#define SPI_IS_BUSY() (((SPI1)->SR & (SPI_SR_TXE | SPI_SR_RXNE)) == 0 || ((SPI1)->SR & SPI_SR_BSY))
#define SPI_WAIT()            while (SPI_IS_BUSY())

#define CS_STATUS           GPIO_ReadOutputDataBit(NXT_OnBoard_Display_CS_Port, NXT_OnBoard_Display_CS_Pin) || GPIO_ReadOutputDataBit(NXT_OnBoard_SD_CS_Port, NXT_OnBoard_SD_CS_Pin);


/*********************************/
/*       macros for Display      */

//#define DISPLAY_CS_HI		GPIO_SetBits(NXT_OnBoard_Display_CS_Port, NXT_OnBoard_Display_CS_Pin)
//#define DISPLAY_CS_LOW		GPIO_ResetBits(NXT_OnBoard_Display_CS_Port, NXT_OnBoard_Display_CS_Pin)
#define DISPLAY_CS_STATUS   GPIO_ReadOutputDataBit(NXT_OnBoard_Display_CS_Port, NXT_OnBoard_Display_CS_Pin)

/*********************************/
/*       macros for SDCard      */

//#define SDCARD_CS_HI		GPIO_SetBits(NXT_OnBoard_SD_CS_Port, NXT_OnBoard_SD_CS_Pin)
//#define SDCARD_CS_LOW		GPIO_ResetBits(NXT_OnBoard_SD_CS_Port, NXT_OnBoard_SD_CS_Pin)
#define SDCARD_CS_STATUS    GPIO_ReadOutputDataBit(NXT_OnBoard_SD_CS_Port, NXT_OnBoard_SD_CS_Pin)

/*********************************/
/*      extern variables         */

/*********************************/
/*      internal variables       */

uint8_t selectedDevice = 0;
/*********************************/
/*          enums        SPI_DataSize_8b        */

/*********************************/
/*          structs              */

/*********************************/
/*        typedefs               */

/*********************************/
/*      function implementation  */

volatile uint16_t tempValue = 0x00;

static inline void SPI_Flush(){
	for(uint8_t i = 0; i < SPI_FLUSH_INIT; i++){
		SPI_ReadWrite8((uint8_t[]){0xFF}, 1, Display);
	}
}
static inline void SPI1_ClearOVR(){
	SPI_I2S_ReceiveData(SPI1);
}

em_spi_error_t SPI_Startup_Init(){

	GPIO_InitTypeDef GPIO_Init_Struct;
	SPI_InitTypeDef SPI_Init_Struct;

	em_spi_error_t ret_var = em_spi_error_Noerror;

	GPIO_Init_Struct.GPIO_Mode = GPIO_Mode_AF;
	GPIO_Init_Struct.GPIO_OType = GPIO_OType_PP;
	GPIO_Init_Struct.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init_Struct.GPIO_Speed = GPIO_Speed_100MHz;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);

	GPIO_Init_Struct.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_5 | GPIO_Pin_4;	// sclk and mosi and miso
	GPIO_Init(GPIOB, &GPIO_Init_Struct);

	/*GPIO_Init_Struct.GPIO_Mode = GPIO_Mode_AF;	//miso
	GPIO_Init_Struct.GPIO_Pin = GPIO_Pin_4;
	GPIO_Init(GPIOB, &GPIO_Init_Struct);*/

	GPIO_PinAFConfig(GPIOB, GPIO_PinSource3, GPIO_AF_SPI1);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource4, GPIO_AF_SPI1);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource5, GPIO_AF_SPI1);

	SPI_Init_Struct.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;
	//SPI_Init_Struct.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;
	selectedDevice = Display;
	SPI_Init_Struct.SPI_CPHA = SPI_CPHA_1Edge;
	//SPI_Init_Struct.SPI_CPOL = SPI_CPOL_High; //Changed for SDCard support
	SPI_Init_Struct.SPI_CPOL = SPI_CPOL_Low;
	SPI_Init_Struct.SPI_CRCPolynomial = 0;
	SPI_Init_Struct.SPI_DataSize = SPI_DataSize_8b;
	SPI_Init_Struct.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_Init_Struct.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_Init_Struct.SPI_Mode = SPI_Mode_Master;
	SPI_Init_Struct.SPI_NSS = SPI_NSS_Soft;
	//SPI_Init_Struct.SPI_NSS = SPI_NSS_Hard;

	SPI_I2S_DeInit(SPI1);
	SPI_Init(SPI1, &SPI_Init_Struct);

	SPI_Cmd(SPI1,ENABLE);

	SPI_Flush();

	return ret_var;
}

void ChangeSettings(Device device)
{
    selectedDevice = device;
    switch(device)
    {
        case Display:
            SPI1->CR1 = (SPI1->CR1 & ~0x38) | NXT_OnBoard_Display_Init_Speed;
            selectedDevice = Display;
            break;

        case SDCard:

            SPI1->CR1 = (SPI1->CR1 & ~0x38) | NXT_OnBoard_SDCard_Init_Speed;
            selectedDevice = SDCard;
            break;
    }
}

em_spi_error_t SPI_Write8(const uint8_t *data, const uint32_t length, const Device device)
{
    em_spi_error_t ret_var = em_spi_error_Noerror;

    SPI_WAIT();

    SPI_DataSizeConfig(SPI1, SPI_DataSize_8b);

    if(device != selectedDevice)
    {
        ChangeSettings(device);
    }

    for(uint32_t i = 0; i< length; i++, data++)
    {
		SPI_WAIT();
		SPI_I2S_SendData(SPI1, *data);
		SPI1_ClearOVR();
    }

    while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_BSY) == SET);


	return ret_var;
}

em_spi_error_t SPI_Write16(const uint16_t *data, const uint32_t length, const Device device)
{
    em_spi_error_t ret_var = em_spi_error_Noerror;

    SPI_WAIT();

    SPI_DataSizeConfig(SPI1, SPI_DataSize_16b);

    if(device != selectedDevice)
    {
        ChangeSettings(device);
    }

    for(uint32_t i = 0; i< length; i++, data++)
    {
		SPI_WAIT();
		SPI_I2S_SendData(SPI1, *data);
    }

    while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_BSY) == SET);

    SPI1_ClearOVR();

	return ret_var;
}

em_spi_error_t SPI_ReadWrite16(volatile uint16_t *data, const uint16_t length, const Device device)
{

    em_spi_error_t ret_var = em_spi_error_Noerror;


    SPI_I2S_ClearFlag(SPI1, SPI_I2S_FLAG_RXNE);
    SPI_DataSizeConfig(SPI1, SPI_DataSize_16b);

    if(device != selectedDevice)
    {
        ChangeSettings(device);
    }

    SPI_WAIT();

    for(uint32_t i = 0; i< length; i++, data++)
    {
		while((SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET));
		SPI_I2S_SendData(SPI1, *data);
		while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);
		*data = SPI_I2S_ReceiveData(SPI1);
    }
    while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_BSY) == SET);
	return ret_var;
}

em_spi_error_t SPI_ReadWrite8(volatile uint8_t *data, const uint16_t length, const Device device)
{

    em_spi_error_t ret_var = em_spi_error_Noerror;

    SPI_I2S_ClearFlag(SPI1, SPI_I2S_FLAG_RXNE);
    SPI_DataSizeConfig(SPI1, SPI_DataSize_8b);

    if(device != selectedDevice)
    {
        ChangeSettings(device);
    }

    for(uint32_t i = 0; i< length; i++, data++)
    {
		while((SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET));
		SPI_I2S_SendData(SPI1, *data);
		Delay(10);
		while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);
		*data = SPI_I2S_ReceiveData(SPI1);
    }
    while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_BSY) == SET);
	return ret_var;
}

em_spi_error_t SPI_Read(volatile uint16_t *data, const Device device)
{
    em_spi_error_t ret_var = em_spi_error_Noerror;
    SPI_WAIT();
    SPI_I2S_ClearFlag(SPI1, SPI_I2S_FLAG_RXNE);
    SPI_DataSizeConfig(SPI1, SPI_DataSize_16b);

    if(device != selectedDevice)
    {
        ChangeSettings(device);
    }

    SPI_WAIT();

    *data = SPI_I2S_ReceiveData(SPI1);
    return ret_var;
}

void ChangePrescaler(const uint16_t prescaler)
{
    SPI1->CR1 = (SPI1->CR1 & ~0x38) | prescaler;
}

void SetChipSelect(const Device device)
{
     switch(device)
    {
        case Display:
            {
              if(DISPLAY_CS_STATUS == DISABLE)
              {
                  GPIO_SetBits(NXT_OnBoard_Display_CS_Port, NXT_OnBoard_Display_CS_Pin);
              }
              break;
            }


        case SDCard:
            {
              if(SDCARD_CS_STATUS == DISABLE)
              {
                  GPIO_SetBits(NXT_OnBoard_SD_CS_Port, NXT_OnBoard_SD_CS_Pin);
              }
              break;
            }
    }
}

void ResetChipSelect(const Device device)
{
    switch(device)
    {
        case Display:
            {
              if(DISPLAY_CS_STATUS == ENABLE)
              {
                  GPIO_ResetBits(NXT_OnBoard_Display_CS_Port, NXT_OnBoard_Display_CS_Pin);
              }
              break;
            }


        case SDCard:
            {
              if(SDCARD_CS_STATUS == ENABLE)
              {
                  GPIO_ResetBits(NXT_OnBoard_SD_CS_Port, NXT_OnBoard_SD_CS_Pin);
              }
              break;
            }
    }
}
