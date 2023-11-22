/**
 * @file
 * nnxt_config.h
 * In this file there is a mapping of the physical pins to speaking macros
 *
 */

#ifndef NXT_APPLICATIONDRIVER_INC_NNXT_CONFIG_H_
#define NXT_APPLICATIONDRIVER_INC_NNXT_CONFIG_H_
/*********************************/
/*           includes            */


#include "stdint.h"
/*********************************/
/*           macros              */

/**
 * @defgroup Debug
 * @brief Macros for advanced debugging while developing the nnxt library or for advanced error searching and handling.
 * Can also be used when trying to analyze System.
 * @warning this macros should only be changed, if the user REALLY know what he does and what it means to work with it!
 * @{
 */

/**
 * @brief Setting for advanced debugging.
 *
 * Value | Description
 * :------:|-----------
 * 0 | all debug features are disabled
 * 1 | there is the possibility to activate trace output over SWO with #NXT_DEV_DEBUG_ENABLE_TRACE
 * 2 | most interrupts creates events which can be watched with SEGGER SystemViewer
 * 3 |  all interrupts creats events (incl. SysTick)
 */
#ifndef NXT_DEV_DEBUG_LEVEL
#define NXT_DEV_DEBUG_LEVEL						0
#endif

/**
 * @brief Defines the name of the application which is displayed in the Segger SystemView Application.
 */
#ifndef NXT_DEV_DEBUG_APP_NAME
#define NXT_DEV_DEBUG_APP_NAME 					"NNXT App"
#endif

/**
 * @brief The device name shown in the Segger SystemView Application.
 */
#ifndef NXT_DEV_DEBUG_DEVICE_NAME
#define NXT_DEV_DEBUG_DEVICE_NAME				"NNXT Board with STM32F407VG"
#endif

/**
 * @brief Option for Segger SystemView Application. Please read the manual of the SystemView Application before changing the values with your own.
 */
#ifndef NXT_DEV_DEBUG_ID_DESC
#define NXT_DEV_DEBUG_ID_DESC					"I#15=SysTick,I#22=MotorC,I#56=MotorB,I#39=MotorA"
#endif

/**
 * @brief Option to enable the trace output over SWO from the µC.
 * Value | Description
 * :----:|:-----------
 * 0 | Trace output is disabled.
 * >0 | Trace output is enabled.
 */
#ifndef NXT_DEV_DEBUG_ENABLE_TRACE
#define NXT_DEV_DEBUG_ENABLE_TRACE				0
#endif

/**
 * @}
 */

/**
 * @defgroup General
 * @brief Macros which are important for the whole project.
 * @{
 */

#define NXT_GPIO_GenSpeed 				GPIO_Speed_50MHz

#define STM_VECTORTABLE_SIZE            (256)

/**
 * @}
 */

/**
 * @defgroup SensorPorts
 * @brief All things related to the sensor ports.
 */

/**
 * @addtogroup SensorPorts_General
 * @brief All macros which are valid for all sensor ports
 * @ingroup SensorPorts
 * @{
 */

#define NXT_SENSORn						5

/**
 * @}
 */

/**
 * @addtogroup I2C
 * @brief All things related to the I2C.
 * @ingroup SensorPorts_General
 * @{
 */

/**
 * @defgroup I2C_lConfigurationMacros
 * @description All configuration macros for the i2c.
 * @{
 */
#define NXT_SENSOR_I2C_SCL_Pin          GPIO_Pin_6
#define NXT_SENSOR_I2C_SCL_Port         GPIOB
#define NXT_SENSOR_I2C_SCL_Clock        RCC_AHB1Periph_GPIOB

#define NXT_SENSOR_I2C_SDA_Pin          GPIO_Pin_7
#define NXT_SENSOR_I2C_SDA_Port         GPIOB
#define NXT_SENSOR_I2C_SDA_Clock        RCC_AHB1Periph_GPIOB

#define NXT_SENSOR_I2C_Mux_Channel      I2C1
#define NXT_SENSOR_I2C_AF               GPIO_AF_I2C1
#define NXT_SENSOR_I2C_Clock            RCC_APB1Periph_I2C1

