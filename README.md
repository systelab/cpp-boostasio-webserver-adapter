[![Build Status](https://travis-ci.org/systelab/cpp-boostasio-webserver-adapter.svg?branch=master)](https://travis-ci.org/systelab/cpp-boostasio-webserver-adapter)
[![Build status](https://ci.appveyor.com/api/projects/status/ea1h2dl6cxgmekv2?svg=true)](https://ci.appveyor.com/project/systelab/cpp-boostasio-webserver-adapter)
[![codecov](https://codecov.io/gh/systelab/cpp-boostasio-webserver-adapter/branch/master/graph/badge.svg)](https://codecov.io/gh/systelab/cpp-boostasio-webserver-adapter)
[![Codacy Badge](https://app.codacy.com/project/badge/Grade/37fdd9df4bb84747af04ad72128517c9)](https://www.codacy.com/gh/systelab/cpp-boostasio-webserver-adapter/dashboard?utm_source=github.com&amp;utm_medium=referral&amp;utm_content=systelab/cpp-boostasio-webserver-adapter&amp;utm_campaign=Badge_Grade)


# C++ Web Server Adapter implementation for Boost Asio

This repository implements the interface for the [C++ Web Server Adapter](https://github.com/systelab/cpp-webserver-adapter) using [Boost Asio](https://www.boost.org/doc/libs/1_67_0/doc/html/boost_asio.html).


## Setup

### Download using Conan

This library is designed to be installed by making use of [Conan](https://conan.io/) package manager. So, you just need to add the following requirement into your Conan recipe:

```python
def requirements(self):
   self.requires("BoostAsioWebServerAdapter/1.0.0@systelab/stable")
```

> Version number of this code snipped is set just as an example. Replace it for the desired package version to retrieve.

As this package is not available on the conan-center, you will also need to configure a remote repository before installing dependencies:

```bash
conan remote add systelab-public https://systelab.jfrog.io/artifactory/api/conan/cpp-conan-production-local
```

See Conan [documentation](https://docs.conan.io/en/latest/) for further details on how to integrate this package with your build system.

### Build from sources

See [BUILD.md](BUILD.md) document for details.


## Usage

Initialize this library by creating an instance of the `systelab::web_server::boostasio::ServerFactory` class:

```cpp
#include "BoostAsioWebServerAdapter/ServerFactory.h"

std::unique_ptr<systelab::web_server::IServerFactory> serverFactory = 
    std::make_unique<systelab::web_server::boostasio::ServerFactory>();
```

Use the `systelab::web_server::IServerFactory` interface of the created object to access to the web server features as described on [C++ Web Server Adapter](https://github.com/systelab/cpp-webserver-adapter) documentation.
