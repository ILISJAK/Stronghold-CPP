cmake_minimum_required(VERSION 3.9)

# Integrate vcpkg with the project
list(APPEND CMAKE_PREFIX_PATH "C:/vcpkg/packages/zlib_x64-windows/share/zlib")
list(APPEND CMAKE_PREFIX_PATH "C:/vcpkg/packages/cpprestsdk_x64-windows/share/cpprestsdk")
set(CMAKE_TOOLCHAIN_FILE "C:/vcpkg/scripts/buildsystems/vcpkg.cmake" CACHE STRING "Vcpkg toolchain file")

# Set the project name and version
project(StrongholdCPP VERSION 1.0)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Find and include the cpprestsdk (and ZLIB) package 
set(ZLIB_ROOT "C:/vcpkg/installed/x64-windows")
find_package(ZLIB REQUIRED)
set(unofficial-brotli_DIR "C:/vcpkg/installed/x64-windows/share/unofficial-brotli")
find_package(unofficial-brotli REQUIRED)
find_package(cpprestsdk REQUIRED)

# Add all the source files to the SOURCES variable
file(GLOB_RECURSE SOURCES "src/*.cpp" "api/*.cpp")

# Create the executable target and link the libraries
add_executable(StrongholdCPP ${SOURCES})

# Find brotli libraries
find_library(BROTLI_LIB 
    NAMES brotlidec brotlienc brotlicommon 
    PATHS "C:/vcpkg/installed/x64-windows/lib"
    NO_DEFAULT_PATH
)

target_link_libraries(StrongholdCPP PRIVATE cpprestsdk::cpprest ${BROTLI_LIB})

# Add the include directory
target_include_directories(StrongholdCPP PRIVATE include api)

include_directories(${CMAKE_CURRENT_SOURCE_DIR} C:/vcpkg/installed/x64-windows/include)
link_directories(C:/vcpkg/installed/x64-windows/lib)

#output dir
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
