
#include "client_cbs.h"

void MqttClientCallback(
    int32_t event, void *metaData, uint32_t metaDateLen, void *data, uint32_t dataLen)
{
    switch ((MQTTClient_EventCB)event)
    {
        // Operation Callback Event Related to Connection Status
        case MQTTClient_OPERATION_CB_EVENT:
        {
            switch (((MQTTClient_OperationMetaDataCB *)metaData)->messageType)
            {
                case MQTTCLIENT_OPERATION_CONNACK:
                {
                    uint16_t *ConnACK = (uint16_t *)data;
                    APP_PRINT("CONNACK:\n\r");
                    /* Check if Conn Ack return value is Success (0) or       */
                    /* Error - Negative value                                 */
                    if (0 == (MQTTClientCbs_ConnackRC(*ConnACK)))
                    {
                        APP_PRINT("Connection Success\n\r");
                    }
                    else
                    {
                        APP_PRINT("Connection Error: %d\n\r", *ConnACK);
                    }
                    break;
                }

                case MQTTCLIENT_OPERATION_EVT_PUBACK:
                {
                    char *PubAck = (char *)data;
                    APP_PRINT("PubAck:\n\r");
                    APP_PRINT("%s\n\r", PubAck);
                    break;
                }

                case MQTTCLIENT_OPERATION_SUBACK:
                {
                    APP_PRINT("Sub Ack:\n\r");
                    APP_PRINT("Granted QoS Levels are:\n\r");
                    int i;
                    for (i = 0; i < dataLen; i++)
                    {
                        APP_PRINT("%s :QoS %d\n\r", topic[i], ((unsigned char *)data)[i]);
                    }
                    break;
                }

                case MQTTCLIENT_OPERATION_UNSUBACK:
                {
                    char *UnSub = (char *)data;
                    APP_PRINT("UnSub Ack \n\r");
                    APP_PRINT("%s\n\r", UnSub);
                    break;
                }

                default:
                    break;
            }
            break;
        }
        case MQTTClient_RECV_CB_EVENT:
        {
            MQTTClient_RecvMetaDataCB *recvMetaData = (MQTTClient_RecvMetaDataCB *)metaData;

            static char topic[MAX_TOPIC];
            static char payload[MAX_BUFFER];

            Receive_Data queueElem;

            /* copying the topic name into the buffer                        */
            memcpy((void *)(topic), (const void *)recvMetaData->topic, recvMetaData->topLen);
            memset((void *)(topic + recvMetaData->topLen), '\0', 1);

            /* copying the payload into the buffer                           */
            memcpy((void *)(payload), (const void *)data, dataLen);
            memset((void *)(payload + dataLen), '\0', 1);

            /* filling the queue element details                              */
            queueElem.topicPtr   = topic;
            queueElem.payloadPtr = payload;

            /* signal to the main task                                        */
            if (MQTT_SendMsgToQueue(&queueElem))
            {
                UART_PRINT("\n\n\rRecv Queue is full\n\n\r");
            }
            break;
        }
        case MQTTClient_DISCONNECT_CB_EVENT:
        {
            APP_PRINT("\n\rDISCONNECTED !!!\n\r");

            gResetApplication = true;
            code(CLIENT_CALLBACK_BRIDGE_DISCONNECTION);
            stop_everything();
            break;
        }
    }
}
