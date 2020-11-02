//
// Created by Bhavishya Sharma on 27/09/20.
//

#ifndef IITH_LAB_DNSHEADER_H
#define IITH_LAB_DNSHEADER_H

#include <string>
#include <cstring>
#include <bitset>
#include <sys/types.h>
#include <arpa/inet.h>

class DNSHeader {
public:
    u_int16_t id;
    u_int16_t flags;
    u_int16_t qdCount;
    u_int16_t anCount;
    u_int16_t nsCount;
    u_int16_t arCount;

    u_int16_t size();
    u_int16_t serialize(unsigned char* bytes, u_int16_t offset);
    u_int16_t deserialize(unsigned char* bytes, u_int16_t offset);
    void printHeader();
};


#endif //IITH_LAB_DNSHEADER_H
