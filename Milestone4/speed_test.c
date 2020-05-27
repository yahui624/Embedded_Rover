/*
 * speed_test.c
 *
 *  Created on: Mar 26, 2020
 *      Author: Yahui
 */


#include <speed_test.h>


void *speedThread(void *arg0) {
    initSpeedQueue();
    initSpeedTimer();

//    sendSpeedMsgToSpeedQueue(60, 90, 5);
    while(1){

    }
}
