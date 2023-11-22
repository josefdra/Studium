/**
 * @file
 * nnxt_OnBoardPeriph.h
 *
 *  Created on: 21.12.2016
 *      Author: florian
 */

#ifndef NXT_APPLICATIONDRIVER_INC_NNXT_ONBOARDPERIPH_H_
#define NXT_APPLICATIONDRIVER_INC_NNXT_ONBOARDPERIPH_H_
/*********************************/
/*           includes            */
#include "stm32f4xx_gpio.h"
#include "nnxt_common.h"

/** @addtogroup OnBoardPeripherals
  * @{
  */

/**
 * @addtogroup OnBoardPeripherals_LED
 * @{
 */

/** @defgroup LED_Macros
 * @{
 */
 #define OnBoardPeriph_LED_Off(led) 		OnBoardPeriph_LED_SetNewState(led, LED_OFF)
 #define OnBoardPeriph_LED_On(led) 		OnBoardPeriph_LED_SetNewState(led, LED_ON)
/**
 * @}
 */

/**
 * @defgroup LED_enumerations
 * @{
 */

/**
 * @brief The leds which are usable in user SW. Are available as numbers or with colors
 */
enum nnxt_led_e {
    NNXT_LED0 = 0,
    NNXT_LED1 = 1,
    NNXT_LED2 = 2,
    NNXT_LED3 = 3,
	NNXT_LED_Red = NNXT_LED0,
	NNXT_LED_Blue = NNXT_LED1,
	NNXT_LED_Green = NNXT_LED2,
	NNXT_LED_Orange = NNXT_LED3
};

/**
 * @brief The LED states.
 */
enum nnxt_led_state_e {
    LED_OFF = Bit_RESET,
    LED_ON = Bit_SET
};

/**
  * @}
  */

/**
 * @defgroup LED_typedefs
 * @{
 */

 typedef enum nnxt_led_e             onboardled_t;
 typedef enum nnxt_led_state_e       onboardled_state_t;

 /**
  * @}
  */

/**
 * @defgroup LED_functions
 * @ingroup NNXT_ALL_API_FUNCTIONS
 * @{
 */

/**
 * @brief Initialize the onboard user usable leds
 * @param led the led which shall be initialized, one see from #nnxt_led_e
 * @warning This function must not be called in user code! It will called during startup of the controller, after resets and after reprogramming.
 */
void OnBoardPeriph_LED_Init(onboardled_t led);

/**
 * @brief Sets the new state (on/off) for the given LED.
 * @param led   The LED which should be set on/off, see #nnxt_led_e
 * @param state The new state of the LED, see #nnxt_led_state_
 */
void OnBoardPeriph_LED_SetNewState(onboardled_t led, onboardled_state_t state);

/**
 * @brief Toggles the given LED.
 * @param led The LED to toggle.
 */
void OnBoardPeriph_LED_Toggle(onboardled_t led);

/**
  * @}
  */

/**
  * @}
  */

 /**
  * @addtogroup OnBoardPeripherals_Buttons
  * @{
  */

/**
 * @defgroup Button_enumerations
 * @{
 */

 /**
  * @brief The buttons which are available at the board (no touch sensors)
  */
 enum nnxt_button_e {
     Button_Middle = 0,
     Button_Top = 1,
     Button_Bottom = 2,
     Button_Left = 3,
     Button_Right = 4
 };

 /**
  * @brief The button states.
  */
 enum nnxt_button_state_e {
     Button_pressed = Bit_SET,
     Button_released = Bit_RESET
 };

/**
 * @}
 */

 /**
  * @defgroup Button_structs
  * @{
  */

/**
 * @brief      Holding all informations about the buttons
 */
 struct OnBoard_Buttons_s{
     const uController_pin_t pin;         // The pin
     const uint8_t Notification_Bit;      // Which bit of a 32bit vector needs to be set to 1 to say that this button is pressed
     const uint8_t ExtiPortSource;        // The port source for the external interrupt
     const uint8_t ExtiPinSource;         // The pin source for the external interrupt.
     const uint8_t IRQChannel;            // The connected interrupt channel
     const uint32_t ExtiLine;             // The line the interrupt occurs.
     void (*press_function)(void);        // The function connected to the button. Needs to be a function looks like void foo(void);
 };

 /**
  * @}
  */

