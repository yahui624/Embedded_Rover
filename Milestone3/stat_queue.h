/*
 * stat_queue.h
 *
 */

#ifndef STAT_QUEUE_H_
#define STAT_QUEUE_H_

#include <FreeRTOS.h>
#include <queue.h>

#define QUEUE_LENGTH 128
#define QUEUE_ITEM_SIZE 20

typedef enum dataType
{
    noValue, messageValue, errorValue
} dataType;

typedef struct dataValue
{
    unsigned int sequence;
    char * payload;
    unsigned int board;
    unsigned int checksum;

} dataValue;

typedef struct dataStruct
{
    dataType type;
    dataValue value;
} dataStruct;

QueueHandle_t publishQueue;
QueueHandle_t subscribeQueue;

int sendStatsToPubQ(dataStruct statVal); // send value to queue

int sendStatsToSubQ(); // send stats to sub queue

dataStruct readStatsPubQ(); // read stat from pub queue

uint8_t readStatsSubQ(); // read stats from sub queue

void initQueues();

#endif /* STAT_QUEUE_H_ */
