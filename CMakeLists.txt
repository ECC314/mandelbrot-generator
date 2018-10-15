cmake_minimum_required(VERSION 3.5)
project(mandelbrot-generator C)
set(CMAKE_C_STANDARD 99)

set(CMAKE_C_FLAGS_DEBUG "-DMANDELBROT_DEBUG")

link_libraries(png m)
file(GLOB SOURCES src/*.c)

add_executable(mandelbrot-generator ${SOURCES})
