from conan import ConanFile
from conan.tools.microsoft.visual import is_msvc

class Archimedes(ConanFile):
    name = "Archimedes"
    settings = "build_type"
    
    def requirements(self):
        self.requires("glfw/3.3.8")
        self.requires("glm/0.9.9.8")
        self.requires("assimp/5.2.2")
        self.requires("spdlog/1.12.0")
        self.requires("gtest/1.13.0")
        self.requires("stb/cci.20220909")
        self.requires("draco/1.5.6", override=True)

    def configure(self):
        if is_msvc(self, True):
            self.options["spdlog/1.12.0"].use_std_fmt = True
    
    generators = "CMakeDeps", "CMakeToolchain"
