/**
 * @file
 * em_i2c.c
 *
 */

/*********************************/
/*           includes            */
#include "em_i2c.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_i2c.h"
#include "stm32f4xx_gpio.h"
#include <stdio.h>

/*********************************/
/*           macros              */

/*********************************/
/*      extern variables         */

/*********************************/
/* 		global variables    	 */
/// GPIO configuration for I2C1. Will be needed in various functions over runtime for reconfiguration of pins.
GPIO_InitTypeDef GPIO_InitStructure_i2c1;
GPIO_InitTypeDef GPIO_InitStructure_i2c2;

/*********************************/
/* 		static variables   		 */

static const uint32_t timeoutTime = 100000;

/*********************************/
/*          enums                */

/*********************************/
/*          structs              */

/*********************************/
/*        typedefs               */

/*********************************/
/*      Functions      */

/**
 * @brief Sends a start (I2C tart + address + direction bit ) on the bus.
 * @param  I2Cx         the I2C channel you want to send data, can be I2C1, I2C2, I2C3
 * @param  address_7bit the address, in 7 bit format. The address must be in bits [6 downto 0] of the Byte. The direction bit will be added by the function.
 * @param  direction    the direction, could be I2C_Direction_Transmitter and I2C_Direction_Receiver
 * @return              one of the errors described in #em_i2c_error_e
 * @pre                 The given channel must be intialized before it can be proper used.
 * @blocking
 * @todo                implement error handling and timeouts
 */
static inline em_i2c_error_t I2C_Send_Start(I2C_TypeDef *I2Cx, uint8_t address_7bit, uint8_t direction){
	em_i2c_error_t ret_var = em_i2c_error_NoError;
    uint16_t counter = timeoutTime;
	while(I2C_GetFlagStatus(I2Cx, I2C_FLAG_BUSY) && counter > 0)
    {
        counter --;
    }
    if(counter == 0)
    {
        ret_var = em_i2c_error_TimeOut;
        return ret_var;
    }
	I2C_GenerateSTART(I2Cx, ENABLE);
    counter = timeoutTime;
	while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_MODE_SELECT) && counter > 0){
        counter --;
    }

    if(counter == 0)
    {
        ret_var = em_i2c_error_TimeOut;
        return ret_var;
    }

	I2C_Send7bitAddress(I2Cx, (address_7bit << 1), direction);
    if(direction == I2C_Direction_Transmitter){
		while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED) && counter > 0){
        counter --;
    }
	}else if(direction == I2C_Direction_Receiver){
		while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED) && counter > 0){
        counter --;
    }
	}else{
        ret_var = em_i2c_error_Direction;
    }
    if(counter == 0 && ret_var != em_i2c_error_Direction)
    {
        ret_var = em_i2c_error_TimeOut;
    }
	return ret_var;
}

/**
 * @brief generates the stop bit on the specific channel
 * @param I2Cx the I2C channel
 */
static inline void I2C_Stop(I2C_TypeDef *I2Cx){
    I2C_GenerateSTOP(I2Cx, ENABLE);
}

/**
 * @brief Reads one byte from the given I2C channel. Can also handle with ack and nack.
 * @param  I2Cx the I2C channel
 * @param  data a pointer where the data shall be stored
 * @param  ack  for nack and ack this specifies the ack bit
 * @return      one of the errors described in #em_i2c_error_e
 * @pre         The given channel must be intialized before it can be proper used.
 * @blocking
 * @todo        implementing error handling like timeout
 */
static inline em_i2c_error_t I2C_ReadByte(I2C_TypeDef *I2Cx, volatile uint8_t *data, FunctionalState ack){
    em_i2c_error_t ret_var = em_i2c_error_NoError;

    while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_RECEIVED));	//wait until the the to read byte is reveived

    I2C_AcknowledgeConfig(I2Cx, ack);
	if(ack == DISABLE){
		I2C_Stop(I2Cx);
	}
	*data = I2C_ReceiveData(I2Cx);

	return ret_var;
}

em_i2c_error_t EM_I2C1_Init() {
#if (EM_I2C1_USE == 1)

	em_i2c_error_t ret_var = em_i2c_error_NoError;
	I2C_InitTypeDef I2C_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE); //hardcoded because the I2C1 outputs can only be on Port B

	GPIO_InitStructure_i2c1.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure_i2c1.GPIO_Speed = EM_GPIO_SPEED;
	GPIO_InitStructure_i2c1.GPIO_OType = GPIO_OType_OD;
	GPIO_InitStructure_i2c1.GPIO_PuPd = GPIO_PuPd_UP;	//there are externals pullups

#if (EM_I2C1_PINSPACK_NUMBER == EM_I2C1_PINSPACK1)
	GPIO_InitStructure_i2c1.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_Init(GPIOB, &GPIO_InitStructure_i2c1);

	GPIO_PinAFConfig(GPIOB, GPIO_PinSource6, GPIO_AF_I2C1);	// SCL
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource7, GPIO_AF_I2C1); // SDA
#elif (EM_I2C1_PINSPACK_NUMBER == EM_I2C1_PINSPACK2)
	#warning "I2C Channel 2 is actually not implemented!"
#elif (EM_I2C1_PINSPACK_NUMBER == EM_I2C1_PINSPACK3)
	#warning "I2C Channel 3 is actually not implemented!"
#else
	#warning "There is no valid I2C1 channel configuration! You must config the channel outputs!"
