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

int serverSocketFd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
if(!serverSocketFd)
{
   cout<<"Error creating socket"<<endl;
   exit(1);
}

struct sockaddr_in serverSockAddressInfo;
serverSockAddressInfo.sin_family = AF_INET;
serverSockAddressInfo.sin_port = htons(serverPort);
//serverSockAddressInfo.sin_addr.s_addr = INADDR_ANY;
//serverSockAddressInfo.sin_addr = serverIpAddr;

//cout<<"Server listening on IP:PORT"<<serverIpAddr.c_str()<<serverPort<<endl;
inet_pton(AF_INET, serverIpAddr.c_str(), &(serverSockAddressInfo.sin_addr));
memset(&(serverSockAddressInfo.sin_zero), '\0', 8);
printf("Server listening on IP %x:PORT %d \n",serverSockAddressInfo.sin_addr.s_addr, serverPort);


int ret = bind(serverSocketFd, (struct sockaddr *)&serverSockAddressInfo, sizeof(struct sockaddr)); 
if(ret<0)
{
   cout<<"Error binding socket"<<endl;
   close(serverSocketFd);
   exit(1);
}


ret = listen(serverSocketFd, backLog);
if(!serverSocketFd)
{
   cout<<"Error listening socket"<<endl;
   close(serverSocketFd);
   exit(1);
}

socklen_t sinSize = sizeof(struct sockaddr_in);
int flags = 0;
int dataRecvd = 0, dataSent = 0;
struct sockaddr_in clientAddressInfo;
char rcvDataBuf[maxDataSize], sendDataBuf[maxDataSize];
string sendDataStr;

   memset(&clientAddressInfo, 0, sizeof(struct sockaddr_in));
   memset(&rcvDataBuf, 0, maxDataSize);
   int newClientFd = accept(serverSocketFd, (struct sockaddr *)&clientAddressInfo, &sinSize);
   if (!newClientFd)
   {
       cout<<"Error with new client connection "<<endl;
       close(serverSocketFd);
       exit(1);
   }

   cout<<"New client arrived"<<endl;
   cin.ignore();
while(1)
{
   memset(&rcvDataBuf, 0, maxDataSize);

   dataRecvd = recv(newClientFd, &rcvDataBuf, maxDataSize, flags);
   cout<<rcvDataBuf<<endl;

   if(!strcmp(rcvDataBuf, "bye"))
   {
      close(newClientFd);
      break;
   }

   memset(&sendDataBuf, 0, maxDataSize);
   cin.clear();
   cin.getline(sendDataBuf,maxDataSize);


//   cout<<"Data to send"<<sendDataStr.c_str()<<endl;
   //dataSent = send(newClientFd, sendDataStr.c_str(), sendDataStr.length(), flags);
   dataSent = send(newClientFd, sendDataBuf, strlen(sendDataBuf), flags);
   cout<<"data sent number of bytes"<<dataSent<<endl;
   if(!strcmp(sendDataBuf, "bye"))
   {
      close(newClientFd);
      break;
   }

}

cout<<"All done closing server socket now"<<endl;
close(serverSocketFd);


}
