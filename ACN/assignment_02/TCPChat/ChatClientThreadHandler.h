//
// Created by Bhavishya Sharma on 01/10/20.
//

#ifndef IITH_LAB_CHATCLIENTTHREADHANDLER_H
#define IITH_LAB_CHATCLIENTTHREADHANDLER_H

#include <map>
#include <pthread.h>

#include "ChatClientHandler.h"

class ChatClientThreadHandler {
public:
    // shared thread objects
    std::map<int, pthread_t*> *clientThreads;
    // shared client handler objects
    std::map<int, ChatClientHandler*> *clientHandlers;
    int maxClientCount;

    ChatClientThreadHandler(std::map<int, pthread_t*> *clientThreads, std::map<int, ChatClientHandler*> *clientHandlers, int maxClientCount);

    // clear finished thread & remove from list
    void* manageClientThreads();

    static void* threadRunner(void *handler);
};


#endif //IITH_LAB_CHATCLIENTTHREADHANDLER_H
