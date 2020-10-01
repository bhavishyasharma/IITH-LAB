//
// Created by Bhavishya Sharma on 30/09/20.
//

#include <iostream>
#include <string.h>
#include <cstdlib>
#include <unistd.h>
#include <sys/types.h>

#include "ChatServer.h"

int main(int argc, char **argv){

    // Default Server IP & Port
    std::string serverIP = DEFAULT_CHAT_SERVER_IP;
    u_int16_t port = DEFAULT_CHAT_SERVER_PORT;
    int clientThreadCount = MAX_CLIENT_THREADS;

    // Parse Arguments
    int c;
    opterr = 0;
    while ((c = getopt (argc, argv, "i:p:n:")) != -1) {
        switch (c) {
            case 'i':
                serverIP = optarg;
                break;
            case 'p':
                port = atoi(optarg);
                break;
            case 'n':
                clientThreadCount = atoi(optarg);
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

    // Create Server
    ChatServer *server = new ChatServer(serverIP, port, clientThreadCount);

    // Start Server
    server->startServer();

    return 0;
}