#include "mqtt_client_app.h"

//*****************************************************************************
//
//! MQTT_SendMsgToQueue - Utility function that receive msgQueue parameter and
//! tries to push it the queue with minimal time for timeout of 0.
//! If the queue isn't full the parameter will be stored and the function
//! will return 0.
//! If the queue is full and the timeout expired (because the timeout parameter
//! is 0 it will expire immediately), the parameter is thrown away and the
//! function will return -1 as an error for full queue.
//!
//! \param[in] struct msgQueue *queueElement
//!
//! \return 0 on success, -1 on error
//
//*****************************************************************************
int32_t MQTT_SendMsgToQueue(Receive_Data *queueElement)
{
    struct timespec abstime = {0};

    clock_gettime(CLOCK_REALTIME, &abstime);

    if (g_PBQueue)
    {
        /* send message to the queue                                        */
        if (mq_timedsend(g_PBQueue, (char *)queueElement, sizeof(Receive_Data), 0, &abstime) == 0)
        {
            return (0);
        }
    }
    return (-1);
}

void *MqttClientThread(void *pvParameters)
{
    MQTTClient_run((MQTTClient_Handle)pvParameters);

    pthread_exit(0);

    return (NULL);
}

static uint32_t calculate_publish_checksum(char *topic, Publish_Message *message)
{
    uint32_t length;
    uint32_t topic_length = strlen(topic);
    // Calculate large enough arrays
    if (message->message_id == PUBLISH_QUEUE_MOVEMENT_ID)
    {
        // (topic_name)(sequence)(has_block)(block_color)
        length = topic_length + MAX_INT_STRING * MAX_MOVEMENT_ITEMS + 1;
    }
    else if (message->message_id == PUBLISH_QUEUE_STATISTICS_ID)
    {
        // (topic_name)(recv_expect)(rec_actual)
        length = topic_length + MAX_INT_STRING + MAX_INT_STRING + 1;
    }
    else if (message->message_id == PUBLISH_QUEUE_DEBUG_ID)
    {
        // (topic_name)(Kp1...3)(Ki1...3)(reading1..3)(desired1...3)(actual1...3)
        length = topic_length + MAX_INT_STRING * MAX_DEBUG_ITEMS + 1;
    }

    char combined_string[length];

    if (message->message_id == PUBLISH_QUEUE_MOVEMENT_ID)
    {
        // (topic_name)(sequence)(has_block)(block_color)
        snprintf(combined_string,
                 length,
                 "%s%d%d%d",
                 topic,
                 message->sequence,
                 message->has_block,
                 message->block_color);
    }
    else if (message->message_id == PUBLISH_QUEUE_STATISTICS_ID)
    {
        // (topic_name)(recv_expect)(rec_actual)
        snprintf(combined_string,
                 length,
                 "%s%d%d",
                 topic,
                 message->receive_expected,
                 message->receive_actual);
    }
    else if (message->message_id == PUBLISH_QUEUE_DEBUG_ID)
    {
        // (topic_name)(Kp1...3)(Ki1...3)(reading1..3)(desired1...3)(actual1...3)
        int i;
        snprintf(combined_string, length, "%s", topic);
        Motor_Debug current;
        for (i = 0; i < MOTORS; i++)
        {
            switch (i)
            {
                case MOTOR1:
                    current = message->motor1;
                    break;
                case MOTOR2:
                    current = message->motor2;
                    break;
                case MOTOR3:
                    current = message->motor3;
                    break;
                default:
                    stop_everything();
                    break;
            }
            snprintf(combined_string,
                     length,
                     "%s%ld%d%ld%ld%d%d",
                     combined_string,
                     current.encoder_reading,
                     current.direction,
                     current.desired_speed,
                     current.output_speed,
                     current.output_power,
                     current.active);
        }
    }

    return calculate_CRC32((unsigned char *)combined_string, strlen(combined_string));
}

// Copy into one string  : "(topic_name)(sequence)(sensors data)"
static uint32_t calculate_receive_checksum(char *topic, JSON_Parse_Data *data)
{
    uint32_t topic_length = strlen(topic);
    uint32_t length;

    if (data->board_id == SENSORS_ID)
    {
        length = topic_length + MAX_INT_STRING + MAX_INT_STRING;

        char combined_string[length];
        snprintf(combined_string,
                 length,
                 "sensors_status%d%d%d%d",
                 data->sequence,
                 data->pathable,
                 data->color,
                 data->position);

        return calculate_CRC32((unsigned char *)combined_string, strlen(combined_string));
    }
    return ZERO;
}

