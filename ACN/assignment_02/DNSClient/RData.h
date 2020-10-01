//
// Created by Bhavishya Sharma on 27/09/20.
//

#ifndef IITH_LAB_RDATAT_H
#define IITH_LAB_RDATAT_H

#include <string>
#include <string_view>
#include <sys/types.h>

#include "DNSLabel.h"

class RData {
};

class AData : public RData {
public:
    u_int32_t address;

    u_int16_t size();
    u_int16_t serialize(unsigned char* bytes, u_int16_t offset);
    u_int16_t deserialize(unsigned char* bytes, u_int16_t offset);
    void printAData();
};

class NSData : public RData {
public:
    std::list<DNSLabel> nsDName;

    u_int16_t size();
    u_int16_t serialize(unsigned char* bytes, u_int16_t offset);
    u_int16_t deserialize(unsigned char* bytes, u_int16_t offset);
    void printNSData();
};

class CNameData : public RData {
public:
    std::list<DNSLabel> data;

    u_int16_t size();
    u_int16_t serialize(unsigned char* bytes, u_int16_t offset);
    u_int16_t deserialize(unsigned char* bytes, u_int16_t offset);
    void printCNameData();
};

class SoaData : public RData {
public:
    std::list<DNSLabel> mName;
    std::list<DNSLabel> rName;
    u_int32_t serial;
    u_int32_t refresh;
    u_int32_t retry;
    u_int32_t expire;
    u_int32_t minimum;

    u_int16_t size();
    u_int16_t serialize(unsigned char* bytes, u_int16_t offset);
    u_int16_t deserialize(unsigned char* bytes, u_int16_t offset);
    void printSoaData();
};

class MXData : public RData {
public:
    u_int16_t preference;
    std::list<DNSLabel> exchange;

    u_int16_t size();
    u_int16_t serialize(unsigned char* bytes, u_int16_t offset);
    u_int16_t deserialize(unsigned char* bytes, u_int16_t offset);
    void printMXData();
};

class TxtData : public RData {
public:
    std::list<DNSLabel> txtData;

    u_int16_t serialize(unsigned char* bytes, u_int16_t offset, u_int16_t length);
    u_int16_t deserialize(unsigned char* bytes, u_int16_t offset, u_int16_t length);
    void printTxtData();
};

class AAAAData : public RData {
public:
    unsigned char address[16];

    u_int16_t size();
    u_int16_t serialize(unsigned char* bytes, u_int16_t offset);
    u_int16_t deserialize(unsigned char* bytes, u_int16_t offset);
    void printAAAAData();
};

class UnsupportedData : public RData {
public:
    u_int16_t length;
    u_int8_t *data;

    u_int16_t serialize(unsigned char* bytes, u_int16_t offset, u_int16_t length);
    u_int16_t deserialize(unsigned char* bytes, u_int16_t offset, u_int16_t length);
    void printUnsupportedData();
};

#endif //IITH_LAB_RDATA_H
