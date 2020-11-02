cmake_minimum_required(VERSION 3.12)
project(UDPStopAndWait VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 11)

add_library(
        saw-udp SHARED
        SAWPacket.cpp
        SAWSocket.cpp
        UDPSocket.cpp
)

add_library(
        gbn-udp SHARED
        GBNFTPPacket.cpp
        GBNFTPSocket.cpp
        UDPSocket.cpp
)

add_executable(SAWFTPClient main.ftp.saw.client.cpp)
add_executable(SAWFTPServer main.ftp.saw.server.cpp)

add_executable(GBNFTPClient main.ftp.gbn.client.cpp)
add_executable(GBNFTPServer main.ftp.gbn.server.cpp)

target_link_libraries(SAWFTPClient PRIVATE saw-udp)
target_link_libraries(SAWFTPServer PRIVATE saw-udp)

target_link_libraries(GBNFTPClient PRIVATE gbn-udp)
target_link_libraries(GBNFTPServer PRIVATE gbn-udp)