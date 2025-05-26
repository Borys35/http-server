#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sstream>
#include <iostream>

namespace http {
    class TcpServer {
    public:
        TcpServer(const std::string &ip, const int port);
        ~TcpServer();
        void start_listen();

    private:
        const int BUFFER_SIZE = 30720;

        int m_socket;
        int m_new_socket;
        std::string m_ip;
        int m_port;
        long m_incoming_message;
        sockaddr_in m_socket_address;
        unsigned int m_socket_address_len;
        std::string m_server_message;

        int start_server();
        void stop_server();
        void accept_connection(int &new_socket);
        std::string generate_response();
        void handle_request();
        std::string parse_buffer(const char *buffer);
        void send_response();
        void send_static_files(std::string& path);
    };
}