/*
 * statistics_queue.c
 *
 *  Created on: Apr 1, 2020
 *      Author: hungnguyen
 */

#include "statistics_queue.h"

void Statistics_Queue_create()
{
    statistics_queue = xQueueCreate(STATISTICS_QUEUE_LENGTH, sizeof(Statistics_Message));
    if (!statistics_queue)
    {
        code(STATISTICS_QUEUE_CREATE_FAILED);
        stop_everything();
    }
}

int Statistics_Queue_read(int *timer_elapsed, int *board_id, uint32_t *sequence)
{
    int read_ok = TRUE;

    Statistics_Message message;

    xQueueReceive(statistics_queue, &message, portMAX_DELAY);

    switch (message.message_id)
    {
        case STATISTICS_QUEUE_TIMER_ID:
            *timer_elapsed = message.timer_elapsed;
            break;
        case STATISTICS_QUEUE_RECEIVE_ID:
            *board_id = message.board_id;
            *sequence = message.sequence;
            break;
        default:
            read_ok = FALSE;
            break;
    }
    if (!read_ok)
    {
        code(STATISTICS_QUEUE_READ_FAILED);
        stop_everything();
    }
    return read_ok;
}

void Statistics_Queue_send_received_data(int board_id, uint32_t sequence)
{
    Statistics_Message message;
    message.message_id    = STATISTICS_QUEUE_RECEIVE_ID;
    message.board_id      = board_id;
    message.sequence      = sequence;
    message.timer_elapsed = FALSE;

    if (!xQueueSendToBackFromISR(statistics_queue, (void *)&message, NULL))
    {
        // Failed to send queue full
    }
}

void Statistics_Queue_send_timer_elapsed()
{
    Statistics_Message message;

    message.message_id    = STATISTICS_QUEUE_TIMER_ID;
    message.board_id      = ZERO;
    message.sequence      = ZERO;
    message.timer_elapsed = TRUE;

    BaseType_t xHigherPriorityTaskWoken = pdFALSE;

    if (!xQueueSendToBackFromISR(statistics_queue, (void *)&message, &xHigherPriorityTaskWoken))
    {
        // Failed to send queue full
    }
}
