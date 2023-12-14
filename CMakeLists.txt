cmake_minimum_required(VERSION 3.27)
project(ServerClientCpp)

set(CMAKE_CXX_STANDARD 14)
include_directories(.)
include_directories(client)
include_directories(library)
include_directories(library/client)
include_directories(library/server)

add_executable(ServerClientCpp
        client/client.h
        library/client/client.h
        library/server/LinuxServer.h
        library/server/ServerBase.h
        library/server/ServerFactory.h
        library/server/WindowsServer.h
        src/windows/second_server.cpp)

target_link_libraries(ServerClientCpp ws2_32)
