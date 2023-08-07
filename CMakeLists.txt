cmake_minimum_required(VERSION 3.26)
project(chat_room)

set(CMAKE_CXX_STANDARD 17)

add_executable(server server/server.cpp common/common.cpp common/tcp_server.cpp)
add_executable(client client/client.cpp common/common.cpp common/tcp_client.cpp)

target_link_libraries(server pthread)
target_link_libraries(client pthread)
