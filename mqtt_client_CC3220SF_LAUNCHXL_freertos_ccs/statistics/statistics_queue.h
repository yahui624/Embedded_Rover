/*
 * statistics_queue.h
 *
 *  Created on: Apr 1, 2020
 *      Author: hungnguyen
 */

#ifndef STATISTICS_QUEUE_H_
#define STATISTICS_QUEUE_H_

#include <FreeRTOS.h>
#include <queue.h>
#include <string.h>

#include "debug_module/debug.h"

#define STATISTICS_QUEUE_TIMER_ID 0
#define STATISTICS_QUEUE_RECEIVE_ID 1
#define STATISTICS_QUEUE_LENGTH 20

extern QueueHandle_t statistics_queue;

void Statistics_Queue_create();

int Statistics_Queue_read(int *timer_elapsed, int *board_id,
                          uint32_t *sequence);

void Statistics_Queue_send_received_data(int board_id, uint32_t sequence);

void Statistics_Queue_send_timer_elapsed();

#endif /* STATISTICS_QUEUE_H_ */
