/**!
 * @file
 * nnxt_pcf8574lb.c
 *
 *  Author: Johannes Fischer
 *   Email: johannes6.fischer@oth-regensburg.de
 *  Date: 10.12.2018

 */
/*********************************/
/*           includes            */

#include "nnxt_i2c.h"
#include "nnxt_pcf8574lb.h"
/*********************************/
/*           macros              */

/*********************************/
/*      extern variables         */
uint8_t result = 0;
/*********************************/
/*          enums                */

/*********************************/
/*          structs              */

/*********************************/
/*        typedefs               */

/*********************************/
/*      function implementation  */

void WritePort(sensorport_t port, uint8_t input)
{
    result = ReadPorts(port);
    input = result | (1<<input);
    nnxt_I2C_Write(port, PCF8574_Address, &input, sizeof(uint8_t));
}

void WritePorts(sensorport_t port, uint8_t input)
{
    input = ~input;
    nnxt_I2C_Write(port, PCF8574_Address, &input, sizeof(uint8_t));
}

void DeletePort(sensorport_t port, uint8_t input)
{
    result = ReadPorts(port);
    input = result & ~(1<<input);
    nnxt_I2C_Write(port, PCF8574_Address, &input, sizeof(uint8_t));
}

uint8_t ReadPort(sensorport_t port, uint8_t input)
{
    result = ReadPorts(port);
    return (result & (1<<input))?1:0;
}

uint8_t ReadPorts(sensorport_t port)
{
    result = 0;
    nnxt_I2C_Read(port, PCF8574_Address, &result, 1);
    return result;
}
