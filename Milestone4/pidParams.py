import paho.mqtt.client as mqtt
import paho.mqtt.subscribe as subscribe
import os
import time, threading, logging
import json
import datetime


topic_pid_param = ("/Rover/PIDParam", 0)

pid_param = {
    'KP1': 0.89,
    'KP2': 0.89,
    'KP3': 0.89,
    'KI1': 0.06401,
    'KI2': 0.016,
    'KI3': 0.0151
}

# Define event callbacks
def on_connect(client, userdata, flags, rc):
    if rc == 0:
        client.connected_flag = True  # set flag
        print("connected OK")

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



def publish_Params():
    '''
    Function that display all the statistics values
    '''

    # pid_param['KP1'] -= 0.0002
    # pid_param['KP2'] -= 0.00005
    # pid_param['KP3'] -= 0.00003

    # pid_param['KI1'] -= 0.0002
    # pid_param['KI2'] -= 0.00005
    # pid_param['KI3'] -= 0.00003


 
    published_msg = client.publish("/Rover/PIDParam", encode_object(pid_param))
    client.publish("pid_param", "publishing to pid_param")


    threading.Timer(25, publish_Params).start() #slow the message to 5HZ 


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

publish_Params()

client.loop_forever()