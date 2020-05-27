

#include "encoder_queue.h"

void Encoder_Queue_create()
{
    encoder_queue = xQueueCreate(ENCODER_QUEUE_LENGTH, sizeof(uint8_t));

    if (!encoder_queue)
    {
        code(ENCODER_QUEUE_CREATE_FAILED);
        stop_everything();
    }
}

int Encoder_Queue_read(int *spi_transferred, int *timer_elapsed)
{
    int read_ok = TRUE;

    uint8_t message;

    // if (HwiP_inISR())
    // {
    //     return FALSE;
    // }
    xQueueReceive(encoder_queue, &message, portMAX_DELAY);
    switch (message)
    {
        case TIMER_ID:
            *timer_elapsed = TRUE;
            break;
        case SPI_ID:
            *spi_transferred = TRUE;
            break;
        default:
            read_ok = FALSE;
            break;
    }
    if (!read_ok)
    {
        code(ENCODER_QUEUE_READ_FAILED);
        stop_everything();
    }
    return read_ok;
}

void Encoder_Queue_send_timer_elapsed()
{
    uint8_t message = TIMER_ID;

    if (!xQueueSendFromISR(encoder_queue, (void *)&message, NULL))
    {
        // Failed to send queue full
    }
}

void Encoder_Queue_send_SPI_transfer_completed()
{
    uint8_t message = SPI_ID;

    if (!xQueueSendFromISR(encoder_queue, (void *)&message, NULL))
    {
        // Failed to send queue full
    }
}
