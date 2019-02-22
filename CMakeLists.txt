cmake_minimum_required(VERSION 3.2)

# Configure project
project(cpp-boostasio-webserver-adapter)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(conan.cmake)
conan_cmake_run(REQUIRES boost/1.67.0@conan/stable
                BASIC_SETUP
                BUILD missing)

# Include interface
include_directories(${CMAKE_SOURCE_DIR}/interface)

# Add src
add_subdirectory(${CMAKE_SOURCE_DIR}/src)
