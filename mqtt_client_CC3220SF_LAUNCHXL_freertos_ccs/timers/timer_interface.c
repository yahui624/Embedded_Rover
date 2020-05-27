#include "timer_interface.h"

void timer_IR_callback(Timer_Handle timer_handle, int_fast16_t status)
{
    code(TIMER_IR_CALLBACK_START);
    IR_Queue_send_timer_elapsed();
    code(TIMER_IR_CALLBACK_END);
}

Timer_Handle timer_IR_initialize()
{
    Timer_Handle timer;
    Timer_Params params;

    Timer_Params_init(&params);
    params.period        = TIMER_IR_HZ;
    params.periodUnits   = Timer_PERIOD_HZ;
    params.timerMode     = Timer_CONTINUOUS_CALLBACK;
    params.timerCallback = timer_IR_callback;

    timer = Timer_open(CONFIG_TIMER_IR, &params);

    if (timer == NULL)
    {
        code(TIMER_OPEN_FAILED);
        stop_everything();
    }
    return timer;
}

void timer_statistics_callback(Timer_Handle timer_handle, int_fast16_t status)
{
    code(TIMER_CALLBACK_START);
    Statistics_Queue_send_timer_elapsed();
    code(TIMER_CALLBACK_END);
}

Timer_Handle timer_statistics_initialize()
{
    Timer_Handle timer;
    Timer_Params params;

    Timer_Params_init(&params);
    params.period        = TIMER_STATISTICS_PERIOD;
    params.periodUnits   = Timer_PERIOD_US;
    params.timerMode     = Timer_CONTINUOUS_CALLBACK;
    params.timerCallback = timer_statistics_callback;

    timer = Timer_open(CONFIG_TIMER_STATISTICS, &params);

    if (timer == NULL)
    {
        code(TIMER_OPEN_FAILED);
        stop_everything();
    }
    return timer;
}

void timer_encoder_callback(Timer_Handle timer_handle, int_fast16_t status)
{
    code(TIMER_ENCODER_CALLBACK_START);
    Encoder_Queue_send_timer_elapsed();
    code(TIMER_ENCODER_CALLBACK_END);
}

Timer_Handle timer_encoder_initialize()
{
    Timer_Handle timer;
    Timer_Params params;

    Timer_Params_init(&params);
    params.period        = TIMER_ENCODER_PERIOD;  // timer period of 50 milliseconds
    params.periodUnits   = Timer_PERIOD_US;
    params.timerMode     = Timer_ONESHOT_CALLBACK;
    params.timerCallback = timer_encoder_callback;

    timer = Timer_open(CONFIG_TIMER_ENCODERS, &params);

    if (timer == NULL)
    {
        code(TIMER_OPEN_FAILED);
        stop_everything();
    }
    return timer;
}

void timer_movement_callback(Timer_Handle timer_handle, int_fast16_t status)
{
    code(TIMER_MOVEMENT_CALLBACK_START);
    Movement_Queue_send_timer_elapsed();
    code(TIMER_MOVEMENT_CALLBACK_END);
}

Timer_Handle timer_movement_initialize()
{
    Timer_Handle timer;
    Timer_Params params;

    Timer_Params_init(&params);
    params.period        = TIMER_MOVEMENT_PERIOD;
    params.periodUnits   = Timer_PERIOD_US;
    params.timerMode     = Timer_ONESHOT_CALLBACK;
    params.timerCallback = timer_movement_callback;

    timer = Timer_open(CONFIG_TIMER_MOVEMENT, &params);

    if (timer == NULL)
    {
        code(TIMER_OPEN_FAILED);
        stop_everything();
    }
    return timer;
}
