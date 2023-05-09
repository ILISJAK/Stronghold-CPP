cmake_minimum_required(VERSION 3.1)
project(StrongholdAPI)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(cpprestsdk REQUIRED)

add_executable(StrongholdAPI main.cpp)
target_link_libraries(StrongholdAPI PRIVATE cpprestsdk::cpprest)
