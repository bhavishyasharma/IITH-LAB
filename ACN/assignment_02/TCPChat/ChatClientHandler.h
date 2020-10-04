//
// Created by Bhavishya Sharma on 30/09/20.
//

#ifndef IITH_LAB_CHATCLIENTHANDLER_H
#define IITH_LAB_CHATCLIENTHANDLER_H

#include <list>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/types.h>

#include "ChatMessage.h"

class ChatClientHandler {
public:
    // Client socket descriptor
    int clientSocketFD;
    // Client address
    struct sockaddr_in *clientAddress;
    // Shared message queue
    std::list<ChatMessage*> *messageQueue;
    unsigned char* username;
    // 0 - Created, 1 - Working, -1 - Finished
    int threadStatus;

    pthread_mutex_t* queueMutex;

    ChatClientHandler(unsigned char* username, int socketFD, sockaddr_in *client, std::list<ChatMessage*> *queue, pthread_mutex_t* queueMutex);
    // Receive messages from client
    void* receiveMessage();
    // Thread runner for receiveMessage
    static void* threadRunner(void *handler);
};


#endif //IITH_LAB_CHATCLIENTHANDLER_H
