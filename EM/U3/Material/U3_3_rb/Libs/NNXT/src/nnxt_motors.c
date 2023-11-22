/**!
 * @file
 * nnxt_motors.c
 *
 * @todo some refactoring in terms of the motor designation. There are some problems in naming
 */

/*********************************/
/*           includes            */
#include "stm32f4xx_gpio.h"
#include "nnxt_config.h"
#include "nnxt_Ports.h"
#include "stm32f4xx_tim.h"
#include "em_clk.h"
#include "stm32f4xx_exti.h"
#include "nnxt_common.h"

/*********************************/
/*           macros              */

/*********************************/
/*      extern variables         */

/*********************************/
/*          enums                */

/*********************************/
/*          structs              */

struct timer_s{
	uint32_t clock;
	void(*rcc_pointer)(uint32_t, FunctionalState);
	TIM_TypeDef* timer;
	uint8_t pinsource;
	uint8_t gpio_af;

};

struct motor_port_config_s{
	const uController_pin_t pwm;
	const uController_pin_t mode1;
	const uController_pin_t mode2;
	const struct timer_s	timer;
	const uController_pin_t encoder_pin_a;
	const uController_pin_t encoder_pin_b;
	const uint8_t Encoder_A_ExtiPortSource;
	const uint8_t Encoder_A_ExtiPinSource;
	const uint32_t Encoder_A_ExtiLine;
	const uint8_t Encoder_A_IRQ_Channel;
	const uint8_t Encoder_B_ExtiPortSource;
	const uint8_t Encoder_B_ExtiPinSource;
	const uint32_t Encoder_B_ExtiLine;
	const uint8_t Encoder_B_IRQ_Channel;
};

/*********************************/
/*        typedefs               */
typedef struct motor_port_config_s motor_port_config_t;

/*********************************/
/*      private variables        */
static const motor_port_config_t MotorPorts[NXT_MOTORn] ={
		{
				{NXT_MOTORA_PWM_Pin, NXT_MOTORA_PWM_Port, NXT_MOTORA_PWM_Clock},
				{NXT_MOTORA_MODE1_Pin, NXT_MOTORA_MODE1_Port, NXT_MOTORA_MODE1_Clock},
				{NXT_MOTORA_MODE2_Pin, NXT_MOTORA_MODE2_Port, NXT_MOTORA_MODE2_Clock},
				{NXT_MOTORA_PWM_Timer_Clock, NXT_MOTORA_PWM_Timer_RCCConfig, NXT_MOTORA_PWM_Timer, NXT_MOTORA_PWM_PinSource, NXT_MOTORA_PWM_TIMER_AF},
				{NXT_MOTORA_ENCODER_A_Pin, NXT_MOTORA_ENCODER_A_Port, NXT_MOTORA_ENCODER_A_Clock},
				{NXT_MOTORA_ENCODER_B_Pin, NXT_MOTORA_ENCODER_B_Port, NXT_MOTORA_ENCODER_B_Clock},
				NXT_MOTORA_ENCODER_A_EXTI_Port,
				NXT_MOTORA_ENCODER_A_EXTI_PinSource,
				NXT_MOTORA_ENCODER_A_EXTI_Line,
				NXT_MOTORA_ENCODER_A_IRQChannel,
				NXT_MOTORA_ENCODER_B_EXTI_Port,
				NXT_MOTORA_ENCODER_B_EXTI_PinSource,
				NXT_MOTORA_ENCODER_B_EXTI_Line,
				NXT_MOTORA_ENCODER_B_IRQChannel,
		},
		{
				{NXT_MOTORB_PWM_Pin, NXT_MOTORB_PWM_Port, NXT_MOTORB_PWM_Clock},
				{NXT_MOTORB_MODE1_Pin, NXT_MOTORB_MODE1_Port, NXT_MOTORB_MODE1_Clock},
				{NXT_MOTORB_MODE2_Pin, NXT_MOTORB_MODE2_Port, NXT_MOTORB_MODE2_Clock},
				{NXT_MOTORB_PWM_Timer_Clock, NXT_MOTORB_PWM_Timer_RCCConfig, NXT_MOTORB_PWM_Timer, NXT_MOTORB_PWM_PinSource, NXT_MOTORB_PWM_TIMER_AF},
				{NXT_MOTORB_ENCODER_A_Pin, NXT_MOTORB_ENCODER_A_Port, NXT_MOTORB_ENCODER_A_Clock},
				{NXT_MOTORB_ENCODER_B_Pin, NXT_MOTORB_ENCODER_B_Port, NXT_MOTORB_ENCODER_B_Clock},
				NXT_MOTORB_ENCODER_A_EXTI_Port,
				NXT_MOTORB_ENCODER_A_EXTI_PinSource,
				NXT_MOTORB_ENCODER_A_EXTI_Line,
				NXT_MOTORB_ENCODER_A_IRQChannel,
                NXT_MOTORB_ENCODER_B_EXTI_Port,
				NXT_MOTORB_ENCODER_B_EXTI_PinSource,
				NXT_MOTORB_ENCODER_B_EXTI_Line,
				NXT_MOTORB_ENCODER_B_IRQChannel,
		},
		{
				{NXT_MOTORC_PWM_Pin, NXT_MOTORC_PWM_Port, NXT_MOTORC_PWM_Clock},
				{NXT_MOTORC_MODE1_Pin, NXT_MOTORC_MODE1_Port, NXT_MOTORC_MODE1_Clock},
				{NXT_MOTORC_MODE2_Pin, NXT_MOTORC_MODE2_Port, NXT_MOTORC_MODE2_Clock},
				{NXT_MOTORC_PWM_Timer_Clock, NXT_MOTORC_PWM_Timer_RCCConfig, NXT_MOTORC_PWM_Timer, NXT_MOTORC_PWM_PinSource, NXT_MOTORC_PWM_TIMER_AF},
				{NXT_MOTORC_ENCODER_A_Pin, NXT_MOTORC_ENCODER_A_Port, NXT_MOTORC_ENCODER_A_Clock},
				{NXT_MOTORC_ENCODER_B_Pin, NXT_MOTORC_ENCODER_B_Port, NXT_MOTORC_ENCODER_B_Clock},
				NXT_MOTORC_ENCODER_A_EXTI_Port,
				NXT_MOTORC_ENCODER_A_EXTI_PinSource,
				NXT_MOTORC_ENCODER_A_EXTI_Line,
				NXT_MOTORC_ENCODER_A_IRQChannel,
                NXT_MOTORC_ENCODER_B_EXTI_Port,
				NXT_MOTORC_ENCODER_B_EXTI_PinSource,
				NXT_MOTORC_ENCODER_B_EXTI_Line,
				NXT_MOTORC_ENCODER_B_IRQChannel,
		}
};

