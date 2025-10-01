# ConanFile for packaging archimedes

from conan import ConanFile
from conan.tools.microsoft.visual import is_msvc
from conan.tools.cmake import CMake, CMakeToolchain, cmake_layout
import os
from pathlib import Path
import re

# while packaging, this file is in 'e' directory
# meanwhile, the version.txt is in 'es' directory
def read_version() -> str:
	version_path = Path.cwd() / 'version.txt'
	if not version_path.exists():
		version_path = Path.cwd().parent / 'es/version.txt'
	version = version_path.open().read().strip()
	if not re.search("^[0-9]+\.[0-9]+\.[0-9]+$", version):
		raise ValueError(f'\'{version}\' is not a valid version')
	return version

class ArchimedesConan(ConanFile):
	name = 'archimedes'
	version = read_version()
	license = 'Apache-2.0'
	url = 'https://github.com/AGH-Code-Industry/archimedes'
	description = 'Archimedes Game Engine, @AGH Code Industry'
	settings = 'os', 'compiler', 'build_type', 'arch'

	options = {
		'shared': [True, False],
		'install': [True, False],
		'build_tests': [True, False],
		'build_examples': [True, False],
	}

	default_options = {
		'shared': False,
		'install': True,
		'build_tests': False,
		'build_examples': False,
	}

	package_type = 'library'
	exports_sources = (
		'cmake/*',
		'src/*',
		'examples/*',
		'include/*',
		'src/*',
		'test/*',
		'CMakeLists.txt',
		'conanfile_dev.py',
		'conanfile.py',
		'version.txt',
	)
	generators = 'CMakeDeps'
	
	def layout(self):
		cmake_layout(self)

	def requirements(self):
		self.requires('glfw/3.4')
		self.requires('glm/0.9.9.8', transitive_headers=True)
		self.requires('spdlog/1.12.0')
		self.requires('gtest/1.13.0')
		self.requires('stb/cci.20230920', transitive_headers=True)
		self.requires('draco/1.5.6', override=True)
		self.requires('ctre/3.9.0', transitive_headers=True)
		self.requires('openssl/3.3.2')
		self.requires('jsoncpp/1.9.6')

		# Audio system
		self.requires('openal-soft/1.23.1')
		self.requires('libsndfile/1.2.2')

		# Font atlas generator
		self.requires('msdf-atlas-gen/1.3')
		self.requires('freetype/2.13.2')

		# Vulkan SDK
		self.requires('volk/1.3.268.0')
		self.requires('vulkan-headers/1.4.313.0', override=True, transitive_headers=True)
		self.requires('vulkan-memory-allocator/cci.20231120', transitive_headers=True)
		self.requires('nvrhi/2025.08.19')

		# SPIRV (Shader compiler)
		self.requires('shaderc/2023.6')

	def configure(self):
		self.options['spdlog/1.12.0'].use_std_fmt = True

		if self.settings.os == 'Linux': # use both X11 and wayland in binaries
			self.options['glfw/3.4'].with_x11 = True
			self.options['glfw/3.4'].with_wayland = True

	def build(self):
		cmake = CMake(self)
		cmake.configure(variables={
			'CMAKE_BUILD_TYPE': self.settings.build_type,
			'CMAKE_C_COMPILER': os.environ.get("CC"),
			'CMAKE_CXX_COMPILER': os.environ.get("CXX")
		})
		cmake.build()

	def generate(self):
		tc = CMakeToolchain(self, 'Ninja')
		tc.variables['ARCHIMEDES_SHARED'] = self.options.shared
		tc.variables['ARCHIMEDES_INSTALL'] = self.options.install
		tc.variables['ARCHIMEDES_BUILD_TESTS'] = self.options.build_tests
		tc.variables['ARCHIMEDES_BUILD_EXAMPLES'] = self.options.build_examples
		tc.generate()

	def package(self):
		cmake = CMake(self)
		cmake.install()

	def package_info(self):
		self.cpp_info.libs = ['archimedes']

		self.cpp_info.defines = []
		
		self.cpp_info.defines.append('SPDLOG_USE_STD_FORMAT=1')

		version_major, version_minor, version_patch = self.version.split('.')
		self.cpp_info.defines.append(f'ARCHIMEDES_VERSION_MAJOR={version_major}')
		self.cpp_info.defines.append(f'ARCHIMEDES_VERSION_MINOR={version_minor}')
		self.cpp_info.defines.append(f'ARCHIMEDES_VERSION_PATCH={version_patch}')

		self.cpp_info.defines.append(f'ARCHIMEDES_DEBUG={1 if self.settings.build_type == "Debug" else 0}')
		self.cpp_info.defines.append(f'ARCHIMEDES_RELEASE={0 if self.settings.build_type == "Debug" else 1}')

		self.cpp_info.defines.append(f'ARCHIMEDES_WINDOWS={1 if self.settings.os == "Windows" else 0}')
		self.cpp_info.defines.append(f'ARCHIMEDES_LINUX={1 if self.settings.os == "Linux" else 0}')
		self.cpp_info.defines.append(f'ARCHIMEDES_UNIX={1 if self.settings.os == "Linux" else 0}')
