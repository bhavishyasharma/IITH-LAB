//
// Created by Bhavishya Sharma on 27/09/20.
//

#include <iostream>
#include "DNSQuestion.h"

using namespace std;

DNSQuestion::DNSQuestion() {
    return;
}

DNSQuestion::DNSQuestion(std::string domainName) {
    this->init(domainName, QT_A, QC_IN);
}

DNSQuestion::DNSQuestion(std::string domainName, QType qType) {
    this->init(domainName, qType, QC_IN);
}

DNSQuestion::DNSQuestion(std::string domainName, QType qType, QClass qClass) {
    this->init(domainName, qType, qClass);
}

// concert string into question object
void DNSQuestion::init(std::string domainName, QType qType, QClass qClass) {
    char *domainNameString = new char[domainName.length()+1];
    memcpy(domainNameString, domainName.c_str(), domainName.length()+1);
    // parse domain name as labels
    char *labelValue = strtok(domainNameString, ".");
    while(labelValue!=NULL){
        DNSLabel *label = new DNSLabel();
        label->length = strlen(labelValue);
        label->value = new unsigned char[label->length];
        memcpy(label->value, labelValue, label->length);
        this->qName.push_back(*label);
        labelValue = strtok(NULL, ".");
    }
    // set question type & class
    this->qType = qType;
    this->qClass = qClass;
}

// calculate size for serialization
u_int16_t DNSQuestion::size() {
    u_int16_t size = 5;
    for (std::list<DNSLabel>::iterator it=this->qName.begin(); it != this->qName.end(); ++it) {
        size += it->size();
    }
    return size;
}

// convert object to bytes
u_int16_t DNSQuestion::serialize(unsigned char *bytes, u_int16_t offset) {
    offset = DNSLabel::serializeFromList(&this->qName, bytes, offset);
    u_int16_t temp;
    memcpy(bytes + offset, &(temp = htons(this->qType)), 2);
    memcpy(bytes + offset + 2, &(temp = htons(this->qClass)), 2);
    return offset + 4;
}

// parse bytes into question object
u_int16_t DNSQuestion::deserialize(unsigned char *bytes, u_int16_t offset) {
    offset = DNSLabel::deserializeToList(&this->qName, bytes, offset);
    u_int16_t temp;
    memcpy(&temp, bytes + offset, 2);
    this->qType = ntohs(temp);
    memcpy(&temp, bytes + offset + 2, 2);
    this->qClass = ntohs(temp);
    return offset + 4;
}

// print question
void DNSQuestion::printQuestion() {
    cout<<string (4, ' ')<<"Domain Name : "<<DNSLabel::getDomainNameFromList(&this->qName)<<endl;
    cout<<string (8, ' ')<<"QType : "<<this->qType<<" ("<<qTypeMap.at(this->qType)<<")"<<endl;
    cout<<string (8, ' ')<<"QClass : "<<this->qClass<<" ("<<qClassMap.at(this->qClass)<<")"<<endl;
}

