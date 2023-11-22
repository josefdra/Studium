/**
 * @file
 * @brief
 * nnxt_os_glue.h
 *
 *  Created on: 13.06.2017
 *      Author: florian
 */

#ifndef NXT_APPLICATIONDRIVER_INC_NNXT_OS_GLUE_H_
#define NXT_APPLICATIONDRIVER_INC_NNXT_OS_GLUE_H_
/*********************************/
/*           includes            */
#include <stdint.h>
#include "nnxt_config.h"
#include "stm32f4xx_tim.h"

/*********************************/
/*           macros              */

/**
 * @defgroup OS
 * @brief This section is to sum up all operating systems relevant stuff.
 * @{
 */

/**
 * @defgroup OS_Selection
 * @brief All OS compatiblity we offer
 * @{
 */
/// Macro for using the library with the FREERTOS OS.
#define FREERTOS 2
/// Macro for using the library without OS. Some features will not work in this mode!
#define BAREMETAL 1
/// Macro for using the library with Erika Osek OS.
#define ERIKA 0

/**
 * @}
 */

/**
 * @defgroup OS_Settings
 * @brief All Settings which are available for all OSs.
 * @{
 */
/**
 * @brief The stack depth all tasks use
 */
#define NNXT_STACKDEPTH_DEFAULT         1024
/**
 * @brief All user tasks run with this priority!
 */
#define NNXT_USERTASKS_PRIO_DEFAULT     1

/**
 * @brief The maximum number of tasks a user can create
 * @warning This constant must not be bigger than 31!
 */
#define NNXT_MAX_USER_TASKS 15

/**
 * @}
 */

/*********************************/
/*      extern variables         */

/*********************************/
/*          enums                */

/*********************************/
/*        typedefs               */

/**
 * @brief To use nnxt_tasks instead of void* in function definitions
 */
typedef void (*nnxt_task)( void * );
typedef void * NNXT_TaskHandle;

/*********************************/
/*          structs              */
struct registeredTasks_s{
    nnxt_task task;
    NNXT_TaskHandle handle;
};

/*********************************/
/*      Function prototypes      */

/**
 * @defgroup OS_Functions
 * @brief All functions you can call which are related to OS functionality. This includes Scheduler and also timing things. Most functions must be implemented outside of this scope.
 * @{
 */

/**
 * @brief      Starts the scheduler of the operating system. If there is no scheduler (e.g programming bare metal) you will get a compiler error.
 *             This function sets all until this call created task (with #CreateTask) into the running queue, so the tasks will be executed directly after this call
 *
 * @warning    Take care to call this function only once. If you call it twice or more often there can be some magic and nobody knows what happen then!
 * @developer  This function must be implemented in the os specific part.
 */
static inline void StartScheduler() __attribute__((always_inline));

/**
 * @brief      Creates a task which should be scheduled. All tasks will have the same priority, that means, that all tasks will be scheduled in a round robin order. The name of the task corresponds to the name of the function.
 *             #CreateTask just creates the task. The task will not be executed until a call to #StartTask
 *
 * @param[in]  task      A function pointer to the function which should be treated as a task. Just type in the function name without braces, then it will work!
 * @warning    With this function you will have just a stack depth of 1k (defined in #NNXT_STACKDEPTH_DEFAULT)! So take car how much memory you use!
 * @developer  This function musst be implemented in the os specific part.
 */
static inline void CreateTask(nnxt_task task) __attribute__((always_inline));

/**
 * @brief      Creates a task with a user defined stackdepth. The other behaviour is the same like in #CreateTask. The name of the task corresponds to the name of the function.
 *             #CreateTaskStack just creates the task. The task will not be executed until a call to #StartTask
 *
 * @param[in]  task        A function pointer to the function which should be treated as a task.
 * @param[in]  stackdepth  The size of the stack for the task. Take car of you much you use, you have 128k of RAM and there are some other things which needs RAM!
 * @developer  This function must be implemented in the os specific part.
 */
static inline void CreateTaskStack(nnxt_task task, const uint16_t stackdepth) __attribute__((always_inline));

/**
 * @brief      Creates a task with full control over all parameters. The name of the task corresponds to the name of the function.
 *             #CreateTaskFull just creates the task. The task will not be executed until a call to #StartTask
 *
 * @param[in]  task           The task you want to add. This is usually a function pointer (just give the function name!)
 * @param[in]  stackdepth     The stackdepth the task will get
 * @param      parameters     The parameters you want to pass to the task
 * @param[in]  priority       The priority the tasks run with
 * @param      pxCreatedTask  The handle to the task, you can use this handle to send notification to it.
 */
static inline void CreateTaskFull(nnxt_task task, const uint16_t stackdepth, void * const parameters, uint32_t priority, NNXT_TaskHandle * const pxCreatedTask) __attribute__((always_inline));

/**
 * @brief Insert a busy waiting of \p ms.
 * @param ms The ms to wait.
 * @todo    take care of overflows!
 * @developer This function must be implemented in the os specific part.
 */