#define NXT_SENSOR_I2C_US_SPEED         (uint32_t)10000
#define NXT_SENSOR_I2C_OwnAddress       (uint16_t)0x00

#define NXT_SENSOR_I2C_EXTERN_Channel   I2C2
#define NXT_SENSOR_I2C_Mux_Pin          GPIO_Pin_13
#define NXT_SENSOR_I2C_Mux_Port         GPIOB
#define NXT_SENSOR_I2C_Mux_Clock        RCC_AHB1Periph_GPIOB

#define NXT_SENSOR_I2C_Mux_Addr         (uint8_t)0x70

/**
 * @}
 */

/**
 * @}
 */

/**
 * @addtogroup ADC
 * @ingroup SensorPorts_General
 * @{
 */

/**
 * @defgroup ADC_ConfigurationMacros
 * @description All macros for the adc settings
 * @{
 */
#define NXT_SENSOR_ADC_Resolution		ADC_Resolution_12b
#define NXT_SENSOR_ADC_Prescaler		ADC_Prescaler_Div8
#define NXT_SENSOR_ADC_DataAlign		ADC_DataAlign_Right
#define NXT_SENSOR_ADC_DelayTwoSampling ADC_TwoSamplingDelay_10Cycles
#define NXT_SENSOR_ADC_NbrOfChannels	8
#define NXT_SENSOR_ADC_SampleTime       ADC_SampleTime_15Cycles
#define NXT_SENSOR_ADC_NrOfValues       NXT_SENSOR_ADC_NbrOfConversion
#define NXT_SENSOR_ADC_ADCx             ADC1
#define NXT_SENSOR_ADC_ADCx_Clock       RCC_APB2Periph_ADC1
#define NXT_SENSOR_ADC_DMA_Channel      DMA_Channel_0
#define NXT_SENSOR_ADC_DMA_Stream       DMA2_Stream0
#define NXT_SENSOR_ADC_DMA_Clock        RCC_AHB1Periph_DMA2

/**
 * @}
 */

/**
 * @}
 */

/**
 * @addtogroup Sensorport_3
 * @ingroup SensorPorts
 * @{
 */

/**
* @defgroup SP3_ConfigurationMacros
* @brief All macros for Sensorport 3
* @{
*/
#define NXT_SENSOR3_BB_SDA_Pin				GPIO_Pin_7
#define NXT_SENSOR3_BB_SDA_Port				GPIOE
#define NXT_SENSOR3_BB_SDA_Clock			RCC_AHB1Periph_GPIOE

#define NXT_SENSOR3_BB_SCL_Pin				GPIO_Pin_1
#define NXT_SENSOR3_BB_SCL_Port				GPIOB
#define NXT_SENSOR3_BB_SCL_Clock			RCC_AHB1Periph_GPIOB

#define NXT_SENSOR3_BB_9V_Pin				GPIO_Pin_9
#define NXT_SENSOR3_BB_9V_Port				GPIOE
#define NXT_SENSOR3_BB_9V_Clock				RCC_AHB1Periph_GPIOE

#define NXT_SENSOR3_Analog_Pin				GPIO_Pin_5
#define NXT_SENSOR3_Analog_Port				GPIOC
#define NXT_SENSOR3_Analog_Clock			RCC_AHB1Periph_GPIOC
#define NXT_SENSOR3_Analog_ADC_Channel 		ADC_Channel_15

#define NXT_SENSOR3_SDAnalog_Pin    		GPIO_Pin_7
#define NXT_SENSOR3_SDAnalog_Port   		GPIOA
#define NXT_SENSOR3_SDAnalog_Clock  		RCC_AHB1Periph_GPIOA
#define NXT_SENSOR3_SDAnalog_ADC_Channel	ADC_Channel_7

/**
* @}
*/

/**
* @}
*/

/**
 * @addtogroup Sensorport_2
 * @ingroup SensorPorts
 * @{
 */

/**
* @defgroup SP2_ConfigurationMacros
* @brief All macros for Sensorport 2
* @{
*/
#define NXT_SENSOR2_BB_SDA_Pin		GPIO_Pin_4
#define NXT_SENSOR2_BB_SDA_Port		GPIOC
#define NXT_SENSOR2_BB_SDA_Clock	RCC_AHB1Periph_GPIOC

