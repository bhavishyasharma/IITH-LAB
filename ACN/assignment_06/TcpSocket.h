/*
 * Created on Tue Nov 17 2020
 * Author: Bhavishya Sharma
 *
 * Copyright (c) 2020 IIT Hyderabad
 */

#ifndef IITH_LAB_TCP_SOCKET_H
#define IITH_LAB_TCP_SOCKET_H

#include <arpa/inet.h>
#include <cstring>
#include <errno.h>
#include <iostream>
#include <netinet/tcp.h>
#include <netinet/ip.h>
#include <netinet/udp.h>
#include <sstream>
#include <string>
#include <sys/socket.h>
#include <sys/types.h>

#ifndef MAX_CONNECTION_REQUEST
#define MAX_CONNECTION_REQUEST 32
#endif

#ifndef MAX_TCP_PAYLOAD
#define MAX_TCP_PAYLOAD 65535
#endif

enum class TcpSocketMode { ClientSocket, ServerSocket };

class TcpSocket {
    int socketFD;
    struct sockaddr_in* socketAddress;
    TcpSocketMode socketMode;
public:
    TcpSocket(TcpSocketMode socketMode, std::string* serverIP, u_int16_t port);
    int acceptConnection(struct sockaddr_in* clientAddress);
    int write(std::stringstream* stream);
    int read(std::stringstream* stream);
};

#endif