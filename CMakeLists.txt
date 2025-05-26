cmake_minimum_required(VERSION 3.10)

# project name
project(HttpServer VERSION 1.0)

# c++ version
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# add source files (.cpp)
add_executable(http
    main.cpp
    TcpServer.h
    TcpServer.cpp
)