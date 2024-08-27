cmake_minimum_required(VERSION 3.12)
project(MyProject)

set(CMAKE_CXX_STANDARD 11)

# Add your source files here
set(SOURCES src/main.cpp)

add_executable(MyProject ${SOURCES})