/*
 * mqtt_client_app.h
 *
 *  Created on: Apr 1, 2020
 *      Author: hungnguyen
 */

#ifndef MQTT_CLIENT_MQTT_CLIENT_APP_H_
#define MQTT_CLIENT_MQTT_CLIENT_APP_H_

#include <mqueue.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

/* TI-Driver includes                                                        */
#include <ti/drivers/GPIO.h>
#include <ti/drivers/SPI.h>

/* Simplelink includes                                                       */
#include <ti/drivers/net/wifi/simplelink.h>

/* SlNetSock includes                                                        */
#include <ti/drivers/net/wifi/slnetifwifi.h>

/* MQTT Library includes                                                     */
#include <ti/net/mqtt/mqttclient.h>

/* Common interface includes                                                 */
#include "network_if.h"
#include "uart_term.h"

/* TI-DRIVERS Header files */
#include "ti_drivers_config.h"

/* Application includes                                                      */
#include "debug_module/debug.h"
#include "mqtt_client/client_cbs.h"
#include "mqtt_client/crc32.h"
#include "mqtt_client/mqtt_client_constants.h"
#include "mqtt_client/publish_queue.h"
#include "statistics/statistics_queue.h"
#include "timers/timer_interface.h"
#define JSMN_STATIC

#include "mqtt_client/json_parser/jsmn.h"

//// for crc32
//#include "ti/devices/cc32xx/driverlib/prcm.h"
//#include "ti/devices/cc32xx/driverlib/rom.h"
//#include "ti/devices/cc32xx/driverlib/rom_map.h"
//#include "ti/devices/cc32xx/inc/hw_types.h"

//*****************************************************************************
//                      LOCAL FUNCTION PROTOTYPES
//*****************************************************************************

static uint32_t calculate_publish_checksum(char *topic, Publish_Message *message);
static uint32_t calculate_receive_checksum(char *topic, JSON_Parse_Data *data);

void *          MqttClient(void *pvParameters);
void *          publish_thread(void *pvParameters);
void *          receive_thread(void *pvParameters);
int32_t         MQTT_SendMsgToQueue(Receive_Data *queueElement);

void Mqtt_start();

int32_t Mqtt_IF_Connect();
int32_t MqttClient_start();

//****************************************************************************
//                         EXTERNAL FUNTIONS
//****************************************************************************
extern int32_t ti_net_SlNet_initConfig();

//*****************************************************************************
//                 GLOBAL VARIABLES
//*****************************************************************************

/* Connection state: (0) - connected, (negative) - disconnected              */
int32_t                  gApConnectionState = -1;
uint32_t                 gInitState         = 0;
uint32_t                 memPtrCounterfree  = 0;
bool                     gResetApplication  = false;
static MQTTClient_Handle gMqttClient;
MQTTClient_Params        MqttClientExmple_params;

/* Receive task handle                                                       */
pthread_t g_rx_task_hndl = (pthread_t)NULL;
uint32_t  gUiConnFlag    = 0;

/* AP Security Parameters                                                    */
SlWlanSecParams_t SecurityParams = {0};

/* Client ID                                                              */
char *ClientId = "trirover";

/* Client User Name and Password                                             */
const char *ClientUsername = "track_rover";
const char *ClientPassword = "team15";

/* Subscription topics and qos values                                        */
char *topic[SUBSCRIPTION_TOPIC_COUNT] = {
    SUBSCRIPTION_TOPIC0};

unsigned char qos[SUBSCRIPTION_TOPIC_COUNT] = {MQTT_QOS_2};

/* Message Queue                                                             */
mqd_t g_PBQueue;

pthread_t mqttThread      = (pthread_t)NULL;
pthread_t appThread       = (pthread_t)NULL;
pthread_t receive_pthread = (pthread_t)NULL;
pthread_t publish_pthread = (pthread_t)NULL;

/* Printing new line                                                         */
char lineBreak[] = "\n\r";

//*****************************************************************************
//                 Banner VARIABLES
//*****************************************************************************

MQTTClient_ConnParams Mqtt_ClientCtx = {
    MQTTCLIENT_NETCONN_URL, SERVER_ADDRESS, PORT_NUMBER, 0, 0, 0, NULL};

/* Initialize the will_param structure to the default will parameters        */
MQTTClient_Will will_param = {WILL_TOPIC, WILL_MSG, WILL_QOS, WILL_RETAIN};

#endif /* MQTT_CLIENT_MQTT_CLIENT_APP_H_ */
