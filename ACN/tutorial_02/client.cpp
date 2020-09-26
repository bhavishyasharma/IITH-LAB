#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <iostream>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <arpa/inet.h>

using namespace std;
const int backLog = 3;
const int maxDataSize = 1460;

int main()
{

uint16_t serverPort=3002;
string serverIpAddr = "127.0.0.1";
cout<<"Enter the ip address and port number to listen the connections for"<<endl;
cin>>serverIpAddr;
cin>>serverPort;

int clientSocketFd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
if(!clientSocketFd)
{
   cout<<"Error creating socket"<<endl;
   exit(1);
}

struct sockaddr_in serverSockAddressInfo;
serverSockAddressInfo.sin_family = AF_INET;
serverSockAddressInfo.sin_port = htons(serverPort);
//serverSockAddressInfo.sin_addr.s_addr = INADDR_ANY;
inet_pton(AF_INET, serverIpAddr.c_str(), &(serverSockAddressInfo.sin_addr));

memset(&(serverSockAddressInfo.sin_zero), '\0', 8);


socklen_t sinSize = sizeof(struct sockaddr_in);
int flags = 0;
int dataRecvd = 0, dataSent = 0;
struct sockaddr_in clientAddressInfo;
char rcvDataBuf[maxDataSize], sendDataBuf[maxDataSize];
string sendDataStr, rcvDataStr;

   int ret = connect(clientSocketFd, (struct sockaddr *)&serverSockAddressInfo, sizeof(struct sockaddr));
   if (ret<0)
   {
       cout<<"Error with server connection "<<endl;
       close(clientSocketFd);
       exit(1);
   }
 cin.ignore();
while(1)
{
   cout<<"Enter data to send"<<endl;

   memset(&sendDataBuf, 0, maxDataSize);
//   cin.ignore();
   cin.clear();
   //cin.getline(sendDataBuf,maxDataSize);
   getline(cin, sendDataStr);
   cout<<sendDataStr.c_str();
   
   //getline(cin,sendDataStr);
   dataSent = send(clientSocketFd, sendDataStr.c_str(), sendDataStr.length(), flags);
   //dataSent = send(clientSocketFd, &sendDataBuf[0], strlen(sendDataBuf), flags);
  // cout<<"Is data sent successfully"<<dataSent<<endl;
   if(!strcmp(sendDataStr.c_str(), "bye"))
   {
      break;
   }


   memset(&rcvDataBuf, 0, maxDataSize);
   dataRecvd = recv(clientSocketFd, &rcvDataBuf, maxDataSize, flags);
   rcvDataStr = rcvDataBuf;
   cout<<rcvDataStr.c_str()<<endl;
   if(!strcmp(rcvDataStr.c_str(), "bye"))
   {
      break;
   }

}

cout<<"All done closing socket now"<<endl;
close(clientSocketFd);


}
