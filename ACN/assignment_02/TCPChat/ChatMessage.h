//
// Created by Bhavishya Sharma on 30/09/20.
//

#ifndef IITH_LAB_CHATMESSAGE_H
#define IITH_LAB_CHATMESSAGE_H

#include <sys/types.h>
#include <string>
#include <cstring>
#include <arpa/inet.h>

#include "ChatCommon.hpp"

class ChatMessage {
private:
public:
    int socketFD;
    unsigned char username[MAX_USERNAME_LENGTH];
    unsigned char data[MAX_MESSAGE_LENGTH];
    ChatMessage();
    ChatMessage(char *username, char *message);
    ChatMessage(char *username, std::string message);
    int size();
    void deserialize(unsigned char* bytes);
    void serialize(unsigned char* bytes);
};


#endif //IITH_LAB_CHATMESSAGE_H
