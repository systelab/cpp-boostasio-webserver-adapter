[![Build Status](https://travis-ci.org/systelab/cpp-boostasio-webserver-adapter.svg?branch=master)](https://travis-ci.org/systelab/cpp-boostasio-webserver-adapter)
[![Build status](https://ci.appveyor.com/api/projects/status/ea1h2dl6cxgmekv2?svg=true)](https://ci.appveyor.com/project/systelab/cpp-boostasio-webserver-adapter)
[![codecov](https://codecov.io/gh/systelab/cpp-boostasio-webserver-adapter/branch/master/graph/badge.svg)](https://codecov.io/gh/systelab/cpp-boostasio-webserver-adapter)
[![Codacy Badge](https://api.codacy.com/project/badge/Grade/934c47b9e7ca484692b1c5b2976808b9)](https://www.codacy.com/app/systelab/cpp-boostasio-webserver-adapter?utm_source=github.com&amp;utm_medium=referral&amp;utm_content=systelab/cpp-boostasio-webserver-adapter&amp;utm_campaign=Badge_Grade)

# C++ Web Server Adapter implementation for Boost Asio

This repository implements the interface for the [C++ Web Server Adapter](https://github.com/systelab/cpp-webserver-adapter) using [Boost Asio](https://www.boost.org/doc/libs/1_67_0/doc/html/boost_asio.html).

## Setup

### Build from sources

Prerequisites:
  - [Git](https://git-scm.com/)
  - [Conan](https://conan.io/)
  - [CMake](https://cmake.org/)
  - [Visual Studio](https://visualstudio.microsoft.com/) (only on Windows)
  - [GCC](https://gcc.gnu.org/) (only on Linux)

Build library with the following steps:
  1. Clone this repository in a local drive
  2. Make a build directory (i.e. `build/`)
  3. Install `conan` dependencies in the build directory
  4. Run `cmake` in the build directory to configure build targets
  5. Use `Visual Studio` (on Windows) or `make` (on Linux) to build the library

#### Windows
``` bash
> git clone https://github.com/systelab/cpp-boostasio-webserver-adapter
> md build && cd build
> conan install .. -s arch=x86
> cmake ..
> devenv.exe BoostAsioWebServerAdapter.sln
```

#### Linux
``` bash
> git clone https://github.com/systelab/cpp-boostasio-webserver-adapter
> mkdir build && cd build
> conan install ..
> cmake .. -DCMAKE_BUILD_TYPE=[Debug | Coverage | Release]
> make
```

### Download using Conan (not available yet)

  1. Create/update your `conanfile.txt` to add this library as follows:

```
[requires]
BoostAsioWebServerAdapter/1.0.0@systelab/stable

[generators]
cmake
```

  2. Integrate Conan into CMake by adding the following code into your `CMakeLists.txt`:

```cmake
include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()
```

  3. Link against `${CONAN_LIBS}` when configuring your executables in CMake:

```cmake
set(MY_PROJECT MyProject)
add_executable(${MY_PROJECT} main.cpp)
target_link_libraries(${MY_PROJECT} ${CONAN_LIBS})
```

## Usage

### HTTP server

Set up a new HTTP web server by providing a configuration object that specifies host address and port:

```cpp
systelab::web_server::Configuration configuration;
configuration.setHostAddress("127.0.0.1");
configuration.setPort(8080);

std::unique_ptr<systelab::web_server::IWebServer> webServer =
    std::make_unique<systelab::web_server::boostasio::WebServer>(configuration);
```

Then, register at least a web service by providing an instance of a class that implements the `systelab::web_server::IWebService` interface:

```cpp
class MyWebService : public systelab::web_server::IWebService
{
    std::unique_ptr<systelab::web_server::Reply> execute(const systelab::web_server::Request& request)
    {
        std::unique_ptr<systelab::web_server::Reply> reply;
	
        if (canServiceHandleRequest(request))
        {
	     // Process request and build a reply
        }
	
        return reply;
    }
}

auto webService = std::make_unique<MyWebService>();
webServer->registerWebService(std::move(webService));
```

Finally, start the server calling the `start()` method:

```cpp
webServer->start();
```

See documentation of [C++ Web Server Adapter](https://github.com/systelab/cpp-webserver-adapter) for additional configuration options or for further details about how to implement a web service.

### HTTPS server

`Not available yet`
