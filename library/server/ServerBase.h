#include <string>
#include <unordered_map>

#include <functional>

#include <thread>

#ifndef SERVERBASE_H
#define SERVERBASE_H
class ServerBase {
protected:
    std::unordered_map<std::string, std::function<std::string(void)>> routes;
public:
    void addRoute(std::string name, std::function<std::string(void)> func) {
        routes.insert(std::make_pair(name, func));
    }

    virtual void startListen() {}
};
#endif
