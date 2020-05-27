#ifndef ENCODER_INTERFACE_H_
#define ENCODER_INTERFACE_H_

#include <ti/drivers/GPIO.h>
#include <unistd.h>

#include "debug_module/debug.h"
#include "spi/SPI_interface.h"
#include "ti_drivers_config.h"
#include "timers/timer_interface.h"
#define ENCODER_WRITE_TO_MDR0 0x88
#define ENCODER_4_BYTE_MODE   0x03
#define ENCODER_READ_COUNT    0x60
#define ENCODER_WRITE_TO_DTR  0x98
#define ENCODER_SET_TO_CTR    0xE0

#define ENCODER_READ_LENGTH  5
#define ENCODER_MODE_LENGTH  2
#define ENCODER_CLEAR_LENGTH 5

// Read encoders with pass by reference receive buffer
int encoder_interface_read(SPI_Handle *encoder_SPI, uint8_t *rx_buffer);

// First byte is junk, the 4 bytes after make up the count value
// Assemble higher order byte to low for count value
long encoder_interface_assemble_bytes(uint8_t *rx_buffer);

// Clear encoder combinations
int encoder_interface_reset_data_register(SPI_Handle *encoder_SPI);
int encoder_interface_clear_counter(SPI_Handle *encoder_SPI);

// Set encoder as follow:
//    Clock division factor: 0
//    Negative index input
//    free-running count mode
//    x4 quadrature count mode (four counts per quadrature cycle)
int encoder_interface_set_mode(SPI_Handle *encoder_SPI);

#endif /* ENCODER_INTERFACE_H_ */
