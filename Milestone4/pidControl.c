/*
 * pidControl.c
 *
 *  Created on: Apr 25, 2020
 *      Author: Yahui
 */

#include <pidControl.h>

bool speed_cmd_update (speed_msg current_input){
    bool update = false; //if any speed message changes, become true
    if (current_input.motor1_speed != prev_speed_cmd.motor1_speed ||
            current_input.motor2_speed != prev_speed_cmd.motor2_speed ||
            current_input.motor3_speed != prev_speed_cmd.motor3_speed) {
        update = true;
    }
    return update;
}


uint8_t  pid_algorithm(char address, double *integral, int32_t desire_speed, int32_t prev_speed){
    double temp_error = (double) (desire_speed - prev_speed);
    *integral += temp_error;
     double output;

     switch (address) {
     case MOTOR_1_ADDRESS:
         output = kp_m1 * temp_error +  *integral * ki_m1;
         break;
     case MOTOR_2_ADDRESS:
         output = kp_m2 * temp_error +  *integral * ki_m2;
         break;
     case MOTOR_3_ADDRESS:
         output = kp_m3 * temp_error +  *integral * ki_m3;
         break;
     }
     output += (double) desire_speed;

    return speedToEncoderValue(address, output);
}

uint8_t speedToEncoderValue(char address, double speed) {
    // turn the encoder value to motor power
    double temp;
    uint8_t result;
//    switch (address) {
//    case MOTOR_1_ADDRESS:
//        temp = (speed - 0.5483)/ 0.5342;
//        break;
//    case MOTOR_2_ADDRESS:
//        temp = (speed - 0.5012)/ 0.5322 ;
//        break;
//    case MOTOR_3_ADDRESS:
//        temp = (speed - 0.3073) / 0.5255;
//        break;
//    }

    switch (address) {
    case MOTOR_1_ADDRESS:
        temp = 1.9024 * speed + 0.5991;
        if (speed < -0.35) {
            temp = 0;
        }
        break;
    case MOTOR_2_ADDRESS:
        temp =(speed * 1.8719) + 1.0299;
        if (speed < -0.55) {
            temp = 0;
        }
        break;
    case MOTOR_3_ADDRESS:
        temp = 1.8789 * speed + 0.9462;
        if (speed < -0.5) {
            temp = 0;
        }
        break;
    }

    if (temp > 127) {
        temp = 127;
    }
    else if (temp < 0) {
        temp = 0;
    }
    result = (long)temp;
    return result;
}


void *controlThread(void *arg0) {
    initSpeedQueue();

    speed_msg new_speed;
    current_speed cur_speed;

    moving_msg motor_cmd;

    uint8_t  m1_action = 0;
    uint8_t  m2_action = 0;
    uint8_t  m3_action = 0;

    double wheel0_integral = 0;
    double wheel1_integral = 0;
    double wheel2_integral = 0;

    new_speed.motor1_speed = 0;
    new_speed.motor2_speed = 0;
    new_speed.motor3_speed = 0;

    while (1) {
        new_speed = readSpeedMsgFromSpeedQueue();

        if (speed_cmd_update(new_speed)) { // new speed message coming in
            wheel0_integral = 0;
            wheel1_integral = 0;
            wheel2_integral = 0;

            prev_speed_cmd = new_speed;
        }

        cur_speed = getCurrentSpeed();
        prev_speed[0] = cur_speed.e1_speed;
        prev_speed[1] = cur_speed.e2_speed;
        prev_speed[2] = cur_speed.e3_speed;

        m1_action = pid_algorithm(MOTOR_1_ADDRESS, &wheel0_integral, new_speed.motor1_speed, prev_speed[0]);
        m2_action = pid_algorithm(MOTOR_2_ADDRESS, &wheel1_integral, new_speed.motor2_speed, prev_speed[1]);
        m3_action = pid_algorithm(MOTOR_3_ADDRESS, &wheel2_integral, new_speed.motor3_speed, prev_speed[2]);

        motor_cmd.m1_cmd.direction_cmd = new_speed.motor1_direction;
        motor_cmd.m1_cmd.speed_val = m1_action;
        motor_cmd.m2_cmd.direction_cmd = new_speed.motor2_direction;
        motor_cmd.m2_cmd.speed_val = m2_action;
        motor_cmd.m3_cmd.direction_cmd = new_speed.motor3_direction;
        motor_cmd.m3_cmd.speed_val = m3_action;
//
//        new_speed.motor1_speed = 0;
//        new_speed.motor2_speed = 0;
//        new_speed.motor3_speed = 0;

        sendMsgToMovingQueue(motor_cmd);



    }

}


