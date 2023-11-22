/**
 * @file
 * @brief
 * nnxt_os_glue.c
 *
 *  Created on: 13.06.2017
 *      Author: florian
 */

/*********************************/
/*           includes            */
#include "nnxt_os_glue.h"

/*********************************/
/*           macros              */

/*********************************/
/*      extern variables         */
#if(NNXT_OS==FREERTOS)
const uint8_t appl_mode = FREERTOS;
#elif(NNXT_OS==ERIKA)
const uint8_t appl_mode = ERIKA;
#elif(NNXT_OS==BAREMETAL)
const uint8_t appl_mode = BAREMETAL;
#else
#error "There is no application mode set!"
#endif
uint8_t nnxt_pre_os_flag;
volatile uint32_t SysTickCounter;

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
void MakePreOSSysTick(void){
    SysTickCounter++;
}
