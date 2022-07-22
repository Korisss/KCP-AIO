cmake_minimum_required(VERSION 3.12)
project(kcp_aio)

include(ExternalProject)

set(EXTERNAL_INSTALL_LOCATION ${CMAKE_BINARY_DIR}/external)

ExternalProject_Add(kcp_lib
    GIT_REPOSITORY https://github.com/skywind3000/kcp
    CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${EXTERNAL_INSTALL_LOCATION}
)

include_directories(${EXTERNAL_INSTALL_LOCATION}/include)
link_directories(${EXTERNAL_INSTALL_LOCATION}/lib)

add_library(udp_client UDPClient.h UDPClient.cpp)
target_link_libraries(udp_client wsock32 ws2_32)

add_library(kcp_aio KCPClient.h KCPClient.cpp)
add_dependencies(kcp_aio kcp_lib)
target_link_libraries(kcp_aio udp_client kcp)

install(FILES KCPClient.h DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

install(TARGETS kcp_aio
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

add_executable(kcp_aio_test main.cpp)
target_link_libraries(kcp_aio_test kcp_aio)
