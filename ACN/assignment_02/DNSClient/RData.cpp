//
// Created by Bhavishya Sharma on 27/09/20.
//

#include <iostream>
#include <string>
#include <iomanip>
#include <arpa/inet.h>

#include "RData.h"

using namespace std;

u_int16_t AData::size() {
    return 4;
}

u_int16_t AData::deserialize(unsigned char *bytes, u_int16_t offset) {
    u_int32_t temp;
    memcpy(&temp, bytes + offset, 4);
    this->address = ntohl(temp);
    return offset + 4;
}

u_int16_t AData::serialize(unsigned char *bytes, u_int16_t offset) {
    u_int32_t temp;
    memcpy(bytes + offset, &(temp = htonl(this->address)), 4);
    return offset + 4;
}

void AData::printAData() {
    struct in_addr ip_addr;
    ip_addr.s_addr = this->address;
    char *buffer = new char [17];
    inet_ntop(AF_INET, &ip_addr, buffer, 16);
    cout<<string (12, ' ')<<"Address : "<<buffer<<endl;
}

u_int16_t NSData::size() {
    u_int16_t size = 10;
    for (std::list<DNSLabel>::iterator it=this->nsDName.begin(); it != this->nsDName.end(); ++it) {
        size += it->size();
    }
    return size;
}

u_int16_t NSData::deserialize(unsigned char *bytes, u_int16_t offset) {
    return DNSLabel::deserializeToList(&this->nsDName, bytes, offset);
}

u_int16_t NSData::serialize(unsigned char *bytes, u_int16_t offset) {
    return DNSLabel::serializeFromList(&this->nsDName, bytes, offset);
}

void NSData::printNSData() {
    cout<<string (12, ' ')<<"NS : "<<DNSLabel::getDomainNameFromList(&this->nsDName)<<endl;
}

u_int16_t CNameData::size() {
    u_int16_t size = 10;
    for (std::list<DNSLabel>::iterator it=this->data.begin(); it != this->data.end(); ++it) {
        size += it->size();
    }
    return size;
}

u_int16_t CNameData::deserialize(unsigned char *bytes, u_int16_t offset) {
    return DNSLabel::deserializeToList(&this->data, bytes, offset);
}

u_int16_t CNameData::serialize(unsigned char *bytes, u_int16_t offset) {
    return DNSLabel::serializeFromList(&this->data, bytes, offset);
}

void CNameData::printCNameData() {
    cout<<string (12, ' ')<<"CName : "<<DNSLabel::getDomainNameFromList(&this->data)<<endl;
}

u_int16_t SoaData::size() {
    u_int16_t size = 10;
    for (std::list<DNSLabel>::iterator it=this->mName.begin(); it != this->mName.end(); ++it) {
        size += it->size();
    }
    for (std::list<DNSLabel>::iterator it=this->rName.begin(); it != this->rName.end(); ++it) {
        size += it->size();
    }
    return size + 20;
}

u_int16_t SoaData::deserialize(unsigned char *bytes, u_int16_t offset) {
    offset = DNSLabel::deserializeToList(&this->mName, bytes, offset);
    offset = DNSLabel::deserializeToList(&this->rName, bytes, offset);
    u_int32_t temp;
    memcpy(&temp, bytes + offset, 4);
    this->serial = ntohl(temp);
    memcpy(&temp, bytes + offset + 4, 4);
    this->refresh = ntohl(temp);
    memcpy(&temp, bytes + offset + 8, 4);
    this->retry = ntohl(temp);
    memcpy(&temp, bytes + offset + 12, 4);
    this->expire = ntohl(temp);
    memcpy(&temp, bytes + offset + 16, 4);
    this->minimum = ntohl(temp);
    return offset + 20;

}

u_int16_t SoaData::serialize(unsigned char *bytes, u_int16_t offset) {
    offset = DNSLabel::serializeFromList(&this->mName, bytes, offset);
    offset = DNSLabel::serializeFromList(&this->rName, bytes, offset);
    u_int32_t temp;
    memcpy(bytes + offset, &(temp = htonl(this->serial)), 4);
    memcpy(bytes + offset + 4, &(temp = htonl(this->refresh)), 4);
    memcpy(bytes + offset + 8, &(temp = htonl(this->retry)), 4);
    memcpy(bytes + offset + 12, &(temp = htonl(this->expire)), 4);
    memcpy(bytes + offset + 16, &(temp = htonl(this->minimum)), 4);
    return offset + 20;
}

void SoaData::printSoaData() {
    cout<<string (12, ' ')<<"MName : "<<DNSLabel::getDomainNameFromList(&this->mName)<<endl;
    cout<<string (12, ' ')<<"RName : "<<DNSLabel::getDomainNameFromList(&this->rName)<<endl;
    cout<<string (12, ' ')<<"Serial : "<<this->serial<<endl;
    cout<<string (12, ' ')<<"Refresh : "<<this->refresh<<endl;
    cout<<string (12, ' ')<<"Retry : "<<this->retry<<endl;
    cout<<string (12, ' ')<<"Expire : "<<this->expire<<endl;
    cout<<string (12, ' ')<<"Minimum : "<<this->minimum<<endl;
}

