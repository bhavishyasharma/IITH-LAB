import subprocess 
import os 
import re
import matplotlib.pyplot as plt

service_time = .004
arrival_time = 0.01
interval = 0.01
delays = 1000

data, temp = os.pipe() 
os.write(temp, bytes("5 10\n", "utf-8"))
os.close(temp) 

s = subprocess.check_output("./mm1 {service_time} {arrival_time} {interval} {delays}".format(service_time=service_time, arrival_time=arrival_time, interval=interval, delays=delays), stdin = data, shell = True) 
lines = re.split(r'\n+', s.decode("utf-8"))
lines = lines[1:]
arrival_rates = []
queue_delay = []
num_queue = []
for line in lines:
    x = re.split(r'\t', line)
    print(x)
    if(len(x)<3):
        break
    arrival_rates.append(float(x[0]))
    queue_delay.append(float(x[2]))
    num_queue.append(float(x[3]))

system_delay = [x + service_time for x in queue_delay]

fig, ax = plt.subplots()
ax.grid(True)
plt.xlabel('Arrival Rate (Packet/Second)')
plt.ylabel('Average Delay in Queue (Seconds)')
#plt.xscale('log')
ax.plot(arrival_rates, queue_delay)
#ax.plot(arrival_rates, system_delay)

fig1, ax1 = plt.subplots()
ax1.grid(True)
plt.xlabel('Arrival Rate (Packet/Second)')
plt.ylabel('Average Delay in System (Seconds)')
ax1.plot(arrival_rates, num_queue)

plt.show()
