import paho.mqtt.client as MqttClient
import numpy as np
import argparse
import json
import time
import re
import logging

logging.basicConfig(format='%(process)d-%(levelname)s-%(message)s', level=logging.DEBUG)

parser = argparse.ArgumentParser()
parser.add_argument('--host', metavar='host', type=str, help='MQTT Server Host IP', default='127.0.0.1')
parser.add_argument('--port', metavar='port', type=int, help='MQTT Server Port', default=1883)
parser.add_argument('--user', metavar='user', type=str, help='MQTT Username', default='admin')
parser.add_argument('--password', metavar='password', type=str, help='MQTT Password', default='hivemq')
args = parser.parse_args()

locations = []
with open('vehicle_location.txt', 'r') as input:
    for line in input:
        locations.append(np.array(line.split()).reshape(-1,2).tolist())

total_vehicles = len(locations[0])
uavs_online = []
tracking = {}
output_tracking = []
connected = False
tracking_wait = False

def add_tracking():
    global output_tracking
    global tracking
    global tracking_wait
    output_tracking.append(list(map(tracking.get, sorted(tracking))))
    tracking = {}
    tracking_wait = False
    return

def on_connect(client, userdata, flags, rc):
    global connected
    if rc==0:
        logging.info('Connected to broker')
        connected = True
    else:
        logging.info('Connot connect to broker!')

def on_message(client, userdata, message):
    # print('Message on : ' + str(message.topic))
    global uavs_online
    global tracking
    global total_vehicles
    if re.search('^uav/[0-9]+', message.topic) is not None:
        uav_id = int(message.topic.split('/')[1])
        payload = json.loads(message.payload)
        if 'online' in payload:
            if uav_id not in uavs_online and payload['online'] == True:
                uavs_online.append(uav_id)
            elif uav_id in uavs_online:
                uavs_online.remove(uav_id)
            logging.info('Waiting for UAVs to connect ({uavs_online}/{total_vehicles})'.format(uavs_online=len(uavs_online), total_vehicles=total_vehicles))
        if 'tracking' in payload:
            tracking[uav_id-1] = payload['tracking']
            if len(tracking) == total_vehicles:
                add_tracking()

client = MqttClient.Client('hq')
client.on_connect = on_connect
client.on_message = on_message
client.username_pw_set(args.user, args.password)
client.connect(args.host, args.port)
client.loop_start()

logging.info("Waiting for broker connection...")
while not connected:
    time.sleep(0.1)

for uav_id in range(1, total_vehicles+1):
    client.subscribe('uav/{uav_id}'.format(uav_id=uav_id))

while len(uavs_online) < total_vehicles:
    time.sleep(0.1)

time.sleep(20)
for location_data in locations:
    vehicle_id = 1
    payload = {}
    for vehicle_location in location_data:
        payload[vehicle_id] = {'x': vehicle_location[0], 'y': vehicle_location[1]}
        vehicle_id += 1
    while len(tracking) > 0 or tracking_wait == True:
        time.sleep(0.1)
    tracking_wait = True
    client.publish('vehicle'.format(vehicle_id=vehicle_id), json.dumps(payload), qos=2)
    time.sleep(10)
#    while len(tracking)>0:
#        time.sleep(0.1)

time.sleep(10)

client.disconnect()
client.loop_stop()

with open('output.txt', 'w') as output:
    for tracking in output_tracking:
        logging.debug(tracking)
        output.write(' '.join(map(str, tracking)))
        output.write('\n')
