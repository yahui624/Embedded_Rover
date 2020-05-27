import paho.mqtt.client as mqtt
import paho.mqtt.subscribe as subscribe
import os
import time, threading, logging
import json
import datetime
import pyodbc

# DB access set up
driver = '{Microsoft Access Driver (*.mdb, *.accdb)}'
filepath = r'C:\Users\Yahui\Documents\2020Spring\embedded\MS3_mqttServer\Milestone4\db_Rover_Server.accdb'

myDataSources = pyodbc.dataSources()
access_driver = myDataSources['MS Access Database']

dbconn = pyodbc.connect(driver=access_driver, dbq=filepath, autocommit=True)
cursor = dbconn.cursor()

# broker="m16.cloudmqtt.com"
# port = 16785
# username = "kalttjny"
# pwd = "jcQyavW_3Ko6"

# mimic the incoming data
brokers_out = {"ID": "rover",
               "encoder_status": "idle",
               "encoder_val1": "1",
               "encoder_val2": "2",
               "encoder_val3": "3",
               "seq_number": 1,
               }

# Statistics data JSON
statistics_data = {
    'CLIENT_ID': None,
    'Seq_Num': 2,
    'Client_Num': 2,
    'VALID': 0,
    'Success_Time': None,
    'Failure_Time': None
}

client_Statistics = {}
# Subscribed topic
topic_CCS = ("/cc3200/Board1", 0)
topic_Rover = ("mqtt_Server/Rover", 1)
topic_Statistics = [("/Server/Statistics", 2), ("/Rover/EncoderStat", 2)]
# topic_Count_Info = [(" $SYS/broker/messages/#", 0), ("$SYS/broker/bytes/#", 1),
#                     ("$SYS/broker/load/publish/dropped/+/#", 2)]
# topic_Client_Info = ("$SYS/broker/chen_list", 0)

published_msg = ()  # variable for checking if the message is being published to the client


# Define event callbacks
def on_connect(client, userdata, flags, rc):
    if rc == 0:
        client.connected_flag = True  # set flag
        print("connected OK")

        # after successfully connect to the server, subscrube to the topics
        r = subscribe_topics(client, topic_CCS, 0)  # subscribe single topic
        print("Subscribing to topics ", topic_CCS, r)

        r = subscribe_topics(client, topic_Statistics, 1)  # subscribe single topic
        print("Subscribing to topics ", topic_Statistics, r)

        r = subscribe_topics(client, topic_Rover)  # subscribe single topic
        print("Subscribing to topics ", topic_Rover, r)

        # r = subscribe_topics(client, topic_Count_Info)  # subscribe several topic
        # print("Subscribing to topics ", topic_Count_Info, r)
        #
        #r = subscribe_topics(client, topic_Client_Info)
        # print("Subscribing to topics ", topic_Client_Info, r)

    else:
        print("Bad connection Returned code=", rc)


def on_message_msgs(mosq, obj, msg):
    # This callback will only be called for messages with topics that match
    # $SYS/broker/messages/#
    print("MESSAGES: " + msg.topic + " " + str(msg.qos) + " " + str(msg.payload))


def on_message_bytes(mosq, obj, msg):
    # This callback will only be called for messages with topics that match
    # $SYS/broker/bytes/
    # if msg.topic == "$SYS/broker/bytes/received":
    #     statistics_data['Bytes_received'] = str(msg.payload)
    # elif msg.topic == '$SYS/broker/bytes/sent':
    #     statistics_data['Bytes_sent'] = str(msg.payload)

    print("BYTES: " + msg.topic + " " + str(msg.qos) + " " + str(msg.payload))


def on_message(client, userdata, message):
    topic = message.topic
    m_decode = str(message.payload.decode("utf-8", "ignore"))
    # m_in = json.loads(m_decode)
    #print("message topic=", message.topic)
    # print("message received ", str(message.payload.decode("utf-8")))
    #print(message.payload.decode("utf-8"))

    required_fields = ["sequence_no", "checksum", "payload", "board"]
    transmitting_data = message.payload.decode("utf-8")

    #transmitting_JSON_1 = json.dumps(transmitting_data)  # deserialiaze the content
    transmitting_JSON = json.loads(transmitting_data)
    #print(type(transmitting_JSON))
    # for i in transmitting_JSON:
    #     print(i, transmitting_JSON[i])
    # print("_______________________________938248324")

    if "success" in transmitting_JSON:  # for the client side statistics
         print("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!")
         pub_client_Statistics(transmitting_JSON)
         print("-------------------------")

    if 'checksum' in transmitting_JSON:  # Means the data comes from the mqtt client
        statistics_data['Client_Num'] = 2
       #print(type(transmitting_JSON))
        # print("!!!!!!!!!!!!!!!!!938248324")
        # print(transmitting_JSON['payload'])
        msg_Valid = True

        # Fist check if the JSON object has all the required field
        missing_Field = []
        # for field in required_fields:
        #     if transmitting_data.isNull(field):
        #         missing_Field.append(field)
        #         msg_Valid = False

        if msg_Valid:  # Means this message is VALID
            #print("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!4324")
            statistics_data['CLIENT_ID'] = transmitting_JSON['board']
            statistics_data['VALID'] = 1
            #print(statistics_data['VALID'])
            #print("-----------------------------")
            if statistics_data['Seq_Num'] == transmitting_JSON['sequence_no'] - 1:  # Then check the sequence number
                # without considering the reset condition
                statistics_data['Seq_Num'] += 1  # update the sequence number
                statistics_data['Success_Time'] = datetime.datetime.now().strftime("%Y-%m-%d %H:%M:%S")  # Updating the last sucess time
                statistics_data['VALID'] = 2
                print(statistics_data['VALID'])
                print("++++++++++++++++++++++")

            else:
                # without considering reset condition
                statistics_data['VALID'] = 0
                statistics_data['Failure_Time'] = datetime.datetime.now().strftime("%Y-%m-%d %H:%M:%S")
        else:
            statistics_data['VALID'] = 0
            statistics_data["Missing_Fields"] = missing_Field
            statistics_data['Failure_Time'] = datetime.datetime.now().strftime("%Y-%m-%d %H:%M:%S")


