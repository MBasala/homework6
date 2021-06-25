cmake_minimum_required(VERSION 3.19)
project(homework6)

set(CMAKE_CXX_STANDARD 20)

add_executable(homework6 main.cpp shell_wrap.cpp commands.cpp commands.h)