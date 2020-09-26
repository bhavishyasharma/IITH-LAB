//
// Created by Bhavishya Sharma on 26/09/20.
//

#include <iostream>
#include <string>
#include <cstdlib>
#include <unistd.h>
#include <sys/types.h>

#include "UDPEchoClient.h"

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

    UDPEchoClient *client = new UDPEchoClient(serverIP, port);
    client->start();

    return 0;
}

UDPEchoClient::UDPEchoClient(std::string serverIP, u_int16_t port) {
    this->init(serverIP, port);
}

UDPEchoClient::UDPEchoClient(u_int16_t port) {
    this->init(DEFAULT_ECHO_SERVER_IP, port);
}

UDPEchoClient::UDPEchoClient(std::string serverIP) {
    this->init(serverIP, DEFAULT_ECHO_SERVER_PORT);
}

UDPEchoClient::UDPEchoClient() {
    this->init(DEFAULT_ECHO_SERVER_IP, DEFAULT_ECHO_SERVER_PORT);
}

void UDPEchoClient::init(std::string serverIP, u_int16_t port) {
    this->serverAddress.sin_family = AF_INET;
    inet_aton(serverIP.c_str(), &(this->serverAddress.sin_addr));
    this->serverAddress.sin_port = htons(port);

    if((this->socketFD = socket(PF_INET, SOCK_DGRAM, 0)) < 0) {
        std::cerr<<"Failed: Error creating socket.";
        throw std::runtime_error("Failed: Error creating socket.");
    }
}

void UDPEchoClient::start() {
    char buffer[ECHO_BUFFER_SIZE];
    int messageSize;

    do{
        std::cin>>buffer;
        messageSize = strlen(buffer);
        if (sendto(this->socketFD, buffer, messageSize, 0,( struct sockaddr *) &(this->serverAddress), sizeof(this->serverAddress)) != messageSize) {
            std::cerr<<"Failed: Error sending message to server.";
            throw std::runtime_error("Failed: Error sending message to server.");
        }

        memset(buffer, 0, sizeof(buffer));
        this->serverLen = sizeof(this->serverAddress);
        if ((messageSize = recvfrom(this->socketFD, buffer, ECHO_BUFFER_SIZE, 0, ( struct sockaddr *) &(this->serverAddress), &(this->serverLen))) < 0) {
            std::cerr<<"Failed: Error receiving message from server.";
            throw std::runtime_error("Failed: Error receiving message from server.");
        }
        else{
            std::cout<<buffer<<std::endl;
        }
    } while (strcmp(buffer, "bye") != 0);
}