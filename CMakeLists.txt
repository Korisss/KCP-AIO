cmake_minimum_required(VERSION 3.12)
project(kcp_aio)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/kcp)

add_library(udp_client UDPClient.h UDPClient.cpp)
target_link_libraries(udp_client wsock32 ws2_32)

add_library(kcp_aio KCPClient.h KCPClient.cpp)
target_link_libraries(kcp_aio udp_client kcp)

add_executable(kcp_aio_test main.cpp)
target_link_libraries(kcp_aio_test kcp_aio)
