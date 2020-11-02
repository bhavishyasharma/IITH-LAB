//
// Created by Bhavishya Sharma on 31/10/20.
//

#ifndef IITH_LAB_GBNFTPSOCKET_H
#define IITH_LAB_GBNFTPSOCKET_H

#include <iostream>
#include <fstream>
#include <list>
#include <math.h>
#include <chrono>
#include <string>
#include <sys/types.h>

#include "GBNFTPPacket.h"
#include "UDPSocket.h"

#ifndef DEFAULT_GBN_SERVER_IP
#define DEFAULT_GBN_SERVER_IP "127.0.0.1"
#endif

#ifndef DEFAULT_GBN_SERVER_PORT
#define DEFAULT_GBN_SERVER_PORT 9090
#endif

#ifndef DEFAULT_FTP_WINDOW_SIZE
#define DEFAULT_FTP_WINDOW_SIZE 32768
#endif

#ifndef DEFAULT_FTP_CHUNK_SIZE
#define DEFAULT_FTP_CHUNK_SIZE 4096
#endif

#ifndef DEFAULT_FTP_TIMEOUT
#define DEFAULT_FTP_TIMEOUT 500  // microseconds
#endif

class GBNFTPSocket {
public:
    UDPSocket* udpSocket;
    struct sockaddr_in* partnerAddress;

    u_int16_t seqNumber;
    u_int16_t partnerSeqNumber;

};

class GBNFTPClient : public GBNFTPSocket {
public:
    GBNFTPClient(std::string* serverIP, u_int16_t port);
    void downloadFile(std::string* filename);
};

class GBNFTPServer : public GBNFTPSocket{
public:
    GBNFTPServer(std::string* serverIP, u_int16_t port);
    void listen();
    void sendFile(std::string* filename);
};


#endif //IITH_LAB_GBNFTPSOCKET_H
