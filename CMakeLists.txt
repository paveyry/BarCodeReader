cmake_minimum_required(VERSION 3.5)
project(BarCodeReader)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -Wall -Wextra -pedantic")

file(GLOB_RECURSE SOURCES "src/*")
include_directories("src")

add_executable(BarCodeReader ${SOURCES} src/main.cc)