/*
 * mqtt_client_constants.h
 *
 *  Created on: Apr 1, 2020
 *      Author: hungnguyen
 */

#ifndef MQTT_CLIENT_MQTT_CLIENT_CONSTANTS_H_
#define MQTT_CLIENT_MQTT_CLIENT_CONSTANTS_H_

//*****************************************************************************
//                          LOCAL DEFINES
//*****************************************************************************
/* enables secured client                                                    */
//#define SECURE_CLIENT

/* enables client authentication by the server                               */
#define CLNT_USR_PWD

#define CLIENT_INIT_STATE (0x01)
#define MQTT_INIT_STATE   (0x04)

#define APPLICATION_VERSION "1.1.1"
#define APPLICATION_NAME    "MQTT client"

#define SLNET_IF_WIFI_PRIO (5)

/* Operate Lib in MQTT 3.1 mode.                                             */
#define MQTT_3_1_1 false
#define MQTT_3_1   true

#define WILL_TOPIC  "Client"
#define WILL_MSG    "Client Stopped"
#define WILL_QOS    MQTT_QOS_2
#define WILL_RETAIN false

/* Defining Broker IP address and port Number                                */
//#define SERVER_ADDRESS           "messagesight.demos.ibm.com"
#define SERVER_ADDRESS      "192.168.0.102"  //"m16.cloudmqtt.com"
#define SERVER_IP_ADDRESS   "192.168.0.102"
#define PORT_NUMBER         1883  // 18352
#define SECURED_PORT_NUMBER 28352
#define LOOPBACK_PORT       1882

/* Clean session flag                                                        */
#define CLEAN_SESSION true

/* Retain Flag. Used in publish message.                                     */
#define RETAIN_ENABLE 0

/* Defining Number of subscription topics                                    */
#define SUBSCRIPTION_TOPIC_COUNT 1

/* Defining Subscription Topic Values                                        */

#define SUBSCRIPTION_TOPIC0 "sensors_status"

/* Defining Publish Topic Values                                             */
#define PUBLISH_TOPIC0 "trirover_status"
#define PUBLISH_TOPIC1 "statistics"
#define PUBLISH_TOPIC2 "debug"

#define PUBLISH_TOPIC0_LEN 16
#define PUBLISH_TOPIC1_LEN 11
#define PUBLISH_TOPIC2_LEN 6

// Calculate the amount of ""=5, : =5, {} =1, spaces = 10
#define PUBLISH_TOPIC0_DATA_LENGTH 150  // 5 + 5 + 1 + 10 + 10 * 5, 10 * 5 + 10
// Calculate the amount of ""=4, : =4, {} =1, spaces = 8
#define PUBLISH_TOPIC1_DATA_LENGTH 150  // 4 + 4 + 1 + 10 + 10 * 4, 10 * 4 + 10

#define PUBLISH_TOPIC2_DATA_LENGTH 500

/* Spawn task priority and Task and Thread Stack Size                        */
#define TASKSTACKSIZE       4096
#define RXTASKSIZE          4096
#define MQTTTHREADSIZE      4096
#define PUBSUB_THREAD_SIZE  8192
#define SPAWN_TASK_PRIORITY 9

#define MAX_MOVEMENT_ITEMS 3
#define MAX_DEBUG_ITEMS    15

#define TOPIC_1 "statistics"
#define TOPIC_2 "sensors_status"
#define TOPIC_3 "arm_status"

#define TRIROVER_ID    3
#define SENSORS_ID     1
#define ARM_ID         2
#define HEXADECIMAL    16
#define DECIMAL        10
#define MAX_INT_STRING 10
#define MAX_BUFFER     200
#define MAX_TOPIC      20
#endif /* MQTT_CLIENT_MQTT_CLIENT_CONSTANTS_H_ */
