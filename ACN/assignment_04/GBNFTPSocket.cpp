//
// Created by Bhavishya Sharma on 31/10/20.
//

#include "GBNFTPSocket.h"

GBNFTPClient::GBNFTPClient(std::string *serverIP, u_int16_t port) {
    this->udpSocket = new UDPSocket(UDPSocketType::ClientSocket, serverIP, port);
    this->partnerAddress = this->udpSocket->socketAddress;
    srand(100);
    this->seqNumber = rand() % 65536;
}

void GBNFTPClient::downloadFile(std::string* filename) {
    GBNFTPRequest request;
    request.seqNumber = this->seqNumber;
    request.ackNumber = this->seqNumber;
    request.payload = const_cast<char*>(filename->c_str());
    struct timeval timeout={0,0}; // 0ms
    this->udpSocket->setReceiveTimeout(&timeout);
    this->udpSocket->sendTo(request.serialize(), request.size(), this->partnerAddress);

    char *recvBuffer = new char[UDP_MAX_PAYLOAD];
    int recvLength = UDP_MAX_PAYLOAD;
    if ((recvLength = this->udpSocket->receiveFrom(recvBuffer, recvLength, partnerAddress)) < 0) {
        std::cerr << "UDP Receive failed :" << strerror(errno) << std::endl;
        return;
    }
    if((recvBuffer[0] & 0x40) == 0){
        std::cerr << "Invalid FTP response" << std::endl;
        return;
    }
    GBNFTPResponse response;
    response.deserialize(recvBuffer);
    this->partnerSeqNumber = response.seqNumber;

    std::ofstream downloadedFile;
    downloadedFile.open("downloaded-" + *filename, std::ios::out | std::ios::trunc | std::ios::binary);

    timeout={0,1000000}; // 50ms
    this->udpSocket->setReceiveTimeout(&timeout);
    unsigned int bytesReceived = 0;
    while(bytesReceived < response.fileSize){
        memset(recvBuffer, 0, UDP_MAX_PAYLOAD);
        if ((recvLength = this->udpSocket->receiveFrom(recvBuffer, UDP_MAX_PAYLOAD, partnerAddress)) < 0) {
            std::cerr << "UDP Receive failed :" << strerror(errno) << std::endl;
            continue;
        }
        if((recvBuffer[0] & 0x20) == 0){
            continue;
        }
        GBNFTPData data;
        data.deserialize(recvBuffer);
        if(data.checksum != 0xaa || data.seqNumber != (this->partnerSeqNumber + bytesReceived)%65536){
            // corrupt or out of order
            continue;
        }
        bytesReceived += data.payloadLength;
        downloadedFile.write(data.payload, data.payloadLength);
        GBNFTPAck ack;
        ack.expSeqNumber = data.seqNumber + data.payloadLength;
        ack.ackNumber = this->partnerSeqNumber;
        this->udpSocket->sendTo(ack.serialize(), ack.size(), this->partnerAddress);
    }
}

GBNFTPServer::GBNFTPServer(std::string *serverIP, u_int16_t port) {
    this->udpSocket = new UDPSocket(UDPSocketType::ServerSocket, serverIP, port);
    this->partnerAddress = NULL;
    srand(101);
    this->seqNumber = rand() % 65536;
}

void GBNFTPServer::listen() {
    while(1) {
        char *recvBuffer = new char[UDP_MAX_PAYLOAD];
        int recvLength = UDP_MAX_PAYLOAD;
        if(this->partnerAddress == NULL)
            this->partnerAddress = new sockaddr_in;
        this->seqNumber = rand() % 65536;
        struct timeval timeout={0,0}; // 0ms
        this->udpSocket->setReceiveTimeout(&timeout);
        if ((recvLength = this->udpSocket->receiveFrom(recvBuffer, recvLength, partnerAddress)) < 0) {
            std::cerr << "UDP Receive failed :" << strerror(errno) << std::endl;
            continue;
        }
        // not a request - discard
        if( (recvBuffer[0] & 0x80) == 0)
            continue;

        GBNFTPRequest request;
        request.deserialize(recvBuffer);
        this->partnerSeqNumber = request.seqNumber;
        std::string* filename = new std::string (request.payload);
        this->sendFile(filename);
        if(this->partnerAddress != NULL) {
            delete this->partnerAddress;
            this->partnerAddress = NULL;
        }
    }
}

void GBNFTPServer::sendFile(std::string* filename) {
    GBNFTPResponse response;
    response.seqNumber = this->seqNumber;
    response.windowSize = DEFAULT_FTP_WINDOW_SIZE;
    std::ifstream file;
    file.open(*filename, std::ios::in | std::ios::binary);
    file.seekg (0, file.end);
    response.fileSize = file.tellg();
    file.seekg (0, file.beg);
    this->udpSocket->sendTo(response.serialize(), response.size(), this->partnerAddress);
    char *buffer = new char [DEFAULT_FTP_CHUNK_SIZE];
    unsigned int base = 0, inFlight = 0;
    unsigned int lastAck = this->seqNumber;
    std::list<std::chrono::steady_clock::time_point> timers;

    struct timeval timeout={0,10000}; // 50ms
    this->udpSocket->setReceiveTimeout(&timeout);

    while(!file.eof()){
        file.seekg( base + inFlight, file.beg);
        file.read(buffer, DEFAULT_FTP_CHUNK_SIZE);
        GBNFTPData data;
        data.seqNumber = this->seqNumber + (base + inFlight)%65536;
        data.ackNumber = this->partnerSeqNumber;
        data.payloadLength = file.gcount();
        data.payload = buffer;
        timers.push_back(std::chrono::steady_clock::now());
        this->udpSocket->sendTo(data.serialize(), data.size(), this->partnerAddress);
        inFlight += data.payloadLength;
        memset(buffer, 0, DEFAULT_FTP_CHUNK_SIZE);
        // check ack & timeout
        do{
            char* recvBuffer = new char [UDP_MAX_PAYLOAD];
            if(this->udpSocket->receiveFrom(recvBuffer, UDP_MAX_PAYLOAD, this->partnerAddress)<0){
                // no ack
                // check timeout
                std::chrono::steady_clock::time_point nowTime = std::chrono::steady_clock::now();
                if(std::chrono::duration_cast<std::chrono::milliseconds>(nowTime - timers.front()).count() > DEFAULT_FTP_TIMEOUT){
                    inFlight = 0;
                    timers.clear();
                    break;
                }
                continue;
            }
            else {
                if ((recvBuffer[0] & 0x10) == 0) // not ack
                    continue;

                GBNFTPAck ack;
                ack.deserialize(recvBuffer);
                if(ack.checksum == 0xaa) {
                    unsigned int ackedBytes = ack.expSeqNumber > lastAck ? ack.expSeqNumber - lastAck : ack.expSeqNumber + (65536 - lastAck);
                    inFlight -= ackedBytes;
                    while(ceil(float (inFlight) / DEFAULT_FTP_CHUNK_SIZE) > timers.size() && !timers.empty()){
                        timers.pop_front(); // remove acked timers
                    }
                    base += ackedBytes;
                    lastAck = ack.expSeqNumber;
                }
            }
        }while (inFlight >= DEFAULT_FTP_WINDOW_SIZE);
    }
}