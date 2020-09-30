//
// Created by Bhavishya Sharma on 27/09/20.
//

#ifndef IITH_LAB_DNSMESSAGE_H
#define IITH_LAB_DNSMESSAGE_H

#include <string>
#include <sys/types.h>

#include "DNSHeader.h"
#include "DNSQuestion.h"
#include "DNSLabel.h"
#include "DNSResourceRecord.h"

class DNSMessage {
public:
    DNSHeader header;
    std::list<DNSQuestion> questions;
    std::list<DNSResourceRecord> answers;
    std::list<DNSResourceRecord> authorities;
    std::list<DNSResourceRecord> additionals;

    void setQueryHeaders();
    void addQuery(std::string domainName);
    void addQuery(std::string domainName, QType qType, QClass qClass);

    u_int16_t size();
    u_int16_t serialize(unsigned char* bytes);
    u_int16_t deserialize(unsigned char* bytes);
    void printMessage();
};


#endif //IITH_LAB_DNSMESSAGE_H
