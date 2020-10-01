//
// Created by Bhavishya Sharma on 30/09/20.
//

#include "ChatServer.h"

ChatServer::ChatServer() {
    this->init(DEFAULT_CHAT_SERVER_IP, DEFAULT_CHAT_SERVER_PORT, MAX_CLIENT_THREADS);
}

ChatServer::ChatServer(u_int16_t port) {
    this->init(DEFAULT_CHAT_SERVER_IP, port, MAX_CLIENT_THREADS);
}

ChatServer::ChatServer(std::string serverIP) {
    this->init(serverIP, DEFAULT_CHAT_SERVER_PORT, MAX_CLIENT_THREADS);
}

ChatServer::ChatServer(std::string serverIP, u_int16_t port) {
    this->init(serverIP, port, MAX_CLIENT_THREADS);
}

ChatServer::ChatServer(int clientCount) {
    this->init(DEFAULT_CHAT_SERVER_IP, DEFAULT_CHAT_SERVER_PORT, clientCount);
}

ChatServer::ChatServer(u_int16_t port, int clientCount) {
    this->init(DEFAULT_CHAT_SERVER_IP, port, clientCount);
}

ChatServer::ChatServer(std::string serverIP, int clientCount) {
    this->init(serverIP, DEFAULT_CHAT_SERVER_PORT, clientCount);
}

ChatServer::ChatServer(std::string serverIP, u_int16_t port, int clientCount) {
    this->init(serverIP, port, clientCount);
}

void ChatServer::init(std::string serverIP, u_int16_t port, int clientCount) {
    // initialize objects
    this->serverAddress = new sockaddr_in();
    this->clientThreads = new std::map<int, pthread_t*>();
    this->clientHandlers = new std::map<int, ChatClientHandler*>();
    this->messageQueue = new std::list<ChatMessage*>();
    this->maxClientThreadCount = clientCount;

    // create server socket
    if((this->serverSocketFD = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
        std::cerr<<"Failed: Error creating socket.";
        throw std::runtime_error("Failed: Error creating socket.");
    }
    this->serverAddress->sin_family = AF_INET;
    inet_aton(serverIP.c_str(), &(this->serverAddress->sin_addr));
    this->serverAddress->sin_port = htons(port);
}

void ChatServer::startServer() {

    // bind server socket
    if( bind(this->serverSocketFD, (struct sockaddr *) (this->serverAddress), sizeof(*(this->serverAddress))) < 0 ) {
        std::cerr<<"Failed: Error binding socket.";
        throw std::runtime_error("Failed: Error binding socket.");
    }

    // listen on server socket
    if( listen(serverSocketFD, this->maxClientThreadCount) < 0 ) {
        std::cerr<<"Failed: Error listening on socket.";
        throw std::runtime_error("Failed: Error listening on socket.");
    }
    std::cout<<"Listening on "<<inet_ntoa(this->serverAddress->sin_addr)<<":"<<ntohs(this->serverAddress->sin_port)<<std::endl;

    // Create queue handler & run it on different thread
    this->queueHandler = new ChatQueueHandler(this->messageQueue, this->clientHandlers);
    this->queueThread = new pthread_t ();
    if(pthread_create(this->queueThread, NULL, ChatQueueHandler::threadRunner, this->queueHandler) == 0) {

        // create client-thread-handler and run it on different thread
        // this thread remove finished threads/clients
        this->clientThreadHandler = new ChatClientThreadHandler(this->clientThreads, this->clientHandlers, this->maxClientThreadCount);
        this->clientThreadHandlerThread = new pthread_t ();
        if(pthread_create(this->clientThreadHandlerThread, NULL, ChatClientThreadHandler::threadRunner, this->clientThreadHandler) == 0) {

            // accept connections from client
            while (1) {

                std::cout << "Waiting for client connection...." << "\n";
                int clientSocketFD;
                struct sockaddr_in *clientAddress = new sockaddr_in();
                unsigned int clientLen;

                // accept connection
                if ((clientSocketFD = accept(this->serverSocketFD, (struct sockaddr *) clientAddress, &clientLen)) <
                    0) {
                    std::cerr << "Failed: Error accepting connection.";
                    throw std::runtime_error("Failed: Error accepting connection.");
                }

                // read username from socket - first message sent from client is the username
                unsigned char *username = new unsigned char[MAX_USERNAME_LENGTH];
                int usernameSize;
                if ((usernameSize = recv(clientSocketFD, username, MAX_USERNAME_LENGTH, 0)) < 0) {
                    std::cerr << "Failed: Error receiving from client.";
                    throw std::runtime_error("Failed: Error receiving from client.");
                }

                // create thread & handler to read further data from client
                pthread_t *clientThread = new pthread_t();
                ChatClientHandler *clientHandler = new ChatClientHandler(username, clientSocketFD, clientAddress,
                                                                         this->messageQueue);
                if (pthread_create(clientThread, NULL, ChatClientHandler::threadRunner, clientHandler) == 0) {

                    // add thead and handler to list
                    this->clientHandlers->insert(std::pair<int, ChatClientHandler *>(clientSocketFD, clientHandler));
                    this->clientThreads->insert(std::pair<int, pthread_t *>(clientSocketFD, clientThread));

                    //if ( ! pthread_detach(*clientThread) )
                    //    printf("Thread detached successfully !!!\n");
                }
                std::cout<<"Thread Count : "<<this->clientThreads->size()<<"/"<<maxClientThreadCount<<std::endl;

                // wait if thread count = max thread count (max users connected)
                while (this->clientThreads->size() >= maxClientThreadCount){
                    sleep(1);
                }
            }

            // cancel client-thread-handler thread
            pthread_cancel(*this->clientThreadHandlerThread);
        }
        else{
            throw std::runtime_error("Failed: Error create client threads handler thread.");
        }
        pthread_cancel(*this->queueThread);
    }
    else{
        throw std::runtime_error("Failed: Error create queue handler thread.");
    }
}