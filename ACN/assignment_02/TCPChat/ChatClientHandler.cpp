//
// Created by Bhavishya Sharma on 30/09/20.
//

#include <iostream>

#include "ChatClientHandler.h"
#include "ChatCommon.hpp"
#include <unistd.h>

ChatClientHandler::ChatClientHandler(unsigned char* username, int socketFD, sockaddr_in *client, std::list <ChatMessage*> *queue, pthread_mutex_t* queueMutex) {
    this->username = username;
    this->clientSocketFD = socketFD;
    this->clientAddress = client;
    this->messageQueue = queue;
    this->threadStatus = 0;
    this->queueMutex = queueMutex;
}

void * ChatClientHandler::receiveMessage() {
    // Set thread status to working
    this->threadStatus = 1;

    // Join & Leave messages
    char serverUser[] = "Server";
    std::string joinMessage((char*)this->username, strlen((char*)this->username));
    std::string leaveMessage((char*)this->username, strlen((char*)this->username));
    joinMessage += " has joined the conversation.";
    leaveMessage += " has left the conversation.";

    // Send joining message to all clients
    ChatMessage* message = new ChatMessage(serverUser, joinMessage);
    pthread_mutex_lock(this->queueMutex);
    this->messageQueue->push_back(message);
    pthread_mutex_unlock(this->queueMutex);

    unsigned char *buffer = new unsigned char[MAX_MESSAGE_LENGTH];
    int messageSize = 1;
    while(messageSize) {
        messageSize = 0;
        // receive message from client
        if ((messageSize = recv(this->clientSocketFD, buffer, MAX_MESSAGE_LENGTH, 0)) < 0) {
            std::cerr<<"Failed: Error receiving from client.";
            throw std::runtime_error("Failed: Error receiving from client.");
        }
        else if(messageSize > 0){
            // parse message
            message = new ChatMessage();
            message->deserialize(buffer);
            message->socketFD = clientSocketFD;

            // send message to queue
            pthread_mutex_lock(queueMutex);
            this->messageQueue->push_back(message);
            pthread_mutex_unlock(queueMutex);
            if(strcmp((char*)message->data, "bye") == 0){
                break;
            }

            // clear buffer
            memset(buffer, 0, messageSize);
        }
    }
    // close client socket
    close(this->clientSocketFD);
    // set thread status to finished
    this->threadStatus = -1;

    // send leaving message to all clients
    message = new ChatMessage(serverUser, leaveMessage);
    message->socketFD = this->clientSocketFD;
    pthread_mutex_lock(this->queueMutex);
    this->messageQueue->push_back(message);
    pthread_mutex_unlock(this->queueMutex);
    return 0;
}

void * ChatClientHandler::threadRunner(void *handler) {
    if(handler) {
        return ((ChatClientHandler*)handler)->receiveMessage();
    }
    else{
        throw std::runtime_error("Failed: Null handler found.");
    }
}