from conans import ConanFile, tools, CMake

class BoostAsioWebServerAdapterConan(ConanFile):
    name = "BoostAsioWebServerAdapter"
    description = "Implementation of C++ Web Server based on Boost Asio"
    url = "https://github.com/systelab/cpp-boostasio-webserver-adapter"
    homepage = "https://github.com/systelab/cpp-boostasio-webserver-adapter"
    author = "CSW <csw@werfen.com>"
    topics = ("conan", "web", "server", "http", "ssl", "boost", "asio", "adapter")
    license = "MIT"
    generators = "cmake_find_package"
    settings = "os", "compiler", "build_type", "arch"
    options = {"gtest": ["1.7.0", "1.8.1"], "boost": ["1.66.0", "1.67.0"], "OpenSSL": ["1.0.2n"]}
    default_options = {"gtest":"1.8.1", "boost":"1.67.0", "OpenSSL":"1.0.2n"}
    exports_sources = "*"

    def configure(self):
        self.options["WebServerAdapterTestUtilities"].gtest = self.options.gtest
        self.options["OpenSSL"].shared = True
        self.options["boost"].shared = True

    def requirements(self):
        self.requires("WebServerAdapterInterface/1.0.2@systelab/stable")
        self.requires(("boost/%s@conan/stable") % self.options.boost)
        self.requires(("OpenSSL/%s@conan/stable") % self.options.OpenSSL)

    def build_requirements(self):
        self.build_requires("WebServerAdapterTestUtilities/1.0.2@systelab/stable")
        if self.options.gtest == "1.7.0":
            self.build_requires("gtest/1.7.0@systelab/stable")
        else:
            self.build_requires("gtest/1.8.1@bincrafters/stable")

    def build(self):
        cmake = CMake(self)
        cmake.configure(source_folder=".")
        cmake.build()

    def imports(self):
        self.copy("*.dll", dst=("bin/%s" % self.settings.build_type), src="bin")
        self.copy("*.dylib*", dst=("bin/%s" % self.settings.build_type), src="lib")
        self.copy("*.so*", dst=("bin/%s" % self.settings.build_type), src="lib")

    def package(self):
        self.copy("ServerFactory.h", dst="include/BoostAsioWebServerAdapter", src="src/BoostAsioWebServerAdapter")
        self.copy("*BoostAsioWebServerAdapter.lib", dst="lib", keep_path=False)
        self.copy("*BoostAsioWebServerAdapter.pdb", dst="lib", keep_path=False)
        self.copy("*BoostAsioWebServerAdapter.a", dst="lib", keep_path=False)

    def package_info(self):
        self.cpp_info.libs = tools.collect_libs(self)
