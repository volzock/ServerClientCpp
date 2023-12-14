#include <iostream>
#include "client.h"

int main() {
    Client client;
    std::cout << client.makeRequest("127.0.0.1", 808, "minecraft");
    return 0;
}