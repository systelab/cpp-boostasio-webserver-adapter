from conans import ConanFile, CMake

class BoostAsioWebServerAdapterTestConan(ConanFile):
    settings = "os", "compiler", "build_type", "arch"
    generators = "cmake_find_package"
    options = {"gtest": ["1.7.0", "1.8.1", "1.10.0"], "boost": ["1.66.0", "1.67.0", "1.72.0", "1.75.0"], "openssl": ["1.0.2n", "1.0.2s", "1.1.1g"]}
    default_options = {"gtest":"1.10.0", "boost":"1.72.0", "openssl":"1.1.1g"}

    def configure(self):
        self.options["BoostAsioWebServerAdapter"].gtest = self.options.gtest
        self.options["BoostAsioWebServerAdapter"].boost = self.options.boost
        self.options["BoostAsioWebServerAdapter"].openssl = self.options.openssl

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()

    def imports(self):
        self.copy("*.dll", dst=("bin/%s" % self.settings.build_type), src="bin")
        self.copy("*.dll", dst=("bin/%s" % self.settings.build_type), src="lib")
        self.copy("*.dylib*", dst=("bin/%s" % self.settings.build_type), src="lib")
        self.copy('*.so*', dst=("bin/%s" % self.settings.build_type), src='lib')

    def test(self):
        cmake = CMake(self)
        cmake.test()
