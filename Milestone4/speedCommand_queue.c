/*
 * speedCommand_queue.c
 *
 *  Created on: Mar 3, 2020
 *      Author: Yahui
 */

#ifndef SPEEDCOMMAND_QUEUE_C_
#define SPEEDCOMMAND_QUEUE_C_

#include <speedCommand_queue.h>

int writeMsgToSpeedQueue(speed_msg *msg_ptr) {
//    TickType_t xHigherPriorityTaskWoken = pdFALSE;
    BaseType_t status = xQueueSendToBack(speed_queue_handler, msg_ptr, portMAX_DELAY); //Wait for 10 ticks for space to become available if necessary.
    return  (pdPASS == status);
}

int sendSpeedMsgToSpeedQueue(int32_t m1_speed, motor_direction m1_direction,
                             int32_t m2_speed, motor_direction m2_direction,
                             int32_t m3_speed, motor_direction m3_direction){
    id_msg += 1;
    speed_msg msg = {.status=running, .msg_no = id_msg,
                     .motor1_speed=m1_speed, .motor1_direction=m1_direction,
                     .motor2_speed=m2_speed, .motor2_direction=m2_direction,
                     .motor3_speed=m3_speed, .motor3_direction=m3_direction};
    return writeMsgToSpeedQueue(&msg);
}

speed_msg readSpeedMsgFromSpeedQueue(){
    speed_msg msg = {.status=idle_waitng, .msg_no = id_msg,
                     .motor1_speed=0, .motor1_direction=idle_direction,
                     .motor2_speed=0, .motor2_direction=idle_direction,
                     .motor3_speed=0, .motor3_direction=idle_direction};
    xQueueReceive(speed_queue_handler, &msg, portMAX_DELAY);
    return msg;
}

void initSpeedQueue(){
    speed_queue_handler = xQueueCreate(NUM_MSG_Motor, sizeof(speed_msg));
}


#endif /* SPEEDCOMMAND_QUEUE_C_ */
