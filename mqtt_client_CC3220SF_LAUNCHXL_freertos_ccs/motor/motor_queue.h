/*
 * motor_queue.h
 *
 *  Created on: Feb 25, 2020
 *      Author: hungn
 */
#include <FreeRTOS.h>
#include <queue.h>
#include <string.h>

#include "debug_module/debug.h"

#ifndef MOTOR_QUEUE_H_
#define MOTOR_QUEUE_H_

#define MOTOR_QUEUE_LENGTH 20

extern QueueHandle_t motor_queue;

void Motor_Queue_create();

int Motor_Queue_read(Motor_Message *message);

int Motor_Queue_send_command(Motor_Message *message);

#endif /* MOTOR_QUEUE_H_ */
