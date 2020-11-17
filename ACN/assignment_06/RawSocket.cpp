/*
 * Created on Tue Nov 17 2020
 * Author: Bhavishya Sharma
 *
 * Copyright (c) 2020 IIT Hyderabad
 */

#include "RawSocket.h"

RawSocket::RawSocket(RawSocketMode socketMode, std::string* serverIP, u_int16_t port) {
    this->socketMode = socketMode;
    this->socketAddress = new sockaddr_in();
    this->socketAddress->sin_family = AF_INET;
    inet_aton(serverIP->c_str(), &(this->socketAddress->sin_addr));
    this->socketAddress->sin_port = htons(port);
    if((this->socketFD = socket(AF_INET, SOCK_RAW, IPPROTO_UDP)) < 0){
        std::cerr<<"RawSocket::RawSocket | socket() failed | "<<strerror(errno)<<std::endl;
        return;
    }
    /**if(this->socketMode == RawSocketMode::ServerSocket){
        if(bind(this->socketFD, (struct sockaddr*) &(this->socketAddress), sizeof(*this->socketAddress)) < 0) {
            std::cerr<<"RawSocket::RawSocket | bind() failed | "<<strerror(errno)<<std::endl;
            return;
        }
    }*/
    int one = 1;
	const int *val = &one;
	if (setsockopt (this->socketFD, IPPROTO_IP, IP_HDRINCL, val, sizeof (one)) < 0)
	{
        std::cerr<<"RawSocket::RawSocket | setsockopt() failed | "<<strerror(errno)<<std::endl;
		return;
	}   
}

int RawSocket::write(std::stringstream* stream, struct sockaddr_in* remoteAddress){
    char *buffer = new char[MTU];
    stream->read(buffer, MTU);
    int length = stream->gcount();
    unsigned int remoteLength = sizeof(*remoteAddress);
    if(sendto(this->socketFD, buffer, length, 0, (struct sockaddr*) remoteAddress, remoteLength) != length){
        std::cerr<<"RawSocket::write | sendto() failed | "<<strerror(errno)<<std::endl;
        return 0;
    }
    return length;
}

int RawSocket::read(std::stringstream* stream, struct sockaddr_in* remoteAddress){
    char *buffer = new char[MTU];
    int length = 0;
    unsigned int remoteLength = sizeof(*remoteAddress);
    std::cout<<"Reading"<<std::endl;
    if((length = recvfrom(this->socketFD, buffer, MTU, 0, (struct sockaddr*) remoteAddress, &remoteLength))<0){
        std::cerr<<"RawSocket::write | recvfrom() failed | "<<strerror(errno)<<std::endl;
        return -1;
    }
    stream->write(buffer, length);
    stream->flush();
    return length;
}
