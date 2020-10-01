//
// Created by Bhavishya Sharma on 30/09/20.
//

#ifndef IITH_LAB_CHATCLIENT_H
#define IITH_LAB_CHATCLIENT_H

#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <pthread.h>
#include <iostream>
#include <time.h>
#include <ncurses.h>

#include "ChatCommon.hpp"

class ChatClient {

private:
    // Socket Data
    struct sockaddr_in serverAddress;
    int socketFD;

    // Client Username
    unsigned char username[MAX_USERNAME_LENGTH];

    // Thread for receiving messages
    pthread_t *receiverThread;

    // NCurses Windows
    WINDOW* headerWindow;
    WINDOW* conversationWindow;
    WINDOW* innerConversationWindow;
    WINDOW* inputWindow;
    WINDOW* innerInputWindow;

    // initialize client
    void init(std::string username, std::string serverIP, u_int16_t port);

    // receive messages from socket
    void* receiveMessages();

public:
    ChatClient(std::string username);
    ChatClient(std::string username, std::string serverIP);
    ChatClient(std::string username, u_int16_t port);
    ChatClient(std::string username, std::string serverIP, u_int16_t port);
    // start client
    void start();
    // thread runner for receiveMessages
    static void* threadRunner(void *handler);
};


#endif //IITH_LAB_CHATCLIENT_H
