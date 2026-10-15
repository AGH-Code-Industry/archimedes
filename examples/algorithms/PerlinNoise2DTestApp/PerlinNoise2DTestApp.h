#pragma once

#include <codecvt>
#include <glm/gtx/string_cast.hpp>
#include <locale>
#include <numbers>
#include <random>

#include <archimedes/Algorithms.h>
#include <archimedes/BuildInfo.h>
#include <archimedes/Camera.h>
#include <archimedes/Ecs.h>
#include <archimedes/Engine.h>
#include <archimedes/Input.h>
#include <archimedes/Scene.h>
#include <archimedes/Text.h>

using namespace arch;

// This example is used to test Perlin Noise 2D generator.
// After you run it, you will see a procedurally generated 2D hipsometric map
// (red means very high terrain, blue means very low)
// You can press space to rebuild it with new seed.
// Also don't be afraid to toy with the generator's parameters in the code to look how it affects the map.

class PerlinNoise2DTestApp: public Application {
	const u32 textureWidth = 256, textureHeight = 256;
	const f32 saturation = 1.0f;
	const f32 value = 0.8f;

	const f32 mapWidth = 600;
	const f32 mapHeight = 600;

	scene::Entity mapEntity;
	Ref<asset::mesh::Mesh> mesh;
	Ref<Scene> scene;
	Ref<gfx::Renderer> renderer;
	PerlinNoise2D noise;

	std::vector<float4> generatePerlinMap(u32 width, u32 height, f32 saturation, f32 value) {
		std::vector<float4> perlinMap;
		perlinMap.reserve(width * height);
		for (u32 i = 0; i < width; i++) {
			for (u32 j = 0; j < height; j++) {
				f32 hueFactor = (noise.generate(i, j) + 1.0f) / 2.0f;
				f32 hue = glm::mix(0.0f, 240.0f, hueFactor);
				perlinMap.push_back(math::hsvToRgb(hue, saturation, value));
			}
		}
		return perlinMap;
	}

	void generateMap() {
		if (mapEntity.valid()) {
			scene->removeEntity(mapEntity);
		}

		noise.build(128, -0.5f, 0.5f);

		auto colors = generatePerlinMap(textureWidth, textureHeight, saturation, value);
		auto colorsI = colors.begin();

		mapEntity = scene->newEntity();

		auto buffer = scene->domain().global<Camera>().buffer();
		auto pipeline = renderer->getPipelineManager()->create(
			{
				.vertexShaderPath = "shaders/vertex_default.glsl",
				.fragmentShaderPath = "shaders/fragment_default.glsl",
				.textures = { gfx::Renderer::current()
								  ->getTextureManager()
								  ->createTexture2D(textureWidth, textureHeight, &*colorsI++) },

				// Use the view-projection matrix from camera
				// Ordering may differ depending on the used shader
				.buffers = { buffer },
			}
		);

		mapEntity.addComponent(
			scene::components::TransformComponent{
				.position = { mapWidth / 2, -mapHeight / 2, 0 },
				.rotation = quaternion(0.0f),
				.scale = {	   mapWidth,	  mapHeight, 1 }
		   }
		);
		mapEntity.addComponent(scene::components::MeshComponent{ .mesh = mesh, .pipeline = pipeline });
	}

	void init() override {
		scene = createRef<Scene>();

		scene::SceneManager::get()->changeScene(scene);

		// 2D square
		struct Vertex {
			float3 position;
			float2 tex_coords;
		};

		std::vector<u32> indices{
			0, 1, 2, 2, 1, 3,
		};

		std::vector<Vertex> vertices{
			{  { 0.f, 0.f, 0.f }, { 1, 0 } },
			{ { -1.f, 0.f, 0.f }, { 0, 0 } },
			{  { 0.f, 1.f, 0.f }, { 1, 1 } },
			{ { -1.f, 1.f, 0.f }, { 0, 1 } },
		};

		renderer = gfx::Renderer::getCurrent();
		mesh = asset::mesh::Mesh::create<Vertex>(vertices, indices);

		// Create camera
		scene->domain().global<Camera>();

		// Set Perlin Noise params
		noise.setMinResult(-1.0f).setMaxResult(1.0f).setBaseAmplitude(1.0f).setBaseFrequency(0.01f);

		generateMap();
	}

	void update() {
		if (input::Keyboard::space.down()) {
			generateMap();
		}

		std::this_thread::sleep_for(std::chrono::milliseconds(16));
	}
};
