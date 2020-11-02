//
// Created by Bhavishya Sharma on 30/10/20.
//

#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <unistd.h>
#include <sys/types.h>

#include "SAWSocket.h"

int main(int argc, char **argv){

    // Default Server IP & Port
    std::string* serverIP = new std::string(DEFAULT_SAW_SERVER_IP);
    u_int16_t port = DEFAULT_SAW_SERVER_PORT;

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
    // Create Client
    SAWServer *sawSocket = new SAWServer(serverIP, port);
    char* buffer = new char[SAW_MAX_PAYLOAD];
    sawSocket->receive(buffer, SAW_MAX_PAYLOAD);

    std::ifstream file;
    file.open(buffer, std::ios::in | std::ios::binary);

    memset(buffer, 0, SAW_MAX_PAYLOAD);

    if(file.is_open()){
        while(!file.eof()){
            file.read(buffer, SAW_MAX_PAYLOAD);
            sawSocket->send(buffer, file.gcount());
            memset(buffer, 0, SAW_MAX_PAYLOAD);
        }
    }
    else{
        char error[] = "Server Error: cannot opening file.";
        sawSocket->send(error,35);
    }

    sawSocket->close();

    return 0;
}
