//
// Created by Bhavishya Sharma on 30/10/20.
//

#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <unistd.h>
#include <sys/types.h>
#include <chrono>

#include "SAWSocket.h"

int main(int argc, char **argv){

    // Default Server IP & Port
    std::string* serverIP = new std::string(DEFAULT_SAW_SERVER_IP);
    u_int16_t port = DEFAULT_SAW_SERVER_PORT;

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

    std::ofstream downloadedFile;
    downloadedFile.open("downloaded-" + *filename, std::ios::out | std::ios::trunc | std::ios::binary);
    if (downloadedFile.is_open()) {
        // Create Client
        SAWClient *sawSocket = new SAWClient(serverIP, port);
        sawSocket->send(const_cast<char *>(filename->c_str()), filename->size());
        char* buffer = new char[SAW_MAX_PAYLOAD];
        int lineSize = 0;
        std::chrono::steady_clock::time_point startTime = std::chrono::steady_clock::now();
        while(sawSocket->isOpen()) {
            int recvCount = sawSocket->receive(buffer, SAW_MAX_PAYLOAD);
            if ( recvCount < 0) {
                std::cerr << "SAW Receive failed.\n";
            } else {
                downloadedFile.write(buffer, recvCount);
                memset(buffer, 0, SAW_MAX_PAYLOAD);
            }
        }
        std::chrono::steady_clock::time_point endTime = std::chrono::steady_clock::now();
        std::cout<<"Download Time (ms): "<<std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count();
        downloadedFile.close();
    }
    else{
        std::cerr<<"Error opening file.\n";
    }
    return 0;
}
