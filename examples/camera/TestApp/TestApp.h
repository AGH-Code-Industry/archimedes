#pragma once

#include <codecvt>
#include <glm/gtx/string_cast.hpp>
#include <locale>
#include <numbers>
#include <random>

#include <archimedes/Camera.h>
#include <archimedes/Ecs.h>
#include <archimedes/Engine.h>
#include <archimedes/Font.h>
#include <archimedes/Input.h>
#include <archimedes/Scene.h>
#include <archimedes/Text.h>

// This example shows usage of the Camera class
// There are rainbow blocks placed as 10x10 grid to make it easier to see the camera changes
// Controls:
// - Scroll up: zoom in
// - Scroll down: zoom out
// - Arrow keys: accordingly move camera in world space
// - Left mouse button: rotate camera counterclockwise
// - Right mouse button: rotate camera clockwise

using namespace arch;

// Helper for making rainbow blocks
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

// Helper for making rainbow blocks
std::vector<float4> rainbowColors(u32 N) {
	std::vector<float4> result;
	result.reserve(N);

	for (u32 i = 0; i != N; ++i) {
		float hue = 360.f * i / N;
		result.push_back(hsvToRgb(hue, 1.f, 1.f));
	}

	return result;
}

class CameraTestApp: public Application {
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

		// rainbow grid
		constexpr auto gridWidth = 10;
		constexpr auto gridHeight = 10;

		auto colors = rainbowColors(gridWidth * gridHeight);
		auto colorsI = colors.begin();

		for (u32 y = 0; y != gridHeight; ++y) {
			for (u32 x = 0; x != gridWidth; ++x) {
				auto rect = scene->newEntity();

				auto pipeline = renderer->getPipelineManager()->create(
					{
						.vertexShaderPath = "shaders/vertex_default.glsl",
						.fragmentShaderPath = "shaders/fragment_default.glsl",
						.textures = { gfx::Renderer::current()
										  ->getTextureManager()
										  ->createTexture2D(1, 1, &*colorsI++) },

						// Use the view-projection matrix from camera
						// Ordering may differ depending on the used shader
						.buffers = { camera.buffer() },
					}
				);
				rect.addComponent(
					scene::components::TransformComponent{
						.position = { x * 100, y * 100, 0 },
						.rotation = { 0, 0, 0, 1 },
						.scale = { 100, 100, 1 }
				 }
				);
				rect.addComponent(scene::components::MeshComponent{ .mesh = mesh, .pipeline = pipeline });
			}
		}
	}

	void update() {
		auto&& scene = *scene::SceneManager::get()->currentScene();
		auto&& camera = scene.domain().global<Camera>();
		auto&& window = *gfx::Renderer::current()->getWindow();

		std::this_thread::sleep_for(std::chrono::milliseconds(16));

		// Camera movement through world space
		if (input::Keyboard::arrowUp.down()) {
			camera.move({ 0, 2 });
		}
		if (input::Keyboard::arrowDown.down()) {
			camera.move({ 0, -2 });
		}
		if (input::Keyboard::arrowLeft.down()) {
			camera.move({ -2, 0 });
		}
		if (input::Keyboard::arrowRight.down()) {
			camera.move({ 2, 0 });
		}

		// Camera zooming
		auto scroll = input::Mouse::scroll.y();
		if (scroll < 0) { // scroll up
			camera.zoomOut(1.1);
		} else if (scroll > 0) { // scroll down
			camera.zoomIn(1.1);
		}

		// Camera rotation
		if (input::Mouse::left.down()) {
			camera.rotate(glm::radians(1.f));
		}
		if (input::Mouse::right.down()) {
			camera.rotate(glm::radians(-1.f));
		}

		if (input::Mouse::dpos() != double2()) {
			// Printing out mouse position in world space
			// The top-left corner of the red block is at (0, 0), each block is 100 in size
			log::debug("{::.2f}", camera.screenToWorldPos(input::Mouse::pos()));
		}

		// Camera works with fullscreen
		if (input::Keyboard::F11.pressed()) {
			window.toggleFullscreen();
		}
	}
};
