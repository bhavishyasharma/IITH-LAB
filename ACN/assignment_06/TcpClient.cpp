/*
 * Created on Tue Nov 17 2020
 * Author: Bhavishya Sharma
 *
 * Copyright (c) 2020 IIT Hyderabad
 */

#include <iostream>
#include <string>
#include <sys/types.h>
#include <unistd.h>

#include "TcpSocket.h"

#ifndef DEFAULT_TCP_SERVER_IP
#define DEFAULT_TCP_SERVER_IP "127.0.0.1"
#endif

#ifndef DEFAULT_TCP_SERVER_PORT
#define DEFAULT_TCP_SERVER_PORT 9090
#endif

int main(int argc, char **argv){

    // Default Server IP & Port
    std::string* serverIP = new std::string(DEFAULT_TCP_SERVER_IP);
    u_int16_t port = DEFAULT_TCP_SERVER_PORT;

    // Parse Arguments
    int c;
    opterr = 0;
    while ((c = getopt (argc, argv, "s:p:")) != -1) {
        switch (c) {
            case 's':
                *serverIP = optarg;
                break;
            case 'p':
                port = atoi(optarg);
                break;
            case '?':
                if (optopt == 's' || optopt == 'p')
                    std::cerr << "Option -" << char(optopt) << " requires an argument.\n";
                else
                    std::cerr << "Unknown option -" << char(optopt) << ".\n";
                return 1;
            default:
                abort();
        }
    }
    TcpSocket* socket = new TcpSocket(TcpSocketMode::ClientSocket, serverIP, port);
    std::string buffer;
    std::stringstream* stream = new std::stringstream();
    while(std::getline(std::cin, buffer)){
        *stream<<buffer;
        socket->write(stream);
    }
    return 0;
}