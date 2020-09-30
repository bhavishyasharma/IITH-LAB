//
// Created by Bhavishya Sharma on 26/09/20.
//

#ifndef IITH_LAB_DNSCLIENT_H
#define IITH_LAB_DNSCLIENT_H

#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/types.h>

#include "QType.hpp"
#include "QClass.hpp"

#ifndef DEFAULT_DNS_SERVER_IP
#define DEFAULT_DNS_SERVER_IP "8.8.8.8"
#endif

#ifndef DEFAULT_DNS_SERVER_PORT
#define DEFAULT_DNS_SERVER_PORT 53
#endif

#ifndef DEFAULT_DOMAIN_NAME
#define DEFAULT_DOMAIN_NAME "www.google.com"
#endif

#ifndef DEFAULT_QCLASS
#define DEFAULT_QCLASS QC_IN
#endif

#ifndef DEFAULT_QTYPE
#define DEFAULT_QTYPE QT_A
#endif

#ifndef DNS_BUFFER_SIZE
#define DNS_BUFFER_SIZE 512
#endif

class DNSClient {
private:
    struct sockaddr_in serverAddress;
    int socketFD;
    unsigned int serverLen;
    void init(std::string serverIP, u_int16_t port);

public:
    DNSClient();
    DNSClient(std::string serverIP);
    DNSClient(u_int16_t port);
    DNSClient(std::string serverIP, u_int16_t port);
    void query(std::string domainName);
    void query(std::string domainName, QType qType, QClass qClass);
};


#endif //IITH_LAB_DNSCLIENT_H
