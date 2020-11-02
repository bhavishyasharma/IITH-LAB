//
// Created by Bhavishya Sharma on 31/10/20.
//

#ifndef IITH_LAB_GBNFTPPACKET_H
#define IITH_LAB_GBNFTPPACKET_H

#include <string>
#include <cstring>
#include <iostream>
#include <iomanip>
#include <sys/types.h>
#include <arpa/inet.h>

/**
 * flag description
 * 1000xxxx = request
 * 0100xxxx = response
 * 0010xxxx = data
 * 0001xxxx = ack
 */

class GBNFTPRequest {
public:
    u_int8_t flags = 0x80;
    u_int8_t checksum = 0xaa;
    u_int16_t seqNumber;
    u_int16_t ackNumber;
    char* payload;
    char* serialize();
    void deserialize(char* bytes);
    int size();
};

class GBNFTPResponse {
public:
    u_int8_t flags = 0x40;
    u_int8_t checksum = 0xaa;
    u_int16_t seqNumber;
    u_int32_t fileSize;
    u_int16_t windowSize;
    char* serialize();
    void deserialize(char* bytes);
    int size();
};

class GBNFTPData {
public:
    u_int8_t flags = 0x20;
    u_int8_t checksum = 0xaa;
    u_int16_t seqNumber;
    u_int16_t ackNumber;
    u_int16_t payloadLength;
    char* payload;
    char* serialize();
    void deserialize(char* bytes);
    int size();
};

class GBNFTPAck {
public:
    u_int8_t flags = 0x10;
    u_int8_t checksum = 0xaa;
    u_int16_t expSeqNumber;
    u_int16_t ackNumber;
    char* serialize();
    void deserialize(char* bytes);
    int size();
};


#endif //IITH_LAB_GBNFTPPACKET_H
