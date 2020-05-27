/*
 * encoder_timer.c
 *
 *  Created on: Feb 25, 2020
 *      Author: Yahui
 */


#include <encoder_timer.h>

current_speed updateCurrentSpeed(int32_t curr_SS1, int32_t curr_SS2, int32_t curr_SS3){

    curr_speed.e1_speed = curr_SS1;
    curr_speed.e2_speed = curr_SS2;
    curr_speed.e3_speed = curr_SS3;

//    getCurrentSpeed();
}

current_speed getCurrentSpeed(){
    return curr_speed;
}



void timer100Callback(Timer_Handle myHandle)
{
    // read the encoder value for three motors
    int32_t init_SS1 = read_SPI_Encoder(Slave_Motor1_Encoder);
    GPIO_write(Slave_Motor1_Encoder, 1);
    clear_SPI_Encoder_Count(Slave_Motor1_Encoder);
    GPIO_write(Slave_Motor1_Encoder, 1);

    int32_t init_SS2 = read_SPI_Encoder(Slave_Motor2_Encoder);
    GPIO_write(Slave_Motor2_Encoder, 1);
    clear_SPI_Encoder_Count(Slave_Motor2_Encoder);
    GPIO_write(Slave_Motor2_Encoder, 1);

    int32_t init_SS3 = read_SPI_Encoder(Slave_Motor3_Encoder);
    GPIO_write(Slave_Motor3_Encoder, 1);
    clear_SPI_Encoder_Count(Slave_Motor3_Encoder);
    GPIO_write(Slave_Motor3_Encoder, 1);

    sendSPIMsgToEncoderQueue(init_SS1, init_SS2, init_SS3); //

    updateCurrentSpeed(init_SS1, init_SS2, init_SS3);

}


// Initialize the timer
void initEncoderTimer()
{

    Timer_Params_init(&encoder_params);

    encoder_params.period = 100000; // period is 100ms
    encoder_params.periodUnits = Timer_PERIOD_US;
    encoder_params.timerMode = Timer_CONTINUOUS_CALLBACK;
    encoder_params.timerCallback = timer100Callback; // callback function

    encoder_timer = Timer_open(ENCODER_TIMER, &encoder_params);

    if (encoder_timer == NULL) {
        /* Failed to initialize timer */
    }

    if (Timer_start(encoder_timer) == Timer_STATUS_ERROR) {
        /* Failed to start timer */
    }

}
