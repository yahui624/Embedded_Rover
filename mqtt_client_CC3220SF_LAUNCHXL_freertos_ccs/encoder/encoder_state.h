#ifndef ENCODER_STATE_H_
#define ENCODER_STATE_H_

#include <stdio.h>

#include "debug_module/debug.h"
#include "encoder/encoder_interface.h"
#include "mqtt_client/publish_queue.h"
#include "pid/pid_queue.h"
#include "spi/SPI_interface.h"
#include "ti/drivers/Timer.h"

typedef enum
{
    WAITING_FOR_TIMER,
    READING1,
    READING2,
    READING3,
    CLEAR1,
    CLEAR2,
    CLEAR3,
    CALLBACK3
} ENCODER_STATES;

typedef struct
{
    long encoder_reading[MOTORS];

    // Pointer to rx_buffer
    uint8_t *rx_buffer;

    ENCODER_STATES encoder_state;
} Encoder_Info;

void update_encoder_state(Timer_Handle *timer,
                          SPI_Handle *  spi,
                          Encoder_Info *info,
                          int *         spi_transferred,
                          int *         timer_elapsed);

#endif /* ENCODER_STATE_H_ */
