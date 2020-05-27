/*
 * pidControl.h
 *
 *  Created on: Apr 25, 2020
 *      Author: Yahui
 */

#ifndef PIDCONTROL_H_
#define PIDCONTROL_H_


#include <FreeRTOS.h>
#include <speedCommand_queue.h>
#include <speedEncoder_queue.h>
#include <encoder_timer.h>
#include <motor_control.h>

#define kp_m1  0.2 // 0.45 * (127/67)
#define kp_m2  0.8 // 0.45 * (127/66)
#define kp_m3  0.8 // 0.45 * (127/70)

//#define kp_m1  0 // 0.45 * (127/67)
//#define kp_m2  0 // 0.45 * (127/66)
//#define kp_m3  0.8 // 0.45 * (127/70)


#define ki_m1  0.8
#define ki_m2  0.2
#define ki_m3  1

#define dt 0.1
//static float  kp_m1 = 0.89;
//static float  kp_m2 = 0.89;
//static float  kp_m3 = 0.89;
//static float  ki_m1 = 0.06401;
//static float  ki_m2 = 0.016;
//static float  ki_m3 = 0.0151;

static speed_msg prev_speed_cmd = {idle_waitng, 0, 0, idle_direction, 0, idle_direction, 0, idle_direction};



static int32_t sum_Error_m1 = 0;
static int32_t sum_Error_m2 = 0;
static int32_t sum_Error_m3 = 0;

static double prev_Error_m1 = 0;
static double prev_Error_m2 = 0;
static double prev_Error_m3 = 0;

static int32_t prev_speed[3] = {0,0,0};

bool speed_cmd_update (speed_msg current_input);

uint8_t  pid_algorithm(char address, double* integral, int32_t desire_speed, int32_t prev_speed);

uint8_t speedToEncoderValue(char address, double speed);

#endif /* PIDCONTROL_H_ */
