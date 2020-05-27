/*
 * debug.h
 *
 *  Created on: Feb 2, 2020
 *      Author: sophie
 */

#ifndef DEBUG_H_
#define DEBUG_H_

#include <stdio.h>
#include <ti/drivers/UART.h>
#include <FreeRTOS.h>

/* Board Header file */
#include "ti_drivers_config.h"

////constants for debugging every task
//#define DEBUG_ENTERING_TASK                 0x01
//#define DEBUG_BEFORE_WHILE                  0x02
//#define DEBUG_BEFORE_SEND_RECEIVE_QUEUE     0x03
//#define DEBUG_AFTER_SEND_RECEIVE_QUEUE      0x04
//#define QUEUE_RECEIVE_BLOCKING              0x05
//
////constants for debugging ISRs
//#define DEBUG_ENTER_ISR                     0x10
//#define DEBUG_BEFORE_LEAVING_ISR            0x11
//#define DEBUG_BEFORE_SEND_RECEIVE_QUEUE_ISR 0x12
//#define DEBUG_AFTER_SEND_RECEIVIE_QUEUE_ISR 0x13
//#define DEBUG_BEFORE_RECEIVE_QUEUE          0x14
//#define DEBUG_AFTER_RECEIVE_QUEUE           0x15
//
////constants for debugging out of bounds
#define DEBUG_OUT_OF_BOUNDS                 0x20
//
////constants for debugging UART
#define DEBUG_UART_NOT_OPEN                 0x30
//
////constant for types of failures in code
//#define DEBUG_DEFAULT_CASE                  0x40
//#define DEBUG_FAILURE                       0x41
//
////constants for adc
//#define DEBUG_NO_ADC_READING                0x42
//#define BEFORE_ADC_CONVERSION               0x43
//#define AFTER_ADC_CONVERSION                0x44
//
////constant for debugging timers
//#define DEBUG_TIMER2_NOT_START              0x50
//#define DEBUG_TIMER2_NOT_OPEN               0x51
//#define DEBUG_TIMER1_NOT_START              0x52
//#define DEBUG_TIMER1_NOT_OPEN               0x53
//#define TIMER1_PERIOD_ISR_ENTER             0x54
//#define TIMER1_PERIOD_ISR_LEAVE             0x55
//#define TIMER2_PERIOD_ISR_ENTER             0x56
//#define TIMER2_PERIOD_ISR_LEAVE             0x57
//#define DEBUG_BEFORE_SEND_RECEIVE_QUEUE_TIMER1  0x58
//#define DEBUG_BEFORE_SEND_QUEUE_TIMER2      0x59
//#define DEBUG_AFTER_SEND_QUEUE_TIMER2       0x70
//
////constants for state machines
//#define STATE_INIT_ENTER                    0x45
//#define STATE_TIME1_ENTER                   0x46
//#define STATE_TIME2_ENTER                   0x47
//#define STATE_TIME3_ENTER                   0x48

//// constants for the arm
//#define DEBUG_ENTER_TIMER                   0x01
//#define DEBUG_BEGIN_TIMER                   0x02
//#define DEBUG_READ_TIME_MESSAGE             0x03
//#define DEBUG_READ_CONFIG_MESSAGE           0x04
//#define SEND_CONFIG_MESSAGE                 0x05

// constants for the milestone3
#define DEBUG_START                         0x01
#define DEBUG_STOP_CLIENT                   0x02
#define DEBUG_COULD_NOT_START               0x03
#define DEBUG_COULD_FIND_DEVICE_INFO        0x04
#define DEBUG_COULD_NOT_STOP                 0x05

UART_Handle uart_handling;
UART_Params uart_parameters;

void dbgUARTVal(unsigned char outVal);
void dbgOutputLoc(unsigned int outLoc);
void failRoutine(unsigned int outLoc);
void initializeUART();
void initializeGPIO();

#endif /* DEBUG_H_ */
