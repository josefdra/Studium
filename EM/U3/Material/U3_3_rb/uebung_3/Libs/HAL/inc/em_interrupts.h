/**
 * @file
 */

#ifndef __STM32F4xx_IT_H
#define __STM32F4xx_IT_H

#ifdef __cplusplus
 extern "C" {
#endif

#include "nnxt.h"

#if(NNXT_OS==ERIKA)
void EE_CORTEX_MX_NMI_ISR(void);
#else
void NMI_Handler(void);
#endif

#if(NNXT_OS==ERIKA)
void EE_CORTEX_MX_HARD_FAULT_ISR(void);
#else
void HardFault_Handler(void);
#endif

#if(NNXT_OS==ERIKA)
void EE_CORTEX_MX_MPU_FAULT_ISR(void);
#else
void MemManage_Handler(void);
#endif

#if(NNXT_OS==ERIKA)
void EE_CORTEX_MX_BUS_FAULT_ISR(void);
#else
void BusFault_Handler(void);
#endif

#if(NNXT_OS==ERIKA)
void EE_CORTEX_MX_USAGE_FAULT_ISR(void);
#else
void UsageFault_Handler(void);
#endif

#if(NNXT_OS==ERIKA)
void EE_cortex_mx_svc_ISR(void);
#elif(NNXT_OS==FREERTOS) //do nothing, the svc handler coming with the os in this mode
#else
void SVC_Handler(void);
#endif

#if(NNXT_OS==ERIKA)
void EE_CORTEX_MX_DEBUG_MONITOR_ISR(void);
#else
void DebugMon_Handler(void);
#endif

#if(NNXT_OS==ERIKA)
void EE_cortex_mx_pendsv_ISR(void);
#elif(NNXT_OS==FREERTOS) //do nothing, the pendsv handler coming with freertos in this mode
#else
void PendSV_Handler(void);
#endif

#if(NNXT_OS==ERIKA)    //ignroing systick handler with erika
#warning "SystickHandler Erika!"
#elif(NNXT_OS==FREERTOS) //do nothing, the systick handler coming with freertos in this mode
#else
void SysTick_Handler(void);
#endif

#if(NNXT_OS==ERIKA)
void EE_CORTEX_MX_EXTI0_ISR(void);
#else
void EXTI0_IRQHandler(void);
#endif

#if(NNXT_OS==ERIKA)
void EE_CORTEX_MX_EXTI1_ISR(void);
#else
void EXTI1_IRQHandler(void);
#endif

#if(NNXT_OS==ERIKA)
void EE_CORTEX_MX_EXTI2_ISR(void);
#else
void EXTI2_IRQHandler(void);
#endif

#if(NNXT_OS==ERIKA)
void EE_CORTEX_MX_EXTI3_ISR(void);
#else
void EXTI3_IRQHandler(void);
#endif

#if(NNXT_OS==ERIKA)
void EE_CORTEX_MX_EXTI4_ISR(void);
#else
void EXTI4_IRQHandler(void);
#endif

#if(NNXT_OS==ERIKA)
void EE_CORTEX_MX_EXTI15_10_ISR(void);
#else
void EXTI15_10_IRQHandler(void);
#endif

#if(NNXT_OS==ERIKA)
void EE_CORTEX_MX_EXTI9_5_ISR(void);
#else
void EXTI9_5_IRQHandler(void);
#endif

#ifdef __cplusplus
}
#endif

#endif /* __STM32F4xx_IT_H */
