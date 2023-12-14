#include "library/server/ServerBase.h"
#include "library/server/LinuxServer.h"
#include "library/server/WindowsServer.h"


class ServerFactory {
public:
    static ServerBase get(const char* ip, int port) {
        #ifdef _WIN32
            return WindowServer(ip, port);
        #else
            return LinuxServer(ip, port);
        #endif
    }
};