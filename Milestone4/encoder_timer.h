/*
 * encoder_timer.h
 *
 *  Created on: Feb 25, 2020
 *      Author: Yahui
 */

#ifndef ENCODER_TIMER_H_
#define ENCODER_TIMER_H_

#include <stddef.h>
#include <stdint.h>
#include <math.h>

/* POSIX Header files */
#include <pthread.h>

/* Driver Header files */
#include <ti/drivers/Timer.h>
#include <ti/drivers/GPIO.h>
/* Board Header file */
#include "ti_drivers_config.h"

#include <spiEncoderVal_queue.h>
#include <spi_encoder.h>
#include <uart_term.h>


#define THREADSTACKSIZE   (4096)

Timer_Handle encoder_timer;
Timer_Params encoder_params;

typedef struct current_speed{int32_t e1_speed; int32_t e2_speed; int32_t e3_speed;} current_speed;

static current_speed curr_speed = {0, 0, 0};
static int32_t prev_SS1 =0;
static int32_t prev_SS2 =0;
static int32_t prev_SS3 = 0; // the previous encoder values

//extern SPI_Handle masterSpi;
// Declare functions

current_speed updateCurrentSpeed(int32_t SS1, int32_t SS2, int32_t SS3);
current_speed getCurrentSpeed();
void timer100Callback(Timer_Handle myHandle); // timer callback function


void initEncoderTimer(); // initialize the timer


#endif /* ENCODER_TIMER_H_ */
