#pragma once

#include <codecvt>
#include <locale>
#include <random>

#include <Ecs.h>
#include <Engine.h>
#include <Font.h>
#include <Input.h>
#include <Scene.h>
#include <Text.h>

using namespace arch;

class InputTestApp: public Application {
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

			topLeft = t.getTransformMatrix() * topLeft;
			bottomRight = t.getTransformMatrix() * bottomRight;
		}
	}

	void update() {
		static double cosArg = 0;
		static double speed = 0;
		static double amplitude = 0;
		static bool speedMul = true;
		static bool showMsg = true;

		// both keyboard and mouse keys are the same type: Key&
		static auto& msgKey = [] -> Key& {
			// read available codes from file
			auto keysFile = std::ifstream("config/easterEggKeys.txt");
			auto codes = std::views::istream<u32>(keysFile) | std::ranges::to<std::vector>();

			// get one random
			u32 code;
			std::ranges::sample(codes, &code, 1, std::mt19937(std::random_device{}()));

			// return Key of picked code
			return Key::get(code);
		}();
		static auto msgMod = std::random_device{}() % 2 ? KeyState::shift : KeyState::capsLock;

		using Clk = std::chrono::high_resolution_clock;

		static std::vector<decltype(Clk::now() - Clk::now())> times;
		static std::vector<decltype(Clk::now() - Clk::now())> timesRef;

		std::this_thread::sleep_for(std::chrono::milliseconds(16));

		for (auto&& [transform, text] : scene::SceneManager::get()
											->currentScene()
											->domain()
											.view<scene::components::TransformComponent, text::TextComponent>()
											.components()) {
			auto scale = amplitude * (cos(cosArg) + 1) / 2.0;
			transform.scale = { scale, scale, 0.f };
		}

		// scroll up -> increase animation speed
		// scroll down -> decrease animation speed
		speed += Mouse::scroll.y() * 0.001;
		if (Mouse::scroll.y()) {
			Logger::debug("speed = {}", speed);
		}

		// scroll pressed -> pause/play text animation
		if (Mouse::scroll.pressed()) {
			speedMul = !speedMul;
			Logger::debug("speedMul = {}", speedMul);
		}

		// arrow up pressed or held -> increase max size
		if (Keyboard::arrowUp.pressed() || Keyboard::arrowUp.repeat()) {
			++amplitude;
			Logger::debug("amplitude = {}", amplitude);
		}

		// arrow down pressed or held -> decrease max size
		if (Keyboard::arrowDown.pressed() || Keyboard::arrowDown.repeat()) {
			--amplitude;
			Logger::debug("amplitude = {}", amplitude);
		}

		if (msgKey.downTime() >= std::chrono::seconds(3) && msgKey.has(msgMod) && showMsg) {
			showMsg = false;
			Logger::critical("Congratulations! You found an easter egg!");
		} else if (msgKey.released()) {
			showMsg = true;
		}

		cosArg += speed * speedMul;
	}
};