#define NXT_SENSOR2_BB_SCL_Pin		GPIO_Pin_4
#define NXT_SENSOR2_BB_SCL_Port		GPIOA
#define NXT_SENSOR2_BB_SCL_Clock	RCC_AHB1Periph_GPIOA

#define NXT_SENSOR2_BB_9V_Pin		GPIO_Pin_0
#define NXT_SENSOR2_BB_9V_Port		GPIOB
#define NXT_SENSOR2_BB_9V_Clock		RCC_AHB1Periph_GPIOB

#define NXT_SENSOR2_Analog_Pin		GPIO_Pin_5
#define NXT_SENSOR2_Analog_Port		GPIOA
#define NXT_SENSOR2_Analog_Clock	RCC_AHB1Periph_GPIOA
#define NXT_SENSOR2_Analog_ADC_Channel 		ADC_Channel_5

#define NXT_SENSOR2_SDAnalog_Pin    GPIO_Pin_2
#define NXT_SENSOR2_SDAnalog_Port   GPIOC
#define NXT_SENSOR2_SDAnalog_Clock  RCC_AHB1Periph_GPIOC
#define NXT_SENSOR2_SDAnalog_ADC_Channel	ADC_Channel_12


/**
* @}
*/

/**
* @}
*/

/**
* @addtogroup Sensorport_1
* @ingroup SensorPorts
* @{
*/

/**
 * @defgroup SP1_ConfigurationMacros
 * @brief All macros for Sensorport 1
 * @{
 */
#define NXT_SENSOR1_BB_SDA_Pin		GPIO_Pin_6
#define NXT_SENSOR1_BB_SDA_Port		GPIOE
#define NXT_SENSOR1_BB_SDA_Clock	RCC_AHB1Periph_GPIOE

#define NXT_SENSOR1_BB_SCL_Pin		GPIO_Pin_14
#define NXT_SENSOR1_BB_SCL_Port		GPIOC
#define NXT_SENSOR1_BB_SCL_Clock	RCC_AHB1Periph_GPIOC

#define NXT_SENSOR1_BB_9V_Pin		GPIO_Pin_4
#define NXT_SENSOR1_BB_9V_Port		GPIOE
#define NXT_SENSOR1_BB_9V_Clock		RCC_AHB1Periph_GPIOE

#define NXT_SENSOR1_Analog_Pin		GPIO_Pin_3
#define NXT_SENSOR1_Analog_Port		GPIOC
#define NXT_SENSOR1_Analog_Clock	RCC_AHB1Periph_GPIOC
#define NXT_SENSOR1_Analog_ADC_Channel 		ADC_Channel_13

#define NXT_SENSOR1_SDAnalog_Pin    GPIO_Pin_1
#define NXT_SENSOR1_SDAnalog_Port   GPIOC
#define NXT_SENSOR1_SDAnalog_Clock  RCC_AHB1Periph_GPIOC
#define NXT_SENSOR1_SDAnalog_ADC_Channel	ADC_Channel_11

/**
* @}
*/

/**
* @}
*/


/**
* @addtogroup Sensorport_0
* @ingroup SensorPorts
* @{
*/

/**
 * @defgroup SP0_ConfigurationMacros
 * @brief All macros for Sensorport 0
 * @{
 */
#define NXT_SENSOR0_BB_SDA_Pin		GPIO_Pin_13
#define NXT_SENSOR0_BB_SDA_Port		GPIOC
#define NXT_SENSOR0_BB_SDA_Clock	RCC_AHB1Periph_GPIOC

#define NXT_SENSOR0_BB_SCL_Pin		GPIO_Pin_15
#define NXT_SENSOR0_BB_SCL_Port		GPIOC
#define NXT_SENSOR0_BB_SCL_Clock	RCC_AHB1Periph_GPIOC

#define NXT_SENSOR0_BB_9V_Pin		GPIO_Pin_5
#define NXT_SENSOR0_BB_9V_Port		GPIOE
#define NXT_SENSOR0_BB_9V_Clock		RCC_AHB1Periph_GPIOE

