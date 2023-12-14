
#ifdef _WIN32
#pragma comment(lib, "ws2_32.lib")

#include <iostream>
#include <string>
#include "library/server/ServerBase.h"
#include <winsock2.h>
#include <cstdio>
#include <ws2tcpip.h>
#include <algorithm>

#define WINDOWS_BUFFER_SIZE 2048


class WindowServer : public ServerBase {
private:
    SOCKET socketfd;

    void handleAccept(SOCKET clientSocket) {
        char buffer[WINDOWS_BUFFER_SIZE];
        for (int i = 0; i < WINDOWS_BUFFER_SIZE; ++i) {
            buffer[i] = '\0';
        }
        int bytesRecieved = recv(clientSocket, buffer, sizeof(buffer), 0);
        if (bytesRecieved == SOCKET_ERROR) {
            perror("Failed to recieve");
            exit(1);
        }

        std::string key = std::string(buffer);
        key.erase(std::
        find_if(key.rbegin(), key.rend(), [](unsigned char ch) {
            return !std::isspace(ch);
        }).base(), key.end());
        std::cout << '\\' << key << '\\' << '\n';

        std::string result = this->routes[key]();
        const char* result_cstr = result.c_str();

        int bytesSent = send(clientSocket, result_cstr, result.length(), 0);
        if (bytesSent < 0) {
            perror("Failed to send");
            exit(1);
        }

        closesocket(clientSocket);
    }
public:
    WindowServer(const char* ip, int port) {
        WSADATA wsaData;
        int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
        if (iResult != 0) {
            std::cout << "WSAStartup failed: " << iResult << std::endl;
            exit(1);
        }

        this->socketfd = socket(AF_INET, SOCK_STREAM, 0);

        struct sockaddr_in server;
        server.sin_family = AF_INET;
        server.sin_port = htons(port);
        if (inet_pton(AF_INET, ip, &(server.sin_addr)) < 0) {
            perror("Invalid IP");
            exit(1);
        }

//        bind(this->socketfd, (struct sockaddr *)&server, sizeof(server));

        if (bind(this->socketfd, (struct sockaddr *)&server, sizeof(server)) < 0) {
            perror("Binding failed");
            exit(1);
        }
    }

    void startListen() override {
        listen(this->socketfd, 3);

        while (true) {
            struct sockaddr_in client;
            int c = sizeof(struct sockaddr_in);
            SOCKET clientSocket = accept(this->socketfd, (struct sockaddr *)&client, &c);

            std::thread t([this, &clientSocket]{this->handleAccept(clientSocket);});
            t.detach();
        }
    }

    ~WindowServer() {
        closesocket(this->socketfd);
        WSACleanup();
    }
};

#endif