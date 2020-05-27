/*
 * pid_queue.c
 *
 *  Created on: Apr 1, 2020
 *      Author: hungnguyen
 */

#include "pid/pid_queue.h"

void PID_Queue_create()
{
    pid_queue = xQueueCreate(PID_QUEUE_LENGTH, sizeof(PID_Message));
    if (!pid_queue)
    {
        code(PID_QUEUE_CREATE_FAILED);
        stop_everything();
    }
}

int PID_Queue_read(PID_Message *data)
{
    int read_ok = TRUE;

    PID_Message message;

    BaseType_t retVal = xQueueReceive(pid_queue, &message, portMAX_DELAY);
    if (retVal == pdFALSE)
    {
        code(PID_QUEUE_READ_FAILED);

        stop_everything();
    }
    data->message_id = message.message_id;

    switch (message.message_id)
    {
        case PID_QUEUE_ENCODER_ID:
            data->encoder_reading[ENCODER1] = message.encoder_reading[ENCODER1];
            data->encoder_reading[ENCODER2] = message.encoder_reading[ENCODER2];
            data->encoder_reading[ENCODER3] = message.encoder_reading[ENCODER3];
            break;

        case PID_QUEUE_MOVEMENT_ID:
            data->desired_speed[MOTOR1] = message.desired_speed[MOTOR1];
            data->desired_speed[MOTOR2] = message.desired_speed[MOTOR2];
            data->desired_speed[MOTOR3] = message.desired_speed[MOTOR3];
            data->direction[MOTOR1]     = message.direction[MOTOR1];
            data->direction[MOTOR2]     = message.direction[MOTOR2];
            data->direction[MOTOR3]     = message.direction[MOTOR3];
            data->motor[MOTOR1]         = message.motor[MOTOR1];
            data->motor[MOTOR2]         = message.motor[MOTOR2];
            data->motor[MOTOR3]         = message.motor[MOTOR3];
            break;

        default:
            read_ok = FALSE;
            break;
    }

    if (!read_ok)
    {
        code(PID_QUEUE_READ_FAILED);
        stop_everything();
    }
    return read_ok;
}

void PID_Queue_send_from_encoder(long reading1, long reading2, long reading3)
{
    PID_Message message;

    message.message_id                = PID_QUEUE_ENCODER_ID;
    message.encoder_reading[ENCODER1] = reading1;
    message.encoder_reading[ENCODER2] = reading2;
    message.encoder_reading[ENCODER3] = reading3;

    if (!xQueueSendToBack(pid_queue, (void *)&message, portMAX_DELAY))
    {
        // Failed to send queue full
    }
}

void PID_Queue_send_from_movement(uint8_t direction1,
                                  uint8_t direction2,
                                  uint8_t direction3,
                                  long    speed1,
                                  long    speed2,
                                  long    speed3,
                                  uint8_t active1,
                                  uint8_t active2,
                                  uint8_t active3)
{
    PID_Message message;

    message.message_id            = PID_QUEUE_MOVEMENT_ID;
    message.desired_speed[MOTOR1] = speed1;
    message.desired_speed[MOTOR2] = speed2;
    message.desired_speed[MOTOR3] = speed3;
    message.direction[MOTOR1]     = direction1;
    message.direction[MOTOR2]     = direction2;
    message.direction[MOTOR3]     = direction3;
    message.motor[MOTOR1]         = active1;
    message.motor[MOTOR2]         = active2;
    message.motor[MOTOR3]         = active3;

    if (!xQueueSendToBack(pid_queue, (void *)&message, portMAX_DELAY))
    {
        // Failed to send queue full
    }
}
