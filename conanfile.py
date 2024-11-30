from conan import ConanFile
from conan.tools.microsoft.visual import is_msvc


class Archimedes(ConanFile):
    name = "Archimedes"
    settings = "build_type"

    def requirements(self):
        print("##################")
        print(self.conf.get("tools.gnu:pkg_config", default=False, check_type=str))
        print("##################")
        self.tool_requires("pkgconf/2.2.0", override=True)
        self.requires("glfw/3.3.8")
        self.requires("glm/0.9.9.8")
        self.requires("assimp/5.2.2")
        self.requires("spdlog/1.12.0")
        self.requires("gtest/1.13.0")
        self.requires("stb/cci.20230920", override=True)
        self.requires("draco/1.5.6", override=True)

        
        # Audio system
        self.requires("openal-soft/1.23.1")
        self.requires("libsndfile/1.2.2")

        # Vulkan SDK
        self.requires("volk/1.3.268.0")
        self.requires("vulkan-headers/1.3.268.0", override=True)
        self.requires("vulkan-memory-allocator/cci.20231120")

        # SPIRV (Shader compiler)
        self.requires("shaderc/2023.6")  # waiting for conan repo update

    def configure(self):
        if is_msvc(self, True):
            self.options["spdlog/1.12.0"].use_std_fmt = True

    generators = "CMakeDeps", "CMakeToolchain"
