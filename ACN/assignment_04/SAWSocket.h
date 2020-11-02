//
// Created by Bhavishya Sharma on 29/10/20.
//

#ifndef IITH_LAB_SAWSOCKET_H
#define IITH_LAB_SAWSOCKET_H

#include <list>
#include <string>
#include <iomanip>
#include <bitset>

#include "UDPSocket.h"
#include "SAWPacket.h"

#ifndef DEFAULT_SAW_SERVER_IP
#define DEFAULT_SAW_SERVER_IP "127.0.0.1"
#endif

#ifndef DEFAULT_SAW_SERVER_PORT
#define DEFAULT_SAW_SERVER_PORT 9090
#endif

#ifndef SAW_MAX_RETRY
#define SAW_MAX_RETRY 16
#endif

class SAWSocket {
public:
    UDPSocket* udpSocket;
    struct sockaddr_in* partnerAddress;
    bool sendSequenceNumber = false;
    bool receiveSequenceNumber = false;
    int send(char* buffer, int length, int retries = SAW_MAX_RETRY);
    int receive(char* buffer, int maxLength, int retries = SAW_MAX_RETRY);
    bool isOpen();
    void close(int retries = SAW_MAX_RETRY);
};

class SAWClient : public SAWSocket{
public:
    SAWClient( std::string* serverIP, u_int16_t port);
    ~SAWClient();
};

class SAWServer : public SAWSocket{
public:
    SAWServer( std::string* serverIP, u_int16_t port);
    ~SAWServer();
};


#endif //IITH_LAB_SAWSOCKET_H