void *receive_thread(void *pvParameters)
{
    Receive_Data message;
    while (TRUE)
    {
        // Receive from queue
        mq_receive(g_PBQueue, (char *)&message, sizeof(Receive_Data), NULL);

        JSON_Parse_Data parsed_data;

        int parse_ok = json_parser_process(&parsed_data, message.payloadPtr);

        if (parse_ok)
        {
            uint32_t checksum_calculated =
                calculate_receive_checksum(message.topicPtr, &parsed_data);

            if (checksum_calculated == parsed_data.checksum)
            {
                if (parsed_data.board_id == SENSORS_ID)
                {
//                    Movement_Queue_send_MQTT_sensors(
//                        parsed_data.pathable, parsed_data.color, parsed_data.position);
                }
                // Send to Statistics Queue
                Statistics_Queue_send_received_data(parsed_data.board_id, parsed_data.sequence);
            }
        }
    }
}

void *publish_thread(void *pvParameters)
{
    Publish_Message message;
    long            lRetVal = -1;

    int board_id = TRIROVER_ID;
    /*handling the signals from various callbacks including the push button  */
    /*prompting the client to publish a msg on PUB_TOPIC OR msg received by  */
    /*the server on enrolled topic(for which the on-board client ha enrolled)*/
    /*from a local client(will be published to the remote broker by the      */
    /*client) OR msg received by the client from the remote broker (need to  */
    /*be sent to the server to see if any local client has subscribed on the */
    /*same topic).                                                           */
    while (TRUE)
    {  // Publish QUeue Recv
        code(CLIENT_PUBLISH_READQ_BEGIN);
        Publish_Queue_read(&message);
        code(CLIENT_PUBLISH_READQ_END);
        if (message.message_id == PUBLISH_QUEUE_MOVEMENT_ID)
        {
            char     topic[PUBLISH_TOPIC0_LEN] = PUBLISH_TOPIC0;
            uint32_t checksum                  = calculate_publish_checksum(topic, &message);

            // Process the message into json string
            // "{"board_id": trirover_id, "sequence": sequence, "has_block": has_block,
            // "block_color": block_color, "checksum": checksum}"
            int  publish_length = PUBLISH_TOPIC0_DATA_LENGTH;
            char publish_data[publish_length];

            snprintf(publish_data,
                     publish_length,
                     "{\"board_id\": %d, \"sequence\": %d, \"has_block\": %d, \"block_color\": %d, "
                     "\"check_sum\": \"0x%x\"}",
                     board_id,
                     message.sequence,
                     message.has_block,
                     message.block_color,
                     checksum);

            lRetVal = MQTTClient_publish(gMqttClient,
                                         (char *)topic,
                                         strlen((char *)topic),
                                         (char *)publish_data,
                                         strlen((char *)publish_data),
                                         MQTT_QOS_2 | ((RETAIN_ENABLE) ? MQTT_PUBLISH_RETAIN : 0));
        }
        else if (message.message_id == PUBLISH_QUEUE_STATISTICS_ID)
        {
            char     topic[PUBLISH_TOPIC1_LEN] = PUBLISH_TOPIC1;
            uint32_t checksum                  = calculate_publish_checksum(topic, &message);

            // Process the message into json string
            // "{"board_id": trirover_id, "receive_expected": receive_expected, "receive_actual":
            // receive_actual, "checksum": checksum}"

            int  publish_length = PUBLISH_TOPIC1_DATA_LENGTH;
            char publish_data[publish_length];

            snprintf(publish_data,
                     publish_length,
                     "{\"board_id\": %d, \"received_expected\": %d, \"received_actual\": %d, "
                     "\"check_sum\": \"0x%x\"}",
                     board_id,
                     message.receive_expected,
                     message.receive_actual,
                     checksum);

            lRetVal = MQTTClient_publish(gMqttClient,
                                         (char *)topic,
                                         strlen((char *)topic),
                                         (char *)publish_data,
                                         strlen((char *)publish_data),
                                         MQTT_QOS_2 | ((RETAIN_ENABLE) ? MQTT_PUBLISH_RETAIN : 0));
        }
        else if (message.message_id == PUBLISH_QUEUE_DEBUG_ID)
        {
            char     topic[PUBLISH_TOPIC2_LEN] = PUBLISH_TOPIC2;
            uint32_t checksum                  = calculate_publish_checksum(topic, &message);

            // Process the message into json string
            // "{ "board_id": trirover_id, "Ki_1": Ki_1, ... reading, desired, output }"

            int  publish_length = PUBLISH_TOPIC2_DATA_LENGTH;
            char publish_data[publish_length];

            snprintf(
                publish_data,
                publish_length,
                "{\"board_id\": %d, \"encoder\": [%ld, %ld, %ld], \"direction\": [%d, %d, %d], "
                "\"desired_speed\": [%ld, %ld, %ld], \"output_speed\": [%ld, %ld, %ld], "
                "\"output_power\": [%d, %d, %d], \"active\": [%d, %d, %d], \"check_sum\": "
                "\"0x%x\"}",
                board_id,
                message.motor1.encoder_reading,
                message.motor2.encoder_reading,
                message.motor3.encoder_reading,
                message.motor1.direction,
                message.motor2.direction,
                message.motor3.direction,
                message.motor1.desired_speed,
                message.motor2.desired_speed,
                message.motor3.desired_speed,
                message.motor1.output_speed,
                message.motor2.output_speed,
                message.motor3.output_speed,
                message.motor1.output_power,
                message.motor2.output_power,
                message.motor3.output_power,
                message.motor1.active,
                message.motor2.active,
                message.motor3.active,
                checksum);
            lRetVal = MQTTClient_publish(gMqttClient,
                                         (char *)topic,
                                         strlen((char *)topic),
                                         (char *)publish_data,
                                         strlen((char *)publish_data),
                                         MQTT_QOS_2 | ((RETAIN_ENABLE) ? MQTT_PUBLISH_RETAIN : 0));
        }
    }
}
void *MqttClient(void *pvParameters)
{
    long               lRetVal   = -1;
    int32_t            threadArg = 100;
    pthread_attr_t     pAttrs;
    struct sched_param priParam;
    int32_t            retc = 0;

    /*Initializing Client and Subscribing to the Broker.                     */
    if (gApConnectionState >= 0)
    {
        lRetVal = MqttClient_start();
        if (lRetVal == -1)
        {
            UART_PRINT("MQTT Client lib initialization failed\n\r");
            stop_everything();
            pthread_exit(0);
            return (NULL);
        }
    }
    /*Set priority and stack size attributes                                 */
    pthread_attr_init(&pAttrs);
    priParam.sched_priority = 2;

    retc = pthread_attr_setschedparam(&pAttrs, &priParam);
    retc |= pthread_attr_setstacksize(&pAttrs, PUBSUB_THREAD_SIZE);
    retc |= pthread_attr_setdetachstate(&pAttrs, PTHREAD_CREATE_DETACHED);

    if (retc != 0)
    {
        gInitState &= ~MQTT_INIT_STATE;
        UART_PRINT("MQTT thread create fail\n\r");
        stop_everything();
    }

    retc = pthread_create(&receive_pthread, &pAttrs, receive_thread, (void *)&threadArg);
    if (retc != 0)
    {
        gInitState &= ~MQTT_INIT_STATE;
        UART_PRINT("MQTT recv thread create fail\n\r");
        stop_everything();
    }
    retc = pthread_create(&publish_pthread, &pAttrs, publish_thread, (void *)&threadArg);
    if (retc != 0)
    {
        gInitState &= ~MQTT_INIT_STATE;
        UART_PRINT("MQTT pub thread create fail\n\r");
        stop_everything();
    }
    pthread_exit(0);
    return (NULL);
}

