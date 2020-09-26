//
// Created by Bhavishya Sharma on 26/09/20.
//

#include <iostream>
#include <string>
#include <cstdlib>
#include <unistd.h>
#include <sys/types.h>

#include "UDPEchoServer.h"

int main(int argc, char **argv){

    // Default Server IP & Port
    std::string serverIP = DEFAULT_ECHO_SERVER_IP;
    u_int16_t port = DEFAULT_ECHO_SERVER_PORT;

    // Parse Arguments
    int c;
    opterr = 0;
    while ((c = getopt (argc, argv, "i:p:")) != -1) {
        switch (c) {
            case 'i':
                serverIP = optarg;
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

    UDPEchoServer *server = new UDPEchoServer(serverIP, port);
    server->startServer();

    return 0;
}

UDPEchoServer::UDPEchoServer(std::string serverIP, u_int16_t port) {
    this->init(serverIP, port);
}

UDPEchoServer::UDPEchoServer(u_int16_t port) {
    this->init(DEFAULT_ECHO_SERVER_IP, port);
}

UDPEchoServer::UDPEchoServer(std::string serverIP) {
    this->init(serverIP, DEFAULT_ECHO_SERVER_PORT);
}

UDPEchoServer::UDPEchoServer() {
    this->init(DEFAULT_ECHO_SERVER_IP, DEFAULT_ECHO_SERVER_PORT);
}

void UDPEchoServer::init(std::string serverIP, u_int16_t port) {
    if((this->serverSocketFD = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0) {
        std::cerr<<"Failed: Error creating socket.";
        throw std::runtime_error("Failed: Error creating socket.");
    }
    this->serverAddress.sin_family = AF_INET;
    inet_aton(serverIP.c_str(), &(this->serverAddress.sin_addr));
    this->serverAddress.sin_port = htons(port);

    if( bind(this->serverSocketFD, (struct sockaddr *) &(this->serverAddress), sizeof(this->serverAddress)) < 0 ) {
        std::cerr<<"Failed: Error binding socket.";
        throw std::runtime_error("Failed: Error binding socket.");
    }
}

void UDPEchoServer::startServer() {
    char buffer[ECHO_BUFFER_SIZE];
    int messageSize;
    while(1){
        this->clientLen = sizeof(this->clientAddress);
        if ((messageSize = recvfrom(this->serverSocketFD, buffer, ECHO_BUFFER_SIZE, 0, ( struct sockaddr *) &(this->clientAddress), &(this->clientLen))) < 0) {
            std::cerr<<"Failed: Error receiving from client.";
            throw std::runtime_error("Failed: Error receiving from client.");
        }
        else {
            std::cout<<"Received from client "<<inet_ntoa(this->clientAddress.sin_addr)<<":"<<ntohs(this->clientAddress.sin_port)<<" -> "<<buffer<<std::endl;
        }

        if (sendto(this->serverSocketFD, buffer, messageSize, 0,( struct sockaddr *) &(this->clientAddress), this->clientLen) != messageSize) {
            std::cerr<<"Failed: Error sending message back to client.";
            throw std::runtime_error("Failed: Error sending message back to client.");
        }

        memset(buffer, 0, sizeof(buffer));
    }
}