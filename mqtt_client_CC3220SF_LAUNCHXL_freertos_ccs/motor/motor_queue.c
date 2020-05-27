#include "motor/motor_queue.h"

void Motor_Queue_create()
{
    motor_queue = xQueueCreate(MOTOR_QUEUE_LENGTH, sizeof(Motor_Message));

    if (!motor_queue)
    {
        code(MOTOR_QUEUE_CREATE_FAILED);
        stop_everything();
    }
}

int Motor_Queue_read(Motor_Message *message)
{
    int read_ok = xQueueReceive(motor_queue, message, portMAX_DELAY);

    if (!read_ok)
    {
        code(MOTOR_QUEUE_READ_FAILED);
        stop_everything();
    }

    return read_ok;
}

int Motor_Queue_send_command(Motor_Message *message)
{
    int send_ok = xQueueSend(motor_queue, (void *)message, portMAX_DELAY);

    if (!send_ok)
    {
        code(MOTOR_QUEUE_SEND_FAILED);
        stop_everything();
    }
    return send_ok;
}
