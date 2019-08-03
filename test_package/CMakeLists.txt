cmake_minimum_required(VERSION 3.2)

project(BoostAsioWebServerAdapterPackageTestProject)

# Configure environment
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../cmake ${CMAKE_BINARY_DIR})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Find external dependencies
find_package(BoostAsioWebServerAdapter)

# Configure test package project
set(BOOSTASIO_WEBSERVER_ADAPTER_PACKAGE_TEST_PROJECT BoostAsioWebServerAdapterPackageTest)
add_executable(${BOOSTASIO_WEBSERVER_ADAPTER_PACKAGE_TEST_PROJECT} BoostAsioWebServerAdapterExample.cpp)
target_link_libraries(${BOOSTASIO_WEBSERVER_ADAPTER_PACKAGE_TEST_PROJECT} BoostAsioWebServerAdapter::BoostAsioWebServerAdapter)

# Register tests
enable_testing()
add_test(NAME BoostAsioWebServerAdapterPackageTest COMMAND ${BOOSTASIO_WEBSERVER_ADAPTER_PACKAGE_TEST_PROJECT})