#define NXT_SENSOR0_Analog_Pin		GPIO_Pin_3
#define NXT_SENSOR0_Analog_Port		GPIOA
#define NXT_SENSOR0_Analog_Clock	RCC_AHB1Periph_GPIOA
#define NXT_SENSOR0_Analog_ADC_Channel 		ADC_Channel_3

#define NXT_SENSOR0_SDAnalog_Pin    GPIO_Pin_0
#define NXT_SENSOR0_SDAnalog_Port   GPIOC
#define NXT_SENSOR0_SDAnalog_Clock  RCC_AHB1Periph_GPIOC
#define NXT_SENSOR0_SDAnalog_ADC_Channel	ADC_Channel_10

/**
* @}
*/

/**
* @}
*/



 /**
  * @defgroup NXT-Timing
  * @brief Some macros for the speed and timing resolution of the nxt.
  * @{
  */

/// @brief Specifies the resolution of the internal systicker for timing measurement etc.
#define NXT_SYSTEMTIMER_RESOLUTION_US   1000

#define NXT_SYSTEMTIMER_BASE_1MHZ		1000000

/// calculating the frequency of the systicker
/// @warning Do NOT changes this manually.
#define NXT_SYSTEMTIMER_SYSTICK_HZ      (NXT_SYSTEMTIMER_BASE_1MHZ/NXT_SYSTEMTIMER_RESOLUTION_US)

/// The timer which is used for precise measurement
#define NXT_PRECISE_TIMER_TIM			TIM14
#define NXT_PRECISE_TIMER_TIM_MAXVAL    (uint32_t)0xFFFF
 /**
  * @}
  */

/**
 * @defgroup MotorPorts
 * @brief All things related to the motor ports.
 */

/**
 * @defgroup General_Motor
 * @ingroup MotorPorts
 * @description General macros for the motor part
 * @{
 */

#define NXT_MOTORn_Period       			(uint32_t)10000
#define NXT_MOTORn              			3
#define NXT_MOTORn_RotaryInterrupt_Prio		(uint8_t)0x0F
#define NXT_MOTORn_RotaryInterrupt_SubPrio	(uint8_t)0xFF

/**
 * @}
 */

/**
 * @addtogroup Motorport_A
 * @ingroup MotorPorts
 * @brief Macros for Motor A
 * @{
 */

/**
 * @defgroup MA_ConfigurationMacros
 * @brief All macros for the motor port A.
 * @{
 */

#define NXT_MOTORA_PWM_Pin                      GPIO_Pin_2
#define NXT_MOTORA_PWM_Port                     GPIOA
#define NXT_MOTORA_PWM_Clock                    RCC_AHB1Periph_GPIOA
#define NXT_MOTORA_PWM_PinSource                GPIO_PinSource2

#define NXT_MOTORA_PWM_Timer                    TIM9
#define NXT_MOTORA_PWM_Timer_RCCConfig          RCC_APB2PeriphClockCmd
#define NXT_MOTORA_PWM_Timer_Clock              RCC_APB2Periph_TIM9
#define NXT_MOTORA_PWM_TIMER_AF                 GPIO_AF_TIM9

#define NXT_MOTORA_MODE1_Pin                    GPIO_Pin_8
#define NXT_MOTORA_MODE1_Port                   GPIOC
#define NXT_MOTORA_MODE1_Clock                  RCC_AHB1Periph_GPIOA

#define NXT_MOTORA_MODE2_Pin                    GPIO_Pin_8
#define NXT_MOTORA_MODE2_Port                   GPIOA
#define NXT_MOTORA_MODE2_Clock                  RCC_AHB1Periph_GPIOC

#define NXT_MOTORA_ENCODER_A_Pin                GPIO_Pin_6
#define NXT_MOTORA_ENCODER_A_Port               GPIOC
#define NXT_MOTORA_ENCODER_A_Clock              RCC_AHB1Periph_GPIOC
//for using with external interrupts
#define NXT_MOTORA_ENCODER_A_EXTI_Port			EXTI_PortSourceGPIOC
#define NXT_MOTORA_ENCODER_A_EXTI_PinSource		EXTI_PinSource6
#define NXT_MOTORA_ENCODER_A_EXTI_Line			EXTI_Line6
#define NXT_MOTORA_ENCODER_A_IRQChannel			EXTI9_5_IRQn

