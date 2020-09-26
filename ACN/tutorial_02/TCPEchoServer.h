//
// Created by Bhavishya Sharma on 26/09/20.
//

#ifndef IITH_LAB_TCP_ECHO_SERVER_H
#define IITH_LAB_TCP_ECHO_SERVER_H

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

#ifndef MAX_CONNECTION_REQUEST
#define MAX_CONNECTION_REQUEST 32
#endif

#ifndef ECHO_BUFFER_SIZE
#define ECHO_BUFFER_SIZE 1024
#endif

class TCPEchoServer {

private:
    int serverSocketFD;
    struct sockaddr_in serverAddress;
    int clientSocketFD;
    struct sockaddr_in clientAddress;
    unsigned int clientLen;

public:
    TCPEchoServer();
    TCPEchoServer(std::string serverIP);
    TCPEchoServer(u_int16_t port);
    TCPEchoServer(std::string serverIP, u_int16_t port);
    void init(std::string serverIP, u_int16_t port);
    void startServer();
    void serveClient();
};

#endif //IITH_LAB_TCP_ECHO_SERVER_H