/**
 * @defgroup Button_typedefs
 * @{
 */
 typedef enum nnxt_button_e          onboardbutton_t;
 typedef enum nnxt_button_state_e    onboardbutton_state_t;
 typedef struct OnBoard_Buttons_s    OnBoard_Buttons_t;
 /**
  * @}
  */

/**
 * @defgroup Button_functions
 * @ingroup NNXT_ALL_API_FUNCTIONS
 * @{
 */

 /**
  * @brief Initialize the button IOs and configure it.
  * @param button The button to initalize. Must be one of #nnxt_button_e
  * @warning This function must not be called in user code! It will called during startup of the controller, after resets and after reprogramming.
  */
 void OnBoardPeriph_Button_Init(onboardbutton_t button);

 /**
  * @brief Returns the actual state of the button. Don't care about the old state, so the user must make some deboncing on his own (at the moment).
  * @param  button The button you want to know the state.
  * @return        The actual button state, see #nnxt_button_state_e
  */
 onboardbutton_state_t OnBoardPeriph_Button_GetState(onboardbutton_t button);

/**
 * @brief      Connects a function to a button. This means, after clicking a button, the connected function will be executed in a background task.
 *
 * @param[in]  button          The button to which the function shall be connected, one of #nnxt_button_e
 * @param[in]  ButtonFunction  The function which shall be executed after button press, needs to be a function looks like void foo(void);
 * @warning    Connecting functions is not a easy task. The function must not be called when working without the OS!
 * @warning    The function you will executed (which is connected here) should contain code which executes fast. This means there should be NO loops insert and also no blocking function calls. Work instead with flags you handle then in a user task!
 */
 void OnBoardPeriph_Button_ConnectFunction(onboardbutton_t button, void (*ButtonFunction)(void));

/**
 * @brief       Deletes a connected function from a button and disables the button interrupt.
 *
 * @param[in]  button  The button, one of #nnxt_button_e
 */
 void OnBoardPeriph_Button_RemoveFunction(onboardbutton_t button);

 /**
  * @}
  */

 /**
  * @}
  */

 /**
  * @addtogroup OnBoardPeripherals_Buzzer
  * @{
  */

/**
 * @defgroup Buzzer_enumerations
 * @{
 */
 enum nnxt_beep_state_e {
 	Beep_off = DISABLE,
 	Beep_on = ENABLE
 };
 /**
  * @}
  */

 /**
  * @defgroup Buzzer_typedefs
  * @{
  */
 /**
  *
  */
typedef enum nnxt_beep_state_e 		onboardbeep_state_t;
  /**
   * @}
   */

/**
 * @defgroup Buzzer_functions
 * @{
 */
 /**
  * @brief Initialize the on board beeper to run with a timer. The frequency is set to 4kHz and the state of the beeper is initail to on, see #nnxt_beep_state_e
  */
 void OnBoardPeriph_Beep_Init();

 /**
  * @brief Sets the dutycycle to the timer of the beeper.
  * @param f Dutycycle of the timer. The value can be any value between 0 and 2^32 -1, but also values smaller than #NXT_OnBoard_BUZZER_PeriodCalc will be change the output.
  */
 void OnBoardPeriph_Beep(uint32_t f);

 /**
  * @brief En-/Disables the onboard beeper. The frequency of the beeper is not affected of this cmd, so you can set the frequency without the beeper is on. When you enable the beeper it will immediatly beep.
  * @param s The new state of the beeper, see #nnxt_beep_state_e
  */
 void OnBoardPeriph_BeepCMD(onboardbeep_state_t s);
/**
 * @}
 */

 /**
  * @}
  */

/**
 * @addtogroup OnBoardPeripherals_Misc
 * @{
 */

/**
 * @defgroup Misc_functions
 * @{
 */

 /**
  * @brief Initialize all gpios the i2c mux need (the reset) and configure it for using in SW.
  * @warning This function must not be called in user code! It will called during startup of the controller, after resets and after reprogramming.
  */
 void OnBoardPeriph_I2CMux_Init();

 /**
  * @brief Resets the i2c mux. After resetting, the mux is in default state and must be configured again!
  * @warning This function should not be called in user code! It will called during startup of the controller, after resets and after reprogramming.
  * @todo make some communication to the function which configure the i2c mux and tell him, that there was a reset
  */
 void OnBoardPeriph_I2CMux_Reset();

/**
 * @}
 */


 /**
  * @}
  */


#endif /* NXT_APPLICATIONDRIVER_INC_NNXT_ONBOARDPERIPH_H_ */

/**
  * @}
  */
