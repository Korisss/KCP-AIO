# KCP-AIO

All-in-one kcp client. Inspired by libkcp by xtaci. Works only with Windows.

## Cmake include example

```cmake
cmake_minimum_required(VERSION 3.12)
project(test)

include(FetchContent)

FetchContent_Declare(kcp-aio
    GIT_REPOSITORY https://github.com/Korisss/KCP-AIO
)

include_directories(${EXTERNAL_INSTALL_LOCATION}/include)

FetchContent_MakeAvailable(kcp-aio)

add_executable(test-test main.cpp)
target_link_libraries(test-test kcp-aio)
```
