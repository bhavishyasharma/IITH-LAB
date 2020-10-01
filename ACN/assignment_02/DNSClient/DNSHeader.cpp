//
// Created by Bhavishya Sharma on 27/09/20.
//

#include <iostream>
#include <iomanip>

#include "DNSHeader.h"

using namespace std;

u_int16_t DNSHeader::size() {
    return 12;
}

// convert header object to bytes for socket
u_int16_t DNSHeader::serialize(unsigned char *bytes, u_int16_t offset) {
    u_int16_t temp;
    memcpy(bytes + offset, &(temp = htons(this->id)), 2);
    memcpy(bytes + offset + 2, &(temp = htons(this->flags)), 2);
    memcpy(bytes + offset + 4, &(temp = htons(this->qdCount)), 2);
    memcpy(bytes + offset + 6, &(temp = htons(this->anCount)), 2);
    memcpy(bytes + offset + 8, &(temp = htons(this->nsCount)), 2);
    memcpy(bytes + offset + 10, &(temp = htons(this->arCount)), 2);
    return offset + 12;
}

// parse header from bytes
u_int16_t DNSHeader::deserialize(unsigned char *bytes, u_int16_t offset) {
    u_int16_t temp;
    memcpy(&temp, bytes + offset, 2);
    this->id = ntohs(temp);
    memcpy(&temp, bytes + offset + 2, 2);
    this->flags = ntohs(temp);
    memcpy(&temp, bytes + offset + 4, 2);
    this->qdCount = ntohs(temp);
    memcpy(&temp, bytes + offset + 6, 2);
    this->anCount = ntohs(temp);
    memcpy(&temp, bytes + offset + 8, 2);
    this->nsCount = ntohs(temp);
    memcpy(&temp, bytes + offset + 10, 2);
    this->arCount = ntohs(temp);
    return offset + 12;
}

// print message header
void DNSHeader::printHeader() {
    cout<<string (4, ' ') << "Transaction Id : "<<hex<<setw(4)<<uppercase<<this->id<<endl;
    cout<<string (4, ' ') << "Flags : "<<hex<<setw(4)<<uppercase<<this->flags<<endl;
    cout<<string (8, ' ') << "Response : "<<dec<<(this->flags & 0x8000 ? "Yes" : "No")<<endl;
    u_int16_t opcode = (this->flags & 0x7800) >> 12;
    cout<<string (8, ' ') << "Opcode : "<<bitset<4>(opcode)<<(opcode == 0 ? " (Standard Query)" : opcode == 1 ? " (Inverse Query)" : opcode == 2 ? " (Server Status Request)" : " (Reserved)")<<endl;
    cout<<string (8, ' ') << "Authoritative Answer : "<<dec<<(this->flags & 0x0400 ? "Yes" : "No")<<endl;
    cout<<string (8, ' ') << "Truncation : "<<dec<<(this->flags & 0x0200 ? "Yes" : "No")<<endl;
    cout<<string (8, ' ') << "Recursion Desired : "<<dec<<(this->flags & 0x0100 ? "Yes" : "No")<<endl;
    cout<<string (8, ' ') << "Recursion Available : "<<dec<<(this->flags & 0x0080 ? "Yes" : "No")<<endl;
    u_int16_t z = this->flags >> 4;
    cout<<string (8, ' ') << "Z - Reserved : "<<bitset<3>(z)<<endl;
    u_int16_t rcode = (this->flags & 0x000F);
    cout<<string (8, ' ') << "Opcode : "<<bitset<4>(rcode)<<(opcode == 0 ? " (No Error)" : opcode == 1 ? " (Format Error)" : opcode == 2 ? " (Server Failure)" : opcode == 3 ? " (Name Error)" : opcode == 4 ? " (Not Implemented)" : opcode == 5 ? " (Refused)" : " (Reserved)")<<endl;
    cout<<string (4, ' ') << "Question Count : "<<dec<<uppercase<<this->qdCount<<endl;
    cout<<string (4, ' ') << "Answer Count : "<<dec<<uppercase<<this->anCount<<endl;
    cout<<string (4, ' ') << "Authority Count : "<<dec<<uppercase<<this->nsCount<<endl;
    cout<<string (4, ' ') << "Additional Count : "<<dec<<uppercase<<this->arCount<<endl;
}