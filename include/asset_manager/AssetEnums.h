#pragma once

#include <shaderc/shaderc.hpp>

namespace arch::assetManager {

enum class ShaderOptimizationMode {
	NONE = shaderc_optimization_level_zero,
	PERFORMANCE = shaderc_optimization_level_performance,
	SIZE = shaderc_optimization_level_size
};

enum class ShaderType {
	VERTEX = shaderc_glsl_vertex_shader,
	FRAGMENT = shaderc_glsl_fragment_shader,
	COMPUTE = shaderc_glsl_compute_shader
};

enum class ShaderSourceLanguage {
	GLSL = shaderc_source_language_glsl,
	HLSL = shaderc_source_language_hlsl
};

}