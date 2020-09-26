//
// Created by Bhavishya Sharma on 26/09/20.
//

#ifndef IITH_LAB_UDPECHOCLIENT_H
#define IITH_LAB_UDPECHOCLIENT_H

#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/types.h>

#ifndef DEFAULT_ECHO_SERVER_IP
#define DEFAULT_ECHO_SERVER_IP "127.0.0.1"
#endif

#ifndef DEFAULT_ECHO_SERVER_PORT
#define DEFAULT_ECHO_SERVER_PORT 7
#endif

#ifndef ECHO_BUFFER_SIZE
#define ECHO_BUFFER_SIZE 1024
#endif

class UDPEchoClient {

private:
    struct sockaddr_in serverAddress;
    int socketFD;
    unsigned int serverLen;

public:
    UDPEchoClient();
    UDPEchoClient(std::string serverIP);
    UDPEchoClient(u_int16_t port);
    UDPEchoClient(std::string serverIP, u_int16_t port);
    void init(std::string serverIP, u_int16_t port);
    void start();

};


#endif //IITH_LAB_UDPECHOCLIENT_H
