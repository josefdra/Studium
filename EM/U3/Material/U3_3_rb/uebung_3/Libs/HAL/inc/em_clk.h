/**!
 * @file
 * em_clk.h
 *
 */

#ifndef HAL_INC_EM_CLK_H_
#define HAL_INC_EM_CLK_H_
/*********************************/
/*           includes            */
#include "stm32f4xx_rcc.h"

/*********************************/
/*           macros              */

#define ENABLE_CLOCK_IO(a, new_state)	RCC_AHB1PeriphClockCmd(a, new_state)


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


#endif /* HAL_INC_EM_CLK_H_ */
