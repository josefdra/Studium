/**
 * @file
 * @brief
 * nnxt_common.h
 *
 *  Created on: 20.02.2017
 *      Author: florian
 */

#ifndef NXT_APPLICATIONDRIVER_INC_NNXT_COMMON_H_
#define NXT_APPLICATIONDRIVER_INC_NNXT_COMMON_H_
/*********************************/
/*           includes            */
#include "stm32f4xx_gpio.h"

/*********************************/
/*           macros              */

/*********************************/
/*      extern variables         */

/*********************************/
/*          enums                */

/*********************************/
/*          structs              */
struct uController_pin_s{
	uint16_t pin;
	GPIO_TypeDef* port;
	uint32_t clock;
};

struct uController_pin_an_s{
    uint16_t        pin;
    GPIO_TypeDef*   port;
    uint32_t        clock;
    uint8_t         adc_channel;
    uint8_t         dma_val_nr;
};

/*********************************/
/*        typedefs               */
typedef struct uController_pin_s uController_pin_t;
typedef struct uController_pin_an_s uController_pin_an_t;

/*********************************/
/*      Function prototypes      */

#endif /* NXT_APPLICATIONDRIVER_INC_NNXT_COMMON_H_ */
