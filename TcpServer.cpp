#include "TcpServer.h"

namespace http {
    TcpServer::TcpServer(const std::string &ip, const int port) : m_ip(ip), m_port(port) {
        m_server_message = generate_response();
        m_socket_address.sin_family = AF_INET;
        m_socket_address.sin_port = htons(m_port);
        m_socket_address.sin_addr.s_addr = inet_addr(m_ip.c_str());
        m_socket_address_len = sizeof(m_socket_address);

        if (start_server() != 0) {
            std::cout << "ERROR: Failed to start server." << std::endl;
        }
    }

    TcpServer::~TcpServer() {
        stop_server();
    }

    int TcpServer::start_server() {
        m_socket = socket(AF_INET, SOCK_STREAM, 0);
        if (m_socket < 0) {
            // Error
            std::cout << "ERROR: socket creation failed." << std::endl;
            return 1;
        }

        if (bind(m_socket, reinterpret_cast<sockaddr *>(&m_socket_address), m_socket_address_len) < 0) {
            std::cout << "ERROR: socket bind failed." << std::endl;
            return 1;
        }
        return 0;
    }

    void TcpServer::stop_server() {
        close(m_socket);
        close(m_new_socket);

        exit(0);
    }

    void TcpServer::start_listen() {
        if (listen(m_socket, 20) < 0) {
            std::cout << "ERROR: listen failed." << std::endl;
            exit(1);
        }

        std::cout << "Listening on address: " << inet_ntoa(m_socket_address.sin_addr) << ", port: " << ntohs(m_socket_address.sin_port) << std::endl;

        int bytesReceived;

        while (true) {
            std::cout << "Waiting for new connection..." << std::endl;
            accept_connection(m_new_socket);

            char buffer[BUFFER_SIZE] = {0};
            bytesReceived = read(m_new_socket, buffer, BUFFER_SIZE);
            if (bytesReceived < 0) {
                std::cout << "ERROR: read failed." << std::endl;
                exit(1);
            }

            std::cout << "Received request from client." << std::endl;

            send_response();

            close(m_new_socket);
        }
    }

    void TcpServer::accept_connection(int &new_socket) {
        new_socket = accept(m_socket, reinterpret_cast<sockaddr *>(&m_socket_address), &m_socket_address_len);
        if (new_socket < 0) {
            std::cout << "ERROR: accept failed from address: " << inet_ntoa(m_socket_address.sin_addr) << ", port: " << ntohs(m_socket_address.sin_port) << std::endl;
            exit(1);
        }
    }

    std::string TcpServer::generate_response() {
        std::string htmlFile = "<!DOCTYPE html><html lang=\"en\"><body><h1> HOME </h1><p> Hello from your Server :) </p></body></html>";
        std::ostringstream ss;
        ss << "HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: " << htmlFile.size() << "\n\n"
           << htmlFile;

        return ss.str();
    }

    void TcpServer::send_response() {
        long bytesSent = 0;

        bytesSent = write(m_new_socket, m_server_message.c_str(), m_server_message.size());

        if (bytesSent == m_server_message.size()) {
            std::cout << "Server sending response." << std::endl;
        } else {
            std::cout << "ERROR: write failed." << std::endl;
        }
    }


}
