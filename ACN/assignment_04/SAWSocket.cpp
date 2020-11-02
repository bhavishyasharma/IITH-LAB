//
// Created by Bhavishya Sharma on 29/10/20.
//

#include "SAWSocket.h"

int SAWSocket::send(char *buffer, int length, int retries) {
    if(!retries)
        return -1;

    if(this->partnerAddress == NULL){
        this->partnerAddress = new sockaddr_in;
    }
    // create packet
    SAWPacket packet;
    packet.payloadLength = length > SAW_MAX_PAYLOAD ? SAW_MAX_PAYLOAD : length;
    packet.payload = buffer;
    if(sendSequenceNumber)
        packet.flags |= 0b10000000;

    char* recvBuffer = new char [UDP_MAX_PAYLOAD];
    struct timeval timeout={0,0}; // 50ms
    this->udpSocket->setReceiveTimeout(&timeout);
    int rVal = this->udpSocket->sendTo(packet.serialize(),packet.size(), this->partnerAddress);
    if(rVal!=packet.size()){
        std::cerr<<"UDP Send failed.\n";
        return -1;
    }
    timeout={0,50000}; // 50ms
    this->udpSocket->setReceiveTimeout(&timeout);
    if(this->udpSocket->receiveFrom(recvBuffer, UDP_MAX_PAYLOAD, this->partnerAddress) < 0){
        // timeout or other error
        std::cerr<<"UDP Receive failed.\n";
        return this->send(buffer, length, --retries);
    }
    else{
        SAWPacket recvPacket;
        recvPacket.deserialize(recvBuffer);
        if(!recvPacket.validate() || recvPacket.ackNumber() != packet.seqNumber()){
            // corrupt ack
            return this->send(buffer, length, --retries);
        }
    }
    this->sendSequenceNumber = !this->sendSequenceNumber;
    if(length!=packet.payloadLength){
        // more data to send
        return this->send(buffer + packet.payloadLength, length-packet.payloadLength) == length-packet.payloadLength ? length : -1;
    }
    return length;
}

int SAWSocket::receive(char* buffer, int maxLength, int retries) {
    if(!retries)
        return -1;

    if(this->partnerAddress == NULL){
        this->partnerAddress = new sockaddr_in;
    }
    char *recvBuffer = new char [UDP_MAX_PAYLOAD];
    int recvLength = (maxLength + 4)> UDP_MAX_PAYLOAD ? UDP_MAX_PAYLOAD : maxLength + 4;
    struct timeval timeout={0,0}; // 0ms
    this->udpSocket->setReceiveTimeout(&timeout);
    if( (recvLength = this->udpSocket->receiveFrom(recvBuffer,recvLength, partnerAddress)) < 0){
        std::cerr<<"UDP Receive failed :"<<strerror(errno)<<std::endl;
        return -1;
    }
    SAWPacket packet;
    packet.deserialize(recvBuffer);
    if(packet.validate() && packet.seqNumber() == this->receiveSequenceNumber){
        SAWPacket ackPacket;
        if(packet.seqNumber())
            ackPacket.flags |= 0b01000000;
        this->udpSocket->sendTo(ackPacket.serialize(),ackPacket.size(), partnerAddress);
        memcpy(buffer, packet.payload, packet.payloadLength);
        this->receiveSequenceNumber = !this->receiveSequenceNumber;
        if(packet.isCloseSignal()){
            this->udpSocket->closeSocket();
            delete this->udpSocket;
            this->udpSocket = NULL;
        }
        return packet.payloadLength;
    }
    else{
        // wrong seq or invalid checksum
        return this->receive(buffer, maxLength, --retries);
    }
}

bool SAWSocket::isOpen() {
    return this->udpSocket != NULL;
}

void SAWSocket::close(int retries) {
    SAWPacket packet;
    packet.payloadLength = 0;
    packet.payload = NULL;
    if(sendSequenceNumber)
        packet.flags |= 0b10000000;

    packet.flags |= 0b00100000;

    if(this->partnerAddress == NULL){
        this->partnerAddress = new sockaddr_in;
    }

    char* recvBuffer = new char [UDP_MAX_PAYLOAD];
    struct timeval timeout={0,0}; // 50ms
    this->udpSocket->setReceiveTimeout(&timeout);
    int rVal = this->udpSocket->sendTo(packet.serialize(),packet.size(), this->partnerAddress);
    if(rVal!=packet.size()){
        std::cerr<<"UDP Send failed.\n";
        return ;
    }
    timeout={0,50000}; // 50ms
    this->udpSocket->setReceiveTimeout(&timeout);
    if(this->udpSocket->receiveFrom(recvBuffer, UDP_MAX_PAYLOAD, this->partnerAddress) < 0){
        // timeout or other error
        std::cerr<<"UDP Receive failed.\n";
        this->close(--retries);
        return ;
    }
    else{
        SAWPacket recvPacket;
        recvPacket.deserialize(recvBuffer);
        if(!recvPacket.validate() || recvPacket.ackNumber() != packet.seqNumber()){
            // corrupt ack
            this->close(--retries);
            return ;
        }
    }
    this->udpSocket->closeSocket();
    delete this->udpSocket;
    this->udpSocket = NULL;
}

SAWClient::SAWClient(std::string* serverIP, u_int16_t port) {
    this->udpSocket = new UDPSocket(UDPSocketType::ClientSocket, serverIP, port);
    this->partnerAddress = this->udpSocket->socketAddress;
}

SAWClient::~SAWClient() {
    if(this->udpSocket) {
        this->udpSocket->closeSocket();
        delete this->udpSocket;
    }
}

SAWServer::SAWServer(std::string *serverIP, u_int16_t port) {
    this->udpSocket = new UDPSocket(UDPSocketType::ServerSocket, serverIP, port);
    this->partnerAddress = NULL;
}

SAWServer::~SAWServer() {
    if(this->udpSocket) {
        this->udpSocket->closeSocket();
        delete this->udpSocket;
    }
}