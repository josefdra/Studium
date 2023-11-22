/**
 * @file
 * @brief this file contains all datastructures which are used globally
 * nnxt_shared_ds.h
 *
 *  Created on: 17.01.2017
 *      Author: florian
 */

#ifndef NXT_APPLICATIONDRIVER_INC_NNXT_SHARED_DS_H_
#define NXT_APPLICATIONDRIVER_INC_NNXT_SHARED_DS_H_

#include "nnxt_Ports.h"
#include "nnxt_OnBoardPeriph.h"
#include "nnxt_config.h"
#include "nnxt_os_glue.h"

/// @todo
extern volatile motor_port_info Motor_Info[NXT_MOTORn];
/// This variable is to have a flag which is set during startup (until OS is started)
extern uint8_t nnxt_pre_os_flag;
/// This is a flag to show in which application mode we are. The application modes comes from the OS we execute.
extern const uint8_t appl_mode;
/// The interrupt vector talbe being in RAM with the original entities.
extern uint32_t vectorTable_RAM_orig[STM_VECTORTABLE_SIZE];
/// A flag to show that the OS is not started yed.
extern uint8_t nnxt_pre_os_flag;
/// The counter for time used before the OS is started
extern volatile uint32_t SysTickCounter;
/// The structure to hold all informations about the buttons.
extern volatile OnBoard_Buttons_t OnBoard_Buttons[NXT_BUTTONn];
/// A handle to the backgroundtask.
extern NNXT_TaskHandle NNXT_Background_Task;
/// All from the user created tasks will be stored here
extern struct registeredTasks_s registeredTasks[NNXT_MAX_USER_TASKS];
/// Every task has a slot. With this variable the occupied slots can be determined. 
extern uint32_t TasksSlots;
/// The number of actual created tasks
extern uint8_t NumberofTasks;


#endif /* NXT_APPLICATIONDRIVER_INC_NNXT_SHARED_DS_H_ */
