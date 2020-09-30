//
// Created by Bhavishya Sharma on 27/09/20.
//

#include <iostream>

#include "DNSResourceRecord.h"
#include "RData.h"
#include "QType.hpp"
#include "QClass.hpp"

using namespace std;

u_int16_t DNSResourceRecord::size() {
    u_int16_t size = 10;
    for (std::list<DNSLabel>::iterator it=this->name.begin(); it != this->name.end(); ++it) {
        size += it->size();
    }
    switch (this->type) {
        case QT_A: {
            AData *dataPointer = (AData *) this->data;
            size += dataPointer->size();
            break;
        }
        case QT_NS: {
            NSData *dataPointer = (NSData *) this->data;
            size += dataPointer->size();
            break;
        }
        case QT_CNAME: {
            CNameData *dataPointer = (CNameData *) this->data;
            size += dataPointer->size();
            break;
        }
        case QT_SOA: {
            SoaData *dataPointer = (SoaData *) this->data;
            size += dataPointer->size();
            break;
        }
        case QT_MX: {
            MXData *dataPointer = (MXData *) this->data;
            size += dataPointer->size();
            break;
        }
        case QT_AAAA: {
            AAAAData *dataPointer = (AAAAData *) this->data;
            size += dataPointer->size();
            break;
        }
        default:{
            size += this->dataLength;
            break;
        }
    }
    return size;
}

u_int16_t DNSResourceRecord::deserialize(unsigned char *bytes, u_int16_t offset) {
    offset = DNSLabel::deserializeToList(&this->name, bytes, offset);
    u_int16_t temp;
    memcpy(&temp, bytes + offset, 2);
    this->type = ntohs(temp);
    memcpy(&temp, bytes + offset + 2, 2);
    this->dataClass = ntohs(temp);
    u_int32_t temp2;
    memcpy(&temp2, bytes + offset + 4, 4);
    this->ttl = ntohl(temp2);
    memcpy(&temp, bytes + offset + 8, 2);
    this->dataLength = ntohs(temp);
    offset+=10;
    switch (this->type) {
        case QT_A: {
            this->data = new AData();
            AData *dataPointer = (AData *) this->data;
            offset = dataPointer->deserialize(bytes, offset);
            break;
        }
        case QT_CNAME: {
            this->data = new CNameData();
            CNameData *dataPointer = (CNameData *) this->data;
            offset = dataPointer->deserialize(bytes, offset);
            break;
        }
        case QT_NS: {
            this->data = new NSData();
            NSData *dataPointer = (NSData *) this->data;
            offset = dataPointer->deserialize(bytes, offset);
            break;
        }
        case QT_SOA: {
            this->data = new SoaData();
            SoaData *dataPointer = (SoaData *) this->data;
            offset = dataPointer->deserialize(bytes, offset);
            break;
        }
        case QT_MX: {
            this->data = new MXData();
            MXData *dataPointer = (MXData *) this->data;
            offset = dataPointer->deserialize(bytes, offset);
            break;
        }
        case QT_TXT: {
            this->data = new TxtData();
            TxtData *dataPointer = (TxtData *) this->data;
            offset = dataPointer->deserialize(bytes, offset, this->dataLength);
            break;
        }
        case QT_AAAA: {
            this->data = new AAAAData();
            AAAAData *dataPointer = (AAAAData *) this->data;
            offset = dataPointer->deserialize(bytes, offset);
            break;
        }
        default:{
            this->data = new UnsupportedData();
            UnsupportedData *dataPointer = (UnsupportedData *) this->data;
            offset = dataPointer->deserialize(bytes, offset, this->dataLength);
            break;
        }
    }
    return offset;
}
u_int16_t DNSResourceRecord::serialize(unsigned char *bytes, u_int16_t offset) {
    offset = DNSLabel::serializeFromList(&this->name, bytes, offset);
    u_int16_t temp;
    memcpy(bytes + offset, &(temp = htons(this->type)), 2);
    memcpy(bytes + offset + 2, &(temp = htons(this->dataClass)), 2);
    u_int32_t temp2;
    memcpy(bytes + offset + 4, &(temp2 = htonl(this->ttl)), 4);
    memcpy(bytes + offset + 8, &(temp = htons(this->dataLength)), 2);
    offset+=10;
    switch (this->type) {
        case QT_A: {
            AData *dataPointer = (AData *) this->data;
            offset = dataPointer->serialize(bytes, offset);
            break;
        }
        case QT_NS: {
            NSData *dataPointer = (NSData *) this->data;
            offset = dataPointer->serialize(bytes, offset);
            break;
        }
        case QT_CNAME: {
            CNameData *dataPointer = (CNameData *) this->data;
            offset = dataPointer->serialize(bytes, offset);
            break;
        }
        case QT_SOA: {
            SoaData *dataPointer = (SoaData *) this->data;
            offset = dataPointer->serialize(bytes, offset);
            break;
        }
        case QT_MX: {
            MXData *dataPointer = (MXData *) this->data;
            offset = dataPointer->serialize(bytes, offset);
            break;
        }
        case QT_TXT: {
            TxtData *dataPointer = (TxtData *) this->data;
            offset = dataPointer->serialize(bytes, offset, this->dataLength);
            break;
        }
        case QT_AAAA: {
            AAAAData *dataPointer = (AAAAData *) this->data;
            offset = dataPointer->serialize(bytes, offset);
            break;
        }
        default:{
            UnsupportedData *dataPointer = (UnsupportedData *) this->data;
            offset = dataPointer->serialize(bytes, offset, this->dataLength);
            break;
        }
    }
    return offset;
}

void DNSResourceRecord::printResourceRecord() {
    cout<<string (4, ' ')<<"Domain Name : "<<DNSLabel::getDomainNameFromList(&this->name)<<endl;
    cout<<string (8, ' ')<<"Size : "<<this->size()<<endl;
    cout<<string (8, ' ')<<"Type : "<<this->type<<" ("<<qTypeMap.at(this->type)<<")"<<endl;
    cout<<string (8, ' ')<<"Class : "<<this->type<<" ("<<qClassMap.at(this->dataClass)<<")"<<endl;
    cout<<string (8, ' ') <<"TTL : "<<dec<<this->ttl<<endl;
    cout<<string (8, ' ') <<"Data Length : "<<dec<<this->dataLength<<endl;
    cout<<string (8, ' ') <<"Data : "<<endl;
    switch (this->type) {
        case QT_A: {
            AData *dataPointer = (AData *) this->data;
            dataPointer->printAData();
            break;
        }
        case QT_NS: {
            NSData *dataPointer = (NSData *) this->data;
            dataPointer->printNSData();
            break;
        }
        case QT_CNAME: {
            CNameData *dataPointer = (CNameData *) this->data;
            dataPointer->printCNameData();
            break;
        }
        case QT_SOA: {
            SoaData *dataPointer = (SoaData *) this->data;
            dataPointer->printSoaData();
            break;
        }
        case QT_MX: {
            MXData *dataPointer = (MXData *) this->data;
            dataPointer->printMXData();
            break;
        }
        case QT_TXT: {
            TxtData *dataPointer = (TxtData *) this->data;
            dataPointer->printTxtData();
            break;
        }
        case QT_AAAA: {
            AAAAData *dataPointer = (AAAAData *) this->data;
            dataPointer->printAAAAData();
            break;
        }
        default:{
            UnsupportedData *dataPointer = (UnsupportedData *) this->data;
            dataPointer->printUnsupportedData();
            break;
        }
    }
}