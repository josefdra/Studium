/**
 * @file
 * @brief
 * nnxt_debug.h
 *
 *  Created on: 18.01.2017
 *      Author: florian
 */

#ifndef NXT_APPLICATIONDRIVER_INC_NNXT_DEBUG_H_
#define NXT_APPLICATIONDRIVER_INC_NNXT_DEBUG_H_

#include "nnxt_config.h"
#if (NXT_DEV_DEBUG_LEVEL >= 2)
#include "SEGGER_SYSVIEW.h"
#else
/**
 * @defgroup DummyMacros_Segger
 * @brief Dummy macros for having calls to Seggers's SystemView tech without really including this. To know what the functions does, please call Segger's homepage for more informations.
 * @{
 */
#define SEGGER_SYSVIEW_Start()

#define SEGGER_SYSVIEW_RecordEnterISR()
#define SEGGER_SYSVIEW_RecordExitISR()
#define SEGGER_SYSVIEW_OnUserStart(a)
#define SEGGER_SYSVIEW_OnUserStop(a)

#define SEGGER_SYSVIEW_Conf()

/**
 * @}
 */

#endif


/*********************************/
/*      Function prototypes      */
/**
 * @brief Prints a message via the debug chain
 * @param s   The string to print
 * @param len The length of the string
 *
 * You must set at least two macros to determine the behaviour for this function. The macros can be set and are explained int nnxt_config.h
 *
 * Macro | Value | Description
 * -----| ----- | -----
 * #NXT_DEV_DEBUG_ENABLE_TRACE | >1 | Enabling trace output, you must additonally set the debug level
 * #NXT_DEV_DEBUG_LEVEL | <1 | no output
 * #NXT_DEV_DEBUG_LEVEL | 1 | trace output via SWO
 * #NXT_DEV_DEBUG_LEVEL | 2 | trace output via SEGGER SystemView technology. The output can only be watched with Seggers SystemViwer
 */
static inline void print_debug(const char *s, uint32_t len){
#if (NXT_DEV_DEBUG_ENABLE_TRACE >= 1)
	#if (NXT_DEV_DEBUG_LEVEL >= 2)
		SEGGER_SYSVIEW_Print(s);
	#elif (NXT_DEV_DEBUG_LEVEL == 1)
		for(uint32_t i = 0; i < len; i++){
			ITM_SendChar(*s++);
		}
	#endif
#endif
}


#endif /* NXT_APPLICATIONDRIVER_INC_NNXT_DEBUG_H_ */
