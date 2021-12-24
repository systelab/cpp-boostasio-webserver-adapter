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
    options = {"gtest": ["1.7.0", "1.8.1", "1.10.0"], "boost": ["1.66.0", "1.67.0", "1.72.0", "1.75.0"], "openssl": ["1.0.2n", "1.0.2s", "1.1.1g", "1.1.1k"]}
    default_options = {"gtest":"1.10.0", "boost":"1.72.0", "openssl":"1.1.1k"}
    exports_sources = "*"

    def configure(self):
        self.options["WebServerAdapterTestUtilities"].gtest = self.options.gtest
        self.options["boost"].shared = True
        if self.options.openssl == "1.0.2n":
            self.options["OpenSSL"].shared = True
        else:
            self.options["openssl"].shared = True

    def requirements(self):
        self.requires("WebServerAdapterInterface/1.1.12@systelab/stable")

        if self.options.boost == "1.66.0":
            self.requires("boost/1.66.0@conan/stable")
        elif self.options.boost == "1.67.0":
            self.requires("boost/1.67.0@conan/stable")
        elif self.options.boost == "1.72.0":
            self.requires("boost/1.72.0")
        elif self.options.boost == "1.75.0":
            self.requires("boost/1.75.0")
        else:
            self.requires(("boost/%s") % self.options.boost)

        if self.options.openssl == "1.0.2n":
            self.requires("OpenSSL/1.0.2n@conan/stable")
        elif self.options.openssl == "1.1.1g":
            self.requires("openssl/1.1.1g")
            self.requires("zlib/1.2.11")
        else:
            self.requires(("openssl/%s") % self.options.openssl)
            self.requires("zlib/1.2.11")

    def build_requirements(self):
        self.build_requires("WebServerAdapterTestUtilities/1.1.12@systelab/stable")

        if self.options.gtest == "1.7.0":
            self.build_requires("gtest/1.7.0@systelab/stable")
        elif self.options.gtest == "1.8.1":
            self.build_requires("gtest/1.8.1")
        elif self.options.gtest == "1.10.0":
            self.build_requires("gtest/1.10.0")
        else:
            self.build_requires(("gtest/%s") % self.options.gtest)

    def build(self):
        cmake = CMake(self)
        cmake.configure(source_folder=".")
        cmake.build()

    def imports(self):
        self.copy("*.dll", dst=("bin/%s" % self.settings.build_type), src="lib")
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
