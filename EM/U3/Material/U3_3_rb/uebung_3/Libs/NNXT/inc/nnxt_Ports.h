/**
 * @file
 * nnxt_Ports.h
 * This file contains all needed functionality to configure and using the onboard lego plugs. It provides functions as well
 * for the sensor plugs as for the motor plugs. Including this file means you can drive and interacting with your environment.
 *
 */

#ifndef NXT_APPLICATIONDRIVER_INC_NNXT_PORTS_H_
#define NXT_APPLICATIONDRIVER_INC_NNXT_PORTS_H_
/*********************************/
/*           includes            */
#include <stdint.h>
#include "stm32f4xx_gpio.h"

/*********************************/
/*           macros              */

/**
 * @defgroup    NXT_BackwardCompability
 * @brief       This macros are used to give a backward compability of the code here and the orginal nxc code.
 *              If you have the choice, don't use this macros. To run old code written in nxc for the orginal NXT using this macros are a proper way without make any changes.
 * @todo commenting all macros
 * @{
 */

 //macros for sensors
 #define SensorUS(port)

 //macros for motors
 #define OnRev(port,percent)
 #define OnFwd(port,percent)
 #define Off(port)
 #define Float(port)

/**
 * @}
 */

#define US_SENSOR_ADDR_8Bit 0x02
#define US_SENSOR_ADDR_7Bit EM_I2C_CALC_7Bit_ADDRESS(US_SENSOR_ADDR_8Bit)

/*********************************/
/*      extern variables         */

/*********************************/
/*          enums                */
enum sensor_ports_e {
    Port_0 = 0,
    Port_1 = 1,
    Port_2 = 2,
    Port_3 = 3,
    Port_extern = 4,
};

enum sensor_types_e {
	SensorUnderConfiguration = -2,
	SensorUnConfigured = -1,
    SensorUS = 0,
    SensorTouch,
    SensorColor,
    SensorExt,
};

enum touch_clicked_e {
    SensorTouch_clicked = Bit_RESET,
    SensorTouch_released = Bit_SET
};

/**
 * All errors which are visible to the user.
 */
enum sensor_error_e {
    sensor_error_NoError = 0,
    sensor_error_SensorNotConfigured,
    sensor_error_FalsePort,
    sensor_error_TooFastToRead,
};

/**
 * @brief      The settings how to break the motors.
 */
enum motor_stop_e {
    Motor_stop_break = 0,   /// Breaking the motor very hard!
    Motor_stop_float        /// Breaking the motor by just sending no new values to him. The motor will go after a little.
};

/**
 * @brief      The motor ports
 */
enum motor_ports_e {
    Port_A = 0,
    Port_B = 1,
    Port_C = 2
};

/**
 * The possible directions for the motors.
 */
typedef enum {
    Motor_dir_forward = 0,
    Motor_dir_backward
}motor_dir_t;


/*********************************/
/*          structs              */
struct motor_port_infos_s{
	uint32_t counterEncoderA;
	uint32_t counterEncoderB;
	motor_dir_t direction;
};

/*********************************/
/*        typedefs               */
typedef enum sensor_ports_e     sensorport_t;
typedef enum motor_ports_e      motorport_t;
typedef enum sensor_types_e     sensortypes_t;
typedef enum sensor_error_e     sensor_error_t;
typedef enum touch_clicked_e	sensor_touch_clicked_t;
typedef enum motor_stop_e		motor_stop_t;
typedef struct motor_port_infos_s motor_port_info;

/*********************************/
/*      Function prototypes      */

/**
 * @addtogroup SensorPorts
 * @{
 */

/**
 * @defgroup SensorPorts_functions
 * @description All functions which are available to use with the sensor ports.
 * @{
 */

/**
 * @brief   Configurates a sensor port with the given sensortype. After calling this function, you can use the specific sensors.
 * @param   port The sensor port the sensor is plugged in.
 * @param   type The sensor type.
 * @return  One of the errros described in #sensor_error_e
 */
sensor_error_t SensorConfig(sensorport_t port, sensortypes_t type);

