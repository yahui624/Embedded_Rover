/*
 * debug.c
 *
 *  Created on: Feb 2, 2020
 *      Author: sophie
 */
#include "debug.h"
#include <ti/devices/cc32xx/inc/hw_types.h>
#include <ti/drivers/GPIO.h>
#include <ti/devices/cc32xx/driverlib/gpio.h>
#include <stdbool.h>
#include <task.h>

//value will be written to the UART
void dbgUARTVal(unsigned char outVal)
{
    UART_write(uart_handling, &outVal, 1);
}

//value of outLoc must be <= 127.
//call your “fail” routine below if it is not in range
void dbgOutputLoc(unsigned int outLoc)
{
    if (outLoc > 127)
    {
        failRoutine(DEBUG_OUT_OF_BOUNDS);
    }

    //write outLoc to 7 of those 8 lines. Toggle the 8th line
    int gpio1 = (outLoc >> 0) & 0x1;
    int gpio2 = (outLoc >> 1) & 0x1;
    int gpio3 = (outLoc >> 2) & 0x1;
    int gpio4 = (outLoc >> 3) & 0x1;
    int gpio5 = (outLoc >> 4) & 0x1;
    int gpio6 = (outLoc >> 5) & 0x1;
    int gpio7 = (outLoc >> 6) & 0x1;

    GPIO_write(CONFIG_GPIO_1, gpio1);
    GPIO_write(CONFIG_GPIO_2, gpio2);
    GPIO_write(CONFIG_GPIO_3, gpio3);
    GPIO_write(CONFIG_GPIO_4, gpio4);
    GPIO_write(CONFIG_GPIO_5, gpio5);
    GPIO_write(CONFIG_GPIO_6, gpio6);
    GPIO_write(CONFIG_GPIO_7, gpio7);
//    GPIO_toggle(CONFIG_GPIO_8);
}

//failed, stop everything we have an error routine
void failRoutine(unsigned int outLoc)
{
    dbgOutputLoc(outLoc);
    //taskDISABLE_INTERRUPTS();
}

//initialize UART
void initializeUART()
{
    /* Create a UART with data processing off. */
    UART_Params_init(&uart_parameters);
    uart_parameters.writeDataMode = UART_DATA_BINARY;
    uart_parameters.readDataMode = UART_DATA_BINARY;
    uart_parameters.readReturnMode = UART_RETURN_FULL;
    uart_parameters.readEcho = UART_ECHO_OFF;
    uart_parameters.baudRate = 115200;

    uart_handling = UART_open(CONFIG_UART_0, &uart_parameters);

    if (uart_handling == NULL)
    {
        failRoutine(DEBUG_UART_NOT_OPEN);
    }
}

//initialize GPIO for debugging
void initializeGPIO()
{
    //Configure the 8 GPIO pins
    GPIO_setConfig(CONFIG_GPIO_1, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_LOW);
    GPIO_setConfig(CONFIG_GPIO_2, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_LOW);
    GPIO_setConfig(CONFIG_GPIO_3, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_LOW);
    GPIO_setConfig(CONFIG_GPIO_4, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_LOW);
    GPIO_setConfig(CONFIG_GPIO_5, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_LOW);
    GPIO_setConfig(CONFIG_GPIO_6, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_LOW);
    GPIO_setConfig(CONFIG_GPIO_7, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_LOW);
//    GPIO_setConfig(CONFIG_GPIO_8, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_LOW);
}
