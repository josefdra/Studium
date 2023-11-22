/*
 * nnxt_OnBoardPeriph.c
 *
 *  Created on: 21.12.2016
 *      Author: florian
 */

/*********************************/
/*           includes            */
#include "nnxt_config.h"
#include "nnxt_OnBoardPeriph.h"
#include "em_clk.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_tim.h"
#include "nnxt_common.h"
#include "nnxt_os_glue.h"

/*********************************/
/*           macros              */

/*********************************/
/*      extern variables         */

/*********************************/
/*          enums                */

/*********************************/
/*          structs              */

/*********************************/
/*      private variables        */
static const uController_pin_t OnBoard_LEDs[NXT_LEDn] = {
	{NXT_OnBoard_LED0_Pin, NXT_OnBoard_LED0_Port, NXT_OnBoard_LED0_Clock},
	{NXT_OnBoard_LED1_Pin, NXT_OnBoard_LED1_Port, NXT_OnBoard_LED1_Clock},
	{NXT_OnBoard_LED2_Pin, NXT_OnBoard_LED2_Port, NXT_OnBoard_LED2_Clock},
	{NXT_OnBoard_LED3_Pin, NXT_OnBoard_LED3_Port, NXT_OnBoard_LED3_Clock}
};

volatile  OnBoard_Buttons_t OnBoard_Buttons[NXT_BUTTONn] = {
    {{NXT_OnBoard_BUTTONM_Pin, NXT_OnBoard_BUTTONM_Port, NXT_OnBoard_BUTTONM_Clock}, 0, NXT_OnBoard_BUTTONM_EXTI_Port, NXT_OnBoard_BUTTONM_EXTI_PinSource, NXT_OnBoard_BUTTONM_IRQChannel, NXT_OnBoard_BUTTONM_EXTI_Line, NULL},
    {{NXT_OnBoard_BUTTONU_Pin, NXT_OnBoard_BUTTONU_Port, NXT_OnBoard_BUTTONU_Clock}, 1, NXT_OnBoard_BUTTONU_EXTI_Port, NXT_OnBoard_BUTTONU_EXTI_PinSource, NXT_OnBoard_BUTTONU_IRQChannel, NXT_OnBoard_BUTTONU_EXTI_Line, NULL},
    {{NXT_OnBoard_BUTTONB_Pin, NXT_OnBoard_BUTTONB_Port, NXT_OnBoard_BUTTONB_Clock}, 2, NXT_OnBoard_BUTTONB_EXTI_Port, NXT_OnBoard_BUTTONB_EXTI_PinSource, NXT_OnBoard_BUTTONB_IRQChannel, NXT_OnBoard_BUTTONB_EXTI_Line, NULL},
    {{NXT_OnBoard_BUTTONL_Pin, NXT_OnBoard_BUTTONL_Port, NXT_OnBoard_BUTTONL_Clock}, 3, NXT_OnBoard_BUTTONL_EXTI_Port, NXT_OnBoard_BUTTONL_EXTI_PinSource, NXT_OnBoard_BUTTONL_IRQChannel, NXT_OnBoard_BUTTONL_EXTI_Line, NULL},
    {{NXT_OnBoard_BUTTONR_Pin, NXT_OnBoard_BUTTONR_Port, NXT_OnBoard_BUTTONR_Clock}, 4, NXT_OnBoard_BUTTONR_EXTI_Port, NXT_OnBoard_BUTTONR_EXTI_PinSource, NXT_OnBoard_BUTTONR_IRQChannel, NXT_OnBoard_BUTTONR_EXTI_Line, NULL}
};
/*********************************/
/*    Private functions          */
static inline uint8_t CheckButtonBorder(onboardbutton_t b);

/*********************************/
/*    Function implementations   */
void OnBoardPeriph_LED_Init(onboardled_t led){
	GPIO_InitTypeDef GPIO_InitStructure;

	ENABLE_CLOCK_IO(OnBoard_LEDs[led].clock, ENABLE);
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Pin = OnBoard_LEDs[led].pin;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed = NXT_GPIO_GenSpeed;

	GPIO_Init(OnBoard_LEDs[led].port, &GPIO_InitStructure);
	GPIO_ResetBits(OnBoard_LEDs[led].port, OnBoard_LEDs[led].pin);
}

void OnBoardPeriph_LED_SetNewState(onboardled_t led, onboardled_state_t state){
	GPIO_WriteBit(OnBoard_LEDs[led].port, OnBoard_LEDs[led].pin, state);
}

void OnBoardPeriph_LED_Toggle(onboardled_t led){
	GPIO_ToggleBits(OnBoard_LEDs[led].port, OnBoard_LEDs[led].pin);
}

void OnBoardPeriph_Button_Init(onboardbutton_t button){
    EXTI_InitTypeDef EXTI_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	ENABLE_CLOCK_IO(OnBoard_Buttons[button].pin.clock, ENABLE);
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Pin = OnBoard_Buttons[button].pin.pin;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed = NXT_GPIO_GenSpeed;

    GPIO_Init(OnBoard_Buttons[button].pin.port, &GPIO_InitStructure);

	SYSCFG_EXTILineConfig(OnBoard_Buttons[button].ExtiPortSource, OnBoard_Buttons[button].ExtiPinSource);

	EXTI_InitStructure.EXTI_Line = OnBoard_Buttons[button].ExtiLine;
	EXTI_InitStructure.EXTI_LineCmd = DISABLE;
	EXTI_Init(&EXTI_InitStructure);

	NVIC_InitStructure.NVIC_IRQChannel = OnBoard_Buttons[button].IRQChannel;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = NXT_BUTTONn_PushButtonInterruptPrio;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = NXT_BUTTONn_PushButtonInterruptSubprio;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

onboardbutton_state_t OnBoardPeriph_Button_GetState(onboardbutton_t button){
	uint8_t button_state = GPIO_ReadInputDataBit(OnBoard_Buttons[button].pin.port, OnBoard_Buttons[button].pin.pin);
	return (button_state != 0) ? Button_released : Button_pressed;
}

void OnBoardPeriph_Button_ConnectFunction(onboardbutton_t button, void (*ButtonFunction)(void)){
    EXTI_InitTypeDef EXTI_InitStructure;
    OnBoard_Buttons[button].press_function = ButtonFunction;
    EXTI_InitStructure.EXTI_Line = OnBoard_Buttons[button].ExtiLine;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);
}

