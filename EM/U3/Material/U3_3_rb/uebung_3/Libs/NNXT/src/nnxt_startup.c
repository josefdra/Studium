/**
 * @file
 * nnxt_startup.c
 *
 */

/*********************************/
/*           includes            */
#include "nnxt_Ports.h"
#include "nnxt_i2c.h"
#include "nnxt_config.h"
#include "system_stm32f4xx.h"
#include "stm32f4xx.h"
#include "nnxt_OnBoardPeriph.h"
#include "misc.h"
#include "nnxt_misc.h"
#include "nnxt.h"
#include "SpiDevice.h"
#include "nnxt_sdcard_glue.h"
#include "stm32f4xx_tim.h"
#include "ff.h"

/*********************************/
/*           macros              */
/// The alignment of the vector table stored in RAM. This is important because the VTOR register only takes the upper 23 bits
#define VECTORTABLE_ALIGN           (0x200ul)
/// The number the systick isr in the vectortable
#define VECTORTABLE_SYSTICK_NR      15
/// The number the pendsv isr in the vectortable
#define VECTORTABLE_PENDSV_NR       14
/// The number the svcall isr in the vectortable
#define VECTORTABLE_SVCALL_NR       11


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

/*********************************/
/*      Local variables          */
/// The table all the interrupt vectoren will be saved in RAM. This table will be changed during runtime and differ from the table in the flash.
static uint32_t vectorTable_RAM[STM_VECTORTABLE_SIZE] __attribute__(( aligned (VECTORTABLE_ALIGN)));
/// This table is a 1:1 image of the table in ROM
uint32_t vectorTable_RAM_orig[STM_VECTORTABLE_SIZE] __attribute__((aligned (VECTORTABLE_ALIGN)));
/// The address the vector table is stored in flash
static uint32_t *vectorTable_ROM = (uint32_t*)0x08000000;    //0x8... is the beginning of flash

uint8_t debug = 0;

NNXT_TaskHandle NNXT_Background_Task;

/**
 * @brief      Dummy function for doing nothing with a nop. This function is needed for change interrupts function to have a dummy function where the vector table points to.
 */
void donothing(){
    asm("nop");
}

/**
 * @brief Startup Function for the NXT. Does all the initialization before the main application starts. This function must not be called from usercode!
 * @warning Do NOT call this function from usercode, it is somethink like a software-reset of the board.
 * @todo thinking about error handling. What make sense if there is an error in startup code?
 * @todo deintialize all peripherals before initailizing
 */
void Startup_NNXT(){

    nnxt_pre_os_flag = 1;
	SCB->CPACR |= ((3UL << 10*2)|(3UL << 11*2)); //activating the FPU

    GPIO_DeInit(GPIOA);
    GPIO_DeInit(GPIOB);
    GPIO_DeInit(GPIOC);
    GPIO_DeInit(GPIOD);
    GPIO_DeInit(GPIOE);

    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);

#if (NNXT_OS==ERIKA)
// doing nothing with the systick handler
#elif(NNXT_OS==FREERTOS)
	 // following code is to change the isr vector table
     // This is important because we use Delay functions in startup (before the OS is started), so we cannot use the FreeRTOS Delay (which would be called by default of the vector table).
     // Because of this we bend all interrupts to the functions we want to use, using them in startup code and bending them back when the OS started.
	 __disable_irq();

	 // copy the vector table from flash into RAM
	 for(uint32_t i = 0; i < STM_VECTORTABLE_SIZE; i++){
	     vectorTable_RAM[i] = *vectorTable_ROM;
	     vectorTable_RAM_orig[i] = *vectorTable_ROM;
	     vectorTable_ROM++;
	 }
	 vectorTable_RAM[VECTORTABLE_SYSTICK_NR] = (uint32_t)MakePreOSSysTick;    // isr function for the SysTickHandler
	 vectorTable_RAM[VECTORTABLE_PENDSV_NR] = (uint32_t)donothing;      // isr function for the pendsv function
	 vectorTable_RAM[VECTORTABLE_SVCALL_NR] = (uint32_t)donothing;      // isr function for the svcall function
	 SCB->VTOR = (uint32_t)&vectorTable_RAM;         // now change the begin of the isr vector table to the table loaded in RAM
	 __DSB();                                        // make clear that all is updatet
	 __enable_irq();                                 // and enable irqs again
	 SysTick_Config(SystemCoreClock/NXT_SYSTEMTIMER_SYSTICK_HZ);    //TODO change to the freertos tick time
