/**!
 * @file
 * em_i2c.h
 *
 */

#ifndef DRIVER_INC_EM_I2C_H_
#define DRIVER_INC_EM_I2C_H_
/*********************************/
/*           includes            */
#include "stm32f4xx_i2c.h"
#include "em_config.h"
#include <stdint.h>

/*********************************/
/*           macros              */

/**
 * @defgroup    EM_I2C_DriverMacros
 * @brief       all macros which must be set before the i2c on a specific channel can be used
 * @todo        commenting all macros
 * @{
 *
 * \par Pinout
            |  I2Cx  | EM_I2Cx_PINSPACK1 | EM_I2Cx_PINSPACK2 | EM_I2Cx_PINSPACK3| note                              |
            |   -    |:-------------------:|:-------------------:|:------------------:|-----------------------------------|
            |   -    | SCL    SDA          | SCL    SDA        | SCL    SDA       |   -                                |
            |  I2C1  | PB6   PB7           | PB8   PB9         | PB6   PB9        | EM_I2Cx_PINSPACK3 does not work on Discovery Board  |
            |  I2C2  | PB10  PB11          | PF1   PF0         | PH4   PH5        |  - |
            |  I2C3  | PA8   PC9           | PH7  PH8          | -    -           | - |

  *
 */

 #define EM_I2C_SPEED_STANDARD       100000
 #define EM_I2C_SPEED_FAST           400000

/* Settings for I2C1 on Discovery Board */
#ifndef EM_I2C1_ACK_ADDRESS
#define EM_I2C1_ACK_ADDRESS 		I2C_AcknowledgedAddress_7bit
#endif
#ifndef EM_I2C1_MODE
#define EM_I2C1_MODE				I2C_Mode_I2C
#endif
#ifndef EM_I2C1_OWN_ADDRESS
#define EM_I2C1_OWN_ADDRESS			0x00
#endif
#ifndef EM_I2C1_ACK
#define EM_I2C1_ACK					I2C_Ack_Disable
#endif
#ifndef EM_I2C1_DUTYCYLCE
#define EM_I2C1_DUTYCYLCE			I2C_DutyCycle_2
#endif
#ifndef EM_I2C1_SPEED
#define EM_I2C1_SPEED			    10000
#endif

/* Settings for I2C2 on Discovery Board */
#ifndef EM_I2C2_ACK_ADDRESS
#define EM_I2C2_ACK_ADDRESS 		I2C_AcknowledgedAddress_7bit

#endif
#ifndef EM_I2C2_MODE
#define EM_I2C2_MODE				I2C_Mode_I2C
#endif
#ifndef EM_I2C2_OWN_ADDRESS
#define EM_I2C2_OWN_ADDRESS			0x00
#endif
#ifndef EM_I2C2_ACK
#define EM_I2C2_ACK					I2C_Ack_Disable
#endif
#ifndef EM_I2C2_DUTYCYLCE
#define EM_I2C2_DUTYCYLCE			I2C_DutyCycle_2
#endif
#ifndef EM_I2C2_SPEED
#define EM_I2C2_SPEED			    10000
#endif

/* Settings for I2C3 on Discovery Board */
#ifndef EM_I2C3_ACK_ADDRESS
#define EM_I2C3_ACK_ADDRESS 		I2C_AcknowledgedAddress_7bit
#endif
#ifndef EM_I2C3_MODE
#define EM_I2C3_MODE				I2C_Mode_I2C
#endif
#ifndef EM_I2C3_OWN_ADDRESS
#define EM_I2C3_OWN_ADDRESS			0x00
#endif
#ifndef EM_I2C3_ACK
#define EM_I2C3_ACK					I2C_Ack_Disable
#endif
#ifndef EM_I2C3_DUTYCYLCE
#define EM_I2C3_DUTYCYLCE			I2C_DutyCycle_2
#endif
#ifndef EM_I2C3_SPEED
#define EM_I2C3_SPEED			    EM_I2C_SPEED_STANDARD
#endif

#define EM_I2C_SPEED_STANDARD       100000
#define EM_I2C_SPEED_FAST           400000


/* Settings for all I2Cs */
#ifndef EM_I2C1_USE
#define EM_I2C1_USE 1
#endif
#define EM_I2C1_PINSPACK1	1
#define EM_I2C1_PINSPACK2	2
#define EM_I2C1_PINSPACK3	3
#ifndef EM_I2C1_PINSPACK_NUMBER
#define EM_I2C1_PINSPACK_NUMBER 	EM_I2C1_PINSPACK1
#endif

