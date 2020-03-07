cmake_minimum_required(VERSION 3.15)
project(throttle)

set(CMAKE_CXX_STANDARD 14)

add_executable(throttle main.cpp Throttle.h Throttle.cpp)

find_package(Threads REQUIRED)
target_link_libraries(throttle Threads::Threads)