void OnBoardPeriph_Button_RemoveFunction(onboardbutton_t button){
    EXTI_InitTypeDef EXTI_InitStructure;
    OnBoard_Buttons[button].press_function = NULL;
    EXTI_InitStructure.EXTI_Line = OnBoard_Buttons[button].ExtiLine;
    EXTI_InitStructure.EXTI_LineCmd = DISABLE;
    EXTI_Init(&EXTI_InitStructure);
}

void OnBoardPeriph_I2CMux_Init(){
	GPIO_InitTypeDef GPIO_InitStructure;

	ENABLE_CLOCK_IO(NXT_OnBoard_I2CMUX_Clock, ENABLE);

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Pin = NXT_OnBoard_I2CMUX_Pin;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed = NXT_GPIO_GenSpeed;

	GPIO_Init(NXT_OnBoard_I2CMUX_Port, &GPIO_InitStructure);
}

void OnBoardPeriph_I2CMux_Reset(){
	GPIO_ResetBits(NXT_OnBoard_I2CMUX_Port, NXT_OnBoard_I2CMUX_Pin);
	Delay_us(5);
	GPIO_SetBits(NXT_OnBoard_I2CMUX_Port, NXT_OnBoard_I2CMUX_Pin);
	Delay_us(10);
}

void OnBoardPeriph_Beep_Init(){
	GPIO_InitTypeDef GPIO_Set;
	TIM_TimeBaseInitTypeDef tim_set;
	TIM_OCInitTypeDef tim_oc;

	ENABLE_CLOCK_IO(NXT_OnBoard_BUZZER_Clock, ENABLE);
	NXT_OnBoard_BUZZER_RCCConfig(NXT_OnBoard_BUZZER_TIM_Clock, ENABLE);

	GPIO_Set.GPIO_Mode = GPIO_Mode_AF;
	GPIO_Set.GPIO_OType = GPIO_OType_PP;
	GPIO_Set.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Set.GPIO_Speed = NXT_GPIO_GenSpeed;
	GPIO_Set.GPIO_Pin = NXT_OnBoard_BUZZER_Pin;
	GPIO_Init(NXT_OnBoard_BUZZER_Port, &GPIO_Set);

	GPIO_PinAFConfig(NXT_OnBoard_BUZZER_Port, NXT_OnBoard_BUZZER_AFPinSource, NXT_OnBoard_BUZZER_AFConfig);

	tim_set.TIM_ClockDivision = TIM_CKD_DIV1;
	tim_set.TIM_CounterMode = TIM_CounterMode_Up;
	tim_set.TIM_Period = NXT_OnBoard_BUZZER_PeriodCalc;
	tim_set.TIM_Prescaler = 0;
	tim_set.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(NXT_OnBoard_BUZZER_TIM, &tim_set);
	TIM_Cmd(NXT_OnBoard_BUZZER_TIM, ENABLE);
	// settings for pwm output
	tim_oc.TIM_OCMode = TIM_OCMode_PWM2;
	tim_oc.TIM_OutputState = TIM_OutputState_Disable;
	tim_oc.TIM_OutputNState = TIM_OutputNState_Enable;
	tim_oc.TIM_OCPolarity = TIM_OCPolarity_High;
	tim_oc.TIM_OCNPolarity = TIM_OCNPolarity_Low;
	tim_oc.TIM_Pulse = 0;	// disable at start
	tim_oc.TIM_OCIdleState = TIM_OCIdleState_Reset;
	tim_oc.TIM_OCNIdleState = TIM_OCNIdleState_Reset;

	TIM_OC3Init(NXT_OnBoard_BUZZER_TIM, &tim_oc);
	TIM_OC3PreloadConfig(NXT_OnBoard_BUZZER_TIM, TIM_OCPreload_Enable);
	TIM_CtrlPWMOutputs(NXT_OnBoard_BUZZER_TIM, ENABLE);	// enable function because using timer 8
}

void OnBoardPeriph_Beep(uint32_t f){
	TIM_SetCompare3(NXT_OnBoard_BUZZER_TIM, f);
}

void OnBoardPeriph_BeepCMD(onboardbeep_state_t s){
	TIM_Cmd(NXT_OnBoard_BUZZER_TIM, s);
}

/**
 * @brief      Checks if the given button is one of the available buttons
 *
 * @param[in]  b     The button to check, should be one of #nnxt_button_e
 *
 * @return     1 if the button is one of the availables, 0 else.
 */
static inline uint8_t CheckButtonBorder(onboardbutton_t b){
    if((b == Button_Bottom) || (b == Button_Top) || (b == Button_Middle) || (b == Button_Left) || (b == Button_Right)) return 1;
    else return 0;
}

