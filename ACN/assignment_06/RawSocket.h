/*
 * Created on Mon Nov 16 2020
 * Author: Bhavishya Sharma
 *
 * Copyright (c) 2020 IIT Hyderabad
 */

#ifndef IITH_LAB_RAW_SOCKET_H
#define IITH_LAB_RAW_SOCKET_H

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

#ifndef MTU
#define MTU 1500
#endif

#ifndef IP_HEADER_LENGTH
#define IP_HEADER_LENGTH 20
#endif

#ifndef TCP_HEADER_LENGTH
#define TCP_HEADER_LENGTH 20
#endif

#ifndef UDP_HEADER_LENGTH
#define UDP_HEADER_LENGTH 8
#endif

enum class RawSocketMode{ ClientSocket, ServerSocket };

class RawSocket {
    int socketFD;
    struct sockaddr_in* socketAddress;
    RawSocketMode socketMode;
public:
    RawSocket(RawSocketMode socketMode, std::string* serverIP, u_int16_t port);
    int write(std::stringstream* stream, struct sockaddr_in* remoteAddress = NULL);
    int read(std::stringstream* stream, struct sockaddr_in* remoteAddress = NULL);
};

#endif