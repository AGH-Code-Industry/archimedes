#include "NvrhiPipeline.h"

#include <fstream>
#include <shaderc/shaderc.hpp>

#include "../NvrhiRenderer.h"
#include "../buffer/NvrhiBuffer.h"
#include "../exception/NvrhiException.h"
#include "../texture/NvrhiTexture.h"
#include "nvrhi/utils.h"

namespace arch::gfx::nvrhi::pipeline {

static std::string readFile(const std::string& path) {
	std::ifstream file(path);
	std::stringstream buffer;
	buffer << file.rdbuf();
	return buffer.str();
}

static shaderc::SpvCompilationResult compileShader(const std::string& shaderPath, shaderc_shader_kind shaderKind) {
	static shaderc::CompileOptions options;
	static shaderc::Compiler compiler;

	std::string shaderCode = readFile(shaderPath);

	auto result =
		compiler.CompileGlslToSpv(shaderCode.data(), shaderCode.size(), shaderKind, shaderPath.c_str(), options);

	if (result.GetCompilationStatus() != shaderc_compilation_status_success) {
		throw exception::NvrhiException(result.GetErrorMessage());
	}

	return result;
}

NvrhiPipeline::NvrhiPipeline(const Desc& desc, const WeakRef<NvrhiRenderer>& renderer): Pipeline(desc) {
	auto _renderer = renderer.lock();

	// todo: take this from shader reflection
	::nvrhi::VertexAttributeDesc attributes[] = { ::nvrhi::VertexAttributeDesc()
													  .setName("POSITION")
													  .setFormat(::nvrhi::Format::RGB32_FLOAT)
													  .setOffset(0)
													  .setElementStride(sizeof(float) * 5),
												  ::nvrhi::VertexAttributeDesc()
													  .setName("TEXCOORD")
													  .setFormat(::nvrhi::Format::RG32_FLOAT)
													  .setOffset(sizeof(float) * 3)
													  .setElementStride(sizeof(float) * 5) };

	auto vertexCode = compileShader(desc.vertexShaderPath, shaderc_glsl_vertex_shader);
	::nvrhi::ShaderHandle vertexShader = _renderer->getDevice()->createShader(
		::nvrhi::ShaderDesc(::nvrhi::ShaderType::Vertex),
		vertexCode.begin(),
		(u64)std::distance(vertexCode.begin(), vertexCode.end()) * sizeof(u32)
	);

	::nvrhi::InputLayoutHandle inputLayout =
		_renderer->getDevice()->createInputLayout(attributes, std::size(attributes), vertexShader);

	auto fragmentCode = compileShader(desc.fragmentShaderPath, shaderc_glsl_fragment_shader);
	::nvrhi::ShaderHandle pixelShader = _renderer->getDevice()->createShader(
		::nvrhi::ShaderDesc(::nvrhi::ShaderType::Pixel),
		fragmentCode.begin(),
		(u64)std::distance(fragmentCode.begin(), fragmentCode.end()) * sizeof(u32)
	);

	// todo: generate binding set layout from shader reflection
	// todo: move binding set creation to Material
	auto bindingSetDesc = ::nvrhi::BindingSetDesc();

	for (u64 i = 0; const auto& t : desc.textures) {
		auto texture = std::static_pointer_cast<texture::NvrhiTexture>(t);
		bindingSetDesc.addItem(::nvrhi::BindingSetItem::Texture_SRV(i, texture->getNativeHandle()))
			.addItem(::nvrhi::BindingSetItem::Sampler(i++, texture->getSampler()));
	}

	for (u64 i = 1; const auto& b : desc.buffers) {
		auto buffer = std::dynamic_pointer_cast<buffer::NvrhiBuffer>(b);
		bindingSetDesc.addItem(::nvrhi::BindingSetItem::ConstantBuffer(i++, buffer->getNativeHandle()));
	}

	// todo: push constant
	bindingSetDesc.addItem(::nvrhi::BindingSetItem::PushConstants(0, 128));

	::nvrhi::utils::CreateBindingSetAndLayout(
		_renderer->getDevice(),
		::nvrhi::ShaderType::All,
		0,
		bindingSetDesc,
		_bindingLayout,
		_bindingSet
	);

	auto pipelineDesc = ::nvrhi::GraphicsPipelineDesc()
							.setInputLayout(inputLayout)
							.setVertexShader(vertexShader)
							.setPixelShader(pixelShader)
							.addBindingLayout(_bindingLayout);

	auto renderTargetBlendStatge = ::nvrhi::BlendState::RenderTarget()
									   .enableBlend()
									   .setColorWriteMask(::nvrhi::ColorMask::All)
									   .setSrcBlend(::nvrhi::BlendFactor::SrcAlpha)
									   .setDestBlend(::nvrhi::BlendFactor::OneMinusSrcAlpha)
									   .setBlendOp(::nvrhi::BlendOp::Add)
									   .setSrcBlendAlpha(::nvrhi::BlendFactor::One)
									   .setDestBlendAlpha(::nvrhi::BlendFactor::Zero)
									   .setBlendOpAlpha(::nvrhi::BlendOp::Add);

	pipelineDesc.renderState.blendState.setRenderTarget(0, renderTargetBlendStatge);
	pipelineDesc.renderState.depthStencilState.depthTestEnable = false;

	_pipeline =
		_renderer->getDevice()->createGraphicsPipeline(pipelineDesc, _renderer->getContext()->getFramebuffer(0));
}

} // namespace arch::gfx::nvrhi::pipeline
