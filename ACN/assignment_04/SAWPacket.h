//
// Created by Bhavishya Sharma on 30/10/20.
//

#ifndef IITH_LAB_SAWPACKET_H
#define IITH_LAB_SAWPACKET_H

#include <cstring>
#include <list>
#include <sys/types.h>
#include <arpa/inet.h>

#ifndef SAW_MAX_PAYLOAD
#define SAW_MAX_PAYLOAD 8192
#endif

class SAWPacket {
public:
    // bit 7 = Sequence Number
    // bit 6 = Ack Number,
    // bin 5 = close signal
    // bit 5-0 = unused
    u_int8_t flags = 0;
    u_int8_t checksum = 0b10101010; // dummy checksum
    u_int16_t payloadLength = 0;
    char* payload;

    char* serialize();
    void deserialize(char* bytes);
    bool validate();
    bool ackNumber();
    bool seqNumber();
    int size();
    bool isCloseSignal();
};


#endif //IITH_LAB_SAWPACKET_H
