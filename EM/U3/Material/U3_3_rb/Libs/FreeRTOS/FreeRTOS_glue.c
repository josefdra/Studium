/**
 * @file
 * @brief
 * FreeRTOS_glue.c
 *
 *  Created on: 19.06.2017
 *      Author: florian
 */

/*********************************/
/*           includes            */
#include "FreeRTOS_glue.h"
#include "portmacro.h"
#include <limits.h>
#include "nnxt.h"

/*********************************/
/*           macros              */

/*********************************/
/*      extern variables         */

/*********************************/
/*          enums                */

/*********************************/
/*          structs              */

/*********************************/
/*      internal variables       */
struct registeredTasks_s registeredTasks[NNXT_MAX_USER_TASKS] = {0};
uint32_t TasksSlots = 0;
uint8_t NumberofTasks = 0;

/*********************************/
/*        typedefs               */

/*********************************/
/*      function implementation  */
void OS_BackgroundTask(){
    uint32_t NotifiedValue;

    while(1){
        xTaskNotifyWait(0x00, ULONG_MAX, &NotifiedValue, portMAX_DELAY);
        if(((uint32_t)(1 << OnBoard_Buttons[Button_Top].Notification_Bit)) & NotifiedValue){
            OnBoard_Buttons[Button_Top].press_function();
        }
        if(((uint32_t)(1 << OnBoard_Buttons[Button_Bottom].Notification_Bit)) & NotifiedValue){
            OnBoard_Buttons[Button_Bottom].press_function();
        }
        if(((uint32_t)(1 << OnBoard_Buttons[Button_Middle].Notification_Bit)) & NotifiedValue){
            OnBoard_Buttons[Button_Middle].press_function();
        }
        if(((uint32_t)(1 << OnBoard_Buttons[Button_Left].Notification_Bit)) & NotifiedValue){
            OnBoard_Buttons[Button_Left].press_function();
        }
        if(((uint32_t)(1 << OnBoard_Buttons[Button_Right].Notification_Bit)) & NotifiedValue){
            OnBoard_Buttons[Button_Right].press_function();
        }
    }
}

void StartAllTasks(){
    uint8_t i;
    uint32_t mask = 1;
    for(i = 0; i < NNXT_MAX_USER_TASKS; i++, mask <<=1){
        if((mask&TasksSlots) && registeredTasks[i].handle != NULL){
            vTaskResume(registeredTasks[i].handle);
        }
    }
}
