#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string>
#include <algorithm>

class Client {
private:
public:
    std::string makeRequest(const char* ip, int port, std::string path) {
        int sockfd = socket(AF_INET, SOCK_STREAM, 0);
        if (sockfd == -1) {
            std::cerr << "Failed to create socket" << std::endl;
            exit(1);
        }

        struct sockaddr_in serverAddr;
        serverAddr.sin_family = AF_INET;
        serverAddr.sin_port = htons(port);
        if (inet_aton(ip, &serverAddr.sin_addr) == 0) {
            std::cerr << "Invalid IP address" << std::endl;
            exit(1);
        }

        if (connect(sockfd, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == -1) {
            std::cerr << "Failed to connect to the server" << std::endl;
            close(sockfd);
            exit(1);
        }

        if (send(sockfd, path.c_str(), path.length(), 0) == -1) {
            std::cerr << "Failed to send message" << std::endl;
            close(sockfd);
            exit(1);
        }

        char buffer[64];
        for (int i = 0; i < 64; ++i) {
            buffer[i] = '\0';
        }

        int bytesRead = recv(sockfd, buffer, sizeof(buffer) - 1, 0);
        if (bytesRead == -1) {
            std::cerr << "Failed to receive message" << std::endl;
            close(sockfd);
            exit(1);
        }

        std::string result = std::string(buffer);
        result.erase(std::find_if(result.rbegin(), result.rend(), [](unsigned char ch) {
            return !std::isspace(ch);
        }).base(), result.end());

        close(sockfd);

        return result;
    }
};
