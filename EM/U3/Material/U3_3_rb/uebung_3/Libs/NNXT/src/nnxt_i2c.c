
/**!
 * @file
 * nnxt_i2c.c
 *
 *  Author: Johannes Fischer
 *   Email: johannes6.fischer@oth-regensburg.de
 *  Date: 19.11.2018

 */

/*********************************/
/*           includes            */

#include "em_i2c.h"
#include "nnxt_Ports.h"
#include <stdbool.h>
#include "nnxt_os_glue.h"
#include "stm32_adafruit_lcd.h"

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
/*      function implementation  */

/**
 * @brief
 * @param port            The sensor port where i2c signals should be going to. Can also be more than one port, but be careful! If there are two us sensors at two
 *                        you want to coummunicate, you must not activate them parallel, because they have the same i2c address!
 * @param hold_old_values if true, the old configuration of the mux will be taken over. So the old configuration will also be active and you can have some multicasting at the ports.
 */
void I2C_MuxConfig(sensorport_t port, bool hold_old_values){
	static const uint8_t lookup_table[NXT_SENSORn] = {3,2,1,0};
    static uint8_t actual_config = 0;
    uint8_t new_config = (1 << lookup_table[port]);
    em_i2c_error_t ret_var;

    if(actual_config != new_config){
    	new_config = (hold_old_values) ? (actual_config | new_config) : new_config;
    	ret_var = EM_I2C_Write(NXT_SENSOR_I2C_Mux_Channel, NXT_SENSOR_I2C_Mux_Addr, &new_config, 1);
    	actual_config = new_config;

        if(ret_var != em_i2c_error_NoError)
        {
            NNXT_LCD_Clear(LCD_COLOR_WHITE);
            NNXT_LCD_DisplayStringAtLine(1, "I2C Error! Maybe wrong Address?");
        }
    }
}

void nnxt_I2C_Init()
{
    EM_I2C1_Init();
    EM_I2C2_Init();

}

void nnxt_I2C_Write(sensorport_t port, uint8_t address_7bit, uint8_t *data, uint32_t len)
{
    em_i2c_error_t ret_var;
    if(port == Port_extern)
    {
        ret_var = EM_I2C_Write(NXT_SENSOR_I2C_EXTERN_Channel, (address_7bit >> 1), data, len);
    }
    else
    {
        I2C_MuxConfig(port, false);
        Delay_us(100);
        ret_var = EM_I2C_Write(NXT_SENSOR_I2C_Mux_Channel, (address_7bit >> 1), data, len);
    }
    if(ret_var != em_i2c_error_NoError)
    {
        NNXT_LCD_Clear(LCD_COLOR_WHITE);
        NNXT_LCD_DisplayStringAtLine(1, "I2C Error! Wrong Address?");
    }
}

void nnxt_I2C_Read(sensorport_t port, uint8_t address_7bit, uint8_t *data, uint32_t len)
{
    em_i2c_error_t ret_var;
    if(port == Port_extern)
    {
        ret_var = EM_I2C_Read(NXT_SENSOR_I2C_EXTERN_Channel, (address_7bit >> 1), data, len);
    }
    else
    {
        I2C_MuxConfig(port, false);
        Delay_us(100);
        ret_var = EM_I2C_Read(NXT_SENSOR_I2C_Mux_Channel, (address_7bit >> 1), data, len);
    }

    if(ret_var != em_i2c_error_NoError)
    {
        NNXT_LCD_Clear(LCD_COLOR_WHITE);
        NNXT_LCD_DisplayStringAtLine(1, "I2C Error! Wrong Address?");
    }
}

void nnxt_I2C_ResetPins(sensorport_t port)
{
    if(port != Port_extern)
    {
        EM_I2C_ResetPins(NXT_SENSOR_I2C_Mux_Channel);
    }
    else
    {
        EM_I2C_ResetPins(NXT_SENSOR_I2C_EXTERN_Channel);
    }
}




