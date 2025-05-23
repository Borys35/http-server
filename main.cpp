#include <iostream>

#include "TcpServer.h"

int main() {
    std::cout << "Http server runs..." << std::endl;

    auto server = http::TcpServer("0.0.0.0", 6969);

    return 0;
}