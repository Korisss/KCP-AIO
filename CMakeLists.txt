cmake_minimum_required(VERSION 3.12)
project(kcp_aio)

include(ExternalProject)
include(GNUInstallDirs)

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

install(FILES UDPClient.h DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

install(TARGETS udp_client
    EXPORT udp_client_targets
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(EXPORT udp_client_targets
    FILE udp-client-config.cmake
    NAMESPACE udp_client::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/udp_client
)

install(FILES KCPClient.h DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

install(TARGETS kcp_aio
    EXPORT kcp_aio_targets
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(EXPORT kcp_aio_targets
    FILE kcp-aio-config.cmake
    NAMESPACE kcp_aio::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/kcp_aio
)

add_executable(kcp_aio_test main.cpp)
target_link_libraries(kcp_aio_test kcp_aio)
