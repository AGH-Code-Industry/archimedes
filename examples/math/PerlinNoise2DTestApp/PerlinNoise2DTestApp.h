#pragma once

#include <archimedes/Camera.h>
#include <archimedes/Ecs.h>
#include <archimedes/Engine.h>
#include <archimedes/Input.h>
#include <archimedes/Scene.h>
#include <archimedes/Text.h>

using namespace arch;

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
			{  { 0.f, 0.f, 0.f }, {} },
			{ { -1.f, 0.f, 0.f }, {} },
			{  { 0.f, 1.f, 0.f }, {} },
			{ { -1.f, 1.f, 0.f }, {} },
		};

		Ref<gfx::Renderer> renderer = gfx::Renderer::getCurrent();
		Ref<asset::mesh::Mesh> mesh = asset::mesh::Mesh::create<Vertex>(vertices, indices);

		// Create camera
		auto&& camera = scene->domain().global<Camera>();

		PerlinNoise2D::initialize(100, 0.0f, 0.1f);
	}

	void update() {
		auto&& scene = *scene::SceneManager::get()->currentScene();
		auto&& camera = scene.domain().global<Camera>();
		auto&& window = *gfx::Renderer::current()->getWindow();

		math::f32 x = 0.01f;
		math::f32 y = 0.01f;

		if (input::Keyboard::space.down()){
			f32 z = PerlinNoise2D::generate(x, y);
			Logger::info("generated value: {}", z);
		}

		std::this_thread::sleep_for(std::chrono::milliseconds(16));
	}
};
