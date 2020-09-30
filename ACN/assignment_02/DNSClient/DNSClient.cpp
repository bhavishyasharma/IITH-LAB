//
// Created by Bhavishya Sharma on 26/09/20.
//

#include <string>
#include <iostream>
#include <iomanip>
#include <getopt.h>

#include "DNSClient.h"
#include "DNSMessage.h"
#include "QClass.hpp"
#include "QType.hpp"

DNSClient::DNSClient(std::string serverIP, u_int16_t port) {
    this->init(serverIP, port);
}

DNSClient::DNSClient(u_int16_t port) {
    this->init(DEFAULT_DNS_SERVER_IP, port);
}

DNSClient::DNSClient(std::string serverIP) {
    this->init(serverIP, DEFAULT_DNS_SERVER_PORT);
}

DNSClient::DNSClient() {
    this->init(DEFAULT_DNS_SERVER_IP, DEFAULT_DNS_SERVER_PORT);
}

void DNSClient::init(std::string serverIP, u_int16_t port) {
    this->serverAddress.sin_family = AF_INET;
    inet_aton(serverIP.c_str(), &(this->serverAddress.sin_addr));
    this->serverAddress.sin_port = htons(port);
}

void DNSClient::query(std::string domainName) {
    this->query(domainName, QT_A, QC_IN);
}

void DNSClient::query(std::string domainName, QType qType, QClass qClass) {
    if((this->socketFD = socket(PF_INET, SOCK_DGRAM, 0)) < 0) {
        std::cerr<<"Failed: Error creating socket.";
        throw std::runtime_error("Failed: Error creating socket.");
    }
    DNSMessage *message = new DNSMessage();
    message->setQueryHeaders();
    message->addQuery(domainName, qType, qClass);
    int messageSize = message->size();
    unsigned char * buffer = new unsigned char [message->size()];
    message->serialize(buffer);
    for(int i=0; i<messageSize; ++i)
        std::cout<< std::setfill('0')<< std::hex <<std::setw(2)<< (int)buffer[i]<<" ";
    std::cout<<std::endl;
    message->printMessage();
    if (sendto(this->socketFD, buffer, messageSize, 0,( struct sockaddr *) &(this->serverAddress), sizeof(this->serverAddress)) != messageSize) {
        std::cerr<<"Failed: Error sending message to server.";
        throw std::runtime_error("Failed: Error sending message to server.");
    }
    unsigned char *recBuffer = new unsigned char[DNS_BUFFER_SIZE];
    this->serverLen = sizeof(this->serverAddress);
    if ((messageSize = recvfrom(this->socketFD, recBuffer, DNS_BUFFER_SIZE, 0, ( struct sockaddr *) &(this->serverAddress), &(this->serverLen))) < 0) {
        std::cerr<<"Failed: Error receiving message from server.";
        throw std::runtime_error("Failed: Error receiving message from server.");
    }
    else{
        for(int i=0; i<messageSize; ++i)
            std::cout<< std::setfill('0')<< std::hex <<std::setw(2)<< (int)recBuffer[i]<<" ";
        std::cout<<std::endl;
        message = new DNSMessage();
        message->deserialize(recBuffer);
        message->printMessage();
        unsigned char *newBuffer = new unsigned char [message->size()];
        message->serialize(newBuffer);
        DNSMessage *newMessage = new DNSMessage();
        newMessage->deserialize(newBuffer);
        newMessage->printMessage();
    }
}