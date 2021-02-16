import subprocess 
import os 
import re
import matplotlib.pyplot as plt
import statistics
import math

packetSize = 1500 * 8
linkSpeed = 10000
delays = 100
service_time = float(packetSize) / float(linkSpeed)

data, temp = os.pipe() 
os.write(temp, bytes("5 10\n", "utf-8"))
os.close(temp) 

totalSysDelays = []
interarrival = []
singleDelays = []
secondDelays = []
for seedOffset in range(10):
    s = subprocess.check_output("./double_mm1 {packetSize} {linkSpeed} {delays} {seedOffset}".format(packetSize=packetSize, linkSpeed=linkSpeed, delays=delays, seedOffset=seedOffset), stdin = data, shell = True) 
    lines = re.split(r'\n+', s.decode("utf-8"))
    lines = lines[1:]
    system_delay = []
    single_delay = []
    second_delay = []
    localinterarrival = []
    for line in lines:
        x = re.split(r'\t', line)
        #print(x)
        if(len(x)<3):
            break
        localinterarrival.append(float(x[0]))
        single_delay.append(float(x[1]) + service_time)
        second_delay.append(float(x[2]) + service_time)
        system_delay.append(float(x[3]) + (2*service_time))
    totalSysDelays.append(system_delay)
    singleDelays.append(single_delay)
    secondDelays.append(second_delay)
    interarrival = localinterarrival

n = 10
s = []
singleS = []
secondS = []
means = []
singleMeans = []
secondMeans = []
for indexJ in range(len(interarrival)):
    ls = 0
    mean = 0
    singleLS = 0
    singleMean = 0
    secondLS = 0
    secondMean = 0
    for indexI in range(10):
        mean += totalSysDelays[indexI][indexJ]
        singleMean += singleDelays[indexI][indexJ]
        secondMean += secondDelays[indexI][indexJ]
    mean /= n
    singleMean /= n
    secondMean /= n
    for indexI in range(10):
        ls = ls + ((totalSysDelays[indexI][indexJ] - mean) * (totalSysDelays[indexI][indexJ] - mean))
        singleLS = singleLS + ((singleDelays[indexI][indexJ] - singleMean) * (singleDelays[indexI][indexJ] - singleMean))
        secondLS = secondLS + ((secondDelays[indexI][indexJ] - secondMean) * (secondDelays[indexI][indexJ] - secondMean))
    means.append(mean)
    singleMeans.append(singleMean)
    secondMeans.append(secondMean)
    s.append(ls/(n-1))
    singleS.append(singleLS/(n-1))
    secondS.append(secondLS/(n-1))


ideal = []
service_rate = 1.0 / service_time
for interarrivaltime in interarrival:
    x = 2.0 / (float(service_rate) - float(1.0 / float(interarrivaltime)))
    if(x>4):
        x = 4
    ideal.append(x)

print(len(ideal))
print(interarrival)
fig, ax = plt.subplots()
ax.grid(True)
plt.title('Total System Delay vs Inter Arrival Time')
plt.xlabel('Inter Arrial Time (Milli Second)')
plt.ylabel('Average Delay in System (Milli Seconds)')
#plt.xscale('log')
#ax.plot(interarrival, queue1_delay)
#ax.plot(interarrival, queue2_delay)
#ax.plot(interarrival, total_delay)
ax.plot(interarrival, means)

fig1, ax1 = plt.subplots()
ax1.grid(True)
plt.title('Total System Delay vs Inter Arrival Time')
plt.xlabel('Inter Arrial Time (Milli Second)')
plt.ylabel('Ideal Average Delay in System (Milli Seconds)')
ax1.plot(interarrival, ideal)

fig2, ax2 = plt.subplots()
ax2.grid(True)
plt.title('Total System Delay vs Inter Arrival Time')
plt.xlabel('Inter Arrial Time (Milli Second)')
plt.ylabel('Average Delay in System (Milli Seconds)')
ax2.plot(interarrival, means)
ax2.plot(interarrival, ideal)
cil = []
cih = []
print(s)
for index in range(len(totalSysDelays[0])):
    diff = 3.17 * math.sqrt(s[index]) / math.sqrt(n)
    cil.append(means[index]-diff)
    cih.append(means[index]+diff)
ax2.fill_between(interarrival, cil, cih, color='r', alpha=0.1)
#ax.plot(arrival_rates, system_delay)

fig3, ax3 = plt.subplots()
ax3.grid(True)
plt.title('Total Delay vs Inter Arrival Time')
plt.xlabel('Inter Arrial Time (Milli Second)')
plt.ylabel('Average Delay (Milli Seconds)')
ax3.plot(interarrival, singleMeans, label="Single Queue")
ax3.plot(interarrival, means, label="2 Queues")
ax3.fill_between(interarrival, cil, cih, color='r', alpha=0.1)
cil = []
cih = []
for index in range(len(singleDelays[0])):
    diff = 3.17 * math.sqrt(singleS[index]) / math.sqrt(n)
    cil.append(singleMeans[index]-diff)
    cih.append(singleMeans[index]+diff)
ax3.fill_between(interarrival, cil, cih, color='b', alpha=0.1)
plt.legend(loc='upper right')


fig3, ax4 = plt.subplots()
ax4.grid(True)
plt.title('Total Delay vs Inter Arrival Time')
plt.xlabel('Inter Arrial Time (Milli Second)')
plt.ylabel('Average Delay (Milli Seconds)')
ax4.plot(interarrival, singleMeans, label="1st Queue")
ax4.plot(interarrival, secondMeans, label="2nd Queues")
cil = []
cih = []
for index in range(len(singleDelays[0])):
    diff = 3.17 * math.sqrt(singleS[index]) / math.sqrt(n)
    cil.append(singleMeans[index]-diff)
    cih.append(singleMeans[index]+diff)
ax4.fill_between(interarrival, cil, cih, color='b', alpha=0.1)
cil = []
cih = []
for index in range(len(secondDelays[0])):
    diff = 3.17 * math.sqrt(secondS[index]) / math.sqrt(n)
    cil.append(secondMeans[index]-diff)
    cih.append(secondMeans[index]+diff)
ax4.fill_between(interarrival, cil, cih, color='r', alpha=0.1)
plt.legend(loc='upper right')

fig2, ax2 = plt.subplots()
ax2.grid(True)
plt.title('Total System Delay vs Inter Arrival Time')
plt.xlabel('Inter Arrial Time (Milli Second)')
plt.ylabel('Average Delay in System (Milli Seconds)')
ax2.plot(interarrival, means)
#ax2.plot(interarrival, ideal)
cil = []
cih = []
print(s)
for index in range(len(totalSysDelays[0])):
    diff = 3.17 * math.sqrt(s[index]) / math.sqrt(n)
    cil.append(means[index]-diff)
    cih.append(means[index]+diff)
ax2.fill_between(interarrival, cil, cih, color='r', alpha=0.1)


plt.show()
