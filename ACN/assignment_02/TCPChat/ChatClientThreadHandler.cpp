//
// Created by Bhavishya Sharma on 01/10/20.
//

#include <unistd.h>
#include <iostream>

#include "ChatClientThreadHandler.h"

ChatClientThreadHandler::ChatClientThreadHandler(std::map<int, pthread_t *> *clientThreads,
                                                 std::map<int, ChatClientHandler *> *clientHandlers, int maxClientCount) {
    this->clientThreads = clientThreads;
    this->clientHandlers = clientHandlers;
    this->maxClientCount = maxClientCount;
}

void * ChatClientThreadHandler::manageClientThreads() {
    while(1) {
        // scan all handlers & check status
        for (std::map<int, ChatClientHandler *>::iterator it = this->clientHandlers->begin();
             it != this->clientHandlers->end();) {
            // remove objects if thread finished
            if (it->second->threadStatus < 0) {
                // join finished thread
                pthread_join(*(this->clientThreads->find(it->first)->second), NULL);
                this->clientThreads->erase(it->first);
                it = this->clientHandlers->erase(it);
                std::cout<<"Thread Count : "<<this->clientThreads->size()<<"/"<<this->maxClientCount<<std::endl;
            } else {
                ++it;
            }
        }
        sleep(1);
    }
    return 0;
}

void * ChatClientThreadHandler::threadRunner(void *handler) {
    if(handler) {
        return ((ChatClientThreadHandler*)handler)->manageClientThreads();
    }
    else{
        throw std::runtime_error("Failed: Null handler found.");
    }
}