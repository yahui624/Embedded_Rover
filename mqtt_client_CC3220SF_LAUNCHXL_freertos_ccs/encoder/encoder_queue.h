
#ifndef ENCODER_QUEUE_H_
#define ENCODER_QUEUE_H_

#include <FreeRTOS.h>
#include <queue.h>

#include "debug_module/debug.h"

#define ENCODER_QUEUE_LENGTH 20

#define TIMER_ID 0
#define SPI_ID   1

extern QueueHandle_t encoder_queue;

void Encoder_Queue_create();

int Encoder_Queue_read(int* timer_elapsed, int* spi_transferred);

// Callback functions should have non-blocking sends to queue
void Encoder_Queue_send_timer_elapsed();
void Encoder_Queue_send_SPI_transfer_completed();

#endif /* ENCODER_QUEUE_H_ */
