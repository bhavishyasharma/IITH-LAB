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

#include "PNat.h"

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
    while ((c = getopt (argc, argv, "i:p:")) != -1) {
        switch (c) {
            case 'i':
                *serverIP = optarg;
                break;
            case 'p':
                port = atoi(optarg);
                break;
            case '?':
                if (optopt == 'i' || optopt == 'p')
                    std::cerr << "Option -" << char(optopt) << " requires an argument.\n";
                else
                    std::cerr << "Unknown option -" << char(optopt) << ".\n";
                return 1;
            default:
                abort();
        }
    }
    PNat *pNat = new PNat(serverIP, port);
    pNat->startServer();
    return 0;
}