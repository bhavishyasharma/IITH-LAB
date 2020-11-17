/*
 * Created on Tue Nov 17 2020
 * Author: Bhavishya Sharma
 *
 * Copyright (c) 2020 IIT Hyderabad
 */

#ifndef IITH_LAB_PNAT_H
#define IITH_LAB_PNAT_H

#include <arpa/inet.h>
#include <bitset>
#include <cstring>
#include <errno.h>
#include <iomanip>
#include <iostream>
#include <netinet/tcp.h>
#include <netinet/ip.h>
#include <netinet/udp.h>
#include <pthread.h>
#include <sstream>
#include <string>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include "RawSocket.h"

class PNat {
    RawSocket* serverSocket;
public:
    PNat(std::string* serverIP, u_int16_t port);
    void startServer();
};

#endif