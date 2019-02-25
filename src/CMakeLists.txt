# configure cpp-boostasio-webserver-adapter as library
set(CPP_BOOSTASIO_ADAPTER cpp-boostasio-webserver-adapter)
file(GLOB_RECURSE CPP_BOOSTASIO_ADAPTER_SRC "${CPP_BOOSTASIO_ADAPTER}/*.cpp")
file(GLOB_RECURSE CPP_BOOSTASIO_ADAPTER_HDR "${CPP_BOOSTASIO_ADAPTER}/*.h")
add_library(${CPP_BOOSTASIO_ADAPTER} STATIC ${CPP_BOOSTASIO_ADAPTER_SRC} ${CPP_BOOSTASIO_ADAPTER_HDR})
target_link_libraries(${CPP_BOOSTASIO_ADAPTER} ${CONAN_LIBS})

# configure demo
set(DEMO demo)

file(GLOB DEMO_SRC "${DEMO}/main.cpp")

# add interface source code to demo project
file(GLOB_RECURSE INTERFACE_SRC "${CMAKE_SOURCE_DIR}/interface/*.cpp")
file(GLOB_RECURSE INTERFACE_HDR "${CMAKE_SOURCE_DIR}/interface/*.h")

include_directories(${CMAKE_SOURCE_DIR}/src/cpp-boostasio-webserver-adapter)

add_executable(demo ${DEMO_SRC} ${INTERFACE_SRC} ${INTERFACE_HDR})

target_link_libraries(${DEMO} ${CPP_BOOSTASIO_ADAPTER})
