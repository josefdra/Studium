/**
 * @file
 * @brief
 * FreeRTOS_glue.h
 *
 *  Created on: 13.06.2017
 *      Author: florian
 */

#ifndef FREERTOS_FREERTOS_GLUE_H_
#define FREERTOS_FREERTOS_GLUE_H_
/*********************************/
/*           includes            */
#include "nnxt_os_glue.h"
#include "FreeRTOS.h"
#include "semphr.h"
#include "task.h"
#include "stm32f4xx.h"
#include "nnxt_shared_ds.h"
#include <stdint.h>

/*********************************/
/*           macros              */
#define GETTASKNAME(name) #name

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
#if(NNXT_OS==FREERTOS)

static inline uint8_t FindandPlaceFreeSlot(nnxt_task task){
    uint8_t freeslot = 0;
    uint32_t mask = 1;
    while(mask&TasksSlots){
        mask <<= 1;
        freeslot++;
    }
    TasksSlots |= mask;
    registeredTasks[freeslot].task = task;
    return freeslot;
}

static inline NNXT_TaskHandle GetTaskHandle(nnxt_task task){
    uint32_t mask = 1;
    uint8_t i;
    NNXT_TaskHandle th;
    for(i = 0; i < NNXT_MAX_USER_TASKS; i++, mask<<=1){
        if((mask&TasksSlots) && registeredTasks[i].task == task){
            th = registeredTasks[i].handle;
            break;
        }
    }
    return th;
}

static inline void StartScheduler(){
    __disable_irq();
    SCB->VTOR = (uint32_t)&vectorTable_RAM_orig;    //set vector table to the table which is in RAM but contains the same vector table like this in flash
    __DSB();
    __enable_irq();
    nnxt_pre_os_flag = 0;
    xTaskCreate(OS_BackgroundTask, GETTASKNAME(OS_BackgroundTask), 256, NULL, 2, &NNXT_Background_Task);  // and create the task for background, there is the software reset functions placed
    vTaskStartScheduler();
}

static inline void CreateTask(nnxt_task task){
    uint8_t freeslot = FindandPlaceFreeSlot(task);
    xTaskCreate(task, GETTASKNAME(task), NNXT_STACKDEPTH_DEFAULT, NULL, NNXT_USERTASKS_PRIO_DEFAULT, &registeredTasks[freeslot].handle);
    vTaskSuspend(registeredTasks[freeslot].handle);
}

static inline void CreateTaskStack(nnxt_task task, const uint16_t stackdepth){
    uint8_t freeslot = FindandPlaceFreeSlot(task);
    xTaskCreate(task, GETTASKNAME(task), stackdepth, NULL, NNXT_USERTASKS_PRIO_DEFAULT, &registeredTasks[freeslot].handle);
    vTaskSuspend(registeredTasks[freeslot].handle);
}

inline void Delay(const uint32_t ms){
    uint32_t act_counter = SysTickCounter;
    if(nnxt_pre_os_flag == 1){
            while(act_counter + ms > SysTickCounter);
    }else if(appl_mode == FREERTOS){
        vTaskDelay(pdMS_TO_TICKS(ms));
    }
}

static inline void CreateTaskFull(nnxt_task task, const uint16_t stackdepth, void * const parameters, uint32_t priority, NNXT_TaskHandle * const pxCreatedTask){
    uint8_t freeslot = FindandPlaceFreeSlot(task);
    xTaskCreate(task, GETTASKNAME(task), stackdepth, parameters, priority, pxCreatedTask);
    registeredTasks[freeslot].handle = *pxCreatedTask;
    vTaskSuspend(registeredTasks[freeslot].handle);
}

inline uint32_t GetSysTime(){
    uint32_t time = 0;
    if(!nnxt_pre_os_flag){
        time = xTaskGetTickCount() / portTICK_PERIOD_MS;
    }else{
        time = SysTickCounter;
    }
    return time;
}

static inline void StopAllTasks(){
    uint8_t i;
    uint32_t mask = 1;
    for(i = 0; i < NNXT_MAX_USER_TASKS; i++, mask <<=1){
        if((mask&TasksSlots) && registeredTasks[i].handle != NULL){
            vTaskSuspend(registeredTasks[i].handle);
        }
    }
}

static inline void StopTask(nnxt_task task){
    NNXT_TaskHandle th = GetTaskHandle(task);
    vTaskSuspend(th);
}

static inline void StartTask(nnxt_task task){
    NNXT_TaskHandle th = GetTaskHandle(task);
    vTaskResume(th);
}

static inline void DeleteTask(nnxt_task task){
    uint8_t i;
    uint32_t mask = 1;
    for(i = 0; i < NNXT_MAX_USER_TASKS; i++, mask<<=1){
        if((mask&TasksSlots) && registeredTasks[i].task == task){
            TasksSlots &= ~mask;
            registeredTasks[i].handle = NULL;
            vTaskDelete(registeredTasks[i].handle);
            break;
        }
    }
}

static inline void DeleteAllTasks(){
    uint8_t i = 0;
    for(i = 0; i < NNXT_MAX_USER_TASKS; i++){
        if(registeredTasks[i].handle != NULL)
            vTaskDelete(registeredTasks[i].handle);
    }
    TasksSlots = 0;
}

#else

#endif

#endif /* FREERTOS_FREERTOS_GLUE_H_ */
