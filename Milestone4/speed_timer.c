/*
 * speed_timer.c
 *
 *  Created on: Mar 26, 2020
 *      Author: Yahui
 */

#include <speed_timer.h>


// timer for sending the speed command

void timer500Callback(Timer_Handle myHandle){
    // gradually change the speed
    speed_1 = speed_1 - 2;
    speed_2 = speed_2 + 50;
    speed_3 = speed_3 + 10;
    sendSpeedMsgToSpeedQueue(speed_1, DRIVE_FORWARD, speed_2, DRIVE_FORWARD, speed_3, DRIVE_FORWARD);
}

void initSpeedTimer() {
    Timer_Params speed_params;
    Timer_Params_init(&speed_params);

    speed_params.period = 500000; // period is 500ms
    speed_params.periodUnits = Timer_PERIOD_US;
    speed_params.timerMode = Timer_CONTINUOUS_CALLBACK;
    speed_params.timerCallback = timer500Callback; // callback function


    speed_timer = Timer_open(SPEED_TIMER, &speed_params);


    if (speed_timer == NULL) {
        /* Failed to initialize timer */
    }

    if (Timer_start(speed_timer) == Timer_STATUS_ERROR) {
        /* Failed to start timer */
        int ok = 1;
    }
}
