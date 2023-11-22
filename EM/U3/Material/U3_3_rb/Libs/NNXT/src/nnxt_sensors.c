/**
 * @file
 * nnxt_sensors.c
 *
 */

/*********************************/
/*           includes            */
#include "nnxt_Ports.h"
#include <stdint.h>
#include "nnxt_config.h"
#include "em_clk.h"
#include "nnxt_i2c.h"
#include <math.h>
#include "nnxt_misc.h"
#include "nnxt_common.h"
#include "stm32f4xx_dma.h"
#include "nnxt_os_glue.h"
#include "nnxt_Filter.h"

/*********************************/
/*           macros              */

/*********************************/
/*      extern variables         */

/*********************************/
/*          enums                */
enum sensor_US_commands{
    US_Command_GetDistance = (uint8_t)0x42,
};

/*********************************/
/*          structs              */

struct sensor_port_config_s{
	uController_pin_t clock;
	uController_pin_t data;
	uController_pin_an_t analog_in;
	uController_pin_an_t analog_sda;
	uController_pin_t n9V_on;
};

struct port_configuration_s{
	sensortypes_t sensor;
	uint32_t last_access_time;
};

/**
 * @brief The analog values on each port. We need the packed pragma for having the values directly behind each other in memory without padding,
 * otherwise the dma would not working in a proper way
 */
typedef struct{
    uint16_t sdanalog_val;
    uint16_t analog_val;
}__attribute__((packed)) sensor_port_analog_vals_t;

/*********************************/
/*        typedefs               */
typedef struct sensor_port_config_s sensor_port_config_t;
typedef struct port_configuration_s port_configuration_t;

/*********************************/
/*      private variables        */

/// holds the complete pin/port configuration for the sensor ports
static const sensor_port_config_t SensorPorts[NXT_SENSORn] = {
    // port 0
    {
            {NXT_SENSOR0_BB_SCL_Pin, NXT_SENSOR0_BB_SCL_Port, NXT_SENSOR0_BB_SCL_Clock},
            {NXT_SENSOR0_BB_SDA_Pin, NXT_SENSOR0_BB_SDA_Port, NXT_SENSOR0_BB_SDA_Clock},
            {NXT_SENSOR0_Analog_Pin, NXT_SENSOR0_Analog_Port, NXT_SENSOR0_Analog_Clock, NXT_SENSOR0_Analog_ADC_Channel},
            {NXT_SENSOR0_SDAnalog_Pin, NXT_SENSOR0_SDAnalog_Port, NXT_SENSOR0_SDAnalog_Clock, NXT_SENSOR0_SDAnalog_ADC_Channel},
            {NXT_SENSOR0_BB_9V_Pin, NXT_SENSOR0_BB_9V_Port, NXT_SENSOR0_BB_9V_Clock}
    },
    // port 1
    {
            {NXT_SENSOR1_BB_SCL_Pin, NXT_SENSOR1_BB_SCL_Port, NXT_SENSOR1_BB_SCL_Clock},
            {NXT_SENSOR1_BB_SDA_Pin, NXT_SENSOR1_BB_SDA_Port, NXT_SENSOR1_BB_SDA_Clock},
            {NXT_SENSOR1_Analog_Pin, NXT_SENSOR1_Analog_Port, NXT_SENSOR1_Analog_Clock, NXT_SENSOR1_Analog_ADC_Channel},
            {NXT_SENSOR1_SDAnalog_Pin, NXT_SENSOR1_SDAnalog_Port, NXT_SENSOR1_SDAnalog_Clock, NXT_SENSOR1_SDAnalog_ADC_Channel},
            {NXT_SENSOR1_BB_9V_Pin, NXT_SENSOR1_BB_9V_Port, NXT_SENSOR1_BB_9V_Clock}
    },
    // port 2
    {
            {NXT_SENSOR2_BB_SCL_Pin, NXT_SENSOR2_BB_SCL_Port, NXT_SENSOR2_BB_SCL_Clock},
            {NXT_SENSOR2_BB_SDA_Pin, NXT_SENSOR2_BB_SDA_Port, NXT_SENSOR2_BB_SDA_Clock},
            {NXT_SENSOR2_Analog_Pin, NXT_SENSOR2_Analog_Port, NXT_SENSOR2_Analog_Clock, NXT_SENSOR2_Analog_ADC_Channel},
            {NXT_SENSOR2_SDAnalog_Pin, NXT_SENSOR2_SDAnalog_Port, NXT_SENSOR2_SDAnalog_Clock, NXT_SENSOR2_SDAnalog_ADC_Channel},
            {NXT_SENSOR2_BB_9V_Pin, NXT_SENSOR2_BB_9V_Port, NXT_SENSOR2_BB_9V_Clock}
    },
    // port 3
    {
            {NXT_SENSOR3_BB_SCL_Pin, NXT_SENSOR3_BB_SCL_Port, NXT_SENSOR3_BB_SCL_Clock},
            {NXT_SENSOR3_BB_SDA_Pin, NXT_SENSOR3_BB_SDA_Port, NXT_SENSOR3_BB_SDA_Clock},
            {NXT_SENSOR3_Analog_Pin, NXT_SENSOR3_Analog_Port, NXT_SENSOR3_Analog_Clock, NXT_SENSOR3_Analog_ADC_Channel},
            {NXT_SENSOR3_SDAnalog_Pin, NXT_SENSOR3_SDAnalog_Port, NXT_SENSOR3_SDAnalog_Clock, NXT_SENSOR3_SDAnalog_ADC_Channel},
            {NXT_SENSOR3_BB_9V_Pin, NXT_SENSOR3_BB_9V_Port, NXT_SENSOR3_BB_9V_Clock}
    }
};

