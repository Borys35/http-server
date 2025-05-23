#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

namespace http {
    class TcpServer {
    public:
        TcpServer(const std::string &ip, const int port);
        ~TcpServer();
    private:
        int socket;
        int new_socket;
        std::string ip;
        int port;
        long incoming_message;
        sockaddr_in socket_address;
        unsigned int socket_address_len;
        std::string server_message;
        int start();
        void stop();
    };
}