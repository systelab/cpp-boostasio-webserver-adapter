from conans import ConanFile, tools

class BoostAsioWebServerAdapterConan(ConanFile):
    name = "BoostAsioWebServerAdapter"
    description = "Implementation of C++ Web Server based on Boost Asio"
    url = "https://github.com/systelab/cpp-boostasio-webserver-adapter"
    homepage = "https://github.com/systelab/cpp-boostasio-webserver-adapter"
    author = "CSW <csw@werfen.com>"
    topics = ("conan", "web", "server", "http", "ssl", "boost", "asio", "adapter")
    license = "MIT"
    generators = "cmake"
    settings = "os", "compiler", "build_type", "arch"
    default_options = "OpenSSL:shared=True", "boost:shared=True"

    def requirements(self):
        self.requires("WebServerAdapter/1.0.0@systelab/stable")
        self.requires("boost/1.67.0@conan/stable")
        self.requires("OpenSSL/1.0.2n@conan/stable")

    def imports(self):
        self.copy("*.dll", dst="bin", src="bin")
        self.copy("*.dylib*", dst="bin", src="lib")
        self.copy("*.so*", dst="bin", src="lib")

    def package(self):
        self.copy("ServerFactory.h", dst="include/BoostAsioWebServerAdapter", src="src/BoostAsioWebServerAdapter")
        self.copy("*BoostAsioWebServerAdapter.lib", dst="lib", keep_path=False)
        self.copy("*BoostAsioWebServerAdapter.pdb", dst="lib", keep_path=False)
        self.copy("*BoostAsioWebServerAdapter.a", dst="lib", keep_path=False)

    def package_info(self):
        self.cpp_info.libs = tools.collect_libs(self)
