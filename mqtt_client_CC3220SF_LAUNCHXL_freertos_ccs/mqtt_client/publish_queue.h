/*
 * publish_queue.h
 *
 *  Created on: Apr 1, 2020
 *      Author: hungnguyen
 */

#ifndef PUBLISH_QUEUE_H_
#define PUBLISH_QUEUE_H_

#include <FreeRTOS.h>
#include <queue.h>
#include <string.h>

#include "debug_module/debug.h"

#define PUBLISH_QUEUE_MOVEMENT_ID   0
#define PUBLISH_QUEUE_STATISTICS_ID 1
#define PUBLISH_QUEUE_DEBUG_ID      2  // UNUSED FOR NOW
#define PUBLISH_QUEUE_LENGTH        10

extern QueueHandle_t publish_queue;

void Publish_Queue_create();

int Publish_Queue_read(Publish_Message *data);

void Publish_Queue_send_movement_status(int has_block, int block_color, int sequence);

void Publish_Queue_send_statistics(uint32_t receive_actual, uint32_t receive_expected);

void Publish_Queue_send_debug(Motor_Debug motor1, Motor_Debug motor2, Motor_Debug motor3);

#endif /* PUBLISH_QUEUE_H_ */
