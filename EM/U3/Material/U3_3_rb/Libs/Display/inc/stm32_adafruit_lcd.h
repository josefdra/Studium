/**
 *	@file stm32_adafruit_lcd.h
 *	@author see copyright license above from here + Florian Laufenböck
 *	@brief Common defines and function prototypes for working with the nnxt onboard display. Some changes for a nice naming following "naming conventions" for nnxt and some little adaptions
 */

/**
  ******************************************************************************
  * @file    stm32_adafruit_lcd.h
  * @author  MCD Application Team
  * @version V2.0.1
  * @date    04-November-2015
  * @brief   This file contains the common defines and functions prototypes for
  *          the stm32_adafruit_lcd.c driver.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2015 STMicroelectronics</center></h2>
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __STM32_ADAFRUIT_LCD_H
#define __STM32_ADAFRUIT_LCD_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "st7735.h"
#include "fonts.h"

/** @addtogroup OnBoardPeripherals
  * @{
  */

/** @addtogroup OnBoardPeripherals_LCD
  * @{
  */


/** @defgroup LCD_types
  * @{
  */

/**
  * @brief  Draw Properties structures definition
  */
typedef struct
{
  uint32_t TextColor;
  uint32_t BackColor;
  sFONT    *pFont;

}LCD_DrawPropTypeDef;

/**
  * @brief  Point structures definition
  */
typedef struct
{
  int16_t X;
  int16_t Y;

}Point, * pPoint;

/**
  * @brief  Line mode structures definition
  */
typedef enum
{
  CENTER_MODE             = 0x01,    /*!< Center mode */
  RIGHT_MODE              = 0x02,    /*!< Right mode  */
  LEFT_MODE               = 0x03     /*!< Left mode   */

}Line_ModeTypdef;

/**
 * The possible display rotation settings. The reference is the default value, the rotation is always set with this as reference point.
 */
typedef enum{
  /// The upper right point is directly under the on/off switch
	LCD_Rotation_Default = 0,
  /// Rotate the display 90 degree clockwise.
	LCD_Rotation_90cw,
  /// Rotate the display 180 degree.
	LCD_Rotation_180,
  /// Rotate the display 270 degree clockwise.
	LCD_Rotation_270cw
}LCD_Rotation;

/**
  * @}
  */

/** @defgroup LCD_Macro_Constants
  * @{
  */

#define __IO    volatile

/**
  * @brief  LCD status structure definition
  */
#define LCD_OK         0x00
#define LCD_ERROR      0x01
#define LCD_TIMEOUT    0x02

/**
  * @brief  LCD color
  */
#define LCD_COLOR_BLACK         0x0000
#define LCD_COLOR_GREY          0xF7DE
#define LCD_COLOR_BLUE          0x001F
#define LCD_COLOR_RED           0xF800
#define LCD_COLOR_GREEN         0x07E0
#define LCD_COLOR_CYAN          0x07FF
#define LCD_COLOR_MAGENTA       0xF81F
#define LCD_COLOR_YELLOW        0xFFE0
#define LCD_COLOR_WHITE         0xFFFF

/**
  * @brief LCD default font
  */
#define LCD_DEFAULT_FONT         Font12

/**
  * @}
  */

/** @defgroup LCD_functions
  * @{
  */
uint8_t     NNXT_LCD_INIT(void);
uint32_t    NNXT_LCD_GetXSize(void);
uint32_t    NNXT_LCD_GetYSize(void);

uint16_t    NNXT_LCD_GetFrontColor(void);
uint16_t    NNXT_LCD_GetBackColor(void);
/**
  * @brief  Sets the LCD front (all pixels which build something to show) color.
  * @param  Color: Text color code RGB(5-6-5)
  * @retval None
  */
void        NNXT_LCD_SetFrontColor(__IO uint16_t Color);
/**
  * @brief  Sets the LCD background color.
  * @param  Color: Background color code RGB(5-6-5)
  * @retval None
  */
void        NNXT_LCD_SetBackColor(__IO uint16_t Color);
void        NNXT_LCD_SetFont(sFONT *fonts);
sFONT       *NNXT_LCD_GetFont(void);

void        NNXT_LCD_Clear(uint16_t Color);
void        NNXT_LCD_ClearStringLine(uint16_t Line);
void        NNXT_LCD_DisplayStringAtLine(uint16_t Line, char *ptr);
void        NNXT_LCD_DisplayStringAt(uint16_t Xpos, uint16_t Ypos, char *Text, Line_ModeTypdef Mode);
void        NNXT_LCD_DisplayChar(uint16_t Xpos, uint16_t Ypos, uint8_t Ascii);

void        NNXT_LCD_DrawPixel(uint16_t Xpos, uint16_t Ypos, uint16_t RGB_Code);
void        NNXT_LCD_DrawHLine(uint16_t Xpos, uint16_t Ypos, uint16_t Length);
void        NNXT_LCD_DrawVLine(uint16_t Xpos, uint16_t Ypos, uint16_t Length);
void        NNXT_LCD_DrawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);
void        NNXT_LCD_DrawRect(uint16_t Xpos, uint16_t Ypos, uint16_t Width, uint16_t Height);
void        NNXT_LCD_DrawCircle(uint16_t Xpos, uint16_t Ypos, uint16_t Radius);
void        NNXT_LCD_DrawPolygon(pPoint Points, uint16_t PointCount);
void        NNXT_LCD_DrawEllipse(int Xpos, int Ypos, int XRadius, int YRadius);
void        NNXT_LCD_DrawBitmap(uint16_t Xpos, uint16_t Ypos, uint8_t *pBmp);
void        NNXT_LCD_FillRect(uint16_t Xpos, uint16_t Ypos, uint16_t Width, uint16_t Height);
void        NNXT_LCD_FillCircle(uint16_t Xpos, uint16_t Ypos, uint16_t Radius);
void        NNXT_LCD_FillPolygon(pPoint Points, uint16_t PointCount);
void        NNXT_LCD_FillEllipse(int Xpos, int Ypos, int XRadius, int YRadius);
/**
 * @brief      Sets the rotation of the display. Default is with the sensor ports above the display
 *
 * @param[in]  r     The new rotation. Can be one of #LCD_Rotation.
 */
void 	    NNXT_LCD_Rotate(LCD_Rotation r);
/**
 * @brief      Calculates the 16 bit display color out of RGB color
 *
 * @param[in]  r     the red portion of the color
 * @param[in]  g     the green portion of the color
 * @param[in]  b     the blue portion of the color
 *
 * @return     the 16 bit color value
 */
uint16_t    NNXT_LCD_CalcColor(uint8_t r, uint8_t g, uint8_t b);

void        NNXT_LCD_DisplayOff(void);
void        NNXT_LCD_DisplayOn(void);

/**
 * @brief      Sets the inverting parameter to the display. 0 means no inversion, other values means inversion on
 *
 * @param[in]  i     inversion on/off
 */
void        NNXT_LCD_Invert(uint8_t i);

/**
  * @}
  */

#ifdef __cplusplus
}
#endif

#endif /* __STM32_ADAFRUIT_LCD_H */
/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
