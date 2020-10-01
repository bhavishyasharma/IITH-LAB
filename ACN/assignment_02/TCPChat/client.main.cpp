//
// Created by Bhavishya Sharma on 30/09/20.
//

#include <iostream>
#include <string>
#include <cstdlib>
#include <unistd.h>
#include <sys/types.h>

#include "ChatClient.h"
#include "ChatCommon.hpp"

int main(int argc, char **argv){

    // Default Server IP & Port
    std::string serverIP = DEFAULT_CHAT_SERVER_IP;
    u_int16_t port = DEFAULT_CHAT_SERVER_PORT;
    std::string username;

    // Parse Arguments
    int c;
    opterr = 0;
    while ((c = getopt (argc, argv, "s:p:u:")) != -1) {
        switch (c) {
            case 's':
                serverIP = optarg;
                break;
            case 'p':
                port = atoi(optarg);
                break;
            case 'u':
                username = optarg;
                break;
            case '?':
                if (optopt == 's' || optopt == 'p' || optopt == 'u')
                    std::cerr << "Option -" << char(optopt) << " requires an argument.\n";
                else
                    std::cerr << "Unknown option -" << char(optopt) << ".\n";
                return 1;
            default:
                abort();
        }
    }
    if(username.length()==0){
        std::cout<<"Username -u is required\n";
        exit(1);
    }
    // Create Client
    ChatClient *client = new ChatClient(username, serverIP, port);

    // Start Client
    client->start();

    return 0;
}
