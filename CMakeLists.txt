cmake_minimum_required(VERSION 3.10)

# Set the project name and version
project(StrongholdCPP VERSION 1.0)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Find and include the cpprestsdk package
find_package(cpprestsdk REQUIRED)

# Add all the source files to the SOURCES variable
file(GLOB_RECURSE SOURCES "src/*.cpp")

# Create the executable target and link the libraries
add_executable(StrongholdCPP ${SOURCES})
target_link_libraries(StrongholdCPP PRIVATE cpprestsdk::cpprest)

# Add the include directory
target_include_directories(StrongholdCPP PRIVATE include)

include_directories(${CMAKE_CURRENT_SOURCE_DIR} C:/vcpkg/installed/x64-windows/include)
link_directories(C:/vcpkg/installed/x64-windows/lib)

#output dir
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

#fastcgi++
include_directories("D:\\dev\\repos\\projekt\\Stronghold++\\bin\\fastcgipp\\include")

#boost
set(Boost_INCLUDE_DIR "D:\\dev\\repos\\projekt\\Stronghold++\\bin\\boost_1_82_0")
set(Boost_LIBRARY_DIR "D:\\dev\\repos\\projekt\\Stronghold++\\bin\\boost_1_82_0/stage/lib")
include_directories(${Boost_INCLUDE_DIR})
link_directories(${Boost_LIBRARY_DIR})