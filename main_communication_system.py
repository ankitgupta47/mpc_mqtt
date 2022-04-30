from pickle import TRUE
import gym
import time
import numpy as np
import paho.mqtt.client as mqtt_client
from paho.mqtt import subscribe


BROKER = 'broker.hivemq.com'
PORT = 1883
PUB_TOPIC = "PENDULUM/STATES"
SUB_TOPIC = "CONTROL/FORCE"
CLIENT_ID = "pendulum"
QOS = 1


def connect_mqtt(broker,port,client_id):
    def on_connect(client, userdata, flags, rc):
        if rc == 0:
            print("Connected to MQTT Broker!")
        else:
            print("Failed to connect, return code %d\n", rc)
    # Set Connecting Client ID
    client = mqtt_client.Client(client_id)
#    client.username_pw_set(username, password)
    client.on_connect = on_connect
    client.connect(broker, port)
    return client

def publish(client,topic,payload):
    result = client.publish(topic, payload)
    status = result[0]
    if status == 0:
        print(f"Send `{payload}` to topic `{topic}`")
    else:
        print(f"Failed to send message to topic {topic}")

# def subscribe(client: mqtt_client,topic):
#     def on_message(client, userdata, msg):
#         print(msg)
#         # print(f"Received `{msg.payload.decode()}` from `{msg.topic}` topic")
#     client.subscribe(topic)
#     client.on_message = on_message

def main():
    client = connect_mqtt(BROKER,PORT,CLIENT_ID)
    simTime = 0
    while(simTime<2000):
        state  = np.array([0.0001, 0.20,0.1000,0.0003])
        payload = np.array2string(state)
        publish(client,PUB_TOPIC,payload)
        rec_msg = subscribe.simple(topics=SUB_TOPIC,hostname=BROKER,port=PORT,retained=True,qos=QOS)
        print(rec_msg.payload.decode())
        # subscribe(client,SUB_TOPIC)
        simTime+=simTime

    client.disconnect()


if __name__ == '__main__':
    main()
