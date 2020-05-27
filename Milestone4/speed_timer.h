/*
 * speed_timer.h
 *
 *  Created on: Mar 26, 2020
 *      Author: Yahui
 */

#ifndef SPEED_TIMER_H_
#define SPEED_TIMER_H_
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

#include <speedCommand_queue.h>

#define THREADSTACKSIZE   (4096)

static int32_t speed_1 = 150;
static int32_t speed_2 = 10;
static int32_t speed_3 = 60;

Timer_Handle speed_timer;
void timer500Callback(Timer_Handle myHandle); // timer callback function


void initSpeedTimer(); // initialize the timer




#endif /* SPEED_TIMER_H_ */
