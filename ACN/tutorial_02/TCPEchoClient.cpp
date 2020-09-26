//
// Created by Bhavishya Sharma on 26/09/20.
//

#include <iostream>
#include <string>
#include <cstdlib>
#include <unistd.h>
#include <sys/types.h>

#include "TCPEchoClient.h"

int main(int argc, char **argv){

    // Default Server IP & Port
    std::string serverIP = DEFAULT_ECHO_SERVER_IP;
    u_int16_t port = DEFAULT_ECHO_SERVER_PORT;

    // Parse Arguments
    int c;
    opterr = 0;
    while ((c = getopt (argc, argv, "s:p:")) != -1) {
        switch (c) {
            case 's':
                serverIP = optarg;
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

    TCPEchoClient *client = new TCPEchoClient(serverIP, port);
    client->start();

    return 0;
}

TCPEchoClient::TCPEchoClient(std::string serverIP, u_int16_t port) {
    this->init(serverIP, port);
}

TCPEchoClient::TCPEchoClient(u_int16_t port) {
    this->init(DEFAULT_ECHO_SERVER_IP, port);
}

TCPEchoClient::TCPEchoClient(std::string serverIP) {
    this->init(serverIP, DEFAULT_ECHO_SERVER_PORT);
}

TCPEchoClient::TCPEchoClient() {
    this->init(DEFAULT_ECHO_SERVER_IP, DEFAULT_ECHO_SERVER_PORT);
}

void TCPEchoClient::init(std::string serverIP, u_int16_t port) {
    this->serverAddress.sin_family = AF_INET;
    inet_aton(serverIP.c_str(), &(this->serverAddress.sin_addr));
    this->serverAddress.sin_port = htons(port);

    if((this->socketFD = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
        std::cerr<<"Failed: Error creating socket.";
        throw std::runtime_error("Failed: Error creating socket.");
    }
}

void TCPEchoClient::start() {
    if (connect(this->socketFD, (struct  sockaddr *) &(this->serverAddress), sizeof(this->serverAddress)) < 0) {
        std::cerr<<"Failed: Error connecting to server.";
        throw std::runtime_error("Failed: Error connecting to server.");
    }

    char buffer[ECHO_BUFFER_SIZE];
    int messageSize;

    std::cout<<"Connected to Echo Server "<< inet_ntoa(this->serverAddress.sin_addr)<< ":" << ntohs(this->serverAddress.sin_port) << std::endl;
    do{
        std::cin>>buffer;
        messageSize = strlen(buffer);
        if (send(this->socketFD, buffer, messageSize, 0) != messageSize) {
            std::cerr<<"Failed: Error sending message to server.";
            throw std::runtime_error("Failed: Error sending message to server.");
        }

        memset(buffer, 0, sizeof(buffer));

        if ((messageSize = recv(this->socketFD, buffer, ECHO_BUFFER_SIZE, 0)) < 0) {
            std::cerr<<"Failed: Error receiving message from server.";
            throw std::runtime_error("Failed: Error receiving message from server.");
        }
        else{
            std::cout<<buffer<<std::endl;
        }
    } while (strcmp(buffer, "bye") != 0);
}