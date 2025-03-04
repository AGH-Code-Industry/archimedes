#pragma once

#include <array>
#include <codecvt>
#include <future>
#include <locale>
#include <random>

#include <Ecs.h>
#include <Engine.h>
#include <Font.h>
#include <InputHandler.h>
#include <Scene.h>
#include <Text.h>

using namespace arch;

constexpr int windowWidth = 1'280;
constexpr int windowHeight = 720;

std::vector<u32> indices{ 0, 1, 2, 2, 1, 3 };

float packRG(float2 rg) {
	return floor(rg.x * 255.0) + floor(rg.y * 255.0) * 256.0;
}

float packBA(float2 ba) {
	return floor(ba.x * 255.0) + floor(ba.y * 255.0) * 256.0;
}

struct Vertex {
	float3 position;
	float2 tex_coords;
};

struct UniformBuffer {
	Mat4x4 projection;
};

UniformBuffer ubo{ glm::ortho(0.f, (float)windowWidth, 0.f, (float)windowHeight) };

struct SelectionFlag {
	static constexpr bool flagComponent = true;
};

class ProjectSelectorApp: public Application {
	float vel = 0;
	bool speedup = false;

	float beginX, endX;

	struct LineFlag {
		static constexpr bool flagComponent = true;
	};

	struct RectFlag {
		static constexpr bool flagComponent = true;
	};

	class RectSprite {
	public:

		RectSprite(ecs::Domain& domain, const ecs::Entity entity, Color color) {
			_entity = Entity(*scene::SceneManager::get()->currentScene(), entity);

			_entity.addComponent<RectFlag>();

			auto&& t = _entity.addComponent<scene::components::TransformComponent>();
			t.position = {};
			t.rotation = { 0.f, 0.f, 0.f, 1.f };
			t.scale = { 100.f, 100.f, 0.f };

			char colorArr[4]{ color.a * 255, color.b * 255, color.g * 255, color.r * 255 };
			float colorFloat = *reinterpret_cast<float*>(&colorArr);

			float rg = packRG({ color.r, color.g });
			float ba = packBA({ color.b, color.a });

			std::vector<Vertex> verts{
				{  { 0.f, 0.f, 0.f }, { rg, ba } },
				{ { -1.f, 0.f, 0.f }, { rg, ba } },
				{  { 0.f, 1.f, 0.f }, { rg, ba } },
				{ { -1.f, 1.f, 0.f }, { rg, ba } },
			};
			auto uniformBuffer = gfx::Renderer::getCurrent()
									 ->getBufferManager()
									 ->createBuffer(gfx::BufferType::uniform, &ubo, sizeof(UniformBuffer));

			auto pipeline = gfx::Renderer::getCurrent()->getPipelineManager()->create({
				.vertexShaderPath = "shaders/vertex_default.glsl",
				.fragmentShaderPath = "shaders/fragment_rect.glsl",
				.textures = {},
				.buffers = { uniformBuffer },
			});

			auto mesh = asset::mesh::Mesh::create<Vertex>(verts, indices);

			_entity.addComponent<scene::components::MeshComponent>(mesh, pipeline);
		}

		RectSprite& setMargin(float margin) {
			_margin = margin;
			return *this;
		}

		void initText(std::string_view text) {
			auto entity = _entity.addChild();
			auto&& tc = entity.addComponent<text::TextComponent>();
			auto text32 = text::convertTo<char32_t>(text);
			tc.setFont("Arial").setFontSize(50).setText(std::u32string_view(text32)).updateText(true);
			auto&& tr = entity.getComponent<scene::components::TransformComponent>();

			auto&& rectTr = _entity.getComponent<scene::components::TransformComponent>();

			float width = rectTr.scale.x - 2 * _margin;
			float height = rectTr.scale.y - 2 * _margin;

			auto scale = width / tc.size().x;

			float textHeight = tc.size().y * scale;
			float textWidth = width * scale;

			float2 baselineDelta = tc.topLeft() - tc.getPosition();

			tr.scale *= scale;
			baselineDelta *= scale;
			tr.position = rectTr.position;
			tr.position += float3{ _margin, -(_margin + (height - textHeight) / 2.f), 0.f };
			tr.position += float3{ -baselineDelta.x, -baselineDelta.y, 0.f };

			auto diff = tc.getPosition() - tc.topLeft();

			std::cout << std::format("{} {}\n", diff.x, diff.y);

			_entityText = entity;
		}

		void move(float dx, float leftBound, float rightBound) {
			auto&& rt = _entity.getComponent<scene::components::TransformComponent>();
			auto&& tt = _entityText.getComponent<scene::components::TransformComponent>();
			rt.position.x += dx;
			tt.position.x += dx;

			if (rt.position.x < leftBound) {
				tt.position.x += rightBound - leftBound;
				rt.position.x += rightBound - leftBound;
			} else if (rt.position.x > rightBound) {
				tt.position.x += leftBound - rightBound;
				rt.position.x += leftBound - rightBound;
			}
		}

	private:
		Entity _entity;
		Entity _entityText;

		float _margin;
	};

	struct TextInfo {
		float3 topLeft{};
		float3 bottomRight{};
	};

