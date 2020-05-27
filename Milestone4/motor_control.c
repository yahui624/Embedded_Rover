/*
 * motor_control.c
 * This file use UART communication to contorl the motor
 *  Created on: Feb 23, 2020
 *      Author: Yahui
 */

#include <motor_control.h>

void initMotorUART(){
    /* Create a UART with data processing off. */
    UART_Params_init(&motoruartParams);
    motoruartParams.writeDataMode = UART_DATA_BINARY;
    motoruartParams.baudRate = MOTOR_BAUD_RATE;

    motoruart = UART_open(MOTOR_UART, &motoruartParams);

    if (motoruart == NULL) {
        /* UART_open() failed */
        while (1);
    }
//    sleep(3);
//    UART_write(uart, START_BAUD_RATE, sizeof(START_BAUD_RATE)); // Start UART
//    sendSpeedMsgToSpeedQueue(14, 44, 8); // only testing allowance for this

}

void DriveMotor(char address,char direction, uint8_t speed) {

    UART_write(motoruart, &address, sizeof(address));
    UART_write(motoruart, &direction, sizeof(direction));
    UART_write(motoruart, &speed, sizeof(speed));
    unsigned char checksum = address + direction + speed;
    unsigned char result = checksum & MASK; // checksum
    UART_write(motoruart, &result, sizeof(checksum));
}

void *motorThread(void *arg0) {
    moving_msg movingMsg_receive;
//    char direction = DRIVE_FORWARD;
    initSpeedEncoderQueue();

    initMotorUART();
    while (1) {
        movingMsg_receive = readMsgFromMovingQueue();// assume always have msg_receive
        DriveMotor(MOTOR_1_ADDRESS, movingMsg_receive.m1_cmd.direction_cmd, movingMsg_receive.m1_cmd.speed_val);
        DriveMotor(MOTOR_2_ADDRESS, movingMsg_receive.m2_cmd.direction_cmd, movingMsg_receive.m2_cmd.speed_val);
        DriveMotor(MOTOR_3_ADDRESS, movingMsg_receive.m3_cmd.direction_cmd, movingMsg_receive.m3_cmd.speed_val);

    }

}
