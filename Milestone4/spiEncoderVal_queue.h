/*
 * spiEncoderVal_queue.h
 *
 *  Created on: Mar 3, 2020
 *      Author: Yahui
 */

#ifndef SPIENCODERVAL_QUEUE_H_
#define SPIENCODERVAL_QUEUE_H_
#include <FreeRTOS.h>
#include <queue.h>

#define NUM_MSG 5
static int32_t id = 0;

typedef enum encoder_status{idle, encoder} encoder_status;
typedef struct encoder_val{int32_t encoderMsg;} encoder_val;
typedef struct encoder_msg{encoder_status status; int32_t seq_num; encoder_val encoder1_val; encoder_val encoder2_val; encoder_val encoder3_val;} encoder_msg;


QueueHandle_t spiEncoder_queue_handler;

int writeMsgToEncoderQueue(encoder_msg *msg);

int sendSPIMsgToEncoderQueue(int32_t enc1_val, int32_t enc2_val, int32_t enc3_val);

encoder_msg readSPIMsgFromEncoderQueue();

void initEncoderQueue();


#endif /* SPIENCODERVAL_QUEUE_H_ */
