/*
 * spiEncoderVal_queue.c
 *
 *  Created on: Mar 3, 2020
 *      Author: Yahui
 */
#include <spiEncoderVal_queue.h>

int writeMsgToEncoderQueue(encoder_msg *msg){
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    BaseType_t status = xQueueSendToBackFromISR(spiEncoder_queue_handler, msg, &xHigherPriorityTaskWoken);
    return  (pdPASS == status);
}

int sendSPIMsgToEncoderQueue(int32_t enc1_val, int32_t enc2_val, int32_t enc3_val){
    encoder_msg msg = {.status=encoder, .seq_num = id ,.encoder1_val=enc1_val, .encoder2_val=enc2_val, .encoder3_val=enc3_val};
    return writeMsgToEncoderQueue(&msg);
}

encoder_msg readSPIMsgFromEncoderQueue(){
    id += 1;
    encoder_msg msg = {.status=idle, .seq_num=id , .encoder1_val=0, .encoder2_val=0, .encoder3_val=0};
//    xQueueReceive(spiEncoder_queue_handler, &msg, portMAX_DELAY);
    xQueueReceive(spiEncoder_queue_handler, &msg, portMAX_DELAY);
    return msg;
}

void initEncoderQueue() {
    spiEncoder_queue_handler = xQueueCreate(NUM_MSG, sizeof(encoder_msg));
}