#endif

	I2C_DeInit(I2C1);

	I2C_InitStructure.I2C_ClockSpeed = EM_I2C1_SPEED;
	I2C_InitStructure.I2C_Mode = EM_I2C1_MODE;			// going to i2c mode
	I2C_InitStructure.I2C_DutyCycle = EM_I2C1_DUTYCYLCE;// 50% duty cycle --> standard
	I2C_InitStructure.I2C_OwnAddress1 = EM_I2C1_OWN_ADDRESS;// not relevant in master mode
	I2C_InitStructure.I2C_Ack = EM_I2C1_ACK;	// when reading, ack is disables
	I2C_InitStructure.I2C_AcknowledgedAddress = EM_I2C1_ACK_ADDRESS; // set address length to 7 bit addresses
	I2C_Init(I2C1, &I2C_InitStructure);
	I2C_Cmd(I2C1, ENABLE);

	return ret_var;
#else
	// do nothing
	return 0;
#endif
}

em_i2c_error_t EM_I2C2_Init() {
#if (EM_I2C2_USE == 1)

	em_i2c_error_t ret_var = em_i2c_error_NoError;
	I2C_InitTypeDef I2C_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C2, ENABLE);
	RCC_AHB2PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE); //hardcoded because the I2C1 outputs can only be on Port B

	GPIO_InitStructure_i2c2.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure_i2c2.GPIO_Speed = EM_GPIO_SPEED;
	GPIO_InitStructure_i2c2.GPIO_OType = GPIO_OType_OD;
	GPIO_InitStructure_i2c2.GPIO_PuPd = GPIO_PuPd_UP;	//there are externals pullups

#if (EM_I2C2_PINSPACK_NUMBER == EM_I2C2_PINSPACK1)
	GPIO_InitStructure_i2c2.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;
	GPIO_Init(GPIOB, &GPIO_InitStructure_i2c2);

	GPIO_PinAFConfig(GPIOB, GPIO_PinSource10, GPIO_AF_I2C2);	// SCL
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource11, GPIO_AF_I2C2);    // SDA
#elif (EM_I2C1_PINSPACK_NUMBER == EM_I2C1_PINSPACK2)
	#warning "I2C Channel 2 is actually not implemented!"
#elif (EM_I2C1_PINSPACK_NUMBER == EM_I2C1_PINSPACK3)
	#warning "I2C Channel 3 is actually not implemented!"
#else
	#warning "There is no valid I2C1 channel configuration! You must config the channel outputs!"
#endif

	I2C_DeInit(I2C2);

	I2C_InitStructure.I2C_ClockSpeed = EM_I2C2_SPEED;
	I2C_InitStructure.I2C_Mode = EM_I2C2_MODE;			// going to i2c mode
	I2C_InitStructure.I2C_DutyCycle = EM_I2C2_DUTYCYLCE;// 50% duty cycle --> standard
	I2C_InitStructure.I2C_OwnAddress1 = EM_I2C2_OWN_ADDRESS;// not relevant in master mode
	I2C_InitStructure.I2C_Ack = EM_I2C2_ACK;	// when reading, ack is disables
	I2C_InitStructure.I2C_AcknowledgedAddress = EM_I2C2_ACK_ADDRESS; // set address length to 7 bit addresses
	I2C_Init(I2C2, &I2C_InitStructure);
	I2C_Cmd(I2C2, ENABLE);

	return ret_var;
#else
	// do nothing
	return 0;
#endif
}


em_i2c_error_t EM_I2CALL_Init(){
	EM_I2C1_Init();
	EM_I2C2_Init();
	return em_i2c_error_NoError;
}

em_i2c_error_t EM_I2C_Write(I2C_TypeDef* I2Cx, uint8_t address_7bit, uint8_t *data, uint32_t len){
    em_i2c_error_t ret_var = em_i2c_error_NoError;
    ret_var = I2C_Send_Start(I2Cx, address_7bit, I2C_Direction_Transmitter);
    if(ret_var == em_i2c_error_NoError){
        for(uint32_t i = 0; i < len; i++){
            I2C_SendData(I2Cx, *data);
            while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
            data++;
        }
    }
    I2C_Stop(I2Cx);
    return ret_var;
}

em_i2c_error_t EM_I2C_Read(I2C_TypeDef* I2Cx, uint8_t address_7bit, volatile uint8_t *data, uint32_t len){
    em_i2c_error_t ret_var = em_i2c_error_NoError;
    ret_var = I2C_Send_Start(I2Cx, address_7bit, I2C_Direction_Receiver);
    if(ret_var == em_i2c_error_NoError){
        for(uint32_t i = 0; i < len; i++){
        	if(i == len - 1){
        		// reading the last byte
        		ret_var = I2C_ReadByte(I2Cx, data, DISABLE);
        	}else{
        		ret_var = I2C_ReadByte(I2Cx, data, ENABLE);
        		data++;
        	}
        	if(ret_var != em_i2c_error_NoError)
        		break;
        }
    }
    return ret_var;
}

void EM_I2C_ResetPins(I2C_TypeDef *I2Cx){
	if(I2Cx == I2C1)
    {
        GPIO_Init(GPIOB, &GPIO_InitStructure_i2c1);
    }
    else if(I2Cx == I2C2)
    {
        GPIO_Init(GPIOB, &GPIO_InitStructure_i2c2);
    }
}
