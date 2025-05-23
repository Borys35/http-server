#include <iostream>

#include "TcpServer.h"

int main() {
    auto server = http::TcpServer("0.0.0.0", 6969);

    return 0;
}