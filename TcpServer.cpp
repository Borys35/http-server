#include "TcpServer.h"

namespace http {
    TcpServer::TcpServer(const std::string &ip, const int port) : ip(ip), port(port) {
        start();
    }

    TcpServer::~TcpServer() {
        stop();
    }

    int TcpServer::start() {
        this->socket = socket(AF_INET, SOCK_STREAM, 0);
        if (this->socket < 0) {
            // Error
            exit(1);
            return 1;
        }
        return 0;
    }

    void TcpServer::stop() {
        close(this->socket);
        close(this->new_socket);
        exit(0);
    }



}
