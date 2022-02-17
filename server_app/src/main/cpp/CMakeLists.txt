cmake_minimum_required(VERSION 3.15)
project(c_socket CXX)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

add_library(csocket
    SHARED
        CSocket.c
        foobar.cpp
)

find_library( log-lib
              log
)

# Specifies libraries CMake should link to your target library. You
# can link multiple libraries, such as libraries you define in this
# build script, prebuilt third-party libraries, or system libraries.

target_link_libraries( csocket
                       ${log-lib}
)