/// holds the dynamic setted configuration of the sensors ports like which sensor is plugged in
static port_configuration_t PortConfig[NXT_SENSORn] = {
		{SensorUnConfigured, 0},
		{SensorUnConfigured, 0},
		{SensorUnConfigured, 0},
		{SensorUnConfigured, 0},
		{SensorUnConfigured, 0},
};

/// The sensor should only be read in an interval of 15ms. If the reading operation on the sensor is too fast, there will be an error an no read will be forced!
static const uint8_t US_TimeBetweenMeasurements = 15;

/// The analog values from all sensors. We need this array of values to store the adcs value there because we store them with a dma continously.
sensor_port_analog_vals_t PortAnalogValues[NXT_SENSORn] = {0};

/// The threshold value for the touch button as an analog value with 10bit adc resolution. A adc value upper this value means that the button is not clicked, a value below this value means the touch sensor is clicked.
static const uint16_t TouchSensorThresholdValue = 1000;

static tFloatAvgFilter Filter;

/*********************************/
/*           Functions           */

/**
 * @brief   Configurates a given \p port as for using with ultrasonic sensors.
 * @param   port one of the ports which can be used as a sensor, see #sensor_ports_e
 */
static inline void SensorUSConfig(sensorport_t port){

	if(PortConfig[port].sensor != SensorUS){
		PortConfig[port].sensor = SensorUnderConfiguration;
		GPIO_InitTypeDef GPIO_InitStructure;

		GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
		GPIO_InitStructure.GPIO_Speed = NXT_GPIO_GenSpeed;

		////we don't need the following pins for working with ultrasonic
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
		GPIO_InitStructure.GPIO_Pin = SensorPorts[port].data.pin;
		GPIO_Init(SensorPorts[port].data.port, &GPIO_InitStructure);

		// pin for modulating signals on the i2c clock line
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
		GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
		GPIO_InitStructure.GPIO_Pin = SensorPorts[port].clock.pin;
		GPIO_Init(SensorPorts[port].clock.port, &GPIO_InitStructure);

		// the us-sensor needs a 9V signal
		GPIO_InitStructure.GPIO_Pin = SensorPorts[port].n9V_on.pin;
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
		GPIO_Init(SensorPorts[port].n9V_on.port, &GPIO_InitStructure);

		GPIO_SetBits(SensorPorts[port].n9V_on.port, SensorPorts[port].n9V_on.pin);
		GPIO_SetBits(SensorPorts[port].clock.port, SensorPorts[port].clock.pin);

		PortConfig[port].sensor = SensorUS;
	}

}

/**
 * @brief Configurates the given Port with a touch sensor. The touch sensor works as a voltage divider, therefore we use the pin as an analog pin and compare the adc value with a constant.
 * @param port The port the touch sensor is plugged in
 */
