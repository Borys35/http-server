#include "TcpServer.h"

namespace http {
    TcpServer::TcpServer(const std::string &ip, const int port) : m_ip(ip), m_port(port) {
        start_server();
    }

    TcpServer::~TcpServer() {
        stop_server();
    }

    int TcpServer::start_server() {
        m_socket = socket(AF_INET, SOCK_STREAM, 0);
        if (m_socket < 0) {
            // Error
            std::cout << "socket creation failed." << std::endl;
            return 1;
        }
        return 0;
    }

    void TcpServer::stop_server() {
        close(m_socket);
        close(m_new_socket);

        exit(0);
    }



}
