#1/bin/bash
gnome-terminal --tab -e "python3 hq.py --host 192.168.0.195"
gnome-terminal --tab -e "python3 uav.py --host 192.168.0.195 1"
gnome-terminal --tab -e "python3 uav.py --host 192.168.0.195 2"
gnome-terminal --tab -e "python3 uav.py --host 192.168.0.195 3"
gnome-terminal --tab -e "python3 uav.py --host 192.168.0.195 4"
gnome-terminal --tab -e "python3 uav.py --host 192.168.0.195 5"
gnome-terminal --tab -e "python3 uav.py --host 192.168.0.195 6"