/**
 * @brief   Initialize a sensor port. This includes pins and ports confiuration, ADC configuration and resetting the port.
 * @param   port The port which to initialize.
 * @warning This function must not be called in user code! It will called during startup of the controller, after resets and after reprogramming.
 * @todo	implement
 */
void SensorPortInit(sensorport_t port);

/**
 * @brief Initialize the ADC for converting all values on all analog inputs. Also initializing a dma to get the values from the adc
 * a      and storing them in a array.
 */
void SensorADCs_Init();

/**
 * @brief   Reads the distance (in cm) of a lego ultrasonic sensor.
 * @param  port     The port a ultrasonic sensor is plugged in.
 * @param  distance an uint32_t pointer where the distance should be stored.
 * @return  One of the errros described in #sensor_error_e
 * @pre     The sensor port must have a proper configuration via #SensorConfig
 * @blocking
 * @todo    thinking about the configuration of the i2c mux when reading from a ussensor...
 * @todo    error handling
 */
sensor_error_t US_GetDistance(sensorport_t port, volatile uint8_t *distance);

/**
 * @brief          Reads the value of the touch sensor.
 * @param  port    The port the touch sensor is plugged in
 * @param  clicked Is the touch sensor clicked, see #touch_clicked_e
 * @return         return always #sensor_error_NoError
 * @pre            The sensor port must have a proper configuration via #SensorConfig
 */
sensor_error_t Touch_Clicked(sensorport_t port, sensor_touch_clicked_t *clicked);

/**
 * @brief          Reads the value of the touch sensor.
 * @param  port    The port the touch sensor is plugged in
 * @param  clicked Is the touch sensor clicked, see #touch_clicked_e
 * @return         return always #sensor_error_NoError
 * @pre            The sensor port must have a proper configuration via #SensorConfig
 */
sensor_error_t SensorADCWithOutFilter(sensorport_t port, uint16_t *value);

/**
 * @brief          Reads the value of the touch sensor.
 * @param  port    The port the touch sensor is plugged in
 * @param  clicked Is the touch sensor clicked, see #touch_clicked_e
 * @return         return always #sensor_error_NoError
 * @pre            The sensor port must have a proper configuration via #SensorConfig
 */
sensor_error_t SensorADCWithFilter(sensorport_t port, uint16_t *value);

/**
 * @}
 */

/**
 * @}
 */

/**
 * @addtogroup MotorPorts
 * @{
 */

/**
 * @addtogroup Motorports_functions
 * @{
 */

/**
 * @brief   Initialize a motor port. This includes pins and ports confiuration and resetting.
 * @param   port The port to initialize.
 * @warning This function must not be called in user code! It will called during startup of the controller, after resets and after reprogramming.
 */
void MotorPortInit(motorport_t port);

/**
 * @brief       Starts the given Motor with direction and the dutycyle. Drive until #Motor_Stop is called
 * @param  port      The motorport, see #motor_ports_e
 * @param  dir       The direction the motor shall drive, see #motor_dir_t
 * @param  dutycycle The dutycycle in percent. Values greater than 100 leads also in full speed.
 * @return           One of the errros described in #sensor_error_e
 */
sensor_error_t Motor_Drive(motorport_t port, motor_dir_t dir, uint8_t dutycycle);

/**
 * @brief Stops the given motor
 * @param  port The motorport, see #motor_ports_e
 * @param  stop Stop condition, see #motor_stop_e. #Motor_stop_break will do a hard break, this costs more current. #Motor_stop_float let roll out the motor.
 * @return      One of the errros described in #sensor_error_e
 */
sensor_error_t Motor_Stop(const motorport_t port, const motor_stop_t stop);

/**
 * @brief Gets the actual Tachovalue of one motor. The counter value will be calculated by counting the rising edges of the motor encoder.
 * @param  port    The motorport, you want to know the counter value, see #motor_port_e
 * @param  counter an uint32_t pointer, where the counter value should be stored
 * @return One of the errors described in #sensor_error_e
 * @todo how much rising edges are 1 umin
 */
sensor_error_t Motor_Tacho_GetCounter(const motorport_t port, uint32_t *counter);

/**
 * @}
 */

/**
 * @}
 */




#endif /* NXT_APPLICATIONDRIVER_INC_NNXT_PORTS_H_ */
