//
// Created by Bhavishya Sharma on 30/10/20.
//

#include "SAWPacket.h"

char * SAWPacket::serialize() {
    char* bytes = new char [this->payloadLength + 4];
    memcpy(bytes, &this->flags, 1);
    memcpy(bytes + 1, &this->checksum, 1);
    this->payloadLength = htons(this->payloadLength);
    memcpy(bytes + 2, &this->payloadLength, 2);
    this->payloadLength = ntohs(this->payloadLength);
    memcpy(bytes + 4, this->payload, this->payloadLength);
    return bytes;
}

void SAWPacket::deserialize(char * bytes) {
    memcpy(&this->flags, bytes, 1);
    memcpy(&this->checksum, bytes + 1, 1);
    memcpy(&this->payloadLength, bytes + 2, 2);
    this->payloadLength = ntohs(this->payloadLength);
    this->payload = bytes + 4;
}

int SAWPacket::size() {
    return this->payloadLength + 4;
}

bool SAWPacket::validate() {
    return this->checksum == 0b10101010;
}

bool SAWPacket::ackNumber() {
    return (this->flags & 0b01000000) != 0;
}

bool SAWPacket::seqNumber() {
    return (this->flags & 0b10000000) != 0;
}

bool SAWPacket::isCloseSignal() {
    return (this->flags & 0b00100000) != 0;
}