cmake_minimum_required(VERSION 3.5)
project(mandelbrot-generator C)
set(CMAKE_C_STANDARD 99)

set(CMAKE_C_FLAGS_DEBUG   "-DLOG_LEVEL=2 -g -Wall -Wextra -Werror")
set(CMAKE_C_FLAGS_RELEASE "-O2")

link_libraries(m png pthread)
file(GLOB SOURCES src/*.c)

add_executable(mandelbrot-generator ${SOURCES})
