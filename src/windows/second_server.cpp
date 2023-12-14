//
// Created by user on 14.12.2023.
//
#include "library/server/WindowsServer.h"
#include <string>
#include <winuser.h>
#include <sstream>

std::string getCursorSize() {
    int cursorWidth = GetSystemMetrics(SM_CXCURSOR);
    int cursorHeight = GetSystemMetrics(SM_CYCURSOR);

    std::stringstream ss;
    ss << cursorWidth << ' ' << cursorHeight;
    return ss.str();
}

std::string getMouseButtonsQuantity() {
    return std::to_string(GetSystemMetrics(SM_CMOUSEBUTTONS));
}


int main() {
    WindowServer server = WindowServer("0.0.0.0", 8080);
    server.addRoute("cursor_size", [](){return getCursorSize();});
    server.addRoute("mouse_buttons", [](){return getMouseButtonsQuantity();});
    server.startListen();
    return 0;
}