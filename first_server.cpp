#include <iostream>
#include <string>
// #include <unordered_map>

#include <unistd.h>
// #include <netinet/in.h>
// #include <arpa/inet.h>
// #include <sys/socket.h>
// #include <functional>

// #include <thread>
#include "server.h"
// #include "client.h"

#include <iostream>
#include <dirent.h>

class ActiveThreadsCount {
private:
    int getByPid(int pid) {
        int quantity = 0;

        std::string taskDirPath = "/proc/" + std::to_string(pid) + "/task";
        DIR* taskDir = opendir(taskDirPath.c_str());
        if (taskDir != nullptr) {
            // Count the number of subdirectories (threads)
            struct dirent* taskEntry;
            while ((taskEntry = readdir(taskDir)) != nullptr) {
                if (taskEntry->d_type == DT_DIR) {
                    quantity++;
                }
            }
            closedir(taskDir);
        }

        return quantity;
    }
public:
    int getAll() {
        int quantity = 0;
        DIR* procDir = opendir("/proc");
        if (procDir == nullptr) {
            std::cerr << "Failed to open /proc directory" << std::endl;
            return -1;
        }

        struct dirent* entry;
        while ((entry = readdir(procDir)) != nullptr) {
            if (entry->d_type == DT_DIR) {
                // Check if the directory name is a valid process ID
                
                int pid = atoi(entry->d_name);
                if (pid > 0) {
                    quantity += this->getByPid(pid);
                }
            }
        }

        closedir(procDir);
        return quantity;
    }

    int getCurrent() {
        return this->getByPid(getpid());
    }
};

int main() {
    // Server server("127.0.0.1", 8081);
    // ActiveThreadsCount counter()

    // server.addRoute("minecraft", [](){return "123";});
    // server.startListen();

    Server server("127.0.0.1", 8081);
    ActiveThreadsCount counter = ActiveThreadsCount();

    std::function<std::string(void)> all = [&counter](){return std::to_string(counter.getAll());};
    std::function<std::string(void)> current = [&counter](){return std::to_string(counter.getCurrent());};


    server.addRoute("all", all);
    server.addRoute("current", current);
    server.startListen();


    return 0;
}