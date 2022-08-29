cmake_minimum_required(VERSION 3.12)
project(kcp_aio)

include(ExternalProject)
include(GNUInstallDirs)

ExternalProject_Add(kcp-lib
    GIT_REPOSITORY https://github.com/skywind3000/kcp
    CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${EXTERNAL_INSTALL_LOCATION}
)

include_directories(${EXTERNAL_INSTALL_LOCATION}/include)
link_directories(${EXTERNAL_INSTALL_LOCATION}/lib)

add_library(udp-client UDPClient.h UDPClient.cpp)
target_link_libraries(udp-client wsock32 ws2_32)

add_library(kcp-aio KCPClient.h KCPClient.cpp)
add_dependencies(kcp-aio kcp-lib)
target_link_libraries(kcp-aio udp-client kcp)

add_executable(kcp-aio-test main.cpp)
target_link_libraries(kcp-aio-test kcp-aio)

install(FILES UDPClient.h DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install(TARGETS udp-client DESTINATION ${CMAKE_INSTALL_LIBDIR} EXPORT find-udp-client)
install(EXPORT find-udp-client DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/udp-client)

install(FILES KCPClient.h DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install(TARGETS kcp-aio DESTINATION ${CMAKE_INSTALL_LIBDIR} EXPORT find-kcp-aio)
install(EXPORT find-kcp-aio DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/kcp-aio)