#else
    //initializing of the system timer
    SysTick_Config(SystemCoreClock/NXT_SYSTEMTIMER_SYSTICK_HZ);		//having a systick every 1ms
#endif
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);			// enabling clock bus for external interrupts

    //initializing and enabling timer for precise time measurement in us
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM14, ENABLE);
    TIM_TimeBaseInitTypeDef Timer_Init_struct;
    Timer_Init_struct.TIM_ClockDivision = TIM_CKD_DIV1;
    Timer_Init_struct.TIM_CounterMode = TIM_CounterMode_Up;
    Timer_Init_struct.TIM_Period = NXT_PRECISE_TIMER_TIM_MAXVAL;
    Timer_Init_struct.TIM_Prescaler = (SystemCoreClock/NXT_SYSTEMTIMER_BASE_1MHZ)/2;
    TIM_TimeBaseInit(NXT_PRECISE_TIMER_TIM, &Timer_Init_struct);
    TIM_Cmd(NXT_PRECISE_TIMER_TIM, ENABLE);

	//initializing the I2C for the sensors
    nnxt_I2C_Init();

	// init spi's for display/sdcard
	SPI_Startup_Init();
	SDCard_Init();
	Delay(1);
	OnBoardPeriph_I2CMux_Init();

	SensorADCs_Init();
	for(uint8_t i= 0; i < NXT_SENSORn; i++){
		SensorPortInit(i);
	}
	ADC_SoftwareStartConv(NXT_SENSOR_ADC_ADCx);

	MotorPortInit(Port_A);
	MotorPortInit(Port_B);
	MotorPortInit(Port_C);

	for(onboardled_t i = NNXT_LED0; i <= NNXT_LED3; i++)
		OnBoardPeriph_LED_Init(i);
	for(onboardbutton_t i = Button_Middle; i <= Button_Right; i++){
		OnBoardPeriph_Button_Init(i);
	}

	OnBoardPeriph_I2CMux_Reset();


	//SDCard_Init_Pins();
	NNXT_LCD_INIT();
	OnBoardPeriph_Beep_Init();
	Delay(20);
	NNXT_LCD_Rotate(LCD_Rotation_Default);

	// at least: initalize the System Trace
	SEGGER_SYSVIEW_Conf();
	Delay(1);
	SEGGER_SYSVIEW_Start();
	Delay(1);
    OnBoardPeriph_Button_ConnectFunction(Button_Top, &SystemSoftwareReset); //configure the top button to act as a software reset button
    OnBoardPeriph_Button_ConnectFunction(Button_Middle, &SoftwareStop); //configure the top button to act as a software stop button

    debug = 0;
    Delay(3000);
    if(CoreDebug->DHCSR & 1)
    {
        debug = 1;
    }
    else
    {
        DisplayStartButton();
    }
    while(!debug)
    {
        debug = OnBoardPeriph_Button_GetState(Button_Bottom);
    }

    NNXT_LCD_Clear(LCD_COLOR_WHITE);

}



void DisplayStartButton()
{
        NNXT_LCD_DisplayStringAt(0,0,"Zum Starten", CENTER_MODE);
        NNXT_LCD_DisplayStringAt(0,12,"Start", CENTER_MODE);
        NNXT_LCD_DisplayStringAt(0,24,"druecken", CENTER_MODE);

        uint16_t colorRed = NNXT_LCD_CalcColor(255,0,0);
        uint16_t colorBlack = 0x000000;

        for(int i = 47; i <= 57; i++)
        {
            for(int j = 47; j <=57; j++)
            {
                NNXT_LCD_DrawPixel(i, j, colorBlack);
            }
        }

        for(int i = 27; i <= 37; i++)
        {
            for(int j = 67; j <=77; j++)
            {
                NNXT_LCD_DrawPixel(i, j, colorBlack);
            }
        }

        for(int i = 47; i <= 57; i++)
        {
            for(int j = 67; j <=77; j++)
            {
                NNXT_LCD_DrawPixel(i, j, colorBlack);
            }
        }

        for(int i = 67; i <= 77; i++)
        {
            for(int j = 67; j <=77; j++)
            {
                NNXT_LCD_DrawPixel(i, j, colorBlack);
            }
        }

        for(int i = 47; i <= 57; i++)
        {
            for(int j = 87; j <=97; j++)
            {
                NNXT_LCD_DrawPixel(i, j, colorRed);
            }
        }
        NNXT_LCD_DisplayStringAt(67,88,"Start", LEFT_MODE);
}


