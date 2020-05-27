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

# clear all the records in db first 
# cursor.execute("DELETE * FROM SPEED_CMD")
# setting up the plotting graphs  
time_label = []
m1_speed = []
m2_speed = []
m3_speed = []

counter = 0

speed_command = {
    'status': "idle",
    'msg_no': 0,
    'motor1_speed': 40,
    'motor1_direction': "idle_direction", 
    'motor2_speed': 40,
    'motor2_direction': "idle_direction",  
    'motor3_speed': 55,
    'motor3_direction': "idle_direction"
}



topic_speed_command = ("/Rover/SpeedCmd", 0)
topic_speed_result = ("/Rover/PIDSpeed", 0)



# Define event callbacks
def on_connect(client, userdata, flags, rc):
    if rc == 0:
        client.connected_flag = True  # set flag
        print("connected OK")

        # after successfully connect to the server, subscrube to the topics
        r = subscribe_topics(client, topic_speed_command, 0)  # subscribe single topic
        print("Subscribing to topics ", topic_speed_command, r)

        r = subscribe_topics(client, topic_speed_result, 0) # subcribe to speed result 
        print("Subscribing to topics ", topic_speed_result, r)

        r = subscribe_topics(client, topic_pid_param, 0)
        print("Subscribing to topics ", topic_pid_param, r)
    else:
        print("Bad connection Returned code=", rc)


def on_message(client, userdata, message):
    topic = message.topic

    # subscribe to /Rover/PIDSpeed
    msg_payload = message.payload.decode("utf-8")
    print(msg_payload)
    speed_data = json.loads(msg_payload)

    if "spiSpeed_m1" in speed_data: 
        # time_label.append(datetime.datetime.now().strftime('%H:%M:%S.%f'))
        # m1_speed.append(speed_data["spiSpeed_m1"])
        # m2_speed.append(speed_data["spiSpeed_m2"])
        # m3_speed.append(speed_data["spiSpeed_m3"])
        query = "INSERT INTO PID_OUTPUT([TIME], [M1_SPEED], [M2_SPEED], [M3_SPEED]) VALUES ('%s','%d','%d','%d')" % (
            datetime.datetime.now().strftime('%Y-%m-%d %H:%M:%S.%f'), speed_data["spiSpeed_m1"], speed_data["spiSpeed_m2"],
            speed_data["spiSpeed_m3"])
        
        cursor.execute(query)



def on_publish(client, userdata, mid):
    # Check the publish return value with the mid value
    if mid == published_msg[1]:
        statistics_data['Sucess_Publishes'] += 1
    print("In on_pub callback mid= ", mid, published_msg[1])


def on_subscribe(client, obj, mid, granted_qos):
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



def publish_Speed():
    '''
    Function that display all the statistics values
    '''

    speed_command['status'] = "running"

    speed_command['msg_no'] += 1

    # speed_command['motor1_speed'] += 5
    # speed_command['motor2_speed'] += 5 
    # speed_command['motor3_speed'] += 0

    # if speed_command['motor1_speed'] >= 75:
    #     speed_command['motor1_speed'] = 50
    # if speed_command['motor2_speed'] >= 75:
    #     speed_command['motor2_speed'] = 50
    # if speed_command['motor3_speed'] >= 75:
    #     speed_command['motor3_speed'] = 1
    
    speed_command['motor1_direction'] = "DRIVE_FORWARD"
    speed_command['motor2_direction'] = "DRIVE_FORWARD"
    speed_command['motor3_direction'] = "DRIVE_FORWARD"
    # speed_command['msg_time'] = datetime.datetime.now().strftime('%Y-%m-%d %H:%M:%S')
    published_msg = client.publish("/Rover/SpeedCmd", encode_object(speed_command))


    # store the statistics information to the database
    # query1 = "INSERT INTO SPEED_CMD([STATUS], [SEQ_NUM], [M1_SPEED], [M1_DIRECTION], [M2_SPEED], [M2_DIRECTION], [M3_SPEED], [M3_DIRECTION]) VALUES ('%s', '%d','%d','%s','%d', '%s', '%d', '%s')" %(speed_command['status'], speed_command['msg_no'], speed_command['motor1_speed'], speed_command['motor1_direction'], 
    #     speed_command['motor2_speed'], speed_command['motor2_direction'], speed_command['motor3_speed'], speed_command['motor3_direction'])
    
    # # print(speed_command)
    # cursor.execute(query1)
    # dbconn.commit()
    threading.Timer(0.1, publish_Speed).start() #slow the message to 5HZ 


client = mqtt.Client()

# Assign event callbacks
client.on_message = on_message
client.on_connect = on_connect
client.on_publish = on_publish
client.on_subscribe = on_subscribe
client.on_disconnect = on_disconnect

# Connect
# client.username_pw_set(username, pwd)
client.connect("192.168.0.102", 1883)
publish_Speed()
dbconn.commit()


client.loop_forever()