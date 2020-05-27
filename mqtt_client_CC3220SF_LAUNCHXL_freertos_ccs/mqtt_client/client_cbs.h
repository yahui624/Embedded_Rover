#ifndef __SERVER_CLIENT_CBS_H__
#define __SERVER_CLIENT_CBS_H__

/* MQTT library includes                                                      */
#include <stdlib.h>
#include <ti/net/mqtt/mqttclient.h>
#include <ti/net/mqtt/mqttserver.h>

#include "debug_module/debug.h"
#include "mqtt_client/mqtt_client_constants.h"
#include "uart_term.h"

//*****************************************************************************
// Macros
//*****************************************************************************
#define MAX_CONNECTION 1

#define MSG_RECV_BY_CLIENT 11
/* Client disconnected from remote broker       */
#define LOCAL_CLIENT_DISCONNECTION 12
#define DISC_PUSH_BUTTON_PRESSED   14
#define THREAD_TERMINATE_REQ       15
#define TOPIC_MAX_SIZE             20
#define PAYLOAD_MAX_SIZE           1024
//*****************************************************************************
//                          LOCAL DEFINES
//*****************************************************************************
#define APP_PRINT Report

#define OS_WAIT_FOREVER (0xFFFFFFFF)
#define OS_NO_WAIT      (0)
#define OS_OK           (0)

#define MQTTClientCbs_ConnackRC(data) (data & 0xff)

//*****************************************************************************
//                 GLOBAL VARIABLES
//*****************************************************************************
extern bool gResetApplication;

/* Message Queue                                                              */
typedef struct
{
    void *topicPtr;
    void *payloadPtr;
} Receive_Data;

extern char *      topic[];
struct client_info client_info_table[MAX_CONNECTION];
extern int32_t MQTT_SendMsgToQueue(Receive_Data *queueElement);
//extern mqd_t       g_PBQueue;

struct publishMsgHeader
{
    uint32_t      topicLen;
    uint32_t      payLen;
    bool          retain;
    bool          dup;
    unsigned char qos;
};

struct client_info
{
    void *ctx;
};

struct msgQueue
{
    int32_t event;
    void *  msgPtr;
    int32_t topLen;
};


//******************************************************************************
// APIs
//******************************************************************************

extern void MqttClientCallback(
    int32_t event, void *metaData, uint32_t metaDateLen, void *data, uint32_t dataLen);
#endif  // __SERVER_CLIENT_CBS_H__
