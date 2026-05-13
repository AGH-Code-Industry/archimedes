#pragma once

#include <codecvt>
#include <locale>
#include <numbers>
#include <random>

#include <archimedes/Ecs.h>
#include <archimedes/Engine.h>
#include <archimedes/Font.h>
#include <archimedes/Input.h>
#include <archimedes/Projection.h>
#include <archimedes/Scene.h>
#include <archimedes/Text.h>

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

std::vector<float4> rainbowColors(u32 N) {
	std::vector<float4> result;
	result.reserve(N);

	for (u32 i = 0; i != N; ++i) {
		float hue = 360.f * i / N;
		result.push_back(hsvToRgb(hue, 1.f, 1.f));
	}

	return result;
}

struct Camera2D {
	float2 extents{};
	float2 position{ 0.f, 0.f };
	float2 zoom = { 1, 1 };
	float rotation = 0.f;

	Ref<gfx::Buffer> buffer = gfx::Renderer::current()->getBufferManager()->createBuffer(gfx::BufferType::uniform);

	void update() {
		auto viewProj = glm::ortho(-extents.x * zoom.x, extents.x * zoom.x, -extents.y * zoom.y, extents.y * zoom.y);

		viewProj *= glm::rotate(glm::mat4(1.f), rotation, glm::vec3(0, 0, 1)) *
			glm::translate(glm::mat4(1.f), glm::vec3(-position.x, -position.y, 0.f));

		buffer->setData(&viewProj, sizeof(viewProj));
	}
};

class CameraTestApp: public Application {
	struct Camera {
		double2 pos{};
		float2 extents{};
		float zoom = 1;
	};

	void init() override {
		Ref<Scene> scene = createRef<Scene>();

		scene::SceneManager::get()->changeScene(scene);

		// 2D square
		struct Vertex {
			float3 position;
			float2 tex_coords;
		};

		/*std::vector<u32> indices{
			2, 3, 1, 0, 2, 1,
		};

		std::vector<Vertex> lineVertices{
			{ { -0.5f, -0.5f, 0.f }, {} },
			{  { 0.5f, -0.5f, 0.f }, {} },
			{  { -0.5f, 0.5f, 0.f }, {} },
			{	  { 0.5f, 0.5f, 0.f }, {} },
		};*/

		// 01
		// 23
		std::vector<u32> indices{
			0, 1, 2, 2, 1, 3,
		};

		std::vector<Vertex> lineVertices{
			{  { 0.f, 0.f, 0.f }, {} },
			{ { -1.f, 0.f, 0.f }, {} },
			{  { 0.f, 1.f, 0.f }, {} },
			{ { -1.f, 1.f, 0.f }, {} },
		};

		// 10
		// 32

		Ref<gfx::Renderer> renderer = gfx::Renderer::getCurrent();

		auto&& window = *gfx::Renderer::current()->getWindow();
		auto windowSize = window.size();

		auto&& cameraTest = scene->domain().global<Camera2D>();
		cameraTest.extents = windowSize / 2;

		cameraTest.update();

		/*auto&& camera = scene->domain().global<Camera>(
			float2{ windowSize.x / 2, windowSize.y / 2 },
			float2{ windowSize.x / 2, windowSize.y / 2 },
			1.f
		);
		auto&& cameraProj = scene->domain().global<Projection>(
			(float2)camera.pos - camera.extents,
			(float2)camera.pos + camera.extents
		);*/

		Ref<asset::mesh::Mesh> mesh = asset::mesh::Mesh::create<Vertex>(lineVertices, indices);

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
						.buffers = { cameraTest.buffer },
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

	float2 middlePressedMousePos{};
	float2 middlePressedCameraPos{};

	float step = 0;

	void update() {
		std::this_thread::sleep_for(std::chrono::milliseconds(16));

		auto&& scene = *scene::SceneManager::get()->currentScene();
		auto&& window = *gfx::Renderer::current()->getWindow();

		auto&& cameraTest = scene.domain().global<Camera2D>();
		bool first = true;
		for (auto&& [_, t] : scene.domain()
								 .view<scene::components::MeshComponent, scene::components::TransformComponent>()
								 .components()) {
			t.position.z = glm::cos(step);
			if (first) {
				first = false;
				Logger::debug("{}", t.position.z);
			}
		}
		step += 0.01;

		if (input::Keyboard::arrowUp.down()) {
			cameraTest.position.y += 2;
		}
		if (input::Keyboard::arrowDown.down()) {
			cameraTest.position.y -= 2;
		}
		if (input::Keyboard::arrowLeft.down()) {
			cameraTest.position.x -= 2;
		}
		if (input::Keyboard::arrowRight.down()) {
			cameraTest.position.x += 2;
		}

		auto scroll = input::Mouse::scroll.y();
		if (scroll < 0) {
			cameraTest.zoom *= 1.1f;
		} else if (scroll > 0) {
			cameraTest.zoom /= 1.1f;
		}

		if (input::Keyboard::one.down()) {
			cameraTest.rotation += std::numbers::pi / 180;
		}
		if (input::Keyboard::two.down()) {
			cameraTest.rotation -= std::numbers::pi / 180;
		}

		cameraTest.update();

		// auto&& camera = scene.domain().global<Camera>();
		// if (input::Mouse::middle.pressed()) {
		//	middlePressedMousePos = input::Mouse::pos();
		//	middlePressedCameraPos = camera.pos;
		// }
		// if (input::Mouse::middle.down()) {
		//	// GLFW cursor position gets desynced with OS cursor position :(
		//	camera.pos = middlePressedCameraPos + (middlePressedMousePos - (float2)input::Mouse::pos()) * camera.zoom;
		// }

		//// F11 to toggle fullscreen
		// if (input::Keyboard::F11.pressed()) {
		//	window.toggleFullscreen();
		//	Logger::debug("{} {}", window.size().x, window.size().y);

		//	if (camera.extents != (float2)window.size() / 2.f) {
		//		auto extentsCopy = camera.extents;
		//		camera.extents = (float2)window.size() / 2.f;

		//		// make it proportional
		//		auto factor = extentsCopy.x / camera.extents.x;
		//		camera.zoom *= factor;
		//	}
		//}

		//// use scroll to control zoom
		// constexpr auto zoomOutFactor = 1.1f;
		// constexpr auto zoomInFactor = 1.f / zoomOutFactor;

		// auto scrollVal = input::Mouse::scroll.y();
		// if (scrollVal < 0) {
		//	camera.zoom *= zoomOutFactor;
		// } else if (scrollVal > 0) {
		//	camera.zoom *= zoomInFactor;
		// }

		// scene.domain().global<Projection>().update(
		//	(float2)camera.pos - camera.extents * camera.zoom,
		//	(float2)camera.pos + camera.extents * camera.zoom
		//);
	}
};
