#include "TcpServer.h"

#include <fstream>
#include <filesystem>
#include <cstring>

namespace http {
    TcpServer::TcpServer(const std::string &ip, const int port) : m_ip(ip), m_port(port) {
        m_server_message = generate_response();
        m_socket_address.sin_family = AF_INET;
        m_socket_address.sin_port = htons(m_port);
        m_socket_address.sin_addr.s_addr = inet_addr(m_ip.c_str());
        m_socket_address_len = sizeof(m_socket_address);

        if (start_server() != 0) {
            std::cout << "ERROR: Failed to start server." << std::endl;
            exit(1);
        }
    }

    TcpServer::~TcpServer() {
        stop_server();
    }

    int TcpServer::start_server() {
        m_socket = socket(AF_INET, SOCK_STREAM, 0);
        if (m_socket < 0) {
            std::cout << "ERROR: Socket creation failed." << std::endl;
            return 1;
        }

        if (bind(m_socket, reinterpret_cast<sockaddr *>(&m_socket_address), m_socket_address_len) < 0) {
            std::cout << "ERROR: Socket bind failed: " << std::strerror(errno) << std::endl;
            
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


        while (true) {
            std::cout << "Waiting for new connection..." << std::endl;
            accept_connection(m_new_socket);

            std::cout << "Received request from client." << std::endl;

            handle_request();

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
            std::cout << "ERROR: Write failed." << std::endl;
        }
    }

    void TcpServer::handle_request() {
        char buffer[BUFFER_SIZE] = {0};
        int bytesReceived = read(m_new_socket, buffer, BUFFER_SIZE);
        if (bytesReceived < 0) {
            std::cout << "ERROR: Read failed." << std::endl;
            exit(1);
        } if (bytesReceived == 0) {
            std::cout << "Client disconnected" << std::endl;
        }

        // std::string path = parse_buffer(buffer);
        ParsedBuffer b = parse_buffer(buffer);

        if (b.method != "GET") {
            send_404_response();
            return;
        }

        send_static_files(b.path);
    }

    TcpServer::ParsedBuffer TcpServer::parse_buffer(const char *buffer) {
        // GET /website1 HTTP/1.1
        // Host: 0.0.0.0:6969
        // User-Agent: curl/8.5.0%
        // Accept: */*

        std::string b_str(buffer);
        size_t first_space = b_str.find(' ');
        size_t second_space = b_str.find(' ', first_space + 1);

        if (first_space == std::string::npos || second_space == std::string::npos) {
            return {"/", "GET"};
        }

        std::string path = b_str.substr(first_space + 1, second_space - first_space - 1);
        std::string method = b_str.substr(0, first_space);

        return {path, method};
    }

    void TcpServer::send_static_files(std::string& path) {
        std::string file_path = BASE_DIR + path; // Valid path: ./htdocs/index.html

        if (!std::filesystem::exists(file_path)) {
            std::cout << "ERROR: File doesn't exist" << std::endl;
            send_404_response();
            return;
        } else if (std::filesystem::is_directory(file_path)) {
            if (file_path.back() == '/')
                file_path += "index.html";
            else {
                send_404_response();
                return;
            }
        }

        std::ifstream file(file_path, std::ios::binary | std::ios::ate);
        if (!file.is_open()) {
            std::cout << "ERROR: File cannot be opened." << std::endl;
            
            send_404_response();
            
            return;
        } 
        size_t file_size = file.tellg();
        file.seekg(std::ios::beg);

        std::string mime = "text/html";
        if (path.rfind(".css") == (path.size() - 4)) {
            mime = "text/css";
        } else if (path.rfind(".js") == (path.size() - 3)) {
            mime = "text/javascript";
        }

        std::ostringstream headers_oss;
        headers_oss << "HTTP/1.1 200 OK\r\n";
        headers_oss << "Content-Type: " << mime << "\r\n";
        headers_oss << "Content-Length: " << file_size << "\r\n";
        headers_oss << "\r\n";
        std::string headers = headers_oss.str();

        long bytesSent = 0;
        bytesSent = write(m_new_socket, headers.c_str(), headers.size());

        if (bytesSent == headers.size()) {
            std::cout << "Server sending headers." << std::endl;
        } else {
            std::cout << "ERROR: write failed." << std::endl;
        }

        char buffer[BUFFER_SIZE];
        while (file.read(buffer, BUFFER_SIZE)) {
            bytesSent = write(m_new_socket, buffer, file.gcount());
            if (bytesSent < 0) {
                std::cout << "ERROR" << std::endl;
            }
        }
        if (file.gcount() > 0) {
            bytesSent = write(m_new_socket, buffer, file.gcount());
            if (bytesSent < 0) {
                std::cout << "ERROR" << std::endl;
            }
        }

        file.close();
        std::cout << "File sent successfully" << std::endl;
    }

    void TcpServer::send_404_response()
    {
        std::string body = "<html><body><h1>404 Not Found</h1></body></html>";
        std::ostringstream headers_oss;
        headers_oss << "HTTP/1.1 404 Not Found\r\n";
        headers_oss << "Content-Type: text/html\r\n";
        headers_oss << "Content-Length: " << body.size() << "\r\n";
        headers_oss << "Connection: close\r\n";
        headers_oss << "\r\n";
        std::string headers = headers_oss.str();
        std::string response = headers + body;
        write(m_new_socket, response.c_str(), response.size());
    }
}
