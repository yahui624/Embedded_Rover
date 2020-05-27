/*
 * speedEncoder_queue.c
 *
 *  Created on: Apr 27, 2020
 *      Author: Yahui
 */


#include <speedEncoder_queue.h>

int writeMsgToMovingQueue(moving_msg *msg_ptr) {
//    TickType_t xHigherPriorityTaskWoken = pdFALSE;
    BaseType_t status = xQueueSendToBack(moving_queue_handler, msg_ptr, portMAX_DELAY); //Wait for 10 ticks for space to become available if necessary.
    return  (pdPASS == status);
}

int sendMsgToMovingQueue(moving_msg motor){
    moving_msg msg = {.m1_cmd.speed_val=motor.m1_cmd.speed_val, .m1_cmd.direction_cmd=motor.m1_cmd.direction_cmd,
                      .m2_cmd.speed_val=motor.m2_cmd.speed_val, .m2_cmd.direction_cmd=motor.m2_cmd.direction_cmd,
                      .m3_cmd.speed_val=motor.m3_cmd.speed_val, .m3_cmd.direction_cmd=motor.m3_cmd.direction_cmd};
    return writeMsgToMovingQueue(&msg);
}

moving_msg readMsgFromMovingQueue(){
    moving_msg msg = {.m1_cmd.speed_val=0, .m1_cmd.direction_cmd=DRIVE_FORWARD,
                      .m2_cmd.speed_val=0, .m2_cmd.direction_cmd=DRIVE_FORWARD,
                      .m3_cmd.speed_val=0, .m3_cmd.direction_cmd=DRIVE_FORWARD};
    xQueueReceive(moving_queue_handler, &msg, portMAX_DELAY);
    return msg;
}

void initSpeedEncoderQueue(){
    moving_queue_handler = xQueueCreate(NUM_MSG_Motor, sizeof(moving_msg));
}
