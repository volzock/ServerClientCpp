#include <string>
#include <unordered_map>

#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <functional>

#include <thread>


class Server {
private:
    int socketfd;
    struct sockaddr_in addr;
    std::unordered_map<std::string, std::function<std::string(void)>> routes;

    void handleAccept(int client_fd){
        if (client_fd < 0) {
            perror("Accept failed");
            exit(1);
        }


        char buffer[64];
        for (int i = 0; i < 64; ++i) {
            buffer[i] = '\0';
        }

        int bytes_received = recv(client_fd, buffer, sizeof(buffer), 0);
        if (bytes_received < 0) {
            perror("Receiving failed");
            exit(1);
        }

        std::string key = std::string(buffer);
        key.erase(std::find_if(key.rbegin(), key.rend(), [](unsigned char ch) {
            return !std::isspace(ch);
        }).base(), key.end());
        std::string result = this->routes[key]();


        const char* result_cstr = result.c_str();

        int bytes_sent = send(client_fd, result_cstr, result.length(), 0);
        if (bytes_sent < 0) {
            perror("Sending failed");
            exit(1);
        }

        close(client_fd);
    }
public:
    Server(const char* ip, int port) {
        this->socketfd = socket(AF_INET, SOCK_STREAM, 0);

        this->addr.sin_family = AF_INET;
        this->addr.sin_port = htons(port);
        if (inet_aton(ip, &this->addr.sin_addr) == 0) {
            std::cerr << "Invalid IP address" << std::endl;
            exit(1);
        }

        if (bind(this->socketfd, (const struct sockaddr *)&this->addr, sizeof(this->addr)) < 0) {
            perror("Binding failed");
            exit(1);
        }
    }

    void addRoute(std::string name, std::function<std::string(void)> func) {
        routes.insert(std::make_pair(name, func));
    }

    void startListen() {
        listen(this->socketfd, 5);

        while (true) {
            int client_fd = accept(this->socketfd, NULL, NULL);

            std::thread t([this, &client_fd]{this->handleAccept(client_fd);});
            t.detach();
        }
    }
    
    ~Server() {
        close(this->socketfd);
    }
};
