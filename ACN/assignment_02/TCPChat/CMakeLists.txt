cmake_minimum_required(VERSION 3.12)
project(TCPChat VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 11)
set(THREADS_PREFER_PTHREAD_FLAG ON)

find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIR})

find_package(Threads REQUIRED)

add_library(
        chat-client SHARED
        ChatClient.cpp
        ChatMessage.cpp
        ChatCommon.hpp
)

add_library(
        chat-server SHARED
        ChatServer.cpp
        ChatQueueHandler.cpp
        ChatClientThreadHandler.cpp
        ChatClientHandler.cpp
        ChatMessage.cpp
        ChatCommon.hpp
)

add_executable(ChatServer server.main.cpp)
add_executable(ChatClient client.main.cpp)

target_link_libraries(chat-client PRIVATE ${CURSES_LIBRARIES})

target_link_libraries(ChatServer PRIVATE chat-server Threads::Threads)

target_link_libraries(ChatClient PRIVATE chat-client Threads::Threads)