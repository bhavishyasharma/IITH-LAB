#1/bin/bash
python3 hq.py --host 192.168.0.195 &
sleep 2
python3 uav.py --host 192.168.0.195 1 &
sleep 2
python3 uav.py --host 192.168.0.195 2 &
sleep 2
python3 uav.py --host 192.168.0.195 3 &
sleep 2
python3 uav.py --host 192.168.0.195 4 &
sleep 2
python3 uav.py --host 192.168.0.195 5 &
sleep 2
python3 uav.py --host 192.168.0.195 6 &
wait