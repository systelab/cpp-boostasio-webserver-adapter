cmake_minimum_required(VERSION 3.2)

project(BoostAsioWebServerAdapterPackageTestProject)

# Configure environment
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Setup 3rd party libraries through conan
include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

# Configure test package project
set(BOOSTASIO_WEBSERVER_ADAPTER_PACKAGE_TEST_PROJECT BoostAsioWebServerAdapterPackageTest)
add_executable(${BOOSTASIO_WEBSERVER_ADAPTER_PACKAGE_TEST_PROJECT} BoostAsioWebServerAdapterExample.cpp)
target_link_libraries(${BOOSTASIO_WEBSERVER_ADAPTER_PACKAGE_TEST_PROJECT} ${CONAN_LIBS})