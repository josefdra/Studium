/**
 * @file
 * nnxt_misc.h
 *
 */

/*********************************/
/*           includes            */
#include <stdint.h>
#include "nnxt_misc.h"
#include "nnxt.h"
/*********************************/
/*           macros              */

/*********************************/
/*      extern variables         */

/*********************************/
/*      static variables         */

/*********************************/
/*          enums                */

/*********************************/
/*          structs              */

/*********************************/
/*        typedefs               */

/*********************************/
/*      Function prototypes      */

void SystemSoftwareReset(){
    NVIC_SystemReset();
}

void DisplayResetButton()
{
        NNXT_LCD_Clear(LCD_COLOR_WHITE);
        NNXT_LCD_DisplayStringAt(0,0,"Bitte", CENTER_MODE);
        NNXT_LCD_DisplayStringAt(0,12,"Software-Reset", CENTER_MODE);
        NNXT_LCD_DisplayStringAt(0,24,"druecken", CENTER_MODE);

        uint16_t colorRed = NNXT_LCD_CalcColor(255,0,0);
        uint16_t colorBlack = 0x000000;

        for(int i = 47; i <= 57; i++)
        {
            for(int j = 47; j <=57; j++)
            {
                NNXT_LCD_DrawPixel(i, j, colorRed);
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
                NNXT_LCD_DrawPixel(i, j, colorBlack);
            }
        }
        NNXT_LCD_DisplayStringAt(67,50,"Reset", LEFT_MODE);
}



void SoftwareStop()
{
     DisplayResetButton();

     Motor_Stop(Port_A, Motor_stop_break);
     Motor_Stop(Port_B, Motor_stop_break);
     Motor_Stop(Port_C, Motor_stop_break);

     OnBoardPeriph_BeepCMD(Beep_off);

     if(!nnxt_pre_os_flag)
     {
         StopAllTasks();
     }
     while(1)
     {
         if(OnBoardPeriph_Button_GetState(Button_Top))
         {
             NVIC_SystemReset();
         }
     }

}

void sprintDouble(char *s, double v, int decimalDigits) {
   int i = 1;
   int intPart, fractPart;
   for (;decimalDigits!=0; i*=10, decimalDigits--);
   intPart = (int)v;
   fractPart = (int)((v-(double)(int)v)*i);
   if(fractPart < 0) fractPart *= -1;
   sprintf(s,"%i.%i", intPart, fractPart); }