//! This function connect the MQTT device to an AP with the SSID which was
//! configured in SSID_NAME definition which can be found in Network_if.h file,
//! if the device can't connect to to this AP a request from the user for other
//! SSID will appear.
int32_t Mqtt_IF_Connect()
{
    int32_t lRetVal;
    char    SSID_Remote_Name[32];
    int8_t  Str_Length;

    memset(SSID_Remote_Name, '\0', sizeof(SSID_Remote_Name));
    Str_Length = strlen(SSID_NAME);

    if (Str_Length)
    {
        /*Copy the Default SSID to the local variable                        */
        strncpy(SSID_Remote_Name, SSID_NAME, Str_Length);
    }

    /*Reset The state of the machine                                         */
    Network_IF_ResetMCUStateMachine();

    /*Start the driver                                                       */
    lRetVal = Network_IF_InitDriver(ROLE_STA);
    if (lRetVal < 0)
    {
        UART_PRINT("Failed to start SimpleLink Device\n\r", lRetVal);
        return (-1);
    }

    /*Initialize AP security params                                          */
    SecurityParams.Key    = (signed char *)SECURITY_KEY;
    SecurityParams.KeyLen = strlen(SECURITY_KEY);
    SecurityParams.Type   = SECURITY_TYPE;

    /*Connect to the Access Point                                            */
    lRetVal = Network_IF_ConnectAP(SSID_Remote_Name, SecurityParams);
    if (lRetVal < 0)
    {
        UART_PRINT("Connection to an AP failed\n\r");
        return (-1);
    }

    return (0);
}

