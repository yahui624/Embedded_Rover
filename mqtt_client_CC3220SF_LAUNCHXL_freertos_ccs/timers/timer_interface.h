/*
 * timer_interface.h
 *
 *  Created on: Feb 25, 2020
 *      Author: hungn
 */

#ifndef TIMER_INTERFACE_H_
#define TIMER_INTERFACE_H_

#include <stddef.h>

#include "debug_module/debug.h"
#include "encoder/encoder_queue.h"
#include "statistics/statistics_queue.h"

/* Driver Header files */
#include <ti/drivers/GPIO.h>
#include <ti/drivers/Timer.h>

#include "debug_module/debug.h"
#include "ti_drivers_config.h"
#define SECOND_IN_MICROSECONDS  1000000
#define TIMER_ENCODER_PERIOD    50000    // 50 ms
#define TIMER_IR_HZ             10        // 0.2 s
#define TIMER_MOVEMENT_PERIOD   1000000  // 1 sec
#define TIMER_STATISTICS_PERIOD 2500000  // 2.5 seconds

void         timer_IR_callback(Timer_Handle timer_handle, int_fast16_t status);
Timer_Handle timer_IR_initialize();

void         timer_movement_callback(Timer_Handle timer_handle, int_fast16_t status);
Timer_Handle timer_movement_initialize();

void         timer_statistics_callback(Timer_Handle timer_handle, int_fast16_t status);
Timer_Handle timer_statistics_initialize();

void         timer_encoder_callback(Timer_Handle timer_handle, int_fast16_t status);
Timer_Handle timer_encoder_initialize();

#endif /* TIMER_INTERFACE_H_ */
