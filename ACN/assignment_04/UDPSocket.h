//
// Created by Bhavishya Sharma on 29/10/20.
//

#ifndef IITH_LAB_UDPSOCKET_H
#define IITH_LAB_UDPSOCKET_H

#include <iostream>
#include <string>
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/time.h>
#include <unistd.h>
#include <errno.h>

#ifndef UDP_MAX_PAYLOAD
#define UDP_MAX_PAYLOAD 65507
#endif

enum class UDPSocketType{ ClientSocket, ServerSocket };

class UDPSocket {
public:
    int socketFD;
    UDPSocketType socketType;
    struct sockaddr_in* socketAddress;

    void initServer(std::string* serverIP, u_int16_t port);
    void initClient(std::string* serverIP, u_int16_t port);

    UDPSocket(UDPSocketType socketType, std::string* serverIP, u_int16_t port);
    ~UDPSocket();
    void closeSocket();
    int sendTo(char* buffer, int length, struct sockaddr_in* partnerAddress);
    int receiveFrom(char* buffer, int maxLength, struct sockaddr_in* partnerAddress);
    void setReceiveTimeout(struct timeval* timeout);
};




#endif //IITH_LAB_UDPSOCKET_H