volatile motor_port_info Motor_Info[NXT_MOTORn];

/*********************************/
/*      functions                */

void MotorPortInit(motorport_t port){
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_BaseStruct;
	TIM_OCInitTypeDef TIM_OCStruct;
	EXTI_InitTypeDef EXTI_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	//enabling all clocks for the ports
	ENABLE_CLOCK_IO(MotorPorts[port].mode1.clock, ENABLE);
	ENABLE_CLOCK_IO(MotorPorts[port].mode2.clock, ENABLE);
	ENABLE_CLOCK_IO(MotorPorts[port].pwm.clock, ENABLE);
	MotorPorts[port].timer.rcc_pointer(MotorPorts[port].timer.clock, ENABLE);
	ENABLE_CLOCK_IO(MotorPorts[port].encoder_pin_a.clock, ENABLE);
	ENABLE_CLOCK_IO(MotorPorts[port].encoder_pin_b.clock, ENABLE);

	//config motor control/drive pins and ports
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_InitStructure.GPIO_Speed = NXT_GPIO_GenSpeed;

	GPIO_InitStructure.GPIO_Pin = MotorPorts[port].mode1.pin;
	GPIO_Init(MotorPorts[port].mode1.port, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = MotorPorts[port].mode2.pin;
	GPIO_Init(MotorPorts[port].mode2.port, &GPIO_InitStructure);

	GPIO_SetBits(MotorPorts[port].mode1.port, MotorPorts[port].mode1.pin);
	GPIO_ResetBits(MotorPorts[port].mode2.port, MotorPorts[port].mode2.pin);

	GPIO_PinAFConfig(MotorPorts[port].pwm.port, MotorPorts[port].timer.pinsource, MotorPorts[port].timer.gpio_af);

	GPIO_InitStructure.GPIO_Pin = MotorPorts[port].pwm.pin;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;

	GPIO_Init(MotorPorts[port].pwm.port, &GPIO_InitStructure);

	//config the timer for the motor port for having pwm
	TIM_BaseStruct.TIM_Prescaler = 0;
	TIM_BaseStruct.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_BaseStruct.TIM_Period = NXT_MOTORn_Period;
	TIM_BaseStruct.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_BaseStruct.TIM_RepetitionCounter = 0;

	TIM_TimeBaseInit(MotorPorts[port].timer.timer, &TIM_BaseStruct);
	TIM_Cmd(MotorPorts[port].timer.timer, ENABLE);

	TIM_OCStruct.TIM_OCMode = TIM_OCMode_PWM2;	//clear on compare match
	TIM_OCStruct.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCStruct.TIM_OCPolarity = TIM_OCPolarity_Low;
	TIM_OCStruct.TIM_Pulse = 0;	//disable to begin

	TIM_OC1Init(MotorPorts[port].timer.timer, &TIM_OCStruct);
	TIM_OC1PreloadConfig(MotorPorts[port].timer.timer, TIM_OCPreload_Enable);

	//config the pins for counting revolutions
	//pin A is always for the external interrupt, pin B is just to determine the direction

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_Pin = MotorPorts[port].encoder_pin_a.pin;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
	//GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	//GPIO_InitStructure.GPIO_Speed = NXT_GPIO_GenSpeed;
	//GPIO_InitStructure.GPIO_Speed = GPIO_Speed_25MHz;
	//GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;

	GPIO_Init(MotorPorts[port].encoder_pin_a.port, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = MotorPorts[port].encoder_pin_b.pin;
	GPIO_Init(MotorPorts[port].encoder_pin_b.port, &GPIO_InitStructure);

	// connect exti line to the pin
	SYSCFG_EXTILineConfig(MotorPorts[port].Encoder_A_ExtiPortSource, MotorPorts[port].Encoder_A_ExtiPinSource);
	SYSCFG_EXTILineConfig(MotorPorts[port].Encoder_B_ExtiPortSource, MotorPorts[port].Encoder_B_ExtiPinSource);

	//configure exti
	EXTI_InitStructure.EXTI_Line = MotorPorts[port].Encoder_A_ExtiLine;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);

	EXTI_InitStructure.EXTI_Line = MotorPorts[port].Encoder_B_ExtiLine;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);


	NVIC_InitStructure.NVIC_IRQChannel = MotorPorts[port].Encoder_A_IRQ_Channel;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = NXT_MOTORn_RotaryInterrupt_Prio;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = NXT_MOTORn_RotaryInterrupt_SubPrio;
	NVIC_InitStructure.NVIC_IRQChannelCmd = DISABLE;
	NVIC_Init(&NVIC_InitStructure);

	NVIC_InitStructure.NVIC_IRQChannel = MotorPorts[port].Encoder_B_IRQ_Channel;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = NXT_MOTORn_RotaryInterrupt_Prio;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = NXT_MOTORn_RotaryInterrupt_SubPrio;
	NVIC_InitStructure.NVIC_IRQChannelCmd = DISABLE;
	NVIC_Init(&NVIC_InitStructure);
}

