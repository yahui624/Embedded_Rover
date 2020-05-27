/*
 * SPI_interface.h
 *
 *  Created on: Feb 25, 2020
 *      Author: hungn
 */

#ifndef SPI_INTERFACE_H_
#define SPI_INTERFACE_H_

#include <ti/drivers/SPI.h>
#include <unistd.h>

#include "debug_module/debug.h"
#include "encoder/encoder_queue.h"
#include "ti_drivers_config.h"
#define SPI_BITRATE 500000
SPI_Handle SPI_interface_initialize();

void SPI_transfer_callback(SPI_Handle handle, SPI_Transaction *transaction);
int  SPI_interface_transfer(SPI_Handle *spi, uint8_t *rx_buffer, uint8_t *tx_buffer, size_t count);

#endif /* SPI_INTERFACE_H_ */
