cmake_minimum_required(VERSION 3.2)

# Find external dependencides
set(BOOST_PACKAGE_NAME boost)
find_package(${BOOST_PACKAGE_NAME})
if (NOT TARGET ${BOOST_PACKAGE_NAME}::${BOOST_PACKAGE_NAME})
    set(BOOST_PACKAGE_NAME Boost)
    find_package(${BOOST_PACKAGE_NAME})
endif()

find_package(OpenSSL)
find_package(WebServerAdapterInterface)

# Configure preprocessor definitions
add_compile_options(-DBOOST_ALL_DYN_LINK)
add_compile_options(-DBOOST_CONFIG_SUPPRESS_OUTDATED_MESSAGE)

# Add project folder into includes
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Configure BoostAsioWebServerAdapter static library
set(BOOST_ASIO_WEB_SERVER_ADAPTER BoostAsioWebServerAdapter)
file(GLOB_RECURSE BOOST_ASIO_WEB_SERVER_ADAPTER_SRC "*.cpp")
file(GLOB_RECURSE BOOST_ASIO_WEB_SERVER_ADAPTER_HDR "*.h")
add_library(${BOOST_ASIO_WEB_SERVER_ADAPTER} STATIC ${BOOST_ASIO_WEB_SERVER_ADAPTER_SRC} ${BOOST_ASIO_WEB_SERVER_ADAPTER_HDR})
target_link_libraries(${BOOST_ASIO_WEB_SERVER_ADAPTER} WebServerAdapterInterface::WebServerAdapterInterface ${BOOST_PACKAGE_NAME}::${BOOST_PACKAGE_NAME} OpenSSL::OpenSSL)

#Configure source groups
foreach(FILE ${BOOST_ASIO_WEB_SERVER_ADAPTER_SRC} ${BOOST_ASIO_WEB_SERVER_ADAPTER_HDR}) 
    get_filename_component(PARENT_DIR "${FILE}" DIRECTORY)
    string(REPLACE "${CMAKE_CURRENT_SOURCE_DIR}" "" GROUP "${PARENT_DIR}")
    string(REPLACE "/" "\\" GROUP "${GROUP}")

    if ("${FILE}" MATCHES ".*\\.cpp")
       set(GROUP "Source Files${GROUP}")
    elseif("${FILE}" MATCHES ".*\\.h")
       set(GROUP "Header Files${GROUP}")
    endif()

    source_group("${GROUP}" FILES "${FILE}")
endforeach()
