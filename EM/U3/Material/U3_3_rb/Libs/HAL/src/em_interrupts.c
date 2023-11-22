/**
 *
 */

/* Includes ------------------------------------------------------------------*/
#include "em_interrupts.h"
#include "nnxt_misc.h"
#include "stm32f4xx_exti.h"
#include "nnxt_config.h"
#include "nnxt.h"
#include "nnxt_shared_ds.h"


/**
  * @brief   This function handles NMI exception.
  * @param  None
  * @retval None
  */

#if(NNXT_OS==ERIKA)
void EE_CORTEX_MX_NMI_ISR(void)
#else
void NMI_Handler(void)
#endif
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */

#if(NNXT_OS==ERIKA)
void EE_CORTEX_MX_HARD_FAULT_ISR(void)
#else
void HardFault_Handler(void)
#endif
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
#if(NNXT_OS==ERIKA)
void EE_CORTEX_MX_MPU_FAULT_ISR(void)
#else
void MemManage_Handler(void)
#endif
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
#if(NNXT_OS==ERIKA)
void EE_CORTEX_MX_BUS_FAULT_ISR(void)
#else
void BusFault_Handler(void)
#endif
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
#if(NNXT_OS==ERIKA)
void EE_CORTEX_MX_USAGE_FAULT_ISR(void)
#else
void UsageFault_Handler(void)
#endif
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
#if(NNXT_OS==ERIKA)
void EE_cortex_mx_svc_ISR(void)
{
}
#elif(NNXT_OS==FREERTOS)
#else
void SVC_Handler(void)
{
}
#endif
/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
#if(NNXT_OS==ERIKA)
void EE_CORTEX_MX_DEBUG_MONITOR_ISR(void)
#else
void DebugMon_Handler(void)
#endif
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
#if(NNXT_OS==ERIKA)
void EE_cortex_mx_pendsv_ISR(void)
{
}
#elif(NNXT_OS==FREERTOS)
#else
void PendSV_Handler(void)
{
}
#endif

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
#if(NNXT_OS==ERIKA) // We are ignoring this systick handler here. The handler is coming from erika
#elif(NNXT_OS==FREERTOS)    //do nothing
#else
void SysTick_Handler(void)
{
#if (NXT_DEV_DEBUG_LEVEL >= 3)
	SEGGER_SYSVIEW_RecordEnterISR();
#endif
	MakePreOSSysTick();
#if (NXT_DEV_DEBUG_LEVEL >= 3)
	SEGGER_SYSVIEW_RecordExitISR();
#endif
}
#endif

/**
 * @brief Interrupt handler for interrupts on exti line 0 (GPIO[A|B|C|D|E]_Pin0)
 * - counting rising edges from motor C
 */
#if(NNXT_OS==ERIKA)
void EE_CORTEX_MX_EXTI0_ISR(void)
#else
void EXTI0_IRQHandler(void)
#endif
{
    //Motor_Info[Port_C].counterEncoderA++;
    SEGGER_SYSVIEW_RecordEnterISR();
	//handles interrupt rotary of motor C
	if(EXTI_GetITStatus(NXT_MOTORC_ENCODER_A_EXTI_Line) != RESET){
		EXTI_ClearITPendingBit(NXT_MOTORC_ENCODER_A_EXTI_Line);
		Motor_Info[Port_C].counterEncoderA++;
	}
	if(EXTI_GetITStatus(NXT_MOTORC_ENCODER_B_EXTI_Line) != RESET)
    {
        EXTI_ClearITPendingBit(NXT_MOTORC_ENCODER_B_EXTI_Line);
        Motor_Info[Port_C].counterEncoderB++;

    }
	SEGGER_SYSVIEW_RecordExitISR();
}

#if(NNXT_OS==ERIKA)
void EE_CORTEX_MX_EXTI1_ISR(void);
#else
void EXTI1_IRQHandler(void)
#endif
{

}


#if(NNXT_OS==ERIKA)
void EE_CORTEX_MX_EXTI2_ISR(void);
#else
void EXTI2_IRQHandler(void)
#endif
{
    SEGGER_SYSVIEW_RecordEnterISR();
    //handles interrupt on Pushbutton up
    if(EXTI_GetITStatus(NXT_OnBoard_BUTTONU_EXTI_Line) != RESET)
    {
        EXTI_ClearITPendingBit(NXT_OnBoard_BUTTONU_EXTI_Line);
        if(nnxt_pre_os_flag)
        {
            NVIC_SystemReset();
        }
    #if(NNXT_OS==FREERTOS)
        xTaskNotifyFromISR(NNXT_Background_Task, (1 << OnBoard_Buttons[Button_Top].Notification_Bit), eSetBits, NULL);
    #else

    #endif
    }
    SEGGER_SYSVIEW_RecordExitISR();
}

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



