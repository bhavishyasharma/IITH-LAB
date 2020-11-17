/*
 * Created on Tue Nov 17 2020
 * Author: Bhavishya Sharma
 *
 * Copyright (c) 2020 IIT Hyderabad
 */

#ifndef IITH_LAB_UDP_SOCKET_H
#define IITH_LAB_UDP_SOCKET_H

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

#ifndef MAX_UDP_PAYLOAD
#define MAX_UDP_PAYLOAD 65535
#endif

enum class UdpSocketMode { ClientSocket, ServerSocket };

class UdpSocket {
    int socketFD;
    struct sockaddr_in* socketAddress;
    UdpSocketMode socketMode;
public:
    UdpSocket(UdpSocketMode socketMode, std::string* serverIP, u_int16_t port);
    int write(std::stringstream* stream, struct sockaddr_in* remoteAddress = NULL);
    int read(std::stringstream* stream, struct sockaddr_in* remoteAddress = NULL);
};

#endif