//! MQTT Start - Initialize and create all the items required to run the MQTT
//! protocol
void Mqtt_start()
{
    int32_t            threadArg = 100;
    pthread_attr_t     pAttrs;
    struct sched_param priParam;
    int32_t            retc = 0;
    mq_attr            attr;
    unsigned           mode = 0;

    /*sync object for inter thread communication                             */
    attr.mq_maxmsg  = 10;
    attr.mq_msgsize = sizeof(Receive_Data);
    g_PBQueue       = mq_open("g_PBQueue", O_CREAT, mode, &attr);  // a freertos queue
    if (g_PBQueue == NULL)
    {
        UART_PRINT("MQTT Message Queue create fail\n\r");
        gInitState &= ~MQTT_INIT_STATE;
        return;
    }
    /*Set priority and stack size attributes                                 */
    pthread_attr_init(&pAttrs);
    priParam.sched_priority = 2;

    retc = pthread_attr_setschedparam(&pAttrs, &priParam);
    retc |= pthread_attr_setstacksize(&pAttrs, MQTTTHREADSIZE);
    retc |= pthread_attr_setdetachstate(&pAttrs, PTHREAD_CREATE_DETACHED);

    if (retc != 0)
    {
        gInitState &= ~MQTT_INIT_STATE;
        UART_PRINT("MQTT thread create fail\n\r");
        stop_everything();
    }

    retc = pthread_create(&mqttThread, &pAttrs, MqttClient, (void *)&threadArg);
    if (retc != 0)
    {
        gInitState &= ~MQTT_INIT_STATE;
        UART_PRINT("MQTT thread create fail\n\r");
        stop_everything();
    }

    gInitState &= ~MQTT_INIT_STATE;
}

int32_t MqttClient_start()
{
    int32_t lRetVal = -1;
    int32_t iCount  = 0;

    int32_t            threadArg = 100;
    pthread_attr_t     pAttrs;
    struct sched_param priParam;

    MqttClientExmple_params.clientId     = ClientId;
    MqttClientExmple_params.connParams   = &Mqtt_ClientCtx;
    MqttClientExmple_params.mqttMode31   = MQTT_3_1;
    MqttClientExmple_params.blockingSend = true;

    gInitState |= CLIENT_INIT_STATE;

    /*Initialize MQTT client lib                                             */
    gMqttClient = MQTTClient_create(MqttClientCallback, &MqttClientExmple_params);
    if (gMqttClient == NULL)
    {
        /*lib initialization failed                                          */
        gInitState &= ~CLIENT_INIT_STATE;
        return (-1);
    }

    /*Open Client Receive Thread start the receive task. Set priority and    */
    /*stack size attributes                                                  */
    pthread_attr_init(&pAttrs);
    priParam.sched_priority = 2;

    lRetVal = pthread_attr_setschedparam(&pAttrs, &priParam);
    lRetVal |= pthread_attr_setstacksize(&pAttrs, RXTASKSIZE);
    lRetVal |= pthread_attr_setdetachstate(&pAttrs, PTHREAD_CREATE_DETACHED);
    lRetVal |= pthread_create(&g_rx_task_hndl, &pAttrs, MqttClientThread, (void *)&threadArg);
    if (lRetVal != 0)
    {
        UART_PRINT("Client Thread Create Failed failed\n\r");
        gInitState &= ~CLIENT_INIT_STATE;
        return (-1);
    }

    /*setting will parameters                                                */
    MQTTClient_set(gMqttClient, MQTTClient_WILL_PARAM, &will_param, sizeof(will_param));

    //    /*Set user name for client connection                                    */
    //    MQTTClient_set(
    //        gMqttClient, MQTTClient_USER_NAME, (void *)ClientUsername, strlen((char
    //        *)ClientUsername));
    //
    //    /*Set password                                                           */
    //    MQTTClient_set(
    //        gMqttClient, MQTTClient_PASSWORD, (void *)ClientPassword, strlen((char
    //        *)ClientPassword));

    /*Initiate MQTT Connect                                                  */
    if (gApConnectionState >= 0)
    {
        /*The return code of MQTTClient_connect is the ConnACK value that
           returns from the server */
        lRetVal = MQTTClient_connect(gMqttClient);

        /*negative lRetVal means error,
           0 means connection successful without session stored by the server,
           greater than 0 means successful connection with session stored by
           the server */
        if (0 > lRetVal)
        {
            /*lib initialization failed                                      */
            UART_PRINT("Connection to broker failed, Error code: %d\n\r", lRetVal);

            gUiConnFlag = 0;
        }
        else
        {
            gUiConnFlag = 1;
        }
        /*Subscribe to topics when session is not stored by the server       */
        if ((gUiConnFlag == 1) && (0 == lRetVal))
        {
            uint8_t                    subIndex;
            MQTTClient_SubscribeParams subscriptionInfo[SUBSCRIPTION_TOPIC_COUNT];

            for (subIndex = 0; subIndex < SUBSCRIPTION_TOPIC_COUNT; subIndex++)
            {
                subscriptionInfo[subIndex].topic = topic[subIndex];
                subscriptionInfo[subIndex].qos   = qos[subIndex];
            }

            if (MQTTClient_subscribe(gMqttClient, subscriptionInfo, SUBSCRIPTION_TOPIC_COUNT) < 0)
            {
                UART_PRINT("\n\r Subscription Error \n\r");
                MQTTClient_disconnect(gMqttClient);
                gUiConnFlag = 0;
            }
            else
            {
                for (iCount = 0; iCount < SUBSCRIPTION_TOPIC_COUNT; iCount++)
                {
                    UART_PRINT("Client subscribed on %s\n\r,", topic[iCount]);
                }
            }
        }
    }

    gInitState &= ~CLIENT_INIT_STATE;

    return (0);
}

