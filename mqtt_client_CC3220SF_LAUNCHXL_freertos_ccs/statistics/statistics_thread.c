/*
 * statistics_thread.c
 *
 *  Created on: Mar 30, 2020
 *      Author: hungn
 */
#include "debug_module/debug.h"
#include "mqtt_client/publish_queue.h"
#include "statistics/statistics_queue.h"
#include "timers/timer_interface.h"

void *statistics_thread(void *arg0)
{
    code(STATISTICS_THREAD_START);

    Timer_Handle timer = timer_statistics_initialize();

    int timer_elapsed = FALSE;

    if (Timer_start(timer) == Timer_STATUS_ERROR)
    {
        code(TIMER_START_FAILED);
        stop_everything();
    }

    int      board_id         = ZERO;
    uint32_t last_sequence    = ZERO;
    uint32_t sequence         = ZERO;
    uint32_t receive_expected = ZERO;
    uint32_t receive_actual   = ZERO;
    uint32_t difference       = ZERO;

    code(STATISTICS_THREAD_WHILE_LOOP);
    while (TRUE)
    {
        code(STATISTICS_THREAD_READQ_BEGIN);
        Statistics_Queue_read(&timer_elapsed, &board_id, &sequence);
        code(STATISTICS_THREAD_READQ_END);
        if (timer_elapsed)
        {
            // SEND DATA TO PUBLISH QUEUE
            Publish_Queue_send_statistics(receive_actual, receive_expected);
            timer_elapsed = FALSE;
        }

        // Always recalculate expected and actual statistics whenever sequence
        // changes
        if (sequence != last_sequence)
        {
            receive_actual++;
            difference = ZERO;
            if (sequence > last_sequence)
            {
                difference = sequence - last_sequence - 1;
            }
            receive_expected = receive_expected + 1 + difference;
        }
        // tODO handle resetting of sequence to zero
        last_sequence = sequence;
    }
}
