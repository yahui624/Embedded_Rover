/*
 * motor_control.h
 *
 *  Created on: Feb 23, 2020
 *      Author: Yahui
 */



#ifndef MOTOR_CONTROL_H_
#define MOTOR_CONTROL_H_

#include <ti/drivers/UART.h>
#include "ti_drivers_config.h"
//#include <speedCommand_queue.h>
#include <speed_timer.h>
#include <speedEncoder_queue.h>

#define MASK 0b01111111
#define MOTOR_BAUD_RATE (9600)
#define START_BAUD_RATE 0xAA
#define MOTOR_1_ADDRESS 0x80 //(128)
#define MOTOR_2_ADDRESS 0x81 //(129)
#define MOTOR_3_ADDRESS 0x82 //(130)
#define DRIVE_FORWARD 0x00 //(0)
#define DRIVE_BACKWARD 0x01 //(1)

UART_Handle motoruart;
UART_Params motoruartParams;

void initMotorUART();

void DriveMotor(char address, char direction, uint8_t speed);

#endif /* MOTOR_CONTROL_H_ */
