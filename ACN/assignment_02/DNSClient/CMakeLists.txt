cmake_minimum_required(VERSION 3.12)
project(DNSClient VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 11)

add_library(
        dns-client SHARED
        DNSClient.cpp
        DNSMessage.cpp
        DNSHeader.cpp
        DNSQuestion.cpp
        DNSResourceRecord.cpp
        DNSLabel.cpp
        QClass.hpp
        QType.hpp
        RData.cpp
)

add_executable(DNSClient main.cpp)

target_link_libraries(DNSClient PRIVATE dns-client)