# ConanFile for developing archimedes

from conan import ConanFile
from conan.tools.microsoft.visual import is_msvc
from conan.tools.cmake import cmake_layout
import re

def read_version() -> str:
	version = open('version.txt').read().strip()
	if not re.search("^[0-9][0-9][0-9][0-9]\.[0-9][0-9]\.[0-9][0-9]$", version):
		raise ValueError(f'\'{version}\' is not a valid version')
	return version

class ArchimedesConan(ConanFile):
	name = 'archimedes'
	version = open('version.txt').read()
	license = 'Apache-2.0'
	url = 'https://github.com/AGH-Code-Industry/archimedes'
	description = 'Archimedes Game Engine, @AGH Code Industry'
	settings = 'os', 'compiler', 'build_type', 'arch'
	generators = 'CMakeDeps', 'CMakeToolchain'

	def requirements(self):
		self.requires('glfw/3.4')
		self.requires('glm/0.9.9.8')
		self.requires('spdlog/1.12.0')
		self.requires('gtest/1.13.0')
		self.requires('stb/cci.20230920')
		self.requires('draco/1.5.6', override=True)
		self.requires('ctre/3.9.0')
		self.requires('openssl/3.3.2')
		self.requires('jsoncpp/1.9.6')

		# Audio system
		self.requires('openal-soft/1.23.1')
		self.requires('libsndfile/1.2.2')

		self.requires('freetype/2.13.2')
		self.requires('msdf-atlas-gen/1.3')

		# Vulkan SDK
		self.requires('volk/1.3.268.0')
		self.requires('vulkan-headers/1.4.313.0', override=True)
		self.requires('vulkan-memory-allocator/cci.20231120')
		self.requires('nvrhi/2025.08.19')

		# SPIRV (Shader compiler)
		self.requires('shaderc/2023.6')

	def configure(self):
		self.options['spdlog/1.12.0'].use_std_fmt = True

		if self.settings.os == 'Linux':
			self.options['glfw/3.4'].with_x11 = True
			self.options['glfw/3.4'].with_wayland = True
			
	def layout(self):
		cmake_layout(self)
