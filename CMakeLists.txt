cmake_minimum_required(VERSION 2.8)
project(libuv_demo)

link_directories("../lib")
include_directories("include"
    "thirdparty/libuv")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
FILE(GLOB SRC "src/*.cpp")
add_executable(test ${SRC})