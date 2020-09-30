//
// Created by Bhavishya Sharma on 28/09/20.
//

#include <string>
#include <iostream>
#include <iomanip>
#include <getopt.h>

#include "DNSClient.h"
#include "DNSMessage.h"
#include "QClass.hpp"
#include "QType.hpp"

int main(int argc, char **argv){

    // Default Server IP & Port
    std::string serverIP = DEFAULT_DNS_SERVER_IP;
    u_int16_t port = DEFAULT_DNS_SERVER_PORT;
    std::string domainName = DEFAULT_DOMAIN_NAME;
    QClass qClass = DEFAULT_QCLASS;
    QType qType = DEFAULT_QTYPE;

    // Parse Arguments
    int c;
    opterr = 0;
    while ((c = getopt (argc, argv, "s:p:d:c:t:")) != -1) {
        switch (c) {
            case 's':
                serverIP = optarg;
                break;
            case 'p':
                port = atoi(optarg);
                break;
            case 'd':
                domainName = optarg;
                break;
            case 'c':
                qClass = QClass(qClassReverseMap.find(optarg)->second);
                break;
            case 't':
                qType = QType(qTypeReverseMap.find(optarg)->second);
                break;
            case '?':
                if (optopt == 's' || optopt == 'p' || optopt =='d' || optopt == 'c' || optopt == 't')
                    std::cerr << "Option -" << char(optopt) << " requires an argument.\n";
                else
                    std::cerr << "Unknown option -" << char(optopt) << ".\n";
                return 1;
            default:
                abort();
        }
    }
    DNSClient *client = new DNSClient(serverIP, port);
    client->query(domainName, qType, qClass);

    return 0;
}
