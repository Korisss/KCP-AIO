cmake_minimum_required(VERSION 3.12)
project(kcp_aio)

include(FetchContent)
include(GNUInstallDirs)

FetchContent_Declare(kcp
    GIT_REPOSITORY https://github.com/skywind3000/kcp
)

include_directories(${EXTERNAL_INSTALL_LOCATION}/include)

FetchContent_MakeAvailable(kcp)

add_library(udp-client UDPClient.h UDPClient.cpp)
target_link_libraries(udp-client wsock32 ws2_32)

install(FILES UDPClient.h DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install(TARGETS udp-client DESTINATION ${CMAKE_INSTALL_LIBDIR})

add_library(kcp-aio KCPClient.h KCPClient.cpp)
target_link_libraries(kcp-aio udp-client kcp)

install(FILES KCPClient.h DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install(TARGETS kcp-aio DESTINATION ${CMAKE_INSTALL_LIBDIR})

add_library(kcp-aio-shared SHARED KCPClient.h KCPClient.cpp)
target_link_libraries(kcp-aio-shared udp-client kcp)

add_executable(kcp-aio-test main.cpp)
target_link_libraries(kcp-aio-test kcp-aio)
