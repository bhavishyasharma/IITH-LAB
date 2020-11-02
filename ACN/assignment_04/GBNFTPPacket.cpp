//
// Created by Bhavishya Sharma on 31/10/20.
//

#include "GBNFTPPacket.h"

char * GBNFTPRequest::serialize() {
    char* bytes = new char [strlen(this->payload) + 7];
    memcpy(bytes, &this->flags, 1);
    memcpy(bytes + 1, &this->checksum, 1);

    this->seqNumber = htons(this->seqNumber);
    memcpy(bytes + 2, &this->seqNumber, 2);
    this->seqNumber = ntohs(this->seqNumber);

    this->ackNumber = htons(this->ackNumber);
    memcpy(bytes + 4, &this->ackNumber, 2);
    this->ackNumber = ntohs(this->ackNumber);

    memcpy(bytes + 6, this->payload, strlen(this->payload) + 1);
    return bytes;
}

void GBNFTPRequest::deserialize(char * bytes) {
    memcpy(&this->flags, bytes, 1);
    memcpy(&this->checksum, bytes + 1, 1);
    memcpy(&this->seqNumber, bytes + 2, 2);
    this->seqNumber = ntohs(this->seqNumber);
    memcpy(&this->ackNumber, bytes + 4, 2);
    this->ackNumber = ntohs(this->ackNumber);
    this->payload = new char [strlen(bytes+6)+1];
    memcpy(this->payload, bytes + 6, strlen(bytes+6)+1);
}

int GBNFTPRequest::size() {
    return strlen(this->payload) + 7;
}

char * GBNFTPResponse::serialize() {
    char *bytes = new char [10];
    memcpy(bytes, &this->flags, 1);
    memcpy(bytes + 1, &this->checksum, 1);

    this->seqNumber = htons(this->seqNumber);
    memcpy(bytes + 2, &this->seqNumber, 2);
    this->seqNumber = ntohs(this->seqNumber);

    this->fileSize = htonl(this->fileSize);
    memcpy(bytes + 4, &this->fileSize, 4);
    this->fileSize = ntohl(this->fileSize);

    this->windowSize = htons(this->windowSize);
    memcpy(bytes + 8, &this->windowSize, 2);
    this->windowSize = ntohs(this->windowSize);
    return bytes;
}

void GBNFTPResponse::deserialize(char *bytes) {
    memcpy(&this->flags, bytes, 1);
    memcpy(&this->checksum, bytes + 1, 1);
    memcpy(&this->seqNumber, bytes + 2, 2);
    this->seqNumber = ntohs(this->seqNumber);

    memcpy(&this->fileSize, bytes + 4, 4);
    this->fileSize = ntohl(this->fileSize);

    memcpy(&this->windowSize, bytes + 8, 2);
    this->windowSize = ntohs(this->windowSize);
}

int GBNFTPResponse::size() {
    return 10;
}

char * GBNFTPData::serialize() {
    char* bytes = new char [this->payloadLength + 8];
    memcpy(bytes, &this->flags, 1);
    memcpy(bytes + 1, &this->checksum, 1);

    this->seqNumber = htons(this->seqNumber);
    memcpy(bytes + 2, &this->seqNumber, 2);
    this->seqNumber = ntohs(this->seqNumber);

    this->ackNumber = htons(this->ackNumber);
    memcpy(bytes + 4, &this->ackNumber, 2);
    this->ackNumber = ntohs(this->ackNumber);

    this->payloadLength = htons(this->payloadLength);
    memcpy(bytes + 6, &this->payloadLength, 2);
    this->payloadLength = ntohs(this->payloadLength);

    memcpy(bytes + 8, this->payload, this->payloadLength);
    return bytes;
}

void GBNFTPData::deserialize(char *bytes) {
    memcpy(&this->flags, bytes, 1);
    memcpy(&this->checksum, bytes + 1, 1);
    memcpy(&this->seqNumber, bytes + 2, 2);
    this->seqNumber = ntohs(this->seqNumber);
    memcpy(&this->ackNumber, bytes + 4, 2);
    this->ackNumber = ntohs(this->ackNumber);
    memcpy(&this->payloadLength, bytes + 6, 2);
    this->payloadLength = ntohs(this->payloadLength);
    if(this->payloadLength) {
        this->payload = new char[this->payloadLength];
        memcpy(this->payload, bytes + 8, this->payloadLength);
    }
}

int GBNFTPData::size() {
    return this->payloadLength + 8;
}

char * GBNFTPAck::serialize() {
    char* bytes = new char [6];
    memcpy(bytes, &this->flags, 1);
    memcpy(bytes + 1, &this->checksum, 1);

    this->expSeqNumber = htons(this->expSeqNumber);
    memcpy(bytes + 2, &this->expSeqNumber, 2);
    this->expSeqNumber = ntohs(this->expSeqNumber);

    this->ackNumber = htons(this->ackNumber);
    memcpy(bytes + 4, &this->ackNumber, 2);
    this->ackNumber = ntohs(this->ackNumber);
    return bytes;
}

void GBNFTPAck::deserialize(char *bytes) {
    memcpy(&this->flags, bytes, 1);
    memcpy(&this->checksum, bytes + 1, 1);
    memcpy(&this->expSeqNumber, bytes + 2, 2);
    this->expSeqNumber = ntohs(this->expSeqNumber);
    memcpy(&this->ackNumber, bytes + 4, 2);
    this->ackNumber = ntohs(this->ackNumber);
}

int GBNFTPAck::size() {
    return 6;
}