def on_publish(client, userdata, mid):
    # Check the publish return value with the mid value
    if mid == published_msg[1]:
        print("YAY!!!!!")
        statistics_data['Sucess_Publishes'] += 1
    print("In on_pub callback mid= ", mid, published_msg[1])


def on_subscribe(client, obj, mid, granted_qos):
    statistics_data['Subscribed_Topics'] = str(mid)
    """removes mid valuse from subscribe list"""
    logging.debug("in on subscribe callback result " + str(mid))

    if len(client.topic_ack) == 0:
        print("All subs acknowledged")
        # statistics_data['Fail_Subscribed_Topics'] = 0
        return

    # statistics_data['Fail_Subscribed_Topics'] = len(client.topic_ack)
    for index, t in enumerate(client.topic_ack):
        if t[1] == mid:
            client.topic_ack.pop(index)  # remove it
            # print("removed from list")


def on_log(client, obj, level, string):
    print(string)


def on_disconnect(client, userdata, rc):
    if rc != 0:
        print("Unexpected disconnection.")
    print("client disconnected ok")


def subscribe_topics(client, topics, qos=0):
    print("topic ", topics, "  ", qos)

    if type(topics) is not list:  # topics should be list of tuples
        if type(topics) is not tuple:  # topics isn't tuple?
            topic_list = [(topics, qos)]  # topic is single
        else:
            topic_list = [topics]
    else:
        topic_list = topics
    try:
        r = client.subscribe(topic_list)
        if r[0] == 0:
            logging.info("subscribed to topic" + str(topic_list) + " return code" + str(r))
            client.topic_ack.append([topic_list, r[1]])  # keep track of subscription

        else:
            logging.info("error on subscribing " + str(r))
            return -1

    except Exception as e:
        logging.info("error on subscribe" + str(e))
        return -1
    return r


def encode_object(message):
    return json.dumps(message)  # encode object to JSON


def encoder_JSON(message):  # At Receiver
    print("\ndata in-type ", type(message))
    print("data in=", message)
    brokers_in = json.loads(message)  # convert incoming JSON to object
    print("brokers_in is a ", type(brokers_in))
    return broker


def pub_client_Statistics(json_msg):
    client_Statistics['Msg_valid'] = json_msg['success']
    client_Statistics['datetime'] = datetime.datetime.now().strftime('%Y-%m-%d %H:%M:%S')  # executing time
    client_Statistics['Seq_No'] = json_msg['sequence_no']
    client_Statistics['CLIENT_ID'] = json_msg['board']

    query = "INSERT INTO Client_STATS([DATETIME], [CLIENT_ID], [SEQ_NUM], [MSG_VALID]) VALUES ('%s', '%s','%d','%d')" % (
    client_Statistics['datetime'], client_Statistics['CLIENT_ID'], client_Statistics['Seq_No'],
    client_Statistics['Msg_valid'])

    cursor.execute(query)


def publish_Statistics():
    '''
    Function that display all the statistics values
    '''
    published_msg = client.publish("mqtt_Server/Statistics", encode_object(statistics_data))
    client.publish("mqtt_Server/Statistics", "publishing to mqtt_Server/Statistics")

    # store the statistics information to the database
    test_day = datetime.timedelta(365)
    exec_time = datetime.datetime.now()  # executing time
    if statistics_data['Success_Time'] == None:
        statistics_data['Success_Time'] = (exec_time - test_day).strftime('%Y-%m-%d %H:%M:%S')
    if statistics_data['Failure_Time'] == None:
        statistics_data['Failure_Time'] = (exec_time - test_day).strftime('%Y-%m-%d %H:%M:%S')
    query = "INSERT INTO Server_STATS([DATETIME], [CLIENT_ID], [SEQ_NUM], [TOTAL_CLIENTS], [MSG_VALID], [SUCESS_TIME], [FAILURE_TIME]) VALUES ('%s', '%s','%d','%d','%d', '%s', '%s')" % (
    exec_time.strftime('%Y-%m-%d %H:%M:%S'), statistics_data['CLIENT_ID'], statistics_data['Seq_Num'],
    statistics_data['Client_Num'], statistics_data['VALID'],
    statistics_data['Success_Time'], statistics_data['Failure_Time'])

    cursor.execute(query)
    dbconn.commit()

    threading.Timer(0.1, publish_Statistics).start()


client = mqtt.Client()

# Add message callbacks that will only trigger on a specific subscription match.
client.message_callback_add("$SYS/broker/messages/#", on_message_msgs)
client.message_callback_add("$SYS/broker/bytes/#", on_message_bytes)
# Assign event callbacks
client.on_message = on_message
client.on_connect = on_connect
client.on_publish = on_publish
client.on_subscribe = on_subscribe
client.on_disconnect = on_disconnect

# Connect
# client.username_pw_set(username, pwd)
client.connect("192.168.0.102", 1883)

publish_Statistics()

client.loop_forever()