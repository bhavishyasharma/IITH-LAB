//
// Created by Bhavishya Sharma on 30/10/20.
//

#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <unistd.h>
#include <sys/types.h>

#include "GBNFTPSocket.h"

int main(int argc, char **argv){

    // Default Server IP & Port
    std::string* serverIP = new std::string(DEFAULT_GBN_SERVER_IP);
    u_int16_t port = DEFAULT_GBN_SERVER_PORT;

    std::string* filename = new std::string ;

    // Parse Arguments
    int c;
    opterr = 0;
    while ((c = getopt (argc, argv, "s:p:f:")) != -1) {
        switch (c) {
            case 's':
                *serverIP = optarg;
                break;
            case 'f':
                *filename = optarg;
                break;
            case 'p':
                port = atoi(optarg);
                break;
            case '?':
                if (optopt == 's' || optopt == 'f' || optopt == 'p')
                    std::cerr << "Option -" << char(optopt) << " requires an argument.\n";
                else
                    std::cerr << "Unknown option -" << char(optopt) << ".\n";
                return 1;
            default:
                abort();
        }
    }
    if(filename->empty()){
        std::cout<<"Enter file name : ";
        std::cin>>*filename;
    }

    GBNFTPClient* gbnftpClient = new GBNFTPClient(serverIP, port);
    gbnftpClient->downloadFile(filename);
    return 0;
}
