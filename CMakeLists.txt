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
