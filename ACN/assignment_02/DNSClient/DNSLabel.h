//
// Created by Bhavishya Sharma on 27/09/20.
//

#ifndef IITH_LAB_DNSLABEL_H
#define IITH_LAB_DNSLABEL_H

#include <list>
#include <sys/types.h>

class DNSLabel {
public:
    u_int8_t length;
    unsigned char* value;

    u_int16_t size();
    u_int16_t serialize(unsigned char* bytes, u_int16_t offset);
    u_int16_t deserialize(unsigned char* bytes, u_int16_t offset);

    static std::string getDomainNameFromList(std::list<DNSLabel> *labels);
    static void getListFromDomainName(std::list<DNSLabel> *label, std::string domainName);
    static u_int16_t deserializeToList(std::list<DNSLabel> *labels, unsigned char *bytes, u_int16_t offset);
    static u_int16_t serializeFromList(std::list<DNSLabel> *labels, unsigned char *bytes, u_int16_t offset);
};


#endif //IITH_LAB_DNSLABEL_H
