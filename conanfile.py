from conan import ConanFile

class Archimedes(ConanFile):
    name = "Archimedes"
    
    def requirements(self):
        self.requires("glad/0.1.36")
        self.requires("glfw/3.3.8")
        self.requires("glm/0.9.9.8")
        self.requires("assimp/5.2.2")
        self.requires("spdlog/1.11.0")
        self.requires("gtest/1.13.0")
        self.requires("stb/cci.20220909")

    def configure(self):
        self.options["glad/0.1.36"].gl_profile = "core"
        self.options["glad/0.1.36"].gl_version = "4.6"
        self.options["glad/0.1.36"].spec = "gl"
        self.options["glad/0.1.36"].no_loader = False

    settings = "build_type",
    
    generators = "CMakeDeps", "CMakeToolchain"
