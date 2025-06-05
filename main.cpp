#include <iostream>

#include "TcpServer.h"

int main() {
    std::cout << "Http server runs... 234" << std::endl;

    auto server = http::TcpServer("0.0.0.0", 7120);
    server.start_listen();

    return 0;
}