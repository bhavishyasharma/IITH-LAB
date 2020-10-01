#### ACN Assignment 02 - Socket Programming

** Note: The client uses ncurses to make a simple ui**

The server accepts max N connections at a time and maintains a backlog of N clients.
If the server thread queue is full, the client waits and "Waiting in queue" is displayed on top.

##### Build Instructions

```
mkdir build && cd build  
cmake ..  
make
```
![Build](./screenshot/build.png?raw=true "Build")

##### Run Instructions - Chat Server
`./ChatServer -i <bind_ip_address> -p <bind_port> -n <max_clients>`  
Parameters: 
- -i bind ip address
- -p bind port
- -n max clients/thread 

![TCP Server TA](./screenshot/chat_server.png?raw=true "TCP Server TA")
##### Run Instructions - Chat Client
`./ChatClient -s <server_ip_address> -p <server_port> -u <username>`
Parameters: 
- -s server ip address
- -p server port
- -u username

![Chat Client](./screenshot/chat_client.png?raw=true "Chat Client")
