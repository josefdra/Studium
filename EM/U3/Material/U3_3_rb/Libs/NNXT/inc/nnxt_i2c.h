/* nnxt_i2c.h
 *
 *  Created on: 19.11.2018
 *      Author: Johannes Fischer
 */

#ifndef NNXT_I2C_H_INCLUDED
#define NNXT_I2C_H_INCLUDED
/*********************************/
/*           includes            */
#include "nnxt_Ports.h"
/*********************************/
/*           macros              */

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

void nnxt_I2C_Init();
void nnxt_I2C_Write(sensorport_t port, uint8_t address_7bit, uint8_t* data, uint32_t len);
void nnxt_I2C_Read(sensorport_t port, uint8_t address_7bit, uint8_t* data, uint32_t len);
void nnxt_I2C_ResetPins(sensorport_t port);

#endif /* NNXT_I2C_H_INCLUDED */
