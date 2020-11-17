/*
 * Created on Tue Nov 17 2020
 * Author: Bhavishya Sharma
 *
 * Copyright (c) 2020 IIT Hyderabad
 */

#include "PNat.h"

PNat::PNat(std::string* serverIP, u_int16_t port){
    this->serverSocket = new RawSocket(RawSocketMode::ServerSocket, serverIP, port);
}

void PNat::startServer(){
    int clientSocketFD;
    struct sockaddr_in *clientAddress = new sockaddr_in();
    unsigned int clientLength;
    std::stringstream* stream = new std::stringstream();
    char* buffer = new char[MTU];
    std::cout<<"Server Started"<<std::endl;
    while(1){
        int length = this->serverSocket->read(stream, clientAddress);
        *stream>>buffer;
        std::cout<<"Read : "<<length<<std::endl;
        struct iphdr *iph = new iphdr();
        memcpy(iph, buffer, 20);
        struct tcphdr* tcph = new tcphdr();
        memcpy(tcph, buffer + 20, 20);
        std::cout<<"IP Version "<<iph->version<<std::endl;
        std::cout<<"IP IHL "<<iph->ihl<<std::endl;
        std::cout<<"IP TOS "<<iph->tos<<std::endl;
        std::cout<<"IP Total Length "<<iph->tot_len<<std::endl;
        std::cout<<"IP Identification "<<iph->id<<std::endl;
        std::cout<<"IP Frag Offset "<<iph->frag_off<<std::endl;
        std::cout<<"IP TTL "<<u_int8_t(iph->ttl)<<std::endl;
        std::cout<<"IP Protocol "<<u_int8_t(iph->protocol)<<std::endl;
        std::cout<<"IP Checksum "<<u_int16_t(iph->check)<<std::endl;
        char *temp = new char [17];
        inet_ntop(AF_INET, &iph->saddr, temp, 16);
        std::cout<<"IP Source "<<temp<<std::endl;
        inet_ntop(AF_INET, &iph->daddr, temp, 16);
        std::cout<<"IP Destination "<<temp<<std::endl;
        /**for(int i=0;i<length;i++){
            std::cout<<std::bitset<8>(buffer[i])<<" ";
        }
        std::cout<<std::endl;*/
        memset(buffer, 0, MTU);
        sleep(1);
    }
}