#define NXT_MOTORA_ENCODER_B_EXTI_Port			EXTI_PortSourceGPIOC
#define NXT_MOTORA_ENCODER_B_EXTI_PinSource		EXTI_PinSource7
#define NXT_MOTORA_ENCODER_B_EXTI_Line			EXTI_Line7
#define NXT_MOTORA_ENCODER_B_IRQChannel			EXTI9_5_IRQn

#define NXT_MOTORA_ENCODER_B_Pin                GPIO_Pin_7
#define NXT_MOTORA_ENCODER_B_Port               GPIOC
#define NXT_MOTORA_ENCODER_B_Clock              RCC_AHB1Periph_GPIOC

/**
 * @}
 */
/**
 * @}
 */

/**
 * @defgroup Motorport_B
 * @ingroup MotorPorts
 * @brief Macros for Motor B
 * @{
 */

/**
 * @defgroup MB_ConfigurationMacros
 * @brief All macros for the motor port B.
 * @{
 */

#define NXT_MOTORB_PWM_Pin                      GPIO_Pin_6
#define NXT_MOTORB_PWM_Port                     GPIOA
#define NXT_MOTORB_PWM_Clock                    RCC_AHB1Periph_GPIOA
#define NXT_MOTORB_PWM_PinSource                GPIO_PinSource6

#define NXT_MOTORB_PWM_Timer                    TIM13
#define NXT_MOTORB_PWM_Timer_RCCConfig          RCC_APB1PeriphClockCmd
#define NXT_MOTORB_PWM_Timer_Clock              RCC_APB1Periph_TIM13
#define NXT_MOTORB_PWM_TIMER_AF                 GPIO_AF_TIM13

#define NXT_MOTORB_MODE1_Pin                    GPIO_Pin_10
#define NXT_MOTORB_MODE1_Port                   GPIOD
#define NXT_MOTORB_MODE1_Clock                  RCC_AHB1Periph_GPIOD

#define NXT_MOTORB_MODE2_Pin                    GPIO_Pin_12
#define NXT_MOTORB_MODE2_Port                   GPIOD
#define NXT_MOTORB_MODE2_Clock                  RCC_AHB1Periph_GPIOD

#define NXT_MOTORB_ENCODER_A_Pin                GPIO_Pin_14
#define NXT_MOTORB_ENCODER_A_Port               GPIOD
#define NXT_MOTORB_ENCODER_A_Clock              RCC_AHB1Periph_GPIOD
// for using with external interrupts
#define NXT_MOTORB_ENCODER_A_EXTI_Port			EXTI_PortSourceGPIOD
#define NXT_MOTORB_ENCODER_A_EXTI_PinSource		EXTI_PinSource15        //this is usually pin b, but we need this for having easy configuration possibilities
#define NXT_MOTORB_ENCODER_A_EXTI_Line			EXTI_Line15
#define NXT_MOTORB_ENCODER_A_IRQChannel			EXTI15_10_IRQn

#define NXT_MOTORB_ENCODER_B_EXTI_Port			EXTI_PortSourceGPIOD
#define NXT_MOTORB_ENCODER_B_EXTI_PinSource		EXTI_PinSource14        //this is usually pin b, but we need this for having easy configuration possibilities
#define NXT_MOTORB_ENCODER_B_EXTI_Line			EXTI_Line13
#define NXT_MOTORB_ENCODER_B_IRQChannel			EXTI15_10_IRQn

#define NXT_MOTORB_ENCODER_B_Pin                GPIO_Pin_15
#define NXT_MOTORB_ENCODER_B_Port               GPIOD
#define NXT_MOTORB_ENCODER_B_Clock              RCC_AHB1Periph_GPIOD
/**
 * @}
 */

/**
 * @}
 */


/**
 * @defgroup Motorport_C
 * @ingroup MotorPorts
 * @brief Macros for Motor C
 * @{
 */

