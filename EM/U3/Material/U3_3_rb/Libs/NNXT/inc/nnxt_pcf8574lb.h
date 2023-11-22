/* FreeRTOS_glue.h
 *
 *  Created on: 10.12.2018
 *      Author: Johannes Fischer
 */

#ifndef NNXT_PCF8574LB_H_INCLUDED
#define NNXT_PCF8574LB_H_INCLUDED
/*********************************/
/*           includes            */

/*********************************/
/*           macros              */
#define PCF8574_Address          0x70
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

void WritePort(sensorport_t port, uint8_t input);
void DeletePort(sensorport_t port, uint8_t input);
uint8_t ReadPort(sensorport_t port, uint8_t input);
uint8_t ReadPorts(sensorport_t port);

#endif /* NNXT_PCF8574LB_H_INCLUDED */
