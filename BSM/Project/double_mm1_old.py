import subprocess 
import os 
import re
import matplotlib.pyplot as plt
import statistics
import math

packetSize = 1500 * 8
linkSpeed = 10000
delays = 100

data, temp = os.pipe() 
os.write(temp, bytes("5 10\n", "utf-8"))
os.close(temp) 

s = subprocess.check_output("./double_mm1 {packetSize} {linkSpeed} {delays} 0".format(packetSize=packetSize, linkSpeed=linkSpeed, delays=delays), stdin = data, shell = True) 
lines = re.split(r'\n+', s.decode("utf-8"))
print(s.decode("utf-8"))
lines = lines[1:]
interarrival = []
queue1_delay = []
queue2_delay = []
total_delay = []
for line in lines:
    x = re.split(r'\t', line)
    #print(x)
    if(len(x)<3):
        break
    interarrival.append(float(x[0]))
    queue1_delay.append(float(x[1]))
    queue2_delay.append(float(x[2]))
    total_delay.append(float(x[3]))

service_time = float(packetSize) / float(linkSpeed)
system_delay = [x + (2*service_time) for x in total_delay]

n = len(interarrival)
s = 0
mean = statistics.mean(system_delay)
for x in system_delay:
    s = s + ((x - mean) * (x - mean))
s = s / (n-1)
s = math.sqrt(s)

ideal = []
service_rate = 1.0 / service_time
for interarrivaltime in interarrival:
    x = 2.0 / (float(service_rate) - float(1.0 / float(interarrivaltime)))
    if(x>4):
        x = 4
    ideal.append(x)

print(len(ideal))
fig, ax = plt.subplots()
ax.grid(True)
plt.xlabel('Inter Arrial Time (Milli Second)')
plt.ylabel('Average Delay in System (Milli Seconds)')
#plt.xscale('log')
#ax.plot(interarrival, queue1_delay)
#ax.plot(interarrival, queue2_delay)
#ax.plot(interarrival, total_delay)
ax.plot(interarrival, system_delay)
ax.plot(interarrival, ideal)
cil = []
cih = []
diff = 9.439 * s / math.sqrt(n)
for sd in system_delay:
    cil.append(sd-diff)
    cih.append(sd+diff)
ax.fill_between(interarrival, cil, cih, color='b', alpha=0.1)
#ax.plot(arrival_rates, system_delay)

plt.show()