/**
 * @brief Interrupt handler for interrupts on exti lines 10-15 (GPIO[A|B|C|D|E]_Pin[10|11|12|13|14|15])
 *  - Counting rising edges from motor B
 *  - Pushbutton down
 *  - Pushbutton right
 *  - Pushbutton left
 */
#if(NNXT_OS==ERIKA)
void EE_CORTEX_MX_EXTI15_10_ISR(void)
#else
void EXTI15_10_IRQHandler(void)
#endif
{
	SEGGER_SYSVIEW_RecordEnterISR();

	//handles interrupt rotary of motor B
	if(EXTI_GetITStatus(NXT_MOTORB_ENCODER_A_EXTI_Line) != RESET){
		EXTI_ClearITPendingBit(NXT_MOTORB_ENCODER_A_EXTI_Line);
		Motor_Info[Port_B].counterEncoderA++;
	}
	if(EXTI_GetITStatus(NXT_MOTORB_ENCODER_B_EXTI_Line) != RESET)
    {
        EXTI_ClearITPendingBit(NXT_MOTORB_ENCODER_B_EXTI_Line);
        Motor_Info[Port_B].counterEncoderB++;

    }
	if(EXTI_GetITStatus(NXT_OnBoard_BUTTONB_EXTI_Line) != RESET){
	    EXTI_ClearITPendingBit(NXT_OnBoard_BUTTONB_EXTI_Line);
#if(NNXT_OS==FREERTOS)
        xTaskNotifyFromISR(NNXT_Background_Task, (1 << OnBoard_Buttons[Button_Bottom].Notification_Bit), eSetBits, NULL);
#else

#endif
	}
    if(EXTI_GetITStatus(NXT_OnBoard_BUTTONR_EXTI_Line) != RESET){
        EXTI_ClearITPendingBit(NXT_OnBoard_BUTTONR_EXTI_Line);
#if(NNXT_OS==FREERTOS)
        xTaskNotifyFromISR(NNXT_Background_Task, (1 << OnBoard_Buttons[Button_Right].Notification_Bit), eSetBits, NULL);
#else

#endif
    }
    if(EXTI_GetITStatus(NXT_OnBoard_BUTTONM_EXTI_Line) != RESET){
        EXTI_ClearITPendingBit(NXT_OnBoard_BUTTONM_EXTI_Line);

#if(NNXT_OS==FREERTOS)
        if(nnxt_pre_os_flag)
         {
             OnBoard_Buttons[Button_Middle].press_function();
         }

        xTaskNotifyFromISR(NNXT_Background_Task, (1 << OnBoard_Buttons[Button_Middle].Notification_Bit), eSetBits, NULL);
#else

#endif
    }
	SEGGER_SYSVIEW_RecordExitISR();
}

/**
 * @brief Interrupt handler for interrupts on exti line 5 - 9 (GPIO[A|B|C|D|E]_Pin[5|6|7|8|9])
 * - Counting rising edges from motor A
 * - Pushbutton Left
 */
#if(NNXT_OS==ERIKA)
void EE_CORTEX_MX_EXTI9_5_ISR(void)
#else
void EXTI9_5_IRQHandler(void)
#endif
{
	SEGGER_SYSVIEW_RecordEnterISR();

	//handles interrupt rotary of motor A
	if(EXTI_GetITStatus(NXT_MOTORA_ENCODER_A_EXTI_Line) != RESET){
		EXTI_ClearITPendingBit(NXT_MOTORA_ENCODER_A_EXTI_Line);
		int8_t diff = Motor_Info[Port_A].counterEncoderB - Motor_Info[Port_A].counterEncoderA;
		//if(diff < 2 || diff > -2)
        //{
            Motor_Info[Port_A].counterEncoderA++;
        //}
	}
	if(EXTI_GetITStatus(NXT_MOTORA_ENCODER_B_EXTI_Line) != RESET)
    {
        EXTI_ClearITPendingBit(NXT_MOTORA_ENCODER_B_EXTI_Line);
        int8_t diff = Motor_Info[Port_A].counterEncoderB - Motor_Info[Port_A].counterEncoderA;
		//if(diff < 2 || diff > -2)
        //{
            Motor_Info[Port_A].counterEncoderB++;
        //}

    }
    if(EXTI_GetITStatus(NXT_OnBoard_BUTTONL_EXTI_Line) != RESET){
        EXTI_ClearITPendingBit(NXT_OnBoard_BUTTONL_EXTI_Line);
#if(NNXT_OS==FREERTOS)
        xTaskNotifyFromISR(NNXT_Background_Task, (1 << OnBoard_Buttons[Button_Left].Notification_Bit), eSetBits, NULL);
#else

#endif
    }
	SEGGER_SYSVIEW_RecordExitISR();
}