/**
 * @defgroup MBC_ConfigurationMacrosStartScheduler();
 * @brief All macros for the motor port B.
 * @{
 */

#define NXT_MOTORC_PWM_Pin                      GPIO_Pin_14
#define NXT_MOTORC_PWM_Port                     GPIOB
#define NXT_MOTORC_PWM_Clock                    RCC_AHB1Periph_GPIOB
#define NXT_MOTORC_PWM_PinSource                GPIO_PinSource14

#define NXT_MOTORC_PWM_Timer                    TIM12
#define NXT_MOTORC_PWM_Timer_RCCConfig          RCC_APB1PeriphClockCmd
#define NXT_MOTORC_PWM_Timer_Clock              RCC_APB1Periph_TIM12
#define NXT_MOTORC_PWM_TIMER_AF                 GPIO_AF_TIM12

#define NXT_MOTORC_MODE1_Pin                    GPIO_Pin_11
#define NXT_MOTORC_MODE1_Port                   GPIOD
#define NXT_MOTORC_MODE1_Clock                  RCC_AHB1Periph_GPIOD

#define NXT_MOTORC_MODE2_Pin                    GPIO_Pin_13
#define NXT_MOTORC_MODE2_Port                   GPIOD
#define NXT_MOTORC_MODE2_Clock                  RCC_AHB1Periph_GPIOD

#define NXT_MOTORC_ENCODER_A_Pin                GPIO_Pin_0
#define NXT_MOTORC_ENCODER_A_Port               GPIOA
#define NXT_MOTORC_ENCODER_A_Clock              RCC_AHB1Periph_GPIOA
//for using with external interrupts
#define NXT_MOTORC_ENCODER_A_EXTI_Port			EXTI_PortSourceGPIOA
#define NXT_MOTORC_ENCODER_A_EXTI_PinSource		EXTI_PinSource0
#define NXT_MOTORC_ENCODER_A_EXTI_Line			EXTI_Line0
#define NXT_MOTORC_ENCODER_A_IRQChannel			EXTI0_IRQn

#define NXT_MOTORC_ENCODER_B_EXTI_Port			EXTI_PortSourceGPIOA
#define NXT_MOTORC_ENCODER_B_EXTI_PinSource		EXTI_PinSource1
#define NXT_MOTORC_ENCODER_B_EXTI_Line			EXTI_Line1
#define NXT_MOTORC_ENCODER_B_IRQChannel			EXTI0_IRQn

#define NXT_MOTORC_ENCODER_B_Pin                GPIO_Pin_1
#define NXT_MOTORC_ENCODER_B_Port               GPIOA
#define NXT_MOTORC_ENCODER_B_Clock              RCC_AHB1Periph_GPIOA
/**
 * @}
 */

/**
 * @}
 */

/**
 * @addtogroup OnBoardPeripherals
 * @{
 */
/**
 * @defgroup OB_ConfigurationMacros
 * @brief all macros for the onboard peripherals like led, buzzer, display etc.
 * @{
 */
#define NXT_LEDn                        4

#define NXT_OnBoard_LED0_Pin            GPIO_Pin_12
#define NXT_OnBoard_LED0_Port           GPIOB
#define NXT_OnBoard_LED0_Clock          RCC_AHB1Periph_GPIOB

#define NXT_OnBoard_LED1_Pin            GPIO_Pin_11
#define NXT_OnBoard_LED1_Port           GPIOE
#define NXT_OnBoard_LED1_Clock          RCC_AHB1Periph_GPIOE

#define NXT_OnBoard_LED2_Pin            GPIO_Pin_13
#define NXT_OnBoard_LED2_Port           GPIOE
#define NXT_OnBoard_LED2_Clock          RCC_AHB1Periph_GPIOE

#define NXT_OnBoard_LED3_Pin            GPIO_Pin_15
#define NXT_OnBoard_LED3_Port           GPIOE
#define NXT_OnBoard_LED3_Clock          RCC_AHB1Periph_GPIOE