	void init() override {
		int baseline = windowHeight / 2;

		Ref<Scene> testScene = createRef<Scene>();

		scene::SceneManager::get()->changeScene(testScene);

		// 2D square

		std::vector<Vertex> lineVertices{
			{  { 0.f, 0.f, 0.1f }, {} },
			{ { -1.f, 0.f, 0.1f }, {} },
			{  { 0.f, 1.f, 0.1f }, {} },
			{ { -1.f, 1.f, 0.1f }, {} },
		};

		std::vector<Vertex> gradientVertices{
			{  { 0.f, 0.f, 0.1f }, { 0.f, 0.f } },
			{ { -1.f, 0.f, 0.1f }, { 1.f, 0.f } },
			{  { 0.f, 1.f, 0.1f }, { 0.f, 1.f } },
			{ { -1.f, 1.f, 0.1f }, { 1.f, 1.f } },
		};

		Ref<gfx::Renderer> renderer = gfx::Renderer::getCurrent();

		Ref<asset::mesh::Mesh> lineMesh = asset::mesh::Mesh::create<Vertex>(lineVertices, indices);
		Ref<asset::mesh::Mesh> gradientMesh = asset::mesh::Mesh::create<Vertex>(gradientVertices, indices);

		/*struct UniformBuffer {
			Mat4x4 projection;
		};

		UniformBuffer ubo{ glm::ortho(0.f, (float)windowWidth, 0.f, (float)windowHeight) };*/
		auto uniformBuffer =
			renderer->getBufferManager()->createBuffer(gfx::BufferType::uniform, &ubo, sizeof(UniformBuffer));

		auto linePipeline = renderer->getPipelineManager()->create({
			.vertexShaderPath = "shaders/vertex_default.glsl",
			.fragmentShaderPath = "shaders/fragment_default2.glsl",
			.textures = {},
			.buffers = { uniformBuffer },
		});
		auto gradientPipeline = renderer->getPipelineManager()->create({
			.vertexShaderPath = "shaders/vertex_default.glsl",
			.fragmentShaderPath = "shaders/fragment_middleGradient.glsl",
			.textures = {},
			.buffers = { uniformBuffer },
		});

		/*{
			auto line = testScene->newEntity();
			auto&& t = line.addComponent<scene::components::TransformComponent>({
				{ 0, baseline, 0 },
				{ 0, 0, 0, 1 },
				{ windowWidth, 1, 0 }
			  });

			line.addComponent<scene::components::MeshComponent>({ lineMesh, linePipeline });
			line.addComponent<LineFlag>();
		}*/

		// auto&& tc = testScene->newEntity()
		//				.addComponent<text::TextComponent>()
		//				.setFont("Arial", "Regular")
		//				.setFontSize(70)
		//				.setBaseline({ 0, windowHeight / 2 })
		//				.setText<char32_t>(U"Zażółć gęślą jaźń <3 AV");
		// tc.updateText();
		// auto width = (tc.bottomRight() - tc.topLeft()).x;
		//// vv- how to center <div> -vv
		// tc.setBaseline({ (1280.f - width) / 2, windowHeight / 2 });
		// tc.updateText();

		const int rectCount = 10;

		const float2 rectSize = { 300.f, 200.f };

		float combinedWidth = rectSize.x * rectCount;

		auto _rng = std::mt19937(std::random_device{}());
		auto rng = std::uniform_real_distribution(0.f, 1.f);

		float initialX = beginX = 1'280 / 2 - combinedWidth / 2;
		endX = 1'280 / 2 + combinedWidth / 2;

		for (int i = 0; i != rectCount; ++i) {
			auto colorRect = testScene->newEntity();
			auto&& rs = colorRect.addComponent<RectSprite>(Color(rng(_rng), rng(_rng), rng(_rng), 1.0));
			colorRect.getComponent<scene::components::TransformComponent>().position = { initialX + i * rectSize.x,
																						 (720.f + rectSize.y) / 2.f,
																						 0.f };
			colorRect.getComponent<scene::components::TransformComponent>().scale = { rectSize.x, rectSize.y, 0.f };

			rs.setMargin(50);
			rs.initText(std::string_view(std::format("{0}{0}{0}", i)));
		}

		{
			auto gradient = testScene->newEntity();
			auto&& t = gradient.addComponent<scene::components::TransformComponent>({
				{ 0, windowHeight, 0 },
				{ 0, 0, 0, 1 },
				{ windowWidth, windowHeight, 0 }
			 });

			gradient.addComponent<scene::components::MeshComponent>({ gradientMesh, gradientPipeline });
			// line.addComponent<LineFlag>();
		}

		InputHandler::get()._bindKey(VK_SPACE, [&]() {
			if (vel == 0 && !speedup) {
				speedup = true;
				vel = 0.1;
			}
		});
	}

	bool brake = false;

	void update() {
		std::this_thread::sleep_for(std::chrono::milliseconds(16));

		if (speedup) {
			vel *= 1.05f;
		} else if (vel > 0.1f && brake) {
			if (vel > 20.f) {
				vel *= 0.99;
			} else if (vel < 20.f && vel > 5.f) {
				vel *= 0.98;
			} else if (vel < 5.f) {
				vel *= 0.95;
			}
			if (vel < 0.1f) {
				vel = 0;
				brake = false;
			}
		}
		if (vel > 200.f && speedup) {
			speedup = false;
			std::jthread([&]() {
				auto random = std::mt19937(std::random_device()());
				std::this_thread::sleep_for(
					std::chrono::milliseconds(std::uniform_int_distribution<int>(1'000, 3'000)(random))
				);
				brake = true;
			}).detach();
		}
		for (auto&& [ent, rs] : scene::SceneManager::get()->currentScene()->domain().view<RectSprite>().all()) {
			rs.move(vel, beginX, endX);
		}
	}
};
