/*
 * spi_encoder.h
 *
 *  Created on: Feb 25, 2020
 *      Author: Yahui
 */

#ifndef SPI_ENCODER_H_
#define SPI_ENCODER_H_


#include <ti/drivers/SPI.h>
#include <ti/drivers/GPIO.h>
#include <ti/drivers/UART.h>
#include "ti_drivers_config.h"
#include <motor_control.h>
#include <uart_term.h>
#include  <spiEncoderVal_queue.h>
#include <encoder_timer.h>

#define LOW (0)
#define HIGH (1)

#define CLEAR_CNTR 0x20 // register instruction
#define RD_CNTR 0x60
#define WR_CNTR 0xA0

#define slave_encoder_motor1 (1)
#define slave_encoder_motor2 (2)
#define slave_encoder_motor3 (3)

static signed long encoder_motor1_count = 0; //counter
static signed long encoder_motor2_count = 0;
static signed long encoder_motor3_count = 0;

SPI_Handle masterSpi;

//void spiCallbackFxn(SPI_Handle handle, SPI_Transaction *tran);

void initSPIEncoder();

int32_t read_SPI_Encoder(int encoder);

int reset_SPI_Encoder(int encoder);

void clear_SPI_Encoder_Count(int encoder);



#endif /* SPI_ENCODER_H_ */
