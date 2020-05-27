/*
 * debug.h
 *
 *  Created on: Feb 5, 2020
 *      Author: hungn
 */

#ifndef DEBUG_H_
#define DEBUG_H_

#include <FreeRTOS.h>
#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <task.h>
#include <ti/drivers/GPIO.h>
#include <ti/drivers/Timer.h>
#include <ti/drivers/UART.h>
#include <ti/drivers/dpl/HwiP.h>

#include "debug_module/debug_constants.h"
#include "ti_drivers_config.h"

static char print_buffer[BUFFER_MAX_LENGTH];

// Define bit masks for GPIO output
#define BIT_0 0
#define BIT_1 1
#define BIT_2 2
#define BIT_3 3
#define BIT_4 4
#define BIT_5 5

// Used to keep debug location
static int debug_location = 0;

// Named for readability
#define code(n) debug_code_location(n);

/*
 * Output current code location to 5 GPIO pins
 * and one status 1 GPIO pin, if location is
 * greater than maximum locations, debug is failed
 */
void debug_code_location(unsigned int location);

/*
 * Failure handler to stop everything and output to GPIO pins
 * OPTIONAL TODO: Blink an LED
 *  */
void stop_everything();

#endif /* DEBUG_H_ */
