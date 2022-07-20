cmake_minimum_required(VERSION 3.12)
project(kcp_aio)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/kcp)

# add the executable
add_executable(kcp_aio_test KCPClient.h KCPClient.cpp main.cpp)
target_link_libraries(kcp_aio_test PUBLIC kcp wsock32 ws2_32) 
