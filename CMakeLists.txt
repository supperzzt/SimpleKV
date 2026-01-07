cmake_minimum_required(VERSION 3.10)
project(SimpleKV)

set(CMAKE_CXX_STANDARD 14)

include_directories(include)

# 编译主测试程序
add_executable(simple_kv_test tests/main.cpp)
