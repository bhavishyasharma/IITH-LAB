//
// Created by Bhavishya Sharma on 30/09/20.
//

#include "ChatQueueHandler.h"

ChatQueueHandler::ChatQueueHandler(std::list<ChatMessage *> *queue, std::map<int, ChatClientHandler *> *handlers, pthread_mutex_t* queueMutex) {
    this->messageQueue = queue;
    this->clientHandlers = handlers;
    this->queueMutex = queueMutex;
}

void * ChatQueueHandler::forwardMessage(ChatMessage *message) {
    char buffer[MAX_MESSAGE_LENGTH + MAX_MESSAGE_LENGTH];
    // serialise message
    message->serialize((unsigned char*)buffer);
    int messageSize = message->size();
    // forward message to all clients
    for (std::map<int, ChatClientHandler*>::iterator it=this->clientHandlers->begin(); it != this->clientHandlers->end();++it) {
        if((it->first != message->socketFD || message->socketFD==-1) && it->second->threadStatus > 0) {
            // send message to client socket
            if (send(it->first, buffer, messageSize, 0) != messageSize) {
                std::cerr << "Failed: Error sending message from "<<message->username<<" to "<<it->second->username<<std::endl;
            }
        }
    }
    free(message);
    return 0;
}

void * ChatQueueHandler::watchQueue() {
    struct timespec timer;
    timer.tv_nsec = 500000000L;
    while(1){
        if(this->messageQueue->empty()){
            // wait 0.5 seconds if queue is empty
            nanosleep(&timer, NULL);
        }
        else {
            // remove message from queue and forward to all clients
            ChatMessage *message = this->messageQueue->front();
            pthread_mutex_lock(queueMutex);
            this->messageQueue->pop_front();
            pthread_mutex_unlock(queueMutex);
            this->forwardMessage(message);
        }
    }
}

void * ChatQueueHandler::threadRunner(void *handler) {
    if(handler) {
        return ((ChatQueueHandler*)handler)->watchQueue();
    }
    else{
        throw std::runtime_error("Failed: Null handler found.");
    }
}