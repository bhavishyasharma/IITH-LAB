#### ACN Assignment 02 - Socket Programming

** Note: The client uses ncurses to make a simple ui**

The server accepts max N connections at a time and maintains a backlog of N clients.
If the server thread queue is full, the client waits and "Waiting in queue" is displayed on top.

Server Threads
- Main Thread - Accept Client Connections, creates a list of connected clients
- ClientHandler Thread - per client thread for receiving messages. Each message received is pushed back to shared queue.
- QueueHandler Thread - Watches queue for any messages. If a message is in the queue, it is poped and forwarded to each connected client.
- ClientThread Handler Thread - watches the list of connected clients and frees the client thread if the client has finished.

Client Threads
- Main Thread - Read input from user and send message to server.
- Receiving Thread - Continuously receive messages from server.

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
