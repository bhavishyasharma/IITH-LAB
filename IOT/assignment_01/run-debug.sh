#1/bin/bash
pip3 install paho-mqtt
pip3 install numpy
python3 hq.py --host 192.168.0.195 -v &
sleep 2
python3 uav.py --host 192.168.0.195 1 -v &
sleep 2
python3 uav.py --host 192.168.0.195 2 -v &
sleep 2
python3 uav.py --host 192.168.0.195 3 -v &
sleep 2
python3 uav.py --host 192.168.0.195 4 -v &
sleep 2
python3 uav.py --host 192.168.0.195 5 -v &
sleep 2
python3 uav.py --host 192.168.0.195 6 -v &
wait