#ifndef EM_I2C2_USE
#define EM_I2C2_USE 1
#endif
#define EM_I2C2_PINSPACK1	1
#define EM_I2C2_PINSPACK2	2
#define EM_I2C2_PINSPACK3	3
#ifndef EM_I2C2_PINSPACK_NUMBER
#define EM_I2C2_PINSPACK_NUMBER     EM_I2C2_PINSPACK1
#endif

#ifndef EM_I2C3_USE
#define EM_I2C3_USE 0
#endif
#ifndef EM_I2C3_PINSPACK_NUMBER
#define EM_I2C3_PINSPACK_NUMBER
#endif

#define em_I2C1	I2C1
#define em_I2C2	I2C2
#define em_I2C3	I2C3

#ifndef EM_GPIO_SPEED
#define EM_GPIO_SPEED GPIO_Speed_50MHz
#endif

#define EM_I2C_CALC_7Bit_ADDRESS(a) (a>>1)


/**
 * @}
 */
/*********************************/
/*      extern variables         */

/*********************************/
/*          enums                */

/*!
 * @brief All possible errors which can happen
 */
enum em_i2c_error_e {
    em_i2c_error_NoError = 0,
    em_i2c_error_TimeOut,
    em_i2c_error_Direction,
};

/*********************************/
/*          structs              */

/*********************************/
/*        typedefs               */
typedef enum em_i2c_error_e         em_i2c_error_t;

/*********************************/
/*      Function prototypes      */

/**
 * @brief   Does the initialisation for I2C Channel 1. Sets the speed, configure the ports and enable the channel(settings taken from the macros).
 * @return  one of the errors described in #em_i2c_error_e
 * @pre     all macros for channel 1 must be set EM_I2C_MACROS
 */
em_i2c_error_t EM_I2C1_Init();
/**
 * @brief   Does the initialisation for I2C Channel 2. Sets the speed, configure the ports and enable the channel(settings taken from the macros).
 * @return  one of the errors described in #em_i2c_error_e
 * @pre     all macros for channel 1 must be set EM_I2C_MACROS
 */
em_i2c_error_t EM_I2C2_Init();
/**
 * @brief   Does the initialisation for I2C Channel 3. Sets the speed, configure the ports and enable the channel(settings taken from the macros).
 * @return  one of the errors described in #em_i2c_error_e
 * @pre     all macros for channel 1 must be set EM_I2C_MACROS
 */
em_i2c_error_t EM_I2C3_Init();
/**
 * @brief   Does the initialisation for all I2C Channels. Sets the speed, configure the ports and enable the channel.
 * @return  one of the errors described in #em_i2c_error_e
 * @pre     all macros for channel 1 must be set EM_I2C_MACROS
 */
em_i2c_error_t EM_I2CALL_Init();
/**
 * @brief               Writes \p len \p data on the bus with the given \p address. Also does the intialization with start and stop to the bus.
 * @param  I2Cx         The I2C channel on wich the data should be sended.
 * @param  address_7bit The address of the slave device. This must be the 7 bit address and only the bits [6 downto 0] of the given address are used! Take care of the address!
 * @param  data         A pointer to the data which shall be sended.
 * @param  len          How much data are in the \p data array stored and should be sended.
 * @return              one of the errors described in #em_i2c_error_e
 * @pre                 The given channel must be intialized before it can be proper used.
 * @blocking
 */
em_i2c_error_t EM_I2C_Write(I2C_TypeDef* I2Cx, uint8_t address_7bit, uint8_t *data, uint32_t len);

/**
 * @brief               Reads \p len \p data from the bus. Also does the initialization and the take care of acks and nacks.
 * @param  I2Cx         The I2C channel on which the data should be sended.
 * @param  address_7bit The address of the slave device. This must be the 7 bit address and only the bits [6 downto 0] of the given address are used! Take care of the address!
 * @param  data         A pointer to a datastorage where the readed data should be stored. It must be have enough space for \p len bytes!
 * @param  len          How much data should be readed.
 * @return              one of the errors described in #em_i2c_error_e
 * @pre                 The given channel must be intialized before it can be proper used.
 * @blocking
 */
em_i2c_error_t EM_I2C_Read(I2C_TypeDef *I2Cx, uint8_t address_7bit, volatile uint8_t* data, uint32_t len);

/**
 * @brief Resets and reconfigurate the pins for the given I2C channel
 * @param I2Cx the channel of which the pins should be resetted
 * @todo  make channel select, actually only I2C1 can be reseted
 */
void EM_I2C_ResetPins(I2C_TypeDef *I2Cx);


#endif /* DRIVER_INC_EM_I2C_H_ */
