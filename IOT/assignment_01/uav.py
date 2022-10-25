import paho.mqtt.client as MqttClient
import numpy as np
import argparse
import json
import time
import math
import re
import logging
from collections import Counter

parser = argparse.ArgumentParser()
parser.add_argument('--host', metavar='host', type=str, help='MQTT Server Host IP', default='127.0.0.1')
parser.add_argument('--port', metavar='port', type=int, help='MQTT Server Port', default=1883)
parser.add_argument('--user', metavar='user', type=str, help='MQTT Username', default='admin')
parser.add_argument('--password', metavar='password', type=str, help='MQTT Password', default='hivemq')
parser.add_argument('uav_id', metavar='uav_id', type=int, help='UAV ID')
parser.add_argument('--uavs', metavar='uavs', type=int, help='Total UAVs', default=6)
parser.add_argument('-v', '--verbose', action='store_true', help='Verbose')
args = parser.parse_args()

logger = logging.getLogger('UAV-'+str(args.uav_id))
if(args.verbose):
    logger.setLevel(logging.DEBUG)
else:
    logger.setLevel(logging.INFO)
handler = logging.StreamHandler()
if(args.verbose):
    handler.setLevel(logging.DEBUG)
else:
    handler.setLevel(logging.INFO)
handler.setFormatter(logging.Formatter('%(process)d \t| %(name)s \t| %(levelname)s\t| %(message)s'))
logger.addHandler(handler)

locations = []
with open('uav{uav_id}.txt'.format(uav_id=args.uav_id), 'r') as input:
    for line in input:
        locations.append(list(map(int,line.split())))

uavs_online = []
uav_locations = {}
vehicle_locations = {}
connected = False

def find_distance(uav_location, vehicle_location):
    return math.sqrt((int(uav_location['x'])-int(vehicle_location['x']))**2+(int(uav_location['y'])-int(vehicle_location['y']))**2)

def find_target():
    global uav_locations
    global vehicle_locations
    global client
    logger.debug("Locations on uav{me} = {locations}".format(me=args.uav_id, locations=uav_locations.keys()))
    if len(uav_locations) != len(vehicle_locations):
        return
    local_vehicle_locations = vehicle_locations
    local_uav_locations = uav_locations
    vehicle_locations = {}
    uav_locations = {}
    cost = np.matrix(np.ones((args.uavs, args.uavs)) * np.inf)
    for uav_id, uav_location in local_uav_locations.items():
        for vehicle_id, vehicle_location in local_vehicle_locations.items():
            cost[uav_id-1, vehicle_id-1] = find_distance(uav_location, vehicle_location)
            #print("Inner Loop")
        #print("Outer Loop")
    mins = np.array(np.argmin(cost, axis=0).flatten().tolist()[0])
    logger.debug(mins)
    dups = [item for item, count in Counter(mins).items() if count > 1]
    counter = 10
    #print(cost)
    while len(dups)>0:
        uav_id = int(np.sort(np.where(mins==dups[0]))[0][0])
        other_uav_id = int(np.sort(np.where(mins==dups[0]))[0][1])
        vehicle_id = int(dups[0])
        print(uav_id)
        print(other_uav_id)
        print(vehicle_id)
        if( uav_id > other_uav_id or vehicle_id == uav_id):
            cost[vehicle_id, uav_id] = np.inf
        else:
            cost[vehicle_id, other_uav_id] = np.inf
        mins = np.array(np.argmin(cost, axis=0).flatten().tolist()[0])
        dups = [item for item, count in Counter(mins).items() if count > 1]
    logger.debug(int(mins[args.uav_id-1])+1)
    client.publish('uav/{uav_id}'.format(uav_id=args.uav_id), json.dumps({'tracking': int(mins[args.uav_id-1])+1}), qos=2)
    return

def on_connect(client, userdata, flags, rc):
    global connected
    global uavs_online
    if rc==0:
        logger.info('Connected to broker')
        connected = True
        uavs_online.append(args.uav_id)
    else:
        logger.info('Connot connect to broker!')
        uavs_online.remove(args.uav_id)

def on_message(client, userdata, message):
    global uavs_online
    global uav_locations
    global vehicle_locations
    logger.debug("uav{me} Message on {source} : {message}".format(me=args.uav_id, source=message.topic, message=message.payload))
    if re.search('^uav/[0-9]+', message.topic) is not None:
        other_uav_id = int(message.topic.split('/')[1])
        payload = json.loads(message.payload)
        if 'online' in payload:
            if other_uav_id not in uavs_online and payload['online'] == True:
                uavs_online.append(other_uav_id)
            elif other_uav_id in uavs_online:
                uavs_online.remove(other_uav_id)
            logger.info('Waiting for other UAVs ({uavs_online}/{total_uavs})'.format(uavs_online=len(uavs_online), total_uavs=args.uavs))
        if 'location' in payload:
            uav_locations[other_uav_id] = payload['location']
            find_target()
    if re.search('^vehicle', message.topic) is not None:
        payload = json.loads(message.payload)
        for vehicle_id, location in payload.items():
            vehicle_locations[int(vehicle_id)] = location

client = MqttClient.Client('uav_'+str(args.uav_id))
client.on_connect = on_connect
client.on_message = on_message
client.username_pw_set(args.user, args.password)
client.will_set('uav/{uav_id}'.format(uav_id=args.uav_id), payload=json.dumps({'online': False}), retain=True)
client.connect(args.host, args.port)
client.loop_start()

logger.info('Waiting for broker connecttion...')
while not connected:
    time.sleep(0.1)

for uav_id in range(1, args.uavs+1):
    if uav_id != args.uav_id:
        client.subscribe('uav/{uav_id}'.format(uav_id=uav_id))

client.subscribe('vehicle')

client.publish('uav/{uav_id}'.format(uav_id=args.uav_id), json.dumps({'online': True}), retain=True, qos=2)

while len(uavs_online) < args.uavs:
    time.sleep(0.1)

time.sleep(2)
for location in locations:
    while len(vehicle_locations) == 0:
        time.sleep(0.1)
    uav_locations[args.uav_id] = {'x': location[0], 'y': location[1]}
    client.publish('uav/{uav_id}'.format(uav_id=args.uav_id), json.dumps({'location': uav_locations[args.uav_id]}), qos=2)
    find_target()
    time.sleep(1)

time.sleep(1)

client.publish('uav/{uav_id}'.format(uav_id=args.uav_id), json.dumps({'online': False}), retain=True, qos=2)
client.disconnect()
client.loop_stop()
