#pragma once

#include <codecvt>
#include <glm/gtx/string_cast.hpp>
#include <locale>
#include <numbers>
#include <random>

#include <archimedes/Camera.h>
#include <archimedes/Ecs.h>
#include <archimedes/Engine.h>
#include <archimedes/Input.h>
#include <archimedes/Scene.h>
#include <archimedes/Text.h>
#include <archimedes/BuildInfo.h>

using namespace arch;

float4 hsvToRgb(float h, float s, float v) {
	float c = v * s;
	float x = c * (1.f - std::fabs(std::fmod(h / 60.f, 2.f) - 1.f));
	float m = v - c;

	float r = 0, g = 0, b = 0;

	if (h < 60) {
		r = c;
		g = x;
		b = 0;
	} else if (h < 120) {
		r = x;
		g = c;
		b = 0;
	} else if (h < 180) {
		r = 0;
		g = c;
		b = x;
	} else if (h < 240) {
		r = 0;
		g = x;
		b = c;
	} else if (h < 300) {
		r = x;
		g = 0;
		b = c;
	} else {
		r = c;
		g = 0;
		b = x;
	}

	return { r + m, g + m, b + m, 1.f };
}

std::vector<float4> generatePerlinMap(u32 width, u32 height, f32 saturation, f32 value) {
	std::vector<float4> perlinMap;
	perlinMap.reserve(width * height);
	for (u32 i = 0; i < width; i++){
		for (u32 j = 0; j < height; j++) {
			f32 hueFactor = (PerlinNoise2D::generate(i, j) + 1.0f) / 2.0f;
			f32 hue = glm::mix(0.0f, 240.0f, hueFactor);
			perlinMap.push_back(hsvToRgb(hue, saturation, value));
		}
	}
	return perlinMap;
}


class PerlinNoise2DTestApp: public Application {
	void init() override {
		Ref<Scene> scene = createRef<Scene>();

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
			{  { 0.f, 0.f, 0.f }, {1, 0} },
			{ { -1.f, 0.f, 0.f }, {0, 0} },
			{  { 0.f, 1.f, 0.f }, {1, 1} },
			{ { -1.f, 1.f, 0.f }, {0, 1} },
		};

		Ref<gfx::Renderer> renderer = gfx::Renderer::getCurrent();
		Ref<asset::mesh::Mesh> mesh = asset::mesh::Mesh::create<Vertex>(vertices, indices);

		// Create camera
		auto&& camera = scene->domain().global<Camera>();

		u32 textureWidth = 256;
		u32 textureHeight = 256;

		f32 saturation = 1.0f;
		f32 value = 0.8f;

		PerlinNoise2D::build(128, -0.5f, 0.5f);
		PerlinNoise2D::minResult = -1.0f;
		PerlinNoise2D::maxResult = 1.0f;
		PerlinNoise2D::baseAmplitude = 1.0f;
		PerlinNoise2D::baseFrequency = 0.01f;
		auto colors = generatePerlinMap(textureWidth, textureHeight, saturation, value);
		auto colorsI = colors.begin();

		auto rect = scene->newEntity();

		auto pipeline = renderer->getPipelineManager()->create(
			{
				.vertexShaderPath = "shaders/vertex_default.glsl",
				.fragmentShaderPath = "shaders/fragment_default.glsl",
				.textures = { gfx::Renderer::current()
									->getTextureManager()
									->createTexture2D(textureWidth, textureHeight, &*colorsI++) },

				// Use the view-projection matrix from camera
				// Ordering may differ depending on the used shader
				.buffers = { camera.buffer() },
			}
		);

		f32 mapWidth = 600;
		f32 mapHeight = 600;

		rect.addComponent(
			scene::components::TransformComponent{
				.position = { -mapWidth / 2, mapHeight / 2, 0 },
				.rotation = { 0, 0, 0, 1 },
				.scale = { mapWidth, mapHeight, 1 }
			}
		);
		rect.addComponent(scene::components::MeshComponent{ .mesh = mesh, .pipeline = pipeline });

	}

	void update() {
		auto&& scene = *scene::SceneManager::get()->currentScene();
		auto&& camera = scene.domain().global<Camera>();
		auto&& window = *gfx::Renderer::current()->getWindow();

		std::this_thread::sleep_for(std::chrono::milliseconds(16));
	}

};
