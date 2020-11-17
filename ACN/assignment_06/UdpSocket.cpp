/*
 * Created on Tue Nov 17 2020
 * Author: Bhavishya Sharma
 *
 * Copyright (c) 2020 IIT Hyderabad
 */

#include "UdpSocket.h"

UdpSocket::UdpSocket(UdpSocketMode socketMode, std::string* serverIP, u_int16_t port) {
    this->socketMode = socketMode;
    this->socketAddress = new sockaddr_in();
    this->socketAddress->sin_family = AF_INET;
    inet_aton(serverIP->c_str(), &(this->socketAddress->sin_addr));
    this->socketAddress->sin_port = htons(port);
    if((this->socketFD = socket(AF_INET, SOCK_RAW, IPPROTO_UDP)) < 0){
        std::cerr<<"UdpSocket::UdpSocket | socket() failed | "<<strerror(errno)<<std::endl;
        return;
    }
    if(this->socketMode == UdpSocketMode::ServerSocket){
        if(bind(this->socketFD, (struct sockaddr*) &(this->socketAddress), sizeof(*this->socketAddress)) < 0) {
            std::cerr<<"UdpSocket::UdpSocket | bind() failed | "<<strerror(errno)<<std::endl;
            return;
        }
    }
}

int UdpSocket::write(std::stringstream* stream, struct sockaddr_in* remoteAddress){
    char *buffer = new char[MAX_UDP_PAYLOAD];
    stream->read(buffer, MAX_UDP_PAYLOAD);
    int length = stream->gcount();
    unsigned int remoteLength = sizeof(*remoteAddress);
    if(sendto(this->socketFD, buffer, length, 0, (struct sockaddr*) remoteAddress, remoteLength) != length){
        std::cerr<<"UdpSocket::write | sendto() failed | "<<strerror(errno)<<std::endl;
        return 0;
    }
    return length;
}

int UdpSocket::read(std::stringstream* stream, struct sockaddr_in* remoteAddress){
    char *buffer = new char[MAX_UDP_PAYLOAD];
    int length = 0;
    unsigned int remoteLength = sizeof(*remoteAddress);
    if((length = recvfrom(this->socketFD, buffer, MAX_UDP_PAYLOAD, 0, (struct sockaddr*) remoteAddress, &remoteLength))<0){
        std::cerr<<"UdpSocket::write | recvfrom() failed | "<<strerror(errno)<<std::endl;
        return -1;
    }
    stream->write(buffer, length);
    return length;
}
