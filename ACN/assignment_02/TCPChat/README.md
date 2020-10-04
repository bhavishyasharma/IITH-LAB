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


####PLAGIARISM STATEMENT

I certify that this assignment/report is my own work, based on my personal study and/or research and that I have acknowledged all material and sources used in its preparation, whether they be books, articles, reports, lecture notes, and any other kind of document, electronic or personal communication. I also certify that this assignment/report has not previously been submitted for assessment in any other course, except where specific permission has been granted from all course instructors involved, or at any other time in this course, and that I have not copied in part or whole or otherwise plagiarised the work of other students and/or persons. I pledge to uphold the principles of honesty and responsibility at CSE@IITH. In addition, I understand my responsibility to report honour violations by other students if I become aware of it. 

**Name:** Bhavishya Sharma  
**Roll No:** CS20MTECH12006