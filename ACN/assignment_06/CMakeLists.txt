cmake_minimum_required(VERSION 3.12)
project(PNat VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 11)
set(THREADS_PREFER_PTHREAD_FLAG ON)

find_package(Threads REQUIRED)

add_library(
        pnat SHARED
        PNat.cpp
        RawSocket.cpp
)

add_library(
        tcp-socket SHARED
        TcpSocket.cpp
)

add_library(
        udp-socket SHARED
        UdpSocket.cpp
)

add_executable(TcpClient TcpClient.cpp)
add_executable(PNatServer main.pnat.cpp)

target_link_libraries(TcpClient PRIVATE tcp-socket)

target_link_libraries(PNatServer PRIVATE pnat)