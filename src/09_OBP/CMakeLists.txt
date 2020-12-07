cmake_minimum_required(VERSION 2.6)

project(pas CXX)

set(CXX_FLAGS -g -Wall)
set(CMAKE_CXX_COMPILER "g++")

#添加头文件搜索路径
include_directories(/usr/local/Cellar/boost/1.74.0/include)

#添加库文件搜索路径
link_directories(/usr/local/Cellar/boost/1.74.0/lib)

string(REPLACE ";" " " CMAKE_CXX_FLAGS "${CXX_FLAGS}")

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)

add_executable(Thread_test Thread_test.cpp Thread.cpp)
target_link_libraries(Thread_test pthread)
add_executable(bf_test bf_test.cpp)