#define NXT_BUTTONn                     		5
#define NXT_BUTTONn_PushButtonInterruptPrio		(uint8_t)0x0E
#define NXT_BUTTONn_PushButtonInterruptSubprio	(uint8_t)0xFF

#define NXT_OnBoard_BUTTONU_Pin                 GPIO_Pin_2
#define NXT_OnBoard_BUTTONU_Port                GPIOB
#define NXT_OnBoard_BUTTONU_Clock               RCC_AHB1Periph_GPIOB
#define NXT_OnBoard_BUTTONU_EXTI_Port           EXTI_PortSourceGPIOB
#define NXT_OnBoard_BUTTONU_EXTI_PinSource      EXTI_PinSource2
#define NXT_OnBoard_BUTTONU_EXTI_Line           EXTI_Line2
#define NXT_OnBoard_BUTTONU_IRQChannel          EXTI2_IRQn

#define NXT_OnBoard_BUTTONL_Pin                 GPIO_Pin_8
#define NXT_OnBoard_BUTTONL_Port                GPIOE
#define NXT_OnBoard_BUTTONL_Clock               RCC_AHB1Periph_GPIOE
#define NXT_OnBoard_BUTTONL_EXTI_Port           EXTI_PortSourceGPIOE
#define NXT_OnBoard_BUTTONL_EXTI_PinSource      EXTI_PinSource8
#define NXT_OnBoard_BUTTONL_EXTI_Line           EXTI_Line8
#define NXT_OnBoard_BUTTONL_IRQChannel          EXTI9_5_IRQn

#define NXT_OnBoard_BUTTONM_Pin                 GPIO_Pin_10
#define NXT_OnBoard_BUTTONM_Port                GPIOE
#define NXT_OnBoard_BUTTONM_Clock               RCC_AHB1Periph_GPIOE
#define NXT_OnBoard_BUTTONM_EXTI_Port           EXTI_PortSourceGPIOE
#define NXT_OnBoard_BUTTONM_EXTI_PinSource      EXTI_PinSource10
#define NXT_OnBoard_BUTTONM_EXTI_Line           EXTI_Line10
#define NXT_OnBoard_BUTTONM_IRQChannel          EXTI15_10_IRQn

#define NXT_OnBoard_BUTTONR_Pin                 GPIO_Pin_12
#define NXT_OnBoard_BUTTONR_Port                GPIOE
#define NXT_OnBoard_BUTTONR_Clock               RCC_AHB1Periph_GPIOE
#define NXT_OnBoard_BUTTONR_EXTI_Port           EXTI_PortSourceGPIOE
#define NXT_OnBoard_BUTTONR_EXTI_PinSource      EXTI_PinSource12
#define NXT_OnBoard_BUTTONR_EXTI_Line           EXTI_Line12
#define NXT_OnBoard_BUTTONR_IRQChannel          EXTI15_10_IRQn

#define NXT_OnBoard_BUTTONB_Pin                 GPIO_Pin_14
#define NXT_OnBoard_BUTTONB_Port                GPIOE
#define NXT_OnBoard_BUTTONB_Clock               RCC_AHB1Periph_GPIOE
#define NXT_OnBoard_BUTTONB_EXTI_Port           EXTI_PortSourceGPIOE
#define NXT_OnBoard_BUTTONB_EXTI_PinSource      EXTI_PinSource14
#define NXT_OnBoard_BUTTONB_EXTI_Line           EXTI_Line14
#define NXT_OnBoard_BUTTONB_IRQChannel          EXTI15_10_IRQn


#define NXT_OnBoard_I2CMUX_Pin          GPIO_Pin_13
#define NXT_OnBoard_I2CMUX_Port         GPIOB
#define NXT_OnBoard_I2CMUX_Clock        RCC_AHB1Periph_GPIOB


#define NXT_OnBoard_DisplaySD_SPIn          SPI1

#define NXT_OnBoard_DisplaySD_SCLK_Pin      GPIO_Pin_3
#define NXT_OnBoard_DisplaySD_SCLK_Port     GPIOB
#define NXT_OnBoard_DisplaySD_SCLK_Clock    RCC_AHB1Periph_GPIOB

