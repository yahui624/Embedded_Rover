#ifndef ENCODER_THREAD_H_
#define ENCODER_THREAD_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "debug_module/debug.h"
#include "encoder/encoder_interface.h"
#include "encoder/encoder_queue.h"
#include "encoder/encoder_state.h"
#include "ti_drivers_config.h"
#include "timers/timer_interface.h"

void  setup_encoders(SPI_Handle *spi);
void *encoder_thread(void *arg0);

void setup_encoders(SPI_Handle *spi)
{
    int stimer_elapsed   = FALSE;
    int sspi_transferred = FALSE;

    // Init TX's for all three encoders:
    // 1. Set mode
    // 2. Reset DTR
    // 3. Clear CTR
    GPIO_write(SLAVE_SELECT1, LOW);
    encoder_interface_set_mode(spi);
    Encoder_Queue_read(&sspi_transferred, &stimer_elapsed);
    sspi_transferred = FALSE;
    GPIO_write(SLAVE_SELECT1, HIGH);

    GPIO_write(SLAVE_SELECT1, LOW);
    encoder_interface_reset_data_register(spi);
    Encoder_Queue_read(&sspi_transferred, &stimer_elapsed);
    sspi_transferred = FALSE;
    GPIO_write(SLAVE_SELECT1, HIGH);

    GPIO_write(SLAVE_SELECT1, LOW);
    encoder_interface_clear_counter(spi);
    Encoder_Queue_read(&sspi_transferred, &stimer_elapsed);
    sspi_transferred = FALSE;
    GPIO_write(SLAVE_SELECT1, HIGH);

    GPIO_write(SLAVE_SELECT2, LOW);
    encoder_interface_set_mode(spi);
    Encoder_Queue_read(&sspi_transferred, &stimer_elapsed);
    sspi_transferred = FALSE;
    GPIO_write(SLAVE_SELECT2, HIGH);

    GPIO_write(SLAVE_SELECT2, LOW);
    encoder_interface_reset_data_register(spi);
    Encoder_Queue_read(&sspi_transferred, &stimer_elapsed);
    sspi_transferred = FALSE;
    GPIO_write(SLAVE_SELECT2, HIGH);

    GPIO_write(SLAVE_SELECT2, LOW);
    encoder_interface_clear_counter(spi);
    Encoder_Queue_read(&sspi_transferred, &stimer_elapsed);
    sspi_transferred = FALSE;
    GPIO_write(SLAVE_SELECT2, HIGH);

    GPIO_write(SLAVE_SELECT3, LOW);
    encoder_interface_set_mode(spi);
    Encoder_Queue_read(&sspi_transferred, &stimer_elapsed);
    sspi_transferred = FALSE;
    GPIO_write(SLAVE_SELECT3, HIGH);

    GPIO_write(SLAVE_SELECT3, LOW);
    encoder_interface_reset_data_register(spi);
    Encoder_Queue_read(&sspi_transferred, &stimer_elapsed);
    sspi_transferred = FALSE;
    GPIO_write(SLAVE_SELECT3, HIGH);

    GPIO_write(SLAVE_SELECT3, LOW);
    encoder_interface_clear_counter(spi);
    Encoder_Queue_read(&sspi_transferred, &stimer_elapsed);
    sspi_transferred = FALSE;
    GPIO_write(SLAVE_SELECT3, HIGH);
}

void *encoder_thread(void *arg0)
{
    code(ENCODER_THREAD_START);

    /************************* SPI INIT ******************************/
    SPI_Handle spi = SPI_interface_initialize();

    /************************* TIMER INIT ******************************/
    Timer_Handle timer_encoder = timer_encoder_initialize();

    /************************* LOCAL VARIABLES ******************************/

    // First byte is junk, the 4 bytes after make up the count value
    static uint8_t rx_buffer[] = {JUNK, ZERO, ZERO, ZERO, ZERO};
    Encoder_Info   info        = {{ZERO, ZERO, ZERO}, rx_buffer, WAITING_FOR_TIMER};
    // Info's Pointer to thread local rx buffer
    int timer_elapsed   = FALSE;
    int spi_transferred = FALSE;

    /************************* SETUP TRANSFERS ******************************/

    setup_encoders(&spi);

    /************************* TIMER START ******************************/
    if (Timer_start(timer_encoder) == Timer_STATUS_ERROR)
    {
        code(TIMER_ENCODER_START_FAILED);
        stop_everything();
    }

    code(ENCODER_THREAD_WHILE);
    while (TRUE)
    {
        code(ENCODER_THREAD_READQ_BEGIN);
        Encoder_Queue_read(&spi_transferred, &timer_elapsed);
        code(ENCODER_THREAD_READQ_END);

        /************************* STATE MACHINE ******************************/
        update_encoder_state(&timer_encoder, &spi, &info, &spi_transferred, &timer_elapsed);
    }
}

#endif  // ENCODER_THREAD_H_
