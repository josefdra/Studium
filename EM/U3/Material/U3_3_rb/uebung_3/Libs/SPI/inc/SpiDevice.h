
/**!
 * @file
 * SpiDevice.h
 * @brief API to the SPI-Connection.
 *
 */

#ifndef SPIDEVICE_H_INCLUDED
#define SPIDEVICE_H_INCLUDED
/*********************************/
/*           includes            */
#include "nnxt_config.h"

/*********************************/
/*           macros              */

/*********************************/
/*      extern variables         */

/*********************************/
/*          enums                */

enum em_spi_error_e {
    em_spi_error_Noerror = 0,
};


typedef enum device{
	Display,
	SDCard
}Device;

/*********************************/
/*          structs              */

/*********************************/
/*        typedefs               */

typedef enum em_spi_error_e em_spi_error_t;

em_spi_error_t SPI_Startup_Init();

em_spi_error_t SPI_Write8(const uint8_t *data, const uint32_t length, const Device device);
em_spi_error_t SPI_Write16(const uint16_t *data, const uint32_t length, const Device device);

em_spi_error_t SPI_Read16(volatile uint16_t *data, const Device device);

em_spi_error_t SPI_ReadWrite8(volatile uint8_t *data, const uint16_t length, const Device device);
em_spi_error_t SPI_ReadWrite16(volatile uint16_t *data, const uint16_t length, const Device device);

void SetChipSelect(const Device device);
void ResetChipSelect(const Device device);


void ChangePrescaler(const uint16_t prescaler);

#endif /* SPIDEVICE_H_INCLUDED */