#define NXT_OnBoard_DisplaySD_MOSI_Pin      GPIO_Pin_5
#define NXT_OnBoard_DisplaySD_MOSI_Port     GPIOB
#define NXT_OnBoard_DisplaySD_MOSI_Clock    RCC_AHB1Periph_GPIOB

#define NXT_OnBoard_DisplaySD_MISO_Pin      GPIO_Pin_4
#define NXT_OnBoard_DisplaySD_MISO_Port     GPIOB
#define NXT_OnBoard_DisplaySD_MISO_Clock    RCC_AHB1Periph_GPIOB

#define NXT_OnBoard_Display_CS_Pin          GPIO_Pin_8
#define NXT_OnBoard_Display_CS_Port         GPIOB
#define NXT_OnBoard_Display_CS_Clock        RCC_AHB1Periph_GPIOB

#define NXT_OnBoard_SD_CS_Pin               GPIO_Pin_9
#define NXT_OnBoard_SD_CS_Port              GPIOB
#define NXT_OnBoard_SD_CS_Clock             RCC_AHB1Periph_GPIOB

#define NXT_OnBoard_Display_Reset_Pin       GPIO_Pin_3
#define NXT_OnBoard_Display_Reset_Port      GPIOE
#define NXT_OnBoard_Display_Reset_Clock     RCC_AHB1Periph_GPIOE

#define NXT_OnBoard_Display_DC_Pin          GPIO_Pin_2
#define NXT_OnBoard_Display_DC_Port         GPIOE
#define NXT_OnBoard_Display_DC_Clock        RCC_AHB1Periph_GPIOE

/// The spi speed for sd card init. This should be not higher than 400kHz! Our Peripheral Clock is 84MHz, so we running at ~600kHz (out of spec)!
#define NXT_OnBoard_SDCard_Init_Speed       SPI_BaudRatePrescaler_128
#define NXT_OnBoard_Display_Init_Speed      SPI_BaudRatePrescaler_4
/* macros for the em spi driver */
#define EM_SPI1_USE                         1
#define EM_SPI1_PINSPACK_NUMBER             EM_SPIx_PINSPACK2
#define EM_SPI1_BaudPrescaler               SPI_BaudRatePrescaler_4 //max speed of the display ~ 20 MHz
#define EM_SPI1_CHPA                        SPI_CPHA_2Edge
#define EM_SPI1_CPOL		                SPI_CPOL_High
#define EM_SPI1_CRCPolynomial               0
#define EM_SPI1_Datasize                    SPI_DataSize_8b
#define EM_SPI1_Direction                   SPI_Direction_2Lines_FullDuplex
#define EM_SPI1_FirstBit                    SPI_FirstBit_MSB
#define EM_SPI1_Mode                        SPI_Mode_Master
#define EM_SPI1_NSS                         SPI_NSS_Soft


#define NXT_OnBoard_BUZZER_Pin              GPIO_Pin_15
#define NXT_OnBoard_BUZZER_Port             GPIOB
#define NXT_OnBoard_BUZZER_Clock            RCC_AHB1Periph_GPIOB

#define NXT_OnBoard_BUZZER_TIM              TIM8
#define NXT_OnBoard_BUZZER_RCCConfig        RCC_APB2PeriphClockCmd
#define NXT_OnBoard_BUZZER_TIM_Clock        RCC_APB2Periph_TIM8
#define NXT_OnBoard_BUZZER_AFConfig         GPIO_AF_TIM8
#define NXT_OnBoard_BUZZER_AFPinSource      GPIO_PinSource15
#define NXT_OnBoard_BUZZER_PeriodHz			(uint32_t)4000
#define NXT_OnBoard_BUZZER_PeriodCalc 		(uint32_t)((SystemCoreClock / NXT_OnBoard_BUZZER_PeriodHz ) - 1)
/**
 * @}
 */

/**
 * @}
 */



/*********************************/
/*      extern variables         */

/*********************************/
/*          enums                */

/*********************************/
/*          structs              */

/*********************************/
/*        typedefs               */

/*********************************/
/*      Function prototypes      */


#endif /* NXT_APPLICATIONDRIVER_INC_NNXT_CONFIG_H_ */
