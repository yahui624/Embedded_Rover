

#include "encoder_state.h"

void update_encoder_state(Timer_Handle *timer,
                          SPI_Handle *  spi,
                          Encoder_Info *info,
                          int *         spi_transferred,
                          int *         timer_elapsed)
{
    switch (info->encoder_state)
    {
        case WAITING_FOR_TIMER:
            if (*timer_elapsed)
            {
                *timer_elapsed = FALSE;
                GPIO_write(SLAVE_SELECT1, LOW);
                encoder_interface_read(spi, info->rx_buffer);
                info->encoder_state = READING2;
            }
            break;
        case READING1:
            info->encoder_state = READING2;
            break;
        case READING2:
            if (*spi_transferred)
            {
                GPIO_write(SLAVE_SELECT1, HIGH);
                info->encoder_reading[ENCODER1] = encoder_interface_assemble_bytes(info->rx_buffer);

                *spi_transferred = FALSE;

                GPIO_write(SLAVE_SELECT2, LOW);
                encoder_interface_read(spi, info->rx_buffer);
                info->encoder_state = READING3;
            }
            break;
        case READING3:
            if (*spi_transferred)
            {
                GPIO_write(SLAVE_SELECT2, HIGH);

                info->encoder_reading[ENCODER2] = encoder_interface_assemble_bytes(info->rx_buffer);

                *spi_transferred = FALSE;

                GPIO_write(SLAVE_SELECT3, LOW);
                encoder_interface_read(spi, info->rx_buffer);
                info->encoder_state = CLEAR1;
            }
            break;

        case CLEAR1:
            if (*spi_transferred)
            {
                GPIO_write(SLAVE_SELECT3, HIGH);
                info->encoder_reading[ENCODER3] = encoder_interface_assemble_bytes(info->rx_buffer);

                *spi_transferred = FALSE;
                GPIO_write(SLAVE_SELECT1, LOW);
                encoder_interface_clear_counter(spi);
                info->encoder_state = CLEAR2;
            }
            break;
        case CLEAR2:
            if (*spi_transferred)
            {
                GPIO_write(SLAVE_SELECT1, HIGH);
                *spi_transferred = FALSE;
                GPIO_write(SLAVE_SELECT2, LOW);
                encoder_interface_clear_counter(spi);
                info->encoder_state = CLEAR3;
            }
            break;
        case CLEAR3:
            if (*spi_transferred)
            {
                GPIO_write(SLAVE_SELECT2, HIGH);
                *spi_transferred = FALSE;
                GPIO_write(SLAVE_SELECT3, LOW);
                encoder_interface_clear_counter(spi);
                info->encoder_state = CALLBACK3;
            }
            break;
        case CALLBACK3:
            if (*spi_transferred)
            {
                GPIO_write(SLAVE_SELECT3, HIGH);
                *spi_transferred = FALSE;

                // Send to PI Algorithm
                code(ENCODER_STATE_SENDQ_BEGIN);
                PID_Queue_send_from_encoder(info->encoder_reading[ENCODER1],
                                            info->encoder_reading[ENCODER2],
                                            info->encoder_reading[ENCODER3]);
                code(ENCODER_STATE_SENDQ_END);

                //                Motor_Debug motor1_debug = {ZERO, ZERO, ZERO, ZERO, ZERO, ZERO};
                //                Motor_Debug motor2_debug = {ZERO, ZERO, ZERO, ZERO, ZERO, ZERO};
                //                Motor_Debug motor3_debug = {ZERO, ZERO, ZERO, ZERO, ZERO, ZERO};
                //
                //                motor1_debug.encoder_reading = info->encoder_reading[MOTOR1];
                //                motor2_debug.encoder_reading = info->encoder_reading[MOTOR2];
                //                motor3_debug.encoder_reading = info->encoder_reading[MOTOR3];
                //
                //                code(PID_THREAD_PUBLISH_SENDQ_BEGIN);
                //                Publish_Queue_send_debug(motor1_debug, motor2_debug,
                //                motor3_debug);

                if (Timer_start(*timer) == Timer_STATUS_ERROR)
                {
                    code(TIMER_ENCODER_START_FAILED);
                    stop_everything();
                }

                info->encoder_state = WAITING_FOR_TIMER;
            }
            break;
        default:
            code(ENCODER_STATE_UNKNOWN);
            stop_everything();
            break;
    }
}
