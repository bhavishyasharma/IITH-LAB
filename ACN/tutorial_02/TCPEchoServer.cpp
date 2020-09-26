//
// Created by Bhavishya Sharma on 26/09/20.
//

#include <iostream>
#include <string>
#include <cstdlib>
#include <unistd.h>
#include <sys/types.h>

#include "TCPEchoServer.h"

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

    TCPEchoServer *server = new TCPEchoServer(serverIP, port);
    server->startServer();

    return 0;
}

TCPEchoServer::TCPEchoServer(std::string serverIP, u_int16_t port) {
    this->init(serverIP, port);
}

TCPEchoServer::TCPEchoServer(u_int16_t port) {
    this->init(DEFAULT_ECHO_SERVER_IP, port);
}

TCPEchoServer::TCPEchoServer(std::string serverIP) {
    this->init(serverIP, DEFAULT_ECHO_SERVER_PORT);
}

TCPEchoServer::TCPEchoServer() {
    this->init(DEFAULT_ECHO_SERVER_IP, DEFAULT_ECHO_SERVER_PORT);
}

void TCPEchoServer::init(std::string serverIP, u_int16_t port) {
    if((this->serverSocketFD = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
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

void TCPEchoServer::startServer() {
    if( listen(serverSocketFD, MAX_CONNECTION_REQUEST) < 0 ) {
        std::cerr<<"Failed: Error listening on socket.";
        throw std::runtime_error("Failed: Error listening on socket.");
    }
    std::cout<<"Listening on "<<inet_ntoa(this->serverAddress.sin_addr)<<":"<<ntohs(this->serverAddress.sin_port)<<std::endl;
    while(1) {
        // Accept client connection
        std::cout<<"Waiting for client connection....\n";
        this->clientLen = sizeof(clientAddress);
        if((this->clientSocketFD = accept(this->serverSocketFD, (struct sockaddr *) &(this->clientAddress), &clientLen)) < 0) {
            std::cerr<<"Failed: Error accepting connection.";
            throw std::runtime_error("Failed: Error accepting connection.");
        }

        // Serve Client
        std::cout<<"Serving Client : "<< inet_ntoa(this->clientAddress.sin_addr) << std::endl;
        this->serveClient();
        std::cout<<"Finished Client : "<< inet_ntoa(this->clientAddress.sin_addr) << std::endl;
    }
}

void TCPEchoServer::serveClient() {
    char buffer[ECHO_BUFFER_SIZE];
    int messageSize;

    // Receive data from client
    if( (messageSize = recv(this->clientSocketFD, buffer, ECHO_BUFFER_SIZE, 0)) < 0) {
        std::cerr<<"Failed: Error receiving from client.";
        throw std::runtime_error("Failed: Error receiving from client.");
    }

    while(messageSize > 0) {
        if (send(this->clientSocketFD, buffer, messageSize, 0) != messageSize) {
            std::cerr<<"Failed: Error sending message back to client.";
            throw std::runtime_error("Failed: Error sending message back to client.");
        }

        memset(buffer, 0, sizeof(buffer));

        if ((messageSize = recv(this->clientSocketFD, buffer, ECHO_BUFFER_SIZE, 0)) < 0) {
            std::cerr<<"Failed: Error receiving from client.";
            throw std::runtime_error("Failed: Error receiving from client.");
        }
    }

    // Close client connection
    close(this->clientSocketFD);
}