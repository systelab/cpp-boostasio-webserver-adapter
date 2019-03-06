[![Build Status](https://travis-ci.org/systelab/cpp-boostasio-webserver-adapter.svg?branch=master)](https://travis-ci.org/systelab/cpp-boostasio-webserver-adapter)
[![Build status](https://ci.appveyor.com/api/projects/status/qdrjivrs26gf46lo?svg=true)](https://ci.appveyor.com/project/systelab/cpp-boostasio-webserver-adapter)
[![codecov](https://codecov.io/gh/systelab/cpp-boostasio-webserver-adapter/branch/master/graph/badge.svg)](https://codecov.io/gh/systelab/cpp-boostasio-webserver-adapter)

# C++ Web Server Adapter implementation for Boost Asio

## Build

The easiest way to deal with the cpp-boostasio-webserver-adapter is just open the "CMakeLists.txt" with your QtCreator. However, since it is a CMake project, you don't really need an "IDE" to work with. 

You can build the project from source directly from command line following these steps:  

(In cpp-boostasio-webserver-adapter folder)  

	$mkdir build  
	$cd build  
	$cmake ..

Windows:  

	-	CMake will create a Visual Studio Solution from where you can easily modify and compile your project.  

POSIX:  
	-	CMake will create a Makefile from where you can build your binaries by simply calling "$make" command.  
