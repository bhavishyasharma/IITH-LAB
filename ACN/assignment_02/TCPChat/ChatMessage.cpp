//
// Created by Bhavishya Sharma on 30/09/20.
//

#include <iostream>

#include "ChatMessage.h"

ChatMessage::ChatMessage() {

}

ChatMessage::ChatMessage(char *username, char *message) {
    memcpy(this->username, username, strlen(username)+1);
    memcpy(this->data, message, strlen(message)+1);
    this->socketFD = -1;
}

ChatMessage::ChatMessage(char *username, std::string message) {
    memcpy(this->username, username, strlen(username) + 1);
    strcpy((char*)this->data, message.c_str());
    this->socketFD = -1;
}

int ChatMessage::size() {
    return strlen((char*)this->username) + strlen((char*)this->data) + 2;
}

void ChatMessage::deserialize(unsigned char *bytes) {
    int usernameLength = strlen((char*) bytes);
    memcpy(this->username, bytes, usernameLength + 1);
    bytes += (usernameLength + 1);
    memcpy(this->data, bytes,strlen((char*) bytes) + 1);
}

void ChatMessage::serialize(unsigned char *bytes) {
    int usernameLength = strlen((char*) this->username);
    memcpy(bytes, this->username, usernameLength + 1);
    memcpy(bytes + usernameLength + 1, this->data, strlen((char*)this->data) +1 );
}