/*
 * speedEncoder_queue.h
 *
 *  Created on: Apr 27, 2020
 *      Author: Yahui
 */

#ifndef SPEEDENCODER_QUEUE_H_
#define SPEEDENCODER_QUEUE_H_

// Queue for new speed command request to change

#include <speedCommand_queue.h>

#include <FreeRTOS.h>
#include <queue.h>

#define NUM_MSG_Motor 5

typedef struct moving_speed{uint8_t speed_val; motor_direction direction_cmd;} moving_speed;
typedef struct moving_msg{ moving_speed m1_cmd; moving_speed m2_cmd; moving_speed m3_cmd;} moving_msg;


QueueHandle_t moving_queue_handler;

int writeMsgToMovingQueue(moving_msg *msg);

int sendMsgToMovingQueue(moving_msg motor);

moving_msg readMsgFromMovingQueue();

void initSpeedEncoderQueue();

#endif /* SPEEDENCODER_QUEUE_H_ */