inline void Delay(const uint32_t ms) __attribute__((always_inline));

/**
 * @brief Makes a Systemtick. Is used for counting and delaying in ms.
 * @attention Don't call from user code, it destroy's time.
 * @attention The value of the SystemTick switches back to 0 after calling #StartScheduler. To have a absoulte time since start up of the controller(usually you do not need this) save the current time and then adding it to the new time.
 *
 * @developer This function cannot be inlined because it is used as a interrupt service routine.
 * @developer This function will called from a Interrupt Service Routine, so be aware when changing things in it!
 * @developer This function must be implemented in the os specific part.
 */
void MakePreOSSysTick(void);

/**
 * @brief Insert a busy waiting of \p us.
 * @param us The us to wait.
 * @warning just 16 bit variable! If you need more time, us #Delay
 */
inline void Delay_us(const uint16_t us) __attribute__((always_inline));

/**
 * @brief Gets the actual system time in ms. This can be used for measurement timing, delays or timeouts.
 * @warning The system time switchtes back to 0 after calling #StartScheduler. If you need the absolute system time since startup you need to save the time before calling #StartScheduler.
 * @return The actual System time in ms.
 */
inline uint32_t GetSysTime() __attribute__((always_inline));

/**
 * @brief      The background task running in the system. This task will sleep normally and will be waked up by events.
 */
void OS_BackgroundTask();

/**
 * @brief Sets all tasks defined by the user into the ready queue.
 */
void StartAllTasks();

/**
 * @brief Stops all tasks which are created with #CreateTask. Wake up will be performend through a call to #StartAllTasks
 */
static inline void StopAllTasks() __attribute((always_inline));

/**
 * @brief      Deletes the given task. The task will never be executed again and all memory of the task will be freed.
 *
 * @param[in]  task  The task to delete
 */
static inline void DeleteTask(nnxt_task task) __attribute__((always_inline));

/**
 * @brief      Deletes all tasks created by the user
 */
static inline void DeleteAllTasks() __attribute__((always_inline));

/**
 * @brief      Stops the given task. The task can be started again (will continue from the same line of code on which it stands when it was stopped).
 *
 * @param[in]  task  The task to stop immediatly.
 * @warning    You need to ensure that the task is not stopped when calling this function (e.g. when you call this function several times).
 */
static inline void StopTask(nnxt_task task) __attribute((always_inline));

/**
 * @brief      Starts the given task. A task needs to be created before it can be started.
 *
 * @param[in]  task  The task
 */
static inline void StartTask(nnxt_task task) __attribute((always_inline));

/**
 * @brief      Creates a task via #CreateTask and starts its immediatly after that
 *
 * @param      task  The task (function pointer) to start
 */
#define CreateAndStartTask(task) {CreateTask(task); StartTask(task);}

/**
 * @}
 */

/********************************************************/
/* inline functions implementation                      */

/**
 * @defgroup OS_independent_implementations
 * @brief All implementations for functions which not depends on the operating system.
 * @{
 */

inline void Delay_us(const uint16_t us){
    TIM_ClearFlag(NXT_PRECISE_TIMER_TIM, TIM_FLAG_Update);  //first reset the update flag, if it is set. We need this later to check overflows
    uint32_t a = TIM_GetCounter(NXT_PRECISE_TIMER_TIM);
    uint32_t delta_t = (uint32_t)a + (uint32_t)us;
    uint32_t rest = delta_t;
    if(delta_t > NXT_PRECISE_TIMER_TIM_MAXVAL){ //timer overflow handling
        while(TIM_GetFlagStatus(NXT_PRECISE_TIMER_TIM, TIM_FLAG_Update) != SET);    //waiting until the timer overflows
        TIM_ClearFlag(NXT_PRECISE_TIMER_TIM, TIM_FLAG_Update);
        rest = delta_t - NXT_PRECISE_TIMER_TIM_MAXVAL;
    }
    while(TIM_GetCounter((NXT_PRECISE_TIMER_TIM)) < rest);
}

/**
 * @}
 */


#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wcpp" //ignore for warning in which os mode we are
#if(NNXT_OS==ERIKA)
    // the interrupt handler routines foloowing the scheme EE_CORTEX_MX_[InterruptName]_ISR
    #warning "Using the nnxt library in Erika mode"
    #include "ee.h"
#elif(NNXT_OS==BAREMETAL)
    #warning "Using the nnxt library in bare metal mode"
#elif(NNXT_OS==FREERTOS)
    #warning "Using the nnxt library in FreeRTOS mode"
    #include "FreeRTOS_glue.h"
#else
    #warning "Having no configuration how to use this library! Using this library in bare metal mode..."
#endif
#pragma GCC diagnostic pop

/**
 * @}
 */

#endif /* NXT_APPLICATIONDRIVER_INC_NNXT_OS_GLUE_H_ */
