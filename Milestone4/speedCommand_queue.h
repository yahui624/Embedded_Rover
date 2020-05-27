/*
 * speedCommand_queue.h
 *
 *  Created on: Mar 3, 2020
 *      Author: Yahui
 */

#ifndef SPEEDCOMMAND_QUEUE_H_
#define SPEEDCOMMAND_QUEUE_H_

#include <FreeRTOS.h>
#include <queue.h>

#define NUM_MSG_Motor 5
static int32_t id_msg = 0;

typedef enum motor_status{idle_waitng, running} motor_status;
typedef enum motor_direction{idle_direction, DRIVE_FORWARD, DRIVE_BACKWARD} motor_direction;
//typedef struct motor_speed{int32_t speed; motor_direction direction;} motor_speed;
typedef struct speed_msg{motor_status status; int32_t msg_no;
                        int32_t motor1_speed; motor_direction motor1_direction;
                        int32_t motor2_speed; motor_direction motor2_direction;
                        int32_t motor3_speed; motor_direction motor3_direction;} speed_msg;


QueueHandle_t speed_queue_handler;

int writeMsgToSpeedQueue(speed_msg *msg);

int sendSpeedMsgToSpeedQueue(int32_t motor1_speed, motor_direction motor1_direction,
                                int32_t motor2_speed, motor_direction motor2_direction,
                                int32_t motor3_speed, motor_direction motor3_direction);

speed_msg readSpeedMsgFromSpeedQueue();

void initSpeedQueue();

#endif /* SPEEDCOMMAND_QUEUE_H_ */
