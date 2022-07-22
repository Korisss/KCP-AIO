cmake_minimum_required (VERSION 3.12)
project (kcp_aio)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/kcp)

add_executable (kcp_aio UDPClient.h UDPClient.cpp KCPClient.h KCPClient.cpp main.cpp)
target_link_libraries(kcp_aio PUBLIC kcp wsock32 ws2_32) 
