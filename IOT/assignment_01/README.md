#### IOT Assignment 01 - MQTT

##### Modify run.sh/run-debug.sh script as per your MQTT Broker
```
Change --host parameter to match your MQTT Broker Host
Change --port parameter to match your MQTT Broker Port
```

##### Run Instructions
`./run.sh`

##### Sample Output
```
root ➜ /workspaces/IITH-LAB/IOT/assignment_01 (master ✗) $ ./run.sh
Requirement already satisfied: paho-mqtt in /usr/local/lib/python3.7/dist-packages (1.5.1)
Requirement already satisfied: numpy in /usr/local/lib/python3.7/dist-packages (1.20.1)
52160   | HQ            | INFO  | Waiting for broker connection...
52160   | HQ            | INFO  | Connected to broker
52160   | HQ            | INFO  | Waiting for UAVs to connect (0/6)
52160   | HQ            | INFO  | Waiting for UAVs to connect (0/6)
52160   | HQ            | INFO  | Waiting for UAVs to connect (0/6)
52160   | HQ            | INFO  | Waiting for UAVs to connect (0/6)
52160   | HQ            | INFO  | Waiting for UAVs to connect (0/6)
52160   | HQ            | INFO  | Waiting for UAVs to connect (0/6)
52173   | UAV-1         | INFO  | Waiting for broker connecttion...
52173   | UAV-1         | INFO  | Connected to broker
52173   | UAV-1         | INFO  | Waiting for other UAVs (1/6)
52173   | UAV-1         | INFO  | Waiting for other UAVs (1/6)
52173   | UAV-1         | INFO  | Waiting for other UAVs (1/6)
52160   | HQ            | INFO  | Waiting for UAVs to connect (1/6)
52173   | UAV-1         | INFO  | Waiting for other UAVs (1/6)
52173   | UAV-1         | INFO  | Waiting for other UAVs (1/6)
52186   | UAV-2         | INFO  | Waiting for broker connecttion...
52186   | UAV-2         | INFO  | Connected to broker
52186   | UAV-2         | INFO  | Waiting for other UAVs (2/6)
52186   | UAV-2         | INFO  | Waiting for other UAVs (2/6)
52186   | UAV-2         | INFO  | Waiting for other UAVs (2/6)
52186   | UAV-2         | INFO  | Waiting for other UAVs (2/6)
52186   | UAV-2         | INFO  | Waiting for other UAVs (2/6)
52160   | HQ            | INFO  | Waiting for UAVs to connect (2/6)
52173   | UAV-1         | INFO  | Waiting for other UAVs (2/6)
52199   | UAV-3         | INFO  | Waiting for broker connecttion...
52199   | UAV-3         | INFO  | Connected to broker
52199   | UAV-3         | INFO  | Waiting for other UAVs (2/6)
52199   | UAV-3         | INFO  | Waiting for other UAVs (3/6)
52199   | UAV-3         | INFO  | Waiting for other UAVs (3/6)
52199   | UAV-3         | INFO  | Waiting for other UAVs (3/6)
52160   | HQ            | INFO  | Waiting for UAVs to connect (3/6)
52186   | UAV-2         | INFO  | Waiting for other UAVs (3/6)
52173   | UAV-1         | INFO  | Waiting for other UAVs (3/6)
52199   | UAV-3         | INFO  | Waiting for other UAVs (3/6)
52212   | UAV-4         | INFO  | Waiting for broker connecttion...
52212   | UAV-4         | INFO  | Connected to broker
52212   | UAV-4         | INFO  | Waiting for other UAVs (2/6)
52212   | UAV-4         | INFO  | Waiting for other UAVs (3/6)
52212   | UAV-4         | INFO  | Waiting for other UAVs (4/6)
52212   | UAV-4         | INFO  | Waiting for other UAVs (4/6)
52212   | UAV-4         | INFO  | Waiting for other UAVs (4/6)
52160   | HQ            | INFO  | Waiting for UAVs to connect (4/6)
52186   | UAV-2         | INFO  | Waiting for other UAVs (4/6)
52173   | UAV-1         | INFO  | Waiting for other UAVs (4/6)
52199   | UAV-3         | INFO  | Waiting for other UAVs (4/6)
52225   | UAV-5         | INFO  | Waiting for broker connecttion...
52225   | UAV-5         | INFO  | Connected to broker
52225   | UAV-5         | INFO  | Waiting for other UAVs (2/6)
52225   | UAV-5         | INFO  | Waiting for other UAVs (3/6)
52225   | UAV-5         | INFO  | Waiting for other UAVs (4/6)
52225   | UAV-5         | INFO  | Waiting for other UAVs (5/6)
52225   | UAV-5         | INFO  | Waiting for other UAVs (5/6)
52186   | UAV-2         | INFO  | Waiting for other UAVs (5/6)
52212   | UAV-4         | INFO  | Waiting for other UAVs (5/6)
52173   | UAV-1         | INFO  | Waiting for other UAVs (5/6)
52160   | HQ            | INFO  | Waiting for UAVs to connect (5/6)
52199   | UAV-3         | INFO  | Waiting for other UAVs (5/6)
52238   | UAV-6         | INFO  | Waiting for broker connecttion...
52238   | UAV-6         | INFO  | Connected to broker
52238   | UAV-6         | INFO  | Waiting for other UAVs (2/6)
52238   | UAV-6         | INFO  | Waiting for other UAVs (3/6)
52238   | UAV-6         | INFO  | Waiting for other UAVs (4/6)
52238   | UAV-6         | INFO  | Waiting for other UAVs (5/6)
52238   | UAV-6         | INFO  | Waiting for other UAVs (6/6)
52225   | UAV-5         | INFO  | Waiting for other UAVs (6/6)
52186   | UAV-2         | INFO  | Waiting for other UAVs (6/6)
52160   | HQ            | INFO  | Waiting for UAVs to connect (6/6)
52212   | UAV-4         | INFO  | Waiting for other UAVs (6/6)
52199   | UAV-3         | INFO  | Waiting for other UAVs (6/6)
52173   | UAV-1         | INFO  | Waiting for other UAVs (6/6)
52225   | UAV-5         | INFO  | Waiting for other UAVs (5/6)
52199   | UAV-3         | INFO  | Waiting for other UAVs (5/6)
52212   | UAV-4         | INFO  | Waiting for other UAVs (5/6)
52173   | UAV-1         | INFO  | Waiting for other UAVs (5/6)
52186   | UAV-2         | INFO  | Waiting for other UAVs (5/6)
52186   | UAV-2         | INFO  | Waiting for other UAVs (4/6)
52212   | UAV-4         | INFO  | Waiting for other UAVs (4/6)
52225   | UAV-5         | INFO  | Waiting for other UAVs (4/6)
52173   | UAV-1         | INFO  | Waiting for other UAVs (4/6)
52212   | UAV-4         | INFO  | Waiting for other UAVs (3/6)
52186   | UAV-2         | INFO  | Waiting for other UAVs (3/6)
52173   | UAV-1         | INFO  | Waiting for other UAVs (3/6)
root ➜ /workspaces/IITH-LAB/IOT/assignment_01 (master ✗) $ 
```