void mainThread(void *args)
{
    // MAP_PRCMPeripheralClkEnable(PRCM_DTHE, PRCM_RUN_MODE_CLK);
    pthread_t          spawn_thread = (pthread_t)NULL;
    pthread_attr_t     pAttrs_spawn;
    struct sched_param priParam;
    int32_t            retc = 0;
    UART_Handle        tUartHndl;

    /*Initialize SlNetSock layer with CC31xx/CC32xx interface */
    retc = ti_net_SlNet_initConfig();
    if (0 != retc)
    {
        UART_PRINT("Failed to initialize SlNetSock\n\r");
    }

    /*Configure the UART                                                     */
    tUartHndl = InitTerm();
    /*remove uart receive from LPDS dependency                               */
    UART_control(tUartHndl, UART_CMD_RXDISABLE, NULL);

    /*Create the sl_Task                                                     */
    pthread_attr_init(&pAttrs_spawn);
    priParam.sched_priority = SPAWN_TASK_PRIORITY;

    retc = pthread_attr_setschedparam(&pAttrs_spawn, &priParam);
    retc |= pthread_attr_setstacksize(&pAttrs_spawn, TASKSTACKSIZE);
    retc |= pthread_attr_setdetachstate(&pAttrs_spawn, PTHREAD_CREATE_DETACHED);

    retc = pthread_create(&spawn_thread, &pAttrs_spawn, sl_Task, NULL);

    if (retc != 0)
    {
        UART_PRINT("could not create simplelink task\n\r");
        stop_everything();
    }

    retc = sl_Start(0, 0, 0);
    if (retc < 0)
    {
        /*Handle Error */
        UART_PRINT("\n sl_Start failed\n");
        stop_everything();
    }

    retc = sl_Stop(SL_STOP_TIMEOUT);
    if (retc < 0)
    {
        /*Handle Error */
        UART_PRINT("\n sl_Stop failed\n");
        stop_everything();
    }

    if (retc < 0)
    {
        /*Handle Error */
        UART_PRINT("mqtt_client - Unable to retrieve device information \n");
        stop_everything();
    }

    while (TRUE)
    {
        gResetApplication = false;
        topic[0]          = SUBSCRIPTION_TOPIC0;
        gInitState        = 0;

        /*Connect to AP                                                      */
        gApConnectionState = Mqtt_IF_Connect();

        if (gApConnectionState == -1)
        {
            code(CLIENT_IF_CONNECT_START_FAILURE);
            stop_everything();
        }

        gInitState |= MQTT_INIT_STATE;
        /*Run MQTT Main Thread (it will open the Client and Server)          */
        Mqtt_start();

        /*Wait for init to be completed!!!                                   */
        while (TRUE)
        {
            ;
        }
    }
}
