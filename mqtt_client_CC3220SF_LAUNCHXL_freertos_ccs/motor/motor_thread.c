#ifndef MOTOR_THREAD_H_
#define MOTOR_THREAD_H_

#include <stdio.h>
#include <unistd.h>

#include "debug_module/debug.h"
#include "motor/motor_queue.h"
#include "mqtt_client/uart_term.h"
#include "ti/drivers/UART.h"
#define MOTOR1_ADDRESS       128
#define MOTOR2_ADDRESS       129
#define MOTOR3_ADDRESS       130
#define CHECKSUM_MASK        0b01111111
#define MOTOR_BUFFER_SIZE    4
#define MOTOR_UART_BAUD_RATE 9600
#define BAUD_CHARACTER       170

UART_Handle init_motor_uart();
void *      motor_thread(void *arg0);

UART_Handle init_motor_uart()
{
    /************************* UART INIT ******************************/
    UART_Params uartParams;

    UART_Params_init(&uartParams);
    uartParams.readMode       = UART_MODE_BLOCKING;
    uartParams.writeDataMode  = UART_DATA_BINARY;
    uartParams.readReturnMode = UART_RETURN_FULL;
    uartParams.baudRate       = 9600;
    uartParams.readEcho       = UART_ECHO_OFF;
    return UART_open(CONFIG_UART_MOTOR, &uartParams);
}

void *motor_thread(void *arg0)
{
    code(MOTOR_THREAD_START);

    UART_Handle uart = init_motor_uart();

    if (uart == NULL)
    {
        code(MOTOR_UART_OPEN_FAILED);
        stop_everything();
    }

    /************************* SEND BAUD ******************************/
    sleep(2);
    char baud_char = BAUD_CHARACTER;
    UART_write(uart, &baud_char, ONE);
    /************************* LOCAL VARIABLES ************************/

    Motor_Message message = {{ZERO, ZERO, ZERO}, {ZERO, ZERO, ZERO}, {ZERO, ZERO, ZERO}};

    uint8_t address[MOTORS] = {MOTOR1_ADDRESS, MOTOR2_ADDRESS, MOTOR3_ADDRESS};

    /************************* WHILE LOOP **** ************************/

    code(MOTOR_THREAD_WHILE);
    while (TRUE)
    {
        // Blocking Read Queue
        code(MOTOR_THREAD_READ_QUEUE_BEGIN);
        Motor_Queue_read(&message);
        code(MOTOR_THREAD_READ_QUEUE_DONE);
        int i;
        for (i = 0; i < MOTORS; i++)

        {
            if (message.motor[i])
            {
                char checksum =
                    (address[i] + message.direction[i] + message.power[i]) & CHECKSUM_MASK;
                //UART_PRINT("\n\r%d %d %d %d\n\r", buffer[0], buffer[1], buffer[2], buffer[3]);

                UART_write(uart, &address[i], ONE);
                UART_write(uart, &message.direction[i], ONE);
                UART_write(uart, &message.power[i], ONE);
                UART_write(uart, &checksum, ONE);
            }
        }
    }
}
#endif  // __MOTOR_THREAD_H_
