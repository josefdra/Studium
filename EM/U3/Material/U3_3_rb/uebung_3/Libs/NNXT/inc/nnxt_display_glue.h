/**
 * @file
 * @brief
 * nnxt_display_glue.h
 *
 *  Created on: 22.03.2017
 *      Author: florian
 */

#ifndef NXT_APPLICATIONDRIVER_INC_NNXT_DISPLAY_GLUE_H_
#define NXT_APPLICATIONDRIVER_INC_NNXT_DISPLAY_GLUE_H_
/*********************************/
/*           includes            */
#include <stdint.h>

/*********************************/
/*           macros              */
#define LCD_IO_WriteMultipleData(a,b)      _Generic((a), uint8_t* : LCD_IO_WriteMultipleData8, uint16_t* : LCD_IO_WriteMultipleData16)(a,b)

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
/* LCD IO functions */
void LCD_IO_Init(void);
void LCD_IO_WriteMultipleData8(uint8_t *pData, uint32_t Size);
void LCD_IO_WriteMultipleData16(uint16_t *pData, uint32_t Size);
void LCD_IO_WriteReg(uint8_t Reg);
void LCD_Delay(uint32_t delay);

#endif /* NXT_APPLICATIONDRIVER_INC_NNXT_DISPLAY_GLUE_H_ */
