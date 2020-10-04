//
// Created by Bhavishya Sharma on 30/09/20.
//

#ifndef IITH_LAB_CHATQUEUEHANDLER_H
#define IITH_LAB_CHATQUEUEHANDLER_H

#include <map>
#include <list>
#include <time.h>
#include <iostream>

#include "ChatMessage.h"
#include "ChatClientHandler.h"
#include "ChatMessage.h"

class ChatQueueHandler {
private:
    std::list<ChatMessage*> *messageQueue;
    std::map<int, ChatClientHandler*> *clientHandlers;
    pthread_mutex_t *queueMutex;

public:
    ChatQueueHandler(std::list<ChatMessage*> *queue, std::map<int, ChatClientHandler*> *handlers, pthread_mutex_t *queueMutex);
    void* forwardMessage(ChatMessage *message);
    void* watchQueue();
    static void* threadRunner(void *handler);
};


#endif //IITH_LAB_CHATQUEUEHANDLER_H
