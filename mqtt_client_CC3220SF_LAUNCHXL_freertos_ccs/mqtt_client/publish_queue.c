/*
 * publish_queue.c
 *
 *  Created on: Apr 1, 2020
 *      Author: hungnguyen
 */

#include "publish_queue.h"

void Publish_Queue_create()
{
    publish_queue = xQueueCreate(PUBLISH_QUEUE_LENGTH, sizeof(Publish_Message));
    if (!publish_queue)
    {
        code(PUBLISH_QUEUE_CREATE_FAILED);
        stop_everything();
    }
}

int Publish_Queue_read(Publish_Message *data)
{
    int read_ok = TRUE;

    Publish_Message message;

    xQueueReceive(publish_queue, &message, portMAX_DELAY);

    data->message_id = message.message_id;
    switch (message.message_id)
    {
        case PUBLISH_QUEUE_MOVEMENT_ID:
            data->block_color = message.block_color;
            data->has_block   = message.has_block;
            data->sequence    = message.sequence;

            break;
        case PUBLISH_QUEUE_STATISTICS_ID:
            data->receive_actual   = message.receive_actual;
            data->receive_expected = message.receive_expected;

            break;
        case PUBLISH_QUEUE_DEBUG_ID:

            data->motor1 = message.motor1;
            data->motor2 = message.motor2;
            data->motor3 = message.motor3;
            break;
        default:
            read_ok = FALSE;
            break;
    }

    if (!read_ok)
    {
        code(PUBLISH_QUEUE_READ_FAILED);
        stop_everything();
    }
    return read_ok;
}

void Publish_Queue_send_movement_status(int has_block, int block_color, int sequence)
{
    Publish_Message message;

    message.message_id       = PUBLISH_QUEUE_MOVEMENT_ID;
    message.block_color      = block_color;
    message.has_block        = has_block;
    message.sequence         = sequence;
    message.receive_actual   = ZERO;
    message.receive_expected = ZERO;

    if (!xQueueSend(publish_queue, (void *)&message, portMAX_DELAY))
    {
        // Failed to send queue full
    }
}

void Publish_Queue_send_statistics(uint32_t receive_actual, uint32_t receive_expected)
{
    Publish_Message message;

    message.message_id       = PUBLISH_QUEUE_STATISTICS_ID;
    message.block_color      = ZERO;
    message.has_block        = ZERO;
    message.sequence         = ZERO;
    message.receive_actual   = receive_actual;
    message.receive_expected = receive_expected;

    if (!xQueueSend(publish_queue, (void *)&message, portMAX_DELAY))
    {
        // Failed to send queue full
    }
}

void Publish_Queue_send_debug(Motor_Debug motor1, Motor_Debug motor2, Motor_Debug motor3)
{
    Publish_Message message;

    message.message_id  = PUBLISH_QUEUE_DEBUG_ID;
    message.block_color = ZERO;
    message.has_block   = ZERO;
    message.sequence    = ZERO;
    message.motor1      = motor1;
    message.motor2      = motor2;
    message.motor3      = motor3;

    if (!xQueueSend(publish_queue, (void *)&message, portMAX_DELAY))
    {
        // Failed to send queue full
    }
}
