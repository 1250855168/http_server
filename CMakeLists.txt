cmake_minimum_required(VERSION 3.0.0)
project(http_server)


set(CMAKE_CXX_COMPILER "/usr/bin/g++")
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(./include)

add_executable(server
    server.cpp
    src/Acceptor.cpp
    src/Buffer.cpp
    src/Channel.cpp
    src/Connection.cpp
    src/Epoll.cpp
    src/EventLoop.cpp
    src/Server.cpp
    src/Socket.cpp
    src/InetAddress.cpp
    src/ThreadPool.cpp
    src/util.cpp
    src/Http_request.cpp
    src/Logger.cpp
   
)

# 链接 sqlite3 库和 pthread 库
target_link_libraries(server  pthread  sqlite3)