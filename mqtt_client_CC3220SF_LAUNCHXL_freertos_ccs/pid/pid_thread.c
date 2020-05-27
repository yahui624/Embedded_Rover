#ifndef PID_THREAD_H_
#define PID_THREAD_H_

#include <stdio.h>
#include <stdlib.h>

#include "debug_module/debug.h"
#include "motor/motor_queue.h"
#include "mqtt_client/publish_queue.h"
#include "mqtt_client/uart_term.h"
#include "pid/pid_queue.h"
#include "timers/timer_interface.h"

#define KI1 0.3
#define KI2 0.3
#define KI3 0.3

#define KP1 0.8
#define KP2 0.8
#define KP3 0.8

#define SLOPE1      1.2365
#define SLOPE2      1.2133
#define SLOPE3      1.1795
#define YINTERCEPT1 -4.4188
#define YINTERCEPT2 -3.0015
#define YINTERCEPT3 -3.7148
#define XINTERCEPT1 3.5736
#define XINTERCEPT2 2.4738
#define XINTERCEPT3 3.1495

#define PERIOD 0.05

static const double slope[MOTORS]      = {SLOPE1, SLOPE2, SLOPE3};
static const double yintercept[MOTORS] = {YINTERCEPT1, YINTERCEPT2, YINTERCEPT3};
static const double xintercept[MOTORS] = {XINTERCEPT1, XINTERCEPT2, XINTERCEPT3};

double  pid_calculate(double Kp, long error, double Ki, double *error_accumulated, double time);
uint8_t pid_convert(int motor_index, double output_speed);

double pid_calculate(double Kp, long error, double Ki, double *error_accumulated, double time)
{
    double error_f = (double)error;
    *error_accumulated += error_f * time;
    return (Kp * error_f + Ki * (*error_accumulated));
}

uint8_t pid_convert(int motor_index, double output_speed)
{
    uint8_t output_power;
    double  intermediate_value;

    if (output_speed < xintercept[motor_index])
    {
        return ZERO;
    }
    // power (0-127) = slope * speed (in encoder ticks) + intercept
    intermediate_value = (output_speed - yintercept[motor_index]) / slope[motor_index];

    output_power = (long)intermediate_value;
    if (output_power > MAX_POWER)
    {
        output_power = MAX_POWER;
    }
    if (intermediate_value < ZERO)
    {
        output_power = ZERO;
    }
    return output_power;
}

void *pid_thread(void *arg0)
{
    code(PID_THREAD_START);
    double Ki[MOTORS] = {KI1, KI2, KI3};
    double Kp[MOTORS] = {KP1, KP2, KP3};
    /*************** Received Data *****************/
    // uint8_t message_id;
    // long    encoder_reading[MOTORS];
    // uint8_t desired_speed[MOTORS];
    // uint8_t direction[MOTORS];
    // uint8_t motor[MOTORS]
    PID_Message data = {
        ZERO, {ZERO, ZERO, ZERO}, {ZERO, ZERO, ZERO}, {ZERO, ZERO, ZERO}, {ZERO, ZERO, ZERO}};

    // /************* MQTT Publish Data ***************/
    // long    encoder_reading;
    // int8_t  direction;
    // long    desired_speed;
    // long    output_speed;
    // uint8_t output_power;
    // uint8_t active;
    Motor_Debug debug_info[MOTORS] = {{ZERO, ZERO, ZERO, ZERO, ZERO, ZERO},
                                      {ZERO, ZERO, ZERO, ZERO, ZERO, ZERO},
                                      {ZERO, ZERO, ZERO, ZERO, ZERO, ZERO}};

    /************* Adjusted Motor Powers ***************/
    // uint8_t power[MOTORS];
    // uint8_t direction[MOTORS];
    // uint8_t motor[MOTORS];
    Motor_Message motor_message = {{ZERO, ZERO, ZERO}, {ZERO, ZERO, ZERO}, {ZERO, ZERO, ZERO}};

    long    current_speed[MOTORS]     = {ZERO, ZERO, ZERO};
    long    error[MOTORS]             = {ZERO, ZERO, ZERO};
    double  error_accumulated[MOTORS] = {ZERO, ZERO, ZERO};
    double  output_speed[MOTORS]      = {ZERO, ZERO, ZERO};
    uint8_t output_power[MOTORS]      = {ZERO, ZERO, ZERO};

    code(PID_THREAD_WHILE_LOOP);
    while (TRUE)
    {
        code(PID_THREAD_READQ_BEGIN);
        PID_Queue_read(&data);
        code(PID_THREAD_READQ_END);

        int i;
        for (i = 0; i < MOTORS; i++)
        {
            current_speed[i] = abs(data.encoder_reading[i]);
            error[i]         = data.desired_speed[i] - current_speed[i];

            output_speed[i] = pid_calculate(Kp[i], error[i], Ki[i], &error_accumulated[i], PERIOD) +
                              current_speed[i];

            output_power[i] = pid_convert(i, output_speed[i]);

#ifdef PUBLISH_PID_INFO
            debug_info[i].desired_speed   = data.desired_speed[i];
            debug_info[i].encoder_reading = current_speed[i];
            debug_info[i].output_speed    = (long)output_speed[i];
            debug_info[i].output_power    = output_power[i];
            debug_info[i].direction       = data.direction[i];
            debug_info[i].active          = data.motor[i];
#endif
            motor_message.power[i]     = output_power[i];
            motor_message.direction[i] = data.direction[i];
            motor_message.motor[i]     = data.motor[i];
        }

        code(PID_THREAD_MOTOR_SENDQ_BEGIN);
        Motor_Queue_send_command(&motor_message);
        code(PID_THREAD_MOTOR_SENDQ_END);

#ifdef PUBLISH_PID_INFO
        code(PID_THREAD_PUBLISH_SENDQ_BEGIN);
        Publish_Queue_send_debug(debug_info[MOTOR1], debug_info[MOTOR2], debug_info[MOTOR3]);
        code(PID_THREAD_PUBLISH_SENDQ_END);
#endif
    }
}
#endif  // PID_THREAD_H_