u_int16_t PtrData::size() {
    u_int16_t size = 10;
    for (std::list<DNSLabel>::iterator it=this->ptrDName.begin(); it != this->ptrDName.end(); ++it) {
        size += it->size();
    }
    return size;
}

u_int16_t PtrData::deserialize(unsigned char *bytes, u_int16_t offset) {
    return DNSLabel::deserializeToList(&this->ptrDName, bytes, offset);
}

u_int16_t PtrData::serialize(unsigned char *bytes, u_int16_t offset) {
    return DNSLabel::serializeFromList(&this->ptrDName, bytes, offset);
}

u_int16_t HInfoData::size() {
    return cpu.size() + os.size();
}

u_int16_t HInfoData::deserialize(unsigned char *bytes, u_int16_t offset) {
    offset = this->cpu.deserialize(bytes, offset);
    return this->os.deserialize(bytes, offset);
}

u_int16_t HInfoData::serialize(unsigned char *bytes, u_int16_t offset) {
    offset = this->cpu.serialize(bytes, offset);
    return this->os.serialize(bytes, offset);
}

u_int16_t MXData::size() {
    u_int16_t size = 10;
    for (std::list<DNSLabel>::iterator it=this->exchange.begin(); it != this->exchange.end(); ++it) {
        size += it->size();
    }
    return size + 2;
}

u_int16_t MXData::deserialize(unsigned char *bytes, u_int16_t offset) {
    u_int16_t temp;
    memcpy(&temp, bytes + offset, 2);
    this->preference = ntohs(temp);
    return DNSLabel::deserializeToList(&this->exchange, bytes, offset + 2);
}

u_int16_t MXData::serialize(unsigned char *bytes, u_int16_t offset) {
    u_int16_t temp;
    memcpy(bytes + offset, &(temp = htons(this->preference)), 2);
    return DNSLabel::serializeFromList(&this->exchange, bytes, offset + 2);
}

void MXData::printMXData() {
    cout<<string (12, ' ')<<"Preference : "<<this->preference<<endl;
    cout<<string (12, ' ')<<"Exchange : "<<DNSLabel::getDomainNameFromList(&this->exchange)<<endl;
}

u_int16_t TxtData::deserialize(unsigned char *bytes, u_int16_t offset, u_int16_t length) {
    u_int16_t localOffset = offset;
    while(localOffset < offset + length){
        DNSLabel *label = new DNSLabel();
        localOffset = label->deserialize(bytes, localOffset);
        this->txtData.push_back(*label);
    }
    return offset + length;
}

u_int16_t TxtData::serialize(unsigned char *bytes, u_int16_t offset, u_int16_t length) {
    u_int16_t localOffset = offset;
    for (std::list<DNSLabel>::iterator it=this->txtData.begin(); it != this->txtData.end(); ++it) {
        localOffset = it->serialize(bytes, localOffset);
    }
    return offset + length;
}

void TxtData::printTxtData() {
    for (std::list<DNSLabel>::iterator it=this->txtData.begin(); it != this->txtData.end(); ++it) {
        cout<<string (12, ' ')<<"TXT : "<<it->value<<endl;
    }
}

u_int16_t AAAAData::size() {
    return 16;
}

u_int16_t AAAAData::deserialize(unsigned char *bytes, u_int16_t offset) {
    memcpy(&this->address, bytes + offset, 16);
    return offset + 16;
}

u_int16_t AAAAData::serialize(unsigned char *bytes, u_int16_t offset) {
    memcpy(bytes + offset, &this->address, 16);
    return offset + 16;
}

void AAAAData::printAAAAData() {
    char *buffer = new char [17];
    inet_ntop(AF_INET6, &this->address, buffer, 46);
    cout<<string (12, ' ')<<"Address : "<<buffer<<endl;
}

u_int16_t CAAData::deserialize(unsigned char *bytes, u_int16_t offset, u_int16_t length) {
    memcpy(&this->flags, bytes + offset++, 1);
    offset = this->tag.deserialize(bytes, offset);
    u_int16_t valueLength = length - this->tag.size() - 1;
    this->value = new unsigned char [valueLength];
    memcpy(&this->value, bytes + offset, valueLength);
    return offset + valueLength;
}

u_int16_t CAAData::serialize(unsigned char *bytes, u_int16_t offset, u_int16_t length) {
    memcpy(bytes + offset++, &this->flags, 1);
    offset = this->tag.serialize(bytes, offset);
    u_int16_t valueLength = length - this->tag.size() - 1;
    memcpy(bytes + offset, &this->value, valueLength);
    return offset + valueLength;
}

u_int16_t UnsupportedData::deserialize(unsigned char *bytes, u_int16_t offset, u_int16_t length) {
    this->data = new u_int8_t [length];
    memcpy(this->data, bytes + offset,length);
    this->length = length;
    return offset + length;
}

u_int16_t UnsupportedData::serialize(unsigned char *bytes, u_int16_t offset, u_int16_t length) {
    memcpy(bytes + offset, this->data, length);
    return offset + length;
}

void UnsupportedData::printUnsupportedData() {
    cout<<string (12, ' ')<<"Unsupported Data : ";
    for(int i=0;i<this->length;i++){
        cout<<hex<<setw(2)<<setfill('0')<<(int)this->data[i]<<" ";
    }
    cout<<endl;
}