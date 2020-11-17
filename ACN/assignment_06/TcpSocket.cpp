/*
 * Created on Tue Nov 17 2020
 * Author: Bhavishya Sharma
 *
 * Copyright (c) 2020 IIT Hyderabad
 */

#include "TcpSocket.h"

TcpSocket::TcpSocket(TcpSocketMode socketMode, std::string* serverIP, u_int16_t port) {
    this->socketMode = socketMode;
    this->socketAddress = new sockaddr_in();
    if((this->socketFD = socket(AF_INET, SOCK_RAW, IPPROTO_TCP)) < 0){
        std::cerr<<"RawSocket::RawSocket | socket() failed | "<<strerror(errno)<<std::endl;
        return;
    }
    this->socketAddress->sin_family = AF_INET;
    inet_aton(serverIP->c_str(), &(this->socketAddress->sin_addr));
    this->socketAddress->sin_port = htons(port);
    if(this->socketMode == TcpSocketMode::ClientSocket) {
        if(connect(this->socketFD, (struct sockaddr*) this->socketAddress, sizeof(*this->socketAddress)) <0) {
            std::cerr<<"RawSocket::RawSocket | connect() failed | "<<strerror(errno)<<std::endl;
            return;
        }
    }
    else {
        if(bind(this->socketFD, (struct sockaddr*) this->socketAddress, sizeof(*this->socketAddress)) < 0) {
            std::cerr<<"RawSocket::RawSocket | bind() failed | "<<strerror(errno)<<std::endl;
            return;
        }
        if(listen(this->socketFD, MAX_CONNECTION_REQUEST) < 0) {
            std::cerr<<"RawSocket::RawSocket | listen() failed | "<<strerror(errno)<<std::endl;
            return;
        }
    }   
}

int TcpSocket::acceptConnection(struct sockaddr_in* clientAddress) {
    int clientSocketFD;
    unsigned int clientLength;
    if((clientSocketFD = accept(this->socketFD, (struct sockaddr*) clientAddress, &clientLength)) < 0) {
        std::cerr<<"RawSocket::acceptConnection | accept() failed | "<<strerror(errno)<<std::endl;
    }
    return clientSocketFD;
}

int TcpSocket::write(std::stringstream* stream){
    char *buffer = new char[MAX_TCP_PAYLOAD];
    stream->read(buffer, MAX_TCP_PAYLOAD);
    int length = stream->gcount();
    std::cout<<"Sending : "<<length<<" - "<<buffer<<std::endl;
    if(send(this->socketFD, buffer, length, 0) != length){
        std::cerr<<"RawSocket::write | send() failed | "<<strerror(errno)<<std::endl;
        return 0;
    }
    std::cout<<"Sent : "<<length<<std::endl;
    return length;
}

int TcpSocket::read(std::stringstream* stream){
    char *buffer = new char[MAX_TCP_PAYLOAD];
    int length = 0;
    if((length = recv(this->socketFD, buffer, MAX_TCP_PAYLOAD, 0)) < 0){
        std::cerr<<"RawSocket::write | recv() failed | "<<strerror(errno)<<std::endl;
        return -1;
    }
    stream->write(buffer, length);
    return length;
}
