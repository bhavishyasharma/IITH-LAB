//
// Created by Bhavishya Sharma on 27/09/20.
//

#ifndef IITH_LAB_DNSQUESTION_H
#define IITH_LAB_DNSQUESTION_H

#include <string>
#include <list>
#include <iterator>

#include "DNSLabel.h"
#include "QClass.hpp"
#include "QType.hpp"

class DNSQuestion {
public:
    std::list<DNSLabel> qName;
    u_int16_t qType;
    u_int16_t qClass;

    DNSQuestion();
    DNSQuestion(std::string domainName);
    DNSQuestion(std::string domainName, QType qType);
    DNSQuestion(std::string domainName, QType qType, QClass qClass);
    void init(std::string domainName, QType qType, QClass qClass);
    u_int16_t size();
    u_int16_t serialize(unsigned char* bytes, u_int16_t offset);
    u_int16_t deserialize(unsigned char* bytes, u_int16_t offset);
    void printQuestion();
};


#endif //IITH_LAB_DNSQUESTION_H
