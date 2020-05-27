/*
 * stat_queue.c
 *
 */

#include "stat_queue.h"

// Send statistics to the publish queue
int sendStatsToPubQ(dataStruct statVal)
{
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    BaseType_t result = xQueueSendFromISR(publishQueue, &statVal,
                                         &xHigherPriorityTaskWoken);
    return (pdPASS == result);
}

// Send statistics to the subscribe queue
int sendStatsToSubQ()
{
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    uint8_t ack = 1;
    BaseType_t result = xQueueSendToBack(subscribeQueue, &ack,
                                                &xHigherPriorityTaskWoken);
    return (pdPASS == result);
}

// Read statistics from the publish queue
dataStruct readStatsPubQ()
{
    dataStruct data = { .type = noValue, .value.sequence = 0,
                         .value.payload = "" , .value.board= 0 };
    xQueueReceive(publishQueue, &data, portMAX_DELAY);
    return data;
}

// Read statistics from the subscribe queue
uint8_t readStatsSubQ()
{
    uint8_t ack = 0;
    xQueueReceive(subscribeQueue, &ack, 0);
    return ack;
}

void initQueues()
{
    publishQueue = xQueueCreate(QUEUE_LENGTH, sizeof(dataStruct));
    subscribeQueue = xQueueCreate(QUEUE_LENGTH, sizeof(uint8_t));
}
