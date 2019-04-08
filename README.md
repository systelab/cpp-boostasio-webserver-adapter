[![Build Status](https://travis-ci.org/systelab/cpp-boostasio-webserver-adapter.svg?branch=master)](https://travis-ci.org/systelab/cpp-boostasio-webserver-adapter)
[![Build status](https://ci.appveyor.com/api/projects/status/ea1h2dl6cxgmekv2?svg=true)](https://ci.appveyor.com/project/systelab/cpp-boostasio-webserver-adapter)
[![codecov](https://codecov.io/gh/systelab/cpp-boostasio-webserver-adapter/branch/master/graph/badge.svg)](https://codecov.io/gh/systelab/cpp-boostasio-webserver-adapter)
[![Codacy Badge](https://api.codacy.com/project/badge/Grade/934c47b9e7ca484692b1c5b2976808b9)](https://www.codacy.com/app/systelab/cpp-boostasio-webserver-adapter?utm_source=github.com&amp;utm_medium=referral&amp;utm_content=systelab/cpp-boostasio-webserver-adapter&amp;utm_campaign=Badge_Grade)
[![Download](https://api.bintray.com/packages/systelab/conan/BoostAsioWebServerAdapter:systelab/images/download.svg)](https://bintray.com/systelab/conan/BoostAsioWebServerAdapter:systelab/_latestVersion)

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

### Download using Conan

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

Initialize this libraryr by creating an instance of the `systelab::web_server::boostasio::ServerFactory` class:

```cpp
#include "BoostAsioWebServerAdapter/ServerFactory.h"

std::unique_ptr<systelab::web_server::IServerFactory> serverFactory = 
    std::make_unique<systelab::web_server::boostasio::ServerFactory>();
```

Use the `systelab::web_server::IServerFactory` interface of the created object to access to the web server features as described on [C++ Web Server Adapter](https://github.com/systelab/cpp-webserver-adapter) documentation.