static inline void SensorTouchConfig(sensorport_t port){
	if(PortConfig[port].sensor != SensorTouch){
		PortConfig[port].sensor = SensorUnderConfiguration;
		GPIO_InitTypeDef GPIO_InitStructure;

		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
		GPIO_InitStructure.GPIO_Speed = NXT_GPIO_GenSpeed;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
        GPIO_InitStructure.GPIO_Pin = SensorPorts[port].analog_in.pin;
        GPIO_Init(SensorPorts[port].analog_in.port, &GPIO_InitStructure);

        GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
        GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
        GPIO_Init(SensorPorts[port].n9V_on.port, &GPIO_InitStructure);

		GPIO_ResetBits(SensorPorts[port].n9V_on.port, SensorPorts[port].n9V_on.pin);

		PortConfig[port].sensor = SensorTouch;
	}
}

void SensorPortInit(sensorport_t port){
	PortConfig[port].sensor = SensorUnConfigured;
	GPIO_InitTypeDef GPIO_InitStructure;

	ENABLE_CLOCK_IO(SensorPorts[port].data.clock, ENABLE);
	ENABLE_CLOCK_IO(SensorPorts[port].clock.clock, ENABLE);
	ENABLE_CLOCK_IO(SensorPorts[port].analog_in.clock, ENABLE);
	ENABLE_CLOCK_IO(SensorPorts[port].analog_sda.clock, ENABLE);
	ENABLE_CLOCK_IO(SensorPorts[port].n9V_on.clock, ENABLE);

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed = NXT_GPIO_GenSpeed;

	GPIO_InitStructure.GPIO_Pin = SensorPorts[port].clock.pin;
	GPIO_Init(SensorPorts[port].clock.port, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = SensorPorts[port].data.pin;
	GPIO_Init(SensorPorts[port].data.port, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = SensorPorts[port].analog_sda.pin;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
    GPIO_Init(SensorPorts[port].analog_sda.port, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = SensorPorts[port].analog_in.pin;
    GPIO_Init(SensorPorts[port].analog_in.port, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = SensorPorts[port].n9V_on.pin;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_Init(SensorPorts[port].n9V_on.port, &GPIO_InitStructure);

	GPIO_ResetBits(SensorPorts[port].n9V_on.port, SensorPorts[port].n9V_on.pin);

	ADC_RegularChannelConfig(NXT_SENSOR_ADC_ADCx, SensorPorts[port].analog_sda.adc_channel, port * 2 + 1, NXT_SENSOR_ADC_SampleTime);
    ADC_RegularChannelConfig(NXT_SENSOR_ADC_ADCx, SensorPorts[port].analog_in.adc_channel, port * 2 + 2, NXT_SENSOR_ADC_SampleTime);

	PortConfig[port].sensor = SensorTouch;
}

sensor_error_t SensorConfig(sensorport_t port, sensortypes_t type){
    sensor_error_t ret_var = sensor_error_NoError;
	switch (type) {
		case SensorUS:
			SensorUSConfig(port);
			nnxt_I2C_ResetPins(port);
			break;
		case SensorTouch:
			SensorTouchConfig(port);
			break;
        case SensorExt:
            SensorTouchConfig(port);
            break;
		default:
			ret_var = sensor_error_FalsePort;
	}
	Delay(5);
    return ret_var;
}

sensor_error_t US_GetDistance(sensorport_t port, volatile uint8_t *distance){
    sensor_error_t ret_var = sensor_error_NoError;

    if(PortConfig[port].sensor != SensorUS){
    	ret_var = sensor_error_SensorNotConfigured;
    }
    else if(PortConfig[port].last_access_time + US_TimeBetweenMeasurements > GetSysTime()){
        ret_var = sensor_error_TooFastToRead;
    }
    else{
    	Delay_us(150);
    	nnxt_I2C_Write(port, 0x02, &((uint8_t){US_Command_GetDistance}), 1);
        Delay_us(150);
        GPIO_ResetBits(SensorPorts[port].clock.port, SensorPorts[port].clock.pin);
        Delay_us(50);
        GPIO_SetBits(SensorPorts[port].clock.port, SensorPorts[port].clock.pin);
        Delay_us(100);
        nnxt_I2C_ResetPins(port);
        Delay_us(100);
        nnxt_I2C_Read(port, 0x02, (uint8_t*)distance, sizeof(uint8_t));
        PortConfig[port].last_access_time = GetSysTime();
    }

    return ret_var;
}

sensor_error_t Touch_Clicked(sensorport_t port, sensor_touch_clicked_t *clicked){
	sensor_error_t ret_var = sensor_error_NoError;
	if(PortConfig[port].sensor != SensorTouch){
	    ret_var = sensor_error_SensorNotConfigured;
	}else{
	    *clicked = SensorTouch_released;
	    if(TouchSensorThresholdValue > PortAnalogValues[port].analog_val) *clicked = SensorTouch_clicked;
	}

	return ret_var;
}

void SensorADCs_Init(){
    ADC_InitTypeDef ac;
    ADC_CommonInitTypeDef acc;
    DMA_InitTypeDef dma;
    InitFloatAvg(&Filter, 0);

    // enabling the clocks
    ENABLE_CLOCK_IO(NXT_SENSOR_ADC_DMA_Clock, ENABLE);
    RCC_APB2PeriphClockCmd(NXT_SENSOR_ADC_ADCx_Clock, ENABLE);

    //configuring the dma to get the values from the adc and store them in #PortAnalogValues
    dma.DMA_BufferSize = NXT_SENSOR_ADC_NbrOfChannels;
    dma.DMA_Channel = NXT_SENSOR_ADC_DMA_Channel;
    dma.DMA_DIR = DMA_DIR_PeripheralToMemory;
    dma.DMA_FIFOMode = DMA_FIFOMode_Disable;
    dma.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;
    dma.DMA_Memory0BaseAddr = (uint32_t)PortAnalogValues;
    dma.DMA_MemoryBurst = DMA_MemoryBurst_Single;
    dma.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
    dma.DMA_MemoryInc = DMA_MemoryInc_Enable;
    dma.DMA_Mode = DMA_Mode_Circular;
    dma.DMA_PeripheralBaseAddr = (uint32_t)&NXT_SENSOR_ADC_ADCx->DR;
    dma.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
    dma.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
    dma.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    dma.DMA_Priority = DMA_Priority_High;

    DMA_Init(NXT_SENSOR_ADC_DMA_Stream, &dma);
    DMA_Cmd(NXT_SENSOR_ADC_DMA_Stream, ENABLE);

    acc.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled; //because this is only for multi-channel adc
    acc.ADC_Mode = ADC_Mode_Independent;
    acc.ADC_Prescaler = NXT_SENSOR_ADC_Prescaler;
    acc.ADC_TwoSamplingDelay = NXT_SENSOR_ADC_DelayTwoSampling;
    ADC_CommonInit(&acc);

    ac.ADC_ContinuousConvMode = ENABLE;
    ac.ADC_DataAlign = NXT_SENSOR_ADC_DataAlign;
    ac.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_CC1;
    ac.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;    //we starting the conversion manually at first time, then it runs continuesly
    ac.ADC_NbrOfConversion = NXT_SENSOR_ADC_NbrOfChannels;
    ac.ADC_Resolution = NXT_SENSOR_ADC_Resolution;
    ac.ADC_ScanConvMode = ENABLE;
    ADC_Init(NXT_SENSOR_ADC_ADCx, &ac);

    ADC_DMARequestAfterLastTransferCmd(NXT_SENSOR_ADC_ADCx, ENABLE);
    ADC_DMACmd(NXT_SENSOR_ADC_ADCx, ENABLE);
    ADC_Cmd(NXT_SENSOR_ADC_ADCx, ENABLE);

}

sensor_error_t SensorADCWithOutFilter(sensorport_t port, uint16_t *value){
    sensor_error_t ret_var = sensor_error_NoError;
    if(PortConfig[port].sensor != SensorExt){
        *value = PortAnalogValues[port].analog_val;
    }
    else ret_var = sensor_error_SensorNotConfigured;
    return ret_var;
}

sensor_error_t SensorADCWithFilter(sensorport_t port, uint16_t *value){
    sensor_error_t ret_var = sensor_error_NoError;
    if(PortConfig[port].sensor != SensorExt)
    {
        AddToFloatAvg(&Filter, PortAnalogValues[port].analog_val);

        *value = GetOutputValue(&Filter);
    }
    else ret_var = sensor_error_SensorNotConfigured;
    return ret_var;
}
