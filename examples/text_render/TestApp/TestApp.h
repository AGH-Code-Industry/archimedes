#pragma once

#include <codecvt>
#include <locale>

#include <Ecs.h>
#include <Engine.h>
#include <Font.h>
#include <Scene.h>
#include <Text.h>

using namespace arch;

class TextRenderTestApp: public Application {
	struct LineFlag {
		static constexpr bool flagComponent = true;
	};

	struct TextInfo {
		float3 topLeft{};
		float3 bottomRight{};
	};

	void init() override {
		int windowWidth = 1'200;
		int windowHeight = 600;

		int baseline = windowHeight / 2;

		Ref<Scene> testScene = createRef<Scene>();

		scene::SceneManager::get()->changeScene(testScene);

		// 2D square
		struct Vertex {
			float3 position;
			float2 tex_coords;
		};

		std::vector<u32> indices{ 0, 1, 2, 2, 1, 3 };

		std::vector<Vertex> lineVertices{
			{  { 0.f, 0.f, 0.1f }, {} },
			{ { -1.f, 0.f, 0.1f }, {} },
			{  { 0.f, 1.f, 0.1f }, {} },
			{ { -1.f, 1.f, 0.1f }, {} },
		};

		Ref<gfx::Renderer> renderer = gfx::Renderer::getCurrent();

		Ref<asset::mesh::Mesh> lineMesh = asset::mesh::Mesh::create<Vertex>(lineVertices, indices);

		struct UniformBuffer {
			Mat4x4 projection;
		};

		UniformBuffer ubo{ glm::ortho(0.f, (float)windowWidth, 0.f, (float)windowHeight) };
		auto uniformBuffer =
			renderer->getBufferManager()->createBuffer(gfx::BufferType::uniform, &ubo, sizeof(UniformBuffer));

		auto linePipeline = renderer->getPipelineManager()->create({
			.vertexShaderPath = "shaders/vertex_default.glsl",
			.fragmentShaderPath = "shaders/fragment_default2.glsl",
			.textures = {},
			.buffers = { uniformBuffer },
		});

		{
			auto line = testScene->newEntity();
			auto&& t = line.addComponent<scene::components::TransformComponent>({
				{ 0, baseline, 0 },
				{ 0, 0, 0, 1 },
				{ windowWidth, 1, 0 }
			  });

			line.addComponent<scene::components::MeshComponent>({ lineMesh, linePipeline });
			line.addComponent<LineFlag>();
		}

		{
			auto text = testScene->newEntity();
			auto&& t = text.addComponent<scene::components::TransformComponent>({
				{ 0.f, windowHeight / 2, 0.f },
				{ 0.f, 0.f, 0.f, 1.f },
				{ 70.f, 70.f, 0.f }
			});
			auto&& tc = text.addComponent<text::TextComponent>(
				U"Zażółć gęślą jaźń <3 AV",
				std::vector{ uniformBuffer },
				"Arial"
			);

			auto topLeft = float4{ tc.topLeft(), 1 };
			auto bottomRight = float4{ tc.bottomRight(), 1 };

			Logger::debug("min = ({}, {})\n", topLeft.x, topLeft.y);
			Logger::debug("max = ({}, {})\n", bottomRight.x, bottomRight.y);

			topLeft = t.getTransformMatrix() * topLeft;
			bottomRight = t.getTransformMatrix() * bottomRight;

			Logger::debug("TL = ({}, {})\n", topLeft.x, topLeft.y);
			Logger::debug("BR = ({}, {})\n", bottomRight.x, bottomRight.y);
		}
	}

	void update() {
		static float frame = 0;
		std::this_thread::sleep_for(std::chrono::milliseconds(16));

		for (auto&& [e, transform, text] : scene::SceneManager::get()
											   ->currentScene()
											   ->domain()
											   .view<scene::components::TransformComponent, text::TextComponent>()
											   .all()) {
			auto scale = 100.f * (cos(frame) + 1) / 2.f;
			transform.scale = { scale, scale, 0.f };
		}

		frame += 0.05f;
	}
};
