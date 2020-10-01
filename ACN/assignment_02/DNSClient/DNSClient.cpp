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
    // Create socket
    if((this->socketFD = socket(PF_INET, SOCK_DGRAM, 0)) < 0) {
        std::cerr<<"Failed: Error creating socket.";
        throw std::runtime_error("Failed: Error creating socket.");
    }

    // Create message
    DNSMessage *message = new DNSMessage();
    // Set query headers
    message->setQueryHeaders();
    // add question to message
    message->addQuery(domainName, qType, qClass);

    // serialize message
    int messageSize = message->size();
    unsigned char * buffer = new unsigned char [message->size()];
    message->serialize(buffer);

    // print query message
    message->printMessage();

    // send message
    if (sendto(this->socketFD, buffer, messageSize, 0,( struct sockaddr *) &(this->serverAddress), sizeof(this->serverAddress)) != messageSize) {
        std::cerr<<"Failed: Error sending message to server.";
        throw std::runtime_error("Failed: Error sending message to server.");
    }

    // receive message
    unsigned char *recBuffer = new unsigned char[DNS_BUFFER_SIZE];
    this->serverLen = sizeof(this->serverAddress);
    if ((messageSize = recvfrom(this->socketFD, recBuffer, DNS_BUFFER_SIZE, 0, ( struct sockaddr *) &(this->serverAddress), &(this->serverLen))) < 0) {
        std::cerr<<"Failed: Error receiving message from server.";
        throw std::runtime_error("Failed: Error receiving message from server.");
    }
    else{
        // deserialize response message
        message = new DNSMessage();
        message->deserialize(recBuffer);

        // print response
        message->printMessage();
    }
}