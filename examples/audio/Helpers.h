#pragma once
#include <thread>

#include <archimedes/Application.h>
#include <archimedes/Ecs.h>
#include <archimedes/Engine.h>
#include <archimedes/Scene.h>
#include <archimedes/audio/AudioManager.h>
#include <archimedes/audio/SoundDevice.h>

using namespace arch;

struct GraphicsManager {
	struct Vertex {
		float3 position;
		float2 tex_coords;
	};

	std::vector<Vertex> vertices{
		{ { -.25f, -.25f, 0.1f }, { 0.f, 0.f } },
		{	  { 0.f, .25f, 0.1f }, { .5f, 1.f } },
		{  { .25f, -.25f, 0.1f }, { 1.f, 0.f } },
	};

	std::vector<u32> indices{ 0, 1, 2 };

	Color pixels[1] = {
		Color{ 1, .5, 1, 1 }
	};

	Ref<gfx::Renderer> renderer;

	Ref<gfx::texture::Texture> texture;

	struct UniformBuffer {
		Mat4x4 projection;
	};

	Ref<gfx::Buffer> uniformBuffer;

	UniformBuffer ubo{ glm::ortho(0.f, 640.f, 0.f, 400.f) };

	Ref<gfx::pipeline::Pipeline> pipeline, pipeline2;

	Ref<asset::mesh::Mesh> mesh;

	GraphicsManager() {
		renderer = gfx::Renderer::getCurrent();
		texture = renderer->getTextureManager()->createTexture2D(1, 1, pixels);
		uniformBuffer =
			renderer->getBufferManager()->createBuffer(gfx::BufferType::uniform, &ubo, sizeof(UniformBuffer));
		pipeline = renderer->getPipelineManager()->create(
			{
				.vertexShaderPath = "shaders/vertex_default.glsl",
				.fragmentShaderPath = "shaders/fragment_default.glsl",
				.textures = { texture },
				.buffers = { uniformBuffer },
			}
		);
		pipeline2 = renderer->getPipelineManager()->create(
			{
				.vertexShaderPath = "shaders/vertex_default.glsl",
				.fragmentShaderPath = "shaders/fragment_default2.glsl",
				.textures = {},
				.buffers = { uniformBuffer },
			}
		);
		mesh = asset::mesh::Mesh::create<Vertex>(vertices, indices);
	}

	void clean() {
		mesh = nullptr;
		pipeline = nullptr;
		pipeline2 = nullptr;
		uniformBuffer = nullptr;
		texture = nullptr;
		renderer = nullptr;
	}
};

struct SoundManager {
	audio::SoundDevice device;
	audio::SoundBank soundBank;
	audio::AudioManager* audioManager{};
	std::jthread* audioThread{};

	void init(const std::vector<std::string>& soundFiles) {
		audioManager = new audio::AudioManager(&soundBank);
		audioThread = new std::jthread(&audio::AudioManager::play, audioManager);
		for (auto& soundFile : soundFiles) {
			soundBank.addClip(soundFile);
		}
		soundBank.loadInitialGroups();
	}

	~SoundManager() {
		audioManager->stop();
		audioThread->join();
		delete audioManager;
	}
};
