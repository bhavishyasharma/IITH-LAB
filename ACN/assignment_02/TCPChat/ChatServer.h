//
// Created by Bhavishya Sharma on 30/09/20.
//

#ifndef IITH_LAB_CHATSERVER_H
#define IITH_LAB_CHATSERVER_H

#include <string.h>
#include <map>
#include <list>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <pthread.h>
#include <iostream>
#include <unistd.h>

#include "ChatCommon.hpp"
#include "ChatClientHandler.h"
#include "ChatQueueHandler.h"
#include "ChatMessage.h"
#include "ChatClientThreadHandler.h"

class ChatServer {
private:
    // server socket data
    int serverSocketFD;
    struct sockaddr_in *serverAddress;

    // shared queue for messages
    std::list<ChatMessage*> *messageQueue;

    // list of connected clients
    std::map<int, pthread_t*> *clientThreads;

    // list of threads for clienthandlers
    std::map<int, ChatClientHandler*> *clientHandlers;

    // queue handler (checks queue for message & forwards to call clients)
    ChatQueueHandler *queueHandler;
    pthread_t *queueThread;
    pthread_mutex_t* queueMutex;

    // Reclaims finished clienthandlers
    ChatClientThreadHandler *clientThreadHandler;
    pthread_t *clientThreadHandlerThread;
    int maxClientThreadCount;

public:
    ChatServer();
    ChatServer(std::string serverIP);
    ChatServer(u_int16_t port);
    ChatServer(std::string serverIP, u_int16_t port);
    ChatServer(int clientCount);
    ChatServer(std::string serverIP, int clientCount);
    ChatServer(u_int16_t port, int clientCount);
    ChatServer(std::string serverIP, u_int16_t port, int clientCount);
    void init(std::string serverIP, u_int16_t port, int clientCount);
    void startServer();
};


#endif //IITH_LAB_CHATSERVER_H
