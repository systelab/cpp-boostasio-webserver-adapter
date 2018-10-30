# configure cpp-boostasio-webserver-adapter as library
set(CPP_BOOSTASIO_ADAPTER cpp-boostasio-webserver-adapter)
file(GLOB_RECURSE CPP_BOOSTASIO_ADAPTER_SRC "${CPP_BOOSTASIO_ADAPTER}/*.cpp")
file(GLOB_RECURSE CPP_BOOSTASIO_ADAPTER_HDR "${CPP_BOOSTASIO_ADAPTER}/*.h")
add_library(${CPP_BOOSTASIO_ADAPTER} STATIC ${CPP_BOOSTASIO_ADAPTER_SRC} ${CPP_BOOSTASIO_ADAPTER_HDR})

# configure demo
set(DEMO demo)

file(GLOB DEMO_SRC "${DEMO}/main.cpp")

add_executable(demo ${DEMO_SRC})
