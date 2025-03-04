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

		auto&& tc = testScene->newEntity()
						.addComponent<text::TextComponent>()
						.setFont("Arial", "Regular")
						.setFontSize(70)
						.setBaseline({ 0, windowHeight / 2 })
						.setText<char32_t>(U"Zażółć gęślą jaźń <3 AV");
		tc.updateText();
		auto width = (tc.bottomRight() - tc.topLeft()).x;
		// vv- how to center <div> -vv
		tc.setBaseline({ (1200.f - width) / 2, windowHeight / 2 });
		tc.updateText();
	}

	void update() {
		std::this_thread::sleep_for(std::chrono::milliseconds(16));

		for (auto&& [ent, tr, tc] : scene::SceneManager::get()
										->currentScene()
										->domain()
										.view<scene::components::TransformComponent, text::TextComponent>()
										.all()) {
			tc.rotateDeg(1.f);
		}
	}
};
