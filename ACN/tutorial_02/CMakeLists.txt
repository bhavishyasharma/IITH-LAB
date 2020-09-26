cmake_minimum_required(VERSION 3.12)
project(EchoServer VERSION 1.0.0)

add_executable(TCPEchoServer TCPEchoServer.cpp)
add_executable(TCPEchoClient TCPEchoClient.cpp)
add_executable(UDPEchoServer UDPEchoServer.cpp)
add_executable(UDPEchoClient UDPEchoClient.cpp)
add_executable(tcp_server_ta server.cpp)
add_executable(tcp_client_ta client.cpp)