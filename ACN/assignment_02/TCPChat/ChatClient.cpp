//
// Created by Bhavishya Sharma on 30/09/20.
//

#include "ChatClient.h"
#include "ChatMessage.h"
#include "ChatCommon.hpp"

ChatClient::ChatClient(std::string username, std::string serverIP, u_int16_t port) {
    this->init(username, serverIP, port);
}

ChatClient::ChatClient(std::string username, u_int16_t port) {
    this->init(username, DEFAULT_CHAT_SERVER_IP, port);
}

ChatClient::ChatClient(std::string username, std::string serverIP) {
    this->init(username, serverIP, DEFAULT_CHAT_SERVER_PORT);
}

ChatClient::ChatClient(std::string username) {
    this->init(username, DEFAULT_CHAT_SERVER_IP, DEFAULT_CHAT_SERVER_PORT);
}

void ChatClient::init(std::string username, std::string serverIP, u_int16_t port) {
    memcpy(this->username, username.c_str(), username.length());
    this->serverAddress.sin_family = AF_INET;
    inet_aton(serverIP.c_str(), &(this->serverAddress.sin_addr));
    this->serverAddress.sin_port = htons(port);

    if((this->socketFD = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
        std::cerr<<"Failed: Error creating socket.";
        throw std::runtime_error("Failed: Error creating socket.");
    }
}

void ChatClient::start() {
    if (connect(this->socketFD, (struct  sockaddr *) &(this->serverAddress), sizeof(this->serverAddress)) < 0) {
        std::cerr<<"Failed: Error connecting to server.";
        throw std::runtime_error("Failed: Error connecting to server.");
    }
    initscr();
    cbreak();
    this->headerWindow = newwin(3,80,0,0);
    this->conversationWindow = newwin(20, 80, 3, 0);
    this->innerConversationWindow = newwin(18, 76, 4, 2);
    this->inputWindow = newwin(3, 80, 23, 0);
    this->innerInputWindow = newwin(1, 76, 24, 2);
    scrollok(this->innerConversationWindow,TRUE);
    scrollok(this->innerInputWindow, TRUE);
    refresh();
    box(this->headerWindow,0,0);
    box(this->conversationWindow, 0, 0);
    box(this->inputWindow, 0, 0);
    mvwprintw(this->headerWindow, 0, 2, "Waiting in queue...");
    mvwprintw(this->headerWindow, 1, 3, "Server: %s, Port: %d, Username: %s", inet_ntoa(this->serverAddress.sin_addr), ntohs(this->serverAddress.sin_port), this->username);
    mvwprintw(this->conversationWindow, 0, 2, "Conversation");
    mvwprintw(this->inputWindow, 0, 2, "Input");
    //WINDOW* inputWindow = newwin(3, 80, 23, 0);
    //refresh();
    //box(inputWindow, 0, 0);

    //std::cout << "Connected to Chat Server " << inet_ntoa(this->serverAddress.sin_addr) << ":"
    //          << ntohs(this->serverAddress.sin_port) << std::endl;
    refresh();
    wrefresh(headerWindow);
    wrefresh(conversationWindow);
    wrefresh(innerConversationWindow);
    wrefresh(inputWindow);
    wrefresh(innerInputWindow);

    int usernameSize = strlen((char *) this->username);
    char buffer[MAX_MESSAGE_LENGTH + MAX_USERNAME_LENGTH];
    if (send(this->socketFD, this->username, usernameSize, 0) != usernameSize) {
        std::cerr << "Failed: Error sending username to server.";
        throw std::runtime_error("Failed: Error sending username to server.");
    }
    else{
        memset(buffer, 0, usernameSize);
        int reply;
        if (recv(this->socketFD, buffer, MAX_MESSAGE_LENGTH + MAX_USERNAME_LENGTH, 0) < 0) {
            std::cerr<<"Failed: Error receiving message from server.";
            throw std::runtime_error("Failed: Error receiving message from server.");
        }
        ChatMessage *message = new ChatMessage();
        message->deserialize((unsigned char*)buffer);
        wprintw(this->innerConversationWindow, "%s : %s\n", message->username, message->data);
        box(this->headerWindow,0,0);
        mvwprintw(this->headerWindow, 0, 2, "Connected");
        wrefresh(this->headerWindow);
    }

    this->receiverThread = new pthread_t ();
    if(pthread_create(this->receiverThread, NULL, ChatClient::threadRunner, this) == 0) {
        char messageBuffer[MAX_MESSAGE_LENGTH];
        int messageSize;
        int usernameSize = strlen((char *) this->username) + 1;
        int totalSize;
        memcpy(buffer, this->username, usernameSize);
        do {
            wgetnstr(this->innerInputWindow, messageBuffer, MAX_MESSAGE_LENGTH);
            //std::cin.getline(messageBuffer, MAX_MESSAGE_LENGTH);
            messageSize = strlen(messageBuffer) + 1;
            memcpy(buffer + usernameSize, messageBuffer, messageSize);
            totalSize = messageSize + usernameSize;
            wprintw(this->innerConversationWindow, "Me : %s\n", messageBuffer);
            //std::cout<<"Me : "<<messageBuffer<<std::endl;
            if(messageSize>1) {
                if (send(this->socketFD, buffer, totalSize, 0) != totalSize) {
                    std::cerr << "Failed: Error sending message to server.";
                    throw std::runtime_error("Failed: Error sending message to server.");
                }
            }

            memset(buffer + usernameSize, 0, messageSize);
            wrefresh(innerConversationWindow);
            wrefresh(innerInputWindow);
        } while (strcmp(messageBuffer, "bye") != 0);
        pthread_cancel(*this->receiverThread);
    }
    endwin();
}

void* ChatClient::receiveMessages() {
    struct timespec timer;
    timer.tv_nsec = 500000000L;
    int messageSize;
    unsigned char buffer[MAX_USERNAME_LENGTH + MAX_MESSAGE_LENGTH];
    while(1){
        if ((messageSize = recv(this->socketFD, buffer, MAX_MESSAGE_LENGTH + MAX_USERNAME_LENGTH, 0)) < 0) {
            std::cerr<<"Failed: Error receiving message from server.";
            throw std::runtime_error("Failed: Error receiving message from server.");
        }
        else if(messageSize == 0){
            nanosleep(&timer, NULL);
        }
        else{
            ChatMessage *message = new ChatMessage();
            message->deserialize(buffer);
            wprintw(this->innerConversationWindow, "%s : %s\n", message->username, message->data);
            //mvwprintw(this->inputWindow, 0, 3, "%s : %s", message->username, message->data);
            //std::cout<<message->username<<" : "<<message->data<<std::endl;
            free(message);
            wrefresh(innerConversationWindow);
            wrefresh(inputWindow);
        }
        memset(buffer, 0, sizeof(buffer));
        messageSize = -1;
    }
}

void * ChatClient::threadRunner(void *handler) {
    if(handler) {
        return ((ChatClient*)handler)->receiveMessages();
    }
    else{
        throw std::runtime_error("Failed: Null handler found.");
    }
}