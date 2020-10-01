//
// Created by Bhavishya Sharma on 27/09/20.
//

#include <iostream>
#include "DNSMessage.h"

using namespace std;

// set query headers
void DNSMessage::setQueryHeaders() {
    header.id = rand();
    header.flags = 0x0100; // Set Recursion Flag
    header.qdCount = this->questions.size();
    header.anCount = 0;
    header.nsCount = 0;
    header.arCount = 0;
}

void DNSMessage::addQuery(std::string domainName) {
    this->addQuery(domainName, QT_A, QC_IN);
}

// add domainname as query question
void DNSMessage::addQuery(std::string domainName, QType qType, QClass qClass) {
    DNSQuestion *question = new DNSQuestion(domainName, qType, qClass);
    this->questions.push_back(*question);
    this->header.qdCount = this->questions.size();
}

// calculate bytes needed to serialize the message
u_int16_t DNSMessage::size() {
    u_int16_t size = 12; // header bytes
    for (std::list<DNSQuestion>::iterator it=this->questions.begin(); it != this->questions.end(); ++it) {
        size += it->size(); // bytes for each question
    }
    for (std::list<DNSResourceRecord>::iterator it=this->answers.begin(); it != this->answers.end(); ++it) {
        size += it->size(); // bytes for each answer
    }
    for (std::list<DNSResourceRecord>::iterator it=this->authorities.begin(); it != this->authorities.end(); ++it) {
        size += it->size(); // bytes for each nameserver
    }
    for (std::list<DNSResourceRecord>::iterator it=this->additionals.begin(); it != this->additionals.end(); ++it) {
        size += it->size(); // bytes for each additional record
    }
    return size;
}

// convert message object to bytes
u_int16_t DNSMessage::serialize(unsigned char *bytes) {
    u_int16_t offset = 0;
    offset = this->header.serialize(bytes, offset);
    for (std::list<DNSQuestion>::iterator it=this->questions.begin(); it != this->questions.end(); ++it) {
        offset = it->serialize(bytes, offset);
    }
    for (std::list<DNSResourceRecord>::iterator it=this->answers.begin(); it != this->answers.end(); ++it) {
        offset = it->serialize(bytes, offset);
    }
    for (std::list<DNSResourceRecord>::iterator it=this->authorities.begin(); it != this->authorities.end(); ++it) {
        offset = it->serialize(bytes, offset);
    }
    for (std::list<DNSResourceRecord>::iterator it=this->additionals.begin(); it != this->additionals.end(); ++it) {
        offset = it->serialize(bytes, offset);
    }
    return offset;
}

// parse bytes into message object
u_int16_t DNSMessage::deserialize(unsigned char *bytes) {
    u_int16_t offset = 0;
    offset = this->header.deserialize(bytes, offset);
    for(int i=0;i<this->header.qdCount;i++){
        DNSQuestion *question = new DNSQuestion();
        offset = question->deserialize(bytes, offset);
        this->questions.push_back(*question);
    }
    for(int i=0;i<this->header.anCount;i++){
        DNSResourceRecord *resourceRecord = new DNSResourceRecord();
        offset = resourceRecord->deserialize(bytes, offset);
        this->answers.push_back(*resourceRecord);
    }
    for(int i=0;i<this->header.nsCount;i++){
        DNSResourceRecord *resourceRecord = new DNSResourceRecord();
        offset = resourceRecord->deserialize(bytes, offset);
        this->authorities.push_back(*resourceRecord);
    }
    for(int i=0;i<this->header.arCount;i++){
        DNSResourceRecord *resourceRecord = new DNSResourceRecord();
        offset = resourceRecord->deserialize(bytes, offset);
        this->additionals.push_back(*resourceRecord);
    }
    return offset;
}

// print message
void DNSMessage::printMessage() {
    // print header
    cout<<'+'<<string (58, '-')<<'+'<<endl;
    cout<<'|'<<string (26, ' ')<<"Header"<<string (26,' ')<<'|'<<endl;
    cout<<'+'<<string (58, '-')<<'+'<<endl;
    this->header.printHeader();

    // print questions
    if(this->header.qdCount > 0) {
        cout << '+' << string(58, '-') << '+' << endl;
        cout << '|' << string(24, ' ') << "Questions" << string(25, ' ') << '|' << endl;
        cout << '+' << string(58, '-') << '+' << endl;
    }
    for (std::list<DNSQuestion>::iterator it=this->questions.begin(); it != this->questions.end(); ++it) {
        it->printQuestion();
    }

    // print answers
    if(this->header.anCount > 0) {
        cout << '+' << string(58, '-') << '+' << endl;
        cout << '|' << string(25, ' ') << "Answers" << string(26, ' ') << '|' << endl;
        cout << '+' << string(58, '-') << '+' << endl;
    }
    for (std::list<DNSResourceRecord>::iterator it=this->answers.begin(); it != this->answers.end(); ++it) {
        it->printResourceRecord();
    }

    // print nameservers
    if(this->header.nsCount > 0) {
        cout << '+' << string(58, '-') << '+' << endl;
        cout << '|' << string(23, ' ') << "Authorities" << string(24, ' ') << '|' << endl;
        cout << '+' << string(58, '-') << '+' << endl;
    }
    for (std::list<DNSResourceRecord>::iterator it=this->authorities.begin(); it != this->authorities.end(); ++it) {
        it->printResourceRecord();
    }

    // print additional records
    if(this->header.arCount > 0) {
        cout << '+' << string(58, '-') << '+' << endl;
        cout << '|' << string(24, ' ') << "Additional" << string(24, ' ') << '|' << endl;
        cout << '+' << string(58, '-') << '+' << endl;
    }
    for (std::list<DNSResourceRecord>::iterator it=this->additionals.begin(); it != this->additionals.end(); ++it) {
        it->printResourceRecord();
    }
    return;
}