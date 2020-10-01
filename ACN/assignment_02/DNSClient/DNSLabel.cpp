//
// Created by Bhavishya Sharma on 27/09/20.
//

#include <iostream>
#include <string>

#include "DNSLabel.h"

u_int16_t DNSLabel::size() {
    return this->length+1;
}

// convert label to bytes
u_int16_t DNSLabel::serialize(unsigned char *bytes, u_int16_t offset) {
    memcpy(bytes + offset, &this->length, 1);
    memcpy(bytes + offset + 1, this->value, this->length);
    return offset + this->length + 1;
}

// parse label from bytes
u_int16_t DNSLabel::deserialize(unsigned char *bytes, u_int16_t offset) {
    memcpy(&this->length, bytes + offset, 1);
    this->value = new unsigned char [this->length + 1];
    memcpy(this->value, bytes + offset + 1, this->length);
    this->value[this->length] = '\0';
    return offset + this->length + 1;
}

// convert list of labels to domainname string
std::string DNSLabel::getDomainNameFromList(std::list <DNSLabel> *labels) {
    std::string domainName = "";
    const char delimiter = '.';
    for (std::list<DNSLabel>::iterator it=labels->begin(); it != labels->end(); ++it) {
        domainName.append((char*)it->value, (int)it->length);
        if(std::next(it) != labels->end())
            domainName.append(&delimiter);
    }
    return domainName;
}

// convert bytes to list of labels (also decompress if compressed)
u_int16_t DNSLabel::deserializeToList(std::list<DNSLabel> *labels, unsigned char *bytes, u_int16_t offset) {
    while(bytes[offset] != '\0' && (bytes[offset]>>6) != 3){
        DNSLabel *label = new DNSLabel();
        offset = label->deserialize(bytes, offset);
        labels->push_back(*label);
    }
    // label is compressed, try to decompress
    if((bytes[offset]>>6) == 3){
        u_int16_t pointerOffset;
        memcpy(&pointerOffset, bytes + offset, 2);
        pointerOffset = ntohs(pointerOffset);
        pointerOffset = pointerOffset & 0x03FF;
        DNSLabel::deserializeToList(labels, bytes, pointerOffset);
        offset+=2;
    }
    else {
        offset++;
    }
    return offset;
}

// parse list of labels from bytes
u_int16_t DNSLabel::serializeFromList(std::list <DNSLabel> *labels, unsigned char *bytes, u_int16_t offset) {
    for (std::list<DNSLabel>::iterator it=labels->begin(); it != labels->end(); ++it) {
        offset = it->serialize(bytes, offset);
    }
    bytes[offset++] = '\0';
    return offset;
}