sensor_error_t Motor_Drive(motorport_t port, motor_dir_t dir, uint8_t dutycycle){
    sensor_error_t ret_var = sensor_error_NoError;

    switch (dir) {
		case Motor_dir_forward:
			GPIO_ResetBits(MotorPorts[port].mode1.port, MotorPorts[port].mode1.pin);
			GPIO_SetBits(MotorPorts[port].mode2.port, MotorPorts[port].mode2.pin);
			break;
		case Motor_dir_backward:
			GPIO_SetBits(MotorPorts[port].mode1.port, MotorPorts[port].mode1.pin);
			GPIO_ResetBits(MotorPorts[port].mode2.port, MotorPorts[port].mode2.pin);
			break;
		default:
			break;
	}

    TIM_SetCompare1(MotorPorts[port].timer.timer, dutycycle * 100);

    (dutycycle != 0) ? NVIC_EnableIRQ(MotorPorts[port].Encoder_A_IRQ_Channel) : NVIC_DisableIRQ(MotorPorts[port].Encoder_A_ExtiLine);
    (dutycycle != 0) ? NVIC_EnableIRQ(MotorPorts[port].Encoder_B_IRQ_Channel) : NVIC_DisableIRQ(MotorPorts[port].Encoder_B_ExtiLine);

    return ret_var;
}

sensor_error_t Motor_Stop(motorport_t port, motor_stop_t stop){
	sensor_error_t ret_var = sensor_error_NoError;

	TIM_SetCompare1(MotorPorts[port].timer.timer, 0);

	switch (stop) {
		case Motor_stop_break:
			GPIO_SetBits(MotorPorts[port].mode1.port, MotorPorts[port].mode1.pin);
			GPIO_SetBits(MotorPorts[port].mode2.port, MotorPorts[port].mode2.pin);
			break;
		case Motor_stop_float:
			GPIO_ResetBits(MotorPorts[port].mode1.port, MotorPorts[port].mode1.pin);
			GPIO_ResetBits(MotorPorts[port].mode2.port, MotorPorts[port].mode2.pin);
			break;
		default:
			break;
	}

	NVIC_DisableIRQ(MotorPorts[port].Encoder_A_IRQ_Channel);
	NVIC_DisableIRQ(MotorPorts[port].Encoder_B_IRQ_Channel);

	return ret_var;
}

sensor_error_t Motor_Tacho_GetCounter(const motorport_t port, uint32_t *counter){
	sensor_error_t ret_var = sensor_error_NoError;


	*counter = Motor_Info[port].counterEncoderA;
	//*counterB = Motor_Info[port].counterEncoderB;

	return ret_var;
}
