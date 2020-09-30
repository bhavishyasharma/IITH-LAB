//
// Created by Bhavishya Sharma on 27/09/20.
//

#ifndef IITH_LAB_DNSRESOURCERECORD_H
#define IITH_LAB_DNSRESOURCERECORD_H

#include <string>
#include <sys/types.h>

#include "DNSLabel.h"
#include "RData.h"

class DNSResourceRecord {
public:
    std::list<DNSLabel> name;
    u_int16_t type;
    u_int16_t dataClass;
    u_int32_t ttl;
    u_int16_t dataLength;
    RData *data;

    u_int16_t size();
    u_int16_t serialize(unsigned char* bytes, u_int16_t offset);
    u_int16_t deserialize(unsigned char* bytes, u_int16_t offset);
    void printResourceRecord();
};


#endif //IITH_LAB_DNSRESOURCERECORD_H
