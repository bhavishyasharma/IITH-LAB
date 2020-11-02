//
// Created by Bhavishya Sharma on 29/10/20.
//

#include "UDPSocket.h"

UDPSocket::UDPSocket(UDPSocketType socketType, std::string* serverIP, u_int16_t port) {
    this->socketAddress = new sockaddr_in;
    if(socketType == UDPSocketType::ServerSocket){
        initServer(serverIP, port);
    }
    else{
        initClient(serverIP, port);
    }
}

UDPSocket::~UDPSocket() {
    if(this->socketAddress)
        delete this->socketAddress;
}

void UDPSocket::closeSocket() {
    close(this->socketFD);
}

void UDPSocket::initServer(std::string* serverIP, u_int16_t port) {
    if((this->socketFD = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0) {
        std::cerr<<"Failed: Error creating socket.";
        throw std::runtime_error("Failed: Error creating socket.");
    }
    this->socketAddress->sin_family = AF_INET;
    inet_aton(serverIP->c_str(), &(this->socketAddress->sin_addr));
    this->socketAddress->sin_port = htons(port);

    if( bind(this->socketFD, (struct sockaddr *) this->socketAddress, sizeof(*(this->socketAddress))) < 0 ) {
        std::cerr<<"Error binding socket: "<<strerror(errno)<<std::endl;
        throw std::runtime_error("Failed: Error binding socket.");
    }
}

void UDPSocket::initClient(std::string* serverIP, u_int16_t port) {
    this->socketAddress->sin_family = AF_INET;
    inet_aton(serverIP->c_str(), &(this->socketAddress->sin_addr));
    this->socketAddress->sin_port = htons(port);

    if((this->socketFD = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0) {
        std::cerr<<"Failed: Error creating socket.";
        throw std::runtime_error("Failed: Error creating socket.");
    }
}

int UDPSocket::sendTo(char* buffer, int length, struct sockaddr_in *remoteAddress) {
    if(length > UDP_MAX_PAYLOAD){
        length = UDP_MAX_PAYLOAD;
    }
    unsigned int remoteLength = sizeof(*remoteAddress);
    if(sendto(this->socketFD, buffer, length, 0, (struct sockaddr*) remoteAddress, remoteLength) != length){
        std::cerr<<"UDP sentto failed: "<<strerror(errno)<<std::endl;
        return -1;
    }
    return length;
}

int UDPSocket::receiveFrom(char* buffer, int maxLength, struct sockaddr_in *remoteAddress) {
    if(maxLength > UDP_MAX_PAYLOAD){
        maxLength = UDP_MAX_PAYLOAD;
    }
    unsigned int remoteLength = sizeof(*remoteAddress);
    int messageSize;
    if ((messageSize = recvfrom(this->socketFD, buffer, maxLength, 0, ( struct sockaddr *) remoteAddress, &remoteLength)) < 0) {
        std::cerr<<"UDP recvfrom failed: "<<strerror(errno)<<" / "<<errno<<std::endl;
        return -1;
    }
    return messageSize;
}

void UDPSocket::setReceiveTimeout(struct timeval* timeout) {
    // TODO struct timeval timeout={2,0};
    setsockopt(this->socketFD,SOL_SOCKET,SO_RCVTIMEO,(char*)timeout,sizeof(struct timeval));
}
