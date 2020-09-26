#### ACN Tutorial 02 - Socket Programming

** Note: The code does not use getline, so data is sent word by word**

##### Build Instructions

```
mkdir build && cd build  
cmake ..  
cmake --build ./
```
![Build](./screenshot/build.png?raw=true "Build")

##### Run Instructions - TCP Echo Server (TA)

Server : `./tcp_server_ta`  
Client : `./tcp_client_ta`

![TCP Server TA](./screenshot/tcp_server_ta.png?raw=true "TCP Server TA")
![TCP Client TA](./screenshot/tcp_client_ta.png?raw=true "TCP Client TA")

##### Run Instructions - TCP Echo Server

Server : `./TCPEchoServer -i <bind_ip_address> -p <bind_port>`  
Client : `./TCPEchoClient -s <server_ip_address> -p <server_port>`

![TCP Echo Server](./screenshot/tcp_echo_server.png?raw=true "TCP Echo Server")
![TCP Echo Client](./screenshot/tcp_echo_client.png?raw=true "TCP Echo Client")

##### Run Instructions - UDP Echo Server

Server : `./UDPEchoServer -i <bind_ip_address> -p <bind_port>`   
Client : `./UDPEchoClient -i <server_ip_address> -p <server_port>`

![UDP Echo Server](./screenshot/tcp_echo_server.png?raw=true "UDP Echo Server")
![UDP Echo Client](./screenshot/tcp_echo_server.png?raw=true "UDP Echo Client")
