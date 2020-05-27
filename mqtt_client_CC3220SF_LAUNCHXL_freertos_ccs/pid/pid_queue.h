
#ifndef PID_QUEUE_H_
#define PID_QUEUE_H_

#include <FreeRTOS.h>
#include <queue.h>
#include <string.h>

#include "debug_module/debug.h"

#define PID_QUEUE_ENCODER_ID  0
#define PID_QUEUE_MOVEMENT_ID 1
#define PID_QUEUE_LENGTH      10

extern QueueHandle_t pid_queue;

void PID_Queue_create();
int  PID_Queue_read(PID_Message *data);
void PID_Queue_send_from_encoder(long reading1, long reading2, long reading3);
void PID_Queue_send_from_movement(uint8_t direction1,
                                  uint8_t direction2,
                                  uint8_t direction3,
                                  long    speed1,
                                  long    speed2,
                                  long    speed3,
                                  uint8_t active1,
                                  uint8_t active2,
                                  uint8_t active3);

#endif /* PID_QUEUE_H_ */
