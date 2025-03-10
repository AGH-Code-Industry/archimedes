#pragma once

#include <algorithm>
#include <array>
#include <codecvt>
#include <future>
#include <locale>
#include <numbers>
#include <random>
#include <ranges>

#include <Ecs.h>
#include <Engine.h>
#include <Font.h>
#include <InputHandler.h>
#include <Net.h>
#include <Scene.h>
#include <Text.h>
#include <physics/System.h>
#include <physics/components/Colliding.h>

using namespace arch;

using Transform = scene::components::TransformComponent;

constexpr int windowWidth = 1'280;
constexpr int windowHeight = 720;

std::vector<u32> indices{ 0, 1, 2, 2, 1, 3 };

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

struct RectSpriteComponent {
	float marginPercent;
	Color color;
	int index;
};

template<int x>
struct DebugComponent {
	ecs::Entity entity;

	DebugComponent(ecs::Entity e): entity{ e } { //
		if (size_t(e) == 64) {
			std::cout << "xd\n";
			// Logger::debug("add {:v}", e);
		}
		Logger::debug("add {:v}", e);
	}

	DebugComponent(): entity{ ecs::nullEntity } {}

	~DebugComponent() {
		Logger::debug("destroyed {:v}", entity);
		entity = ecs::nullEntity;
	}

	DebugComponent(const DebugComponent& other) {
		Logger::debug("copy {:v} <- {:v}", entity, other.entity);
		entity = other.entity;
	}

	DebugComponent(DebugComponent&& other) {
		Logger::debug("move {:v} <- {:v}", entity, other.entity);
		entity = other.entity;
		other.entity = ecs::nullEntity;
	}

	DebugComponent& operator=(const DebugComponent& other) {
		Logger::debug("copy= {:v} <- {:v}", entity, other.entity);
		entity = other.entity;
		return *this;
	}

	DebugComponent& operator=(DebugComponent&& other) {
		Logger::debug("move= {:v} <- {:v}", entity, other.entity);
		entity = other.entity;
		other.entity = ecs::nullEntity;
		return *this;
	}

	void swap(DebugComponent& other) {
		Logger::debug("swap {:v} <-> {:v}", entity, other.entity);
		std::swap(entity, other.entity);
	}
};

std::vector<Vertex> defaultVertices{
	{  { 0.f, 0.f, 0.1f }, { 0.f, 0.f } },
	{ { -1.f, 0.f, 0.1f }, { 1.f, 0.f } },
	{  { 0.f, 1.f, 0.1f }, { 0.f, 1.f } },
	{ { -1.f, 1.f, 0.1f }, { 1.f, 1.f } },
};

void updateTextInRect(Entity rect) {
	auto&& rectTransform = rect.getComponent<Transform>();

	auto text = rect.firstChild();
	auto&& textTransform = text.getComponent<Transform>();

	textTransform.position = float3{ 0 };
	textTransform.scale = float3{ 1, 1, 0 };
	auto tmat = textTransform.getTransformMatrix();

	auto marginPercent = rect.getComponent<RectSpriteComponent>().marginPercent;
	auto&& textComp = text.getComponent<text::TextComponent>();

	/*if (rect.hasComponent<SelectionFlag>()) {
		Logger::debug(
			"\n\n({}, {}, {})\n({}, {}, {})\n",
			textTransform.position.x,
			textTransform.position.y,
			textTransform.position.z,
			textTransform.scale.x,
			textTransform.scale.y,
			textTransform.scale.z
		);
	}*/

	auto bottomLeft = textComp.bottomLeft(tmat);
	auto topRight = textComp.topRight(tmat);
	auto size = topRight - bottomLeft;

	auto marginX = rectTransform.scale.x * marginPercent;

	auto availableWidth = rectTransform.scale.x - 2 * marginX;

	auto fontSize = availableWidth / size.x;

	size *= fontSize;
	bottomLeft *= fontSize;

	textTransform.position = rectTransform.position +
		float3{ marginX - bottomLeft.x,
				-bottomLeft.y - (rectTransform.scale.y + size.y) / 2.f,
				rectTransform.position.z - 0.1f };
	textTransform.scale *= fontSize;

	/*if (rect.hasComponent<SelectionFlag>()) {
		Logger::debug(
			"\n\n({}, {}, {})\n({}, {}, {})\n",
			textTransform.position.x,
			textTransform.position.y,
			textTransform.position.z,
			textTransform.scale.x,
			textTransform.scale.y,
			textTransform.scale.z
		);
	}*/
}

Entity newRectWithText(
	Color color,
	std::u32string textToDisplay,
	float3 position,
	float3 scale,
	int index,
	float marginPercent = 0.05f
) {
	auto rect = scene::SceneManager::get()->currentScene()->newEntity();

	// Logger::debug("{:v}", rect.handle());

	rect.addComponent<scene::components::TransformComponent>({
		position,
		{ 0, 0, 0, 1 },
		scale
	  });

	std::vector<Vertex> verts{
		{  { 0.f, 0.f, 0.f }, { 0.f, 0.f } },
		{ { -1.f, 0.f, 0.f }, { 0.f, 0.f } },
		{  { 0.f, 1.f, 0.f }, { 0.f, 0.f } },
		{ { -1.f, 1.f, 0.f }, { 0.f, 0.f } },
	};

	auto texture = gfx::Renderer::getCurrent()->getTextureManager()->createTexture2D(1, 1, &color);
	auto mesh = asset::mesh::Mesh::create(std::span(verts), indices);
	auto uniformBuffer = gfx::Renderer::getCurrent()
							 ->getBufferManager()
							 ->createBuffer(gfx::BufferType::uniform, &ubo, sizeof(UniformBuffer));

	auto pipeline = gfx::Renderer::getCurrent()->getPipelineManager()->create({
		.vertexShaderPath = "shaders/vertex_default.glsl",
		.fragmentShaderPath = "shaders/fragment_defaultGammaCorrected.glsl",
		.textures = { texture },
		.buffers = { uniformBuffer },
	});

	rect.addComponent<scene::components::MeshComponent>(mesh, pipeline);
	rect.addComponent<DebugComponent<0>>(rect.handle());
	rect.addComponent<RectSpriteComponent>(marginPercent, color, index);

	auto text = rect.addChild();
	text.addComponent<DebugComponent<1>>(text.handle());

	auto&& tt = text.addComponent<scene::components::TransformComponent>({
		{ 0, 0, -0.1f },
		{ 0, 0, 0, 1 },
		{ 1, 1, 0 }
	});
	auto tmat = tt.getTransformMatrix();

	auto&& tc = text.addComponent<text::TextComponent>(textToDisplay, std::vector{ uniformBuffer }, "Arial");

	updateTextInRect(rect);

	return rect;
}

void moveRect(Entity rect, float dx, float boundBegin, float boundEnd) {
	auto&& t = rect.getComponent<Transform>();

	t.position.x += dx;
	if (t.position.x < boundBegin) {
		t.position.x += boundEnd - boundBegin;
	} else if (t.position.x > boundEnd) {
		t.position.x += boundBegin - boundEnd;
	}

	updateTextInRect(rect);
}

struct Particle {
	float rotation;
};

void present(
	std::u32string selected,
	bool& presentation,
	std::vector<Ref<gfx::Buffer>> buff,
	std::vector<ecs::Entity>& toRemove,
	std::mutex& mutex
) {
	presentation = true;

	// SEE-THROUGH RECT

	auto screenRect = scene::SceneManager::get()->currentScene()->newEntity();
	auto&& t = screenRect.addComponent<Transform>({
		{ 0, windowHeight, -0.4f },
		{ 0, 0, 0, 1 },
		{ windowWidth, windowHeight, 0 }
	 });

	Color color{ 0, 0, 0, 0.9 };

	auto mesh = asset::mesh::Mesh::create(std::span(defaultVertices), indices);

	auto pipeline = gfx::Renderer::getCurrent()->getPipelineManager()->create(
		{ .vertexShaderPath = "shaders/vertex_default.glsl",
		  .fragmentShaderPath = "shaders/fragment_default.glsl",
		  .textures = { gfx::Renderer::getCurrent()->getTextureManager()->createTexture2D(1, 1, &color) },
		  .buffers = buff }
	);

	screenRect.addComponent<scene::components::MeshComponent>(mesh, pipeline);
	screenRect.addComponent<DebugComponent<0>>(screenRect.handle());

	// TEXT

	auto text = scene::SceneManager::get()->currentScene()->newEntity();

	text.addComponent<DebugComponent<1>>(text.handle());

	auto&& tt = text.addComponent<Transform>({
		{ 0.f, 0.f, 0.f },
		{ 0.f, 0.f, 0.f, 1.f },
		{ 1.f, 1.f, 0.f }
	  });
	auto&& tc = text.addComponent<text::TextComponent>(std::move(selected), buff, "Arial");
	auto tmat = tt.getTransformMatrix();

	auto bottomLeft = tc.bottomLeft(tmat);
	auto topRight = tc.topRight(tmat);
	auto size = topRight - bottomLeft;

	const auto fontWidth = 1000.f;

	auto fontSize = fontWidth / size.x;

	size *= fontSize;
	tt.scale *= fontSize;

	bottomLeft *= fontSize;

	tt.position = { (windowWidth - size.x) / 2, -bottomLeft.y - (windowHeight + size.y) / 2 + windowHeight, -0.5f };

	/*auto bottomLeft = textComp.bottomLeft(tmat);
	auto topRight = textComp.topRight(tmat);
	auto size = topRight - bottomLeft;

	auto marginX = rectTransform.scale.x * marginPercent;

	auto availableWidth = rectTransform.scale.x - 2 * marginX;

	auto fontSize = availableWidth / size.x;

	size *= fontSize;
	bottomLeft *= fontSize;

	textTransform.position = rectTransform.position +
		float3{ marginX - bottomLeft.x,
				-bottomLeft.y - (rectTransform.scale.y + size.y) / 2.f,
				rectTransform.position.z - 0.1f };
	textTransform.scale *= fontSize;*/

	// ADD PARTICLES

	{
		float triangleHeight = sqrtf(3) / 2.f;

		Color red = { 1, 0, 0, 1 };
		Color green = { 0, 1, 0, 1 };
		Color blue = { 0, 0, 1, 1 };

		auto renderer = gfx::Renderer::getCurrent();

		auto redT = renderer->getTextureManager()->createTexture2D(1, 1, &red);
		auto greenT = renderer->getTextureManager()->createTexture2D(1, 1, &green);
		auto blueT = renderer->getTextureManager()->createTexture2D(1, 1, &blue);

		auto mesh = asset::mesh::Mesh::create(std::span(defaultVertices), indices);

		auto pipelineRed =
			renderer->getPipelineManager()->create({ .vertexShaderPath = "shaders/vertex_default.glsl",
													 .fragmentShaderPath = "shaders/fragment_default.glsl",
													 .textures{ redT },
													 .buffers = buff });
		auto pipelineGreen =
			renderer->getPipelineManager()->create({ .vertexShaderPath = "shaders/vertex_default.glsl",
													 .fragmentShaderPath = "shaders/fragment_default.glsl",
													 .textures{ greenT },
													 .buffers = buff });
		auto pipelineBlue =
			renderer->getPipelineManager()->create({ .vertexShaderPath = "shaders/vertex_default.glsl",
													 .fragmentShaderPath = "shaders/fragment_default.glsl",
													 .textures{ blueT },
													 .buffers = buff });

		auto mt = std::mt19937(std::random_device{}());
		auto randInt = std::uniform_int_distribution(0, 2);
		auto randAngle1 = std::uniform_real_distribution(30.f, 85.f);
		auto randAngle2 = std::uniform_real_distribution(95.f, 150.f);
		auto randSpeed = std::uniform_real_distribution(15.f, 40.f);
		auto randRotation = std::uniform_real_distribution(glm::radians(-2.f), glm::radians(2.f));

		std::vector<Vertex> particleVertices{
			{  { 0.5f, -0.5f, 0.1f }, { 0.f, 0.f } },
			{ { -0.5f, -0.5f, 0.1f }, { 1.f, 0.f } },
			{	  { 0.5f, 0.5f, 0.1f }, { 0.f, 1.f } },
			{  { -0.5f, 0.5f, 0.1f }, { 1.f, 1.f } },
		};

		mesh = asset::mesh::Mesh::create(std::span(particleVertices), indices);

		for (int i = 0; i != 300; ++i) {
			auto particle = scene::SceneManager::get()->currentScene()->newEntity();

			particle.addComponent<Particle>(randRotation(mt));
			particle.addComponent<Transform>({
				{ 0, 0, -0.6f },
				{ 0, 0, 0, 1 },
				{ 10, 10, 0 }
			  });
			auto whichPipeline = randInt(mt);
			particle.addComponent<scene::components::MeshComponent>(
				mesh,
				whichPipeline == 0 ? pipelineRed : (whichPipeline == 1 ? pipelineGreen : pipelineBlue)
			);
			auto&& movable = particle.addComponent<physics::Moveable>();
			float angle = glm::radians(randAngle1(mt));
			movable.velocity += float2{ cos(angle), sin(angle) } * randSpeed(mt) / 1.f;
			// Logger::debug("({}, {})", movable.velocity.x, movable.velocity.y);
		}
		for (int i = 0; i != 300; ++i) {
			auto particle = scene::SceneManager::get()->currentScene()->newEntity();

			particle.addComponent<Particle>(randRotation(mt));
			particle.addComponent<Transform>({
				{ windowWidth, 0, -0.6f },
				{ 0, 0, 0, 1 },
				{ 10, 10, 0 }
			  });
			auto whichPipeline = randInt(mt);
			particle.addComponent<scene::components::MeshComponent>(
				mesh,
				whichPipeline == 0 ? pipelineRed : (whichPipeline == 1 ? pipelineGreen : pipelineBlue)
			);
			auto&& movable = particle.addComponent<physics::Moveable>();
			float angle = glm::radians(randAngle2(mt));
			movable.velocity += float2{ cos(angle), sin(angle) } * randSpeed(mt) / 1.f;
			// Logger::debug("({}, {})", movable.velocity.x, movable.velocity.y);
		}
	}

	// REMOVE AFTER 5s

	std::thread([&mutex, &toRemove, &presentation, text, screenRect]() mutable {
		std::this_thread::sleep_for(std::chrono::seconds(5));
		// auto&& scene = scene::SceneManager::get()->currentScene();

		// vvv DEBILIZM vvv
		// scene->removeEntity(screenRect);
		// scene->removeEntity(text);
		// ^^^ IDIOTYZM ^^^

		{
			auto lock = std::lock_guard(mutex);
			toRemove.push_back(screenRect.handle());
			toRemove.push_back(text.handle());
		}

		presentation = false;
	}).detach();
}

class ProjectSelectorApp: public Application {
	net::UDPSocket socket;

	float velocity = 0;
	bool speedup = false;

	float combinedWidth = 0;

	std::vector<ecs::Entity> toDeleteInUpdate;
	std::mutex toDeleteInUpdateMutex;

	Entity rectParent;

	const float3 rectSize = { 300.f, 200.f, 0.f };

	std::vector<Entity> rectEntities;

	void initRects() {
		{
			auto twister = std::mt19937(std::random_device{}());
			auto rng = std::uniform_real_distribution(0.2f, 1.0f);

			std::vector<std::u32string> texts{
				/*U"Bright Bodies",   U"Tacticool Fishing", U"Archimedes",
				U"Student Trainer", U"Czasoport",		 U"Racing Cart",
				U"Monstrous Tide",  U"Moose's Tales",	 U"Brewer",
				U"Ej Mordo Flanki", U"Packet Defender",	 U"Butcher of Monsters",
				U"Student test",	   U"Shotguns"*/
			};
			{
				auto file = std::ifstream("projekty.txt");
				std::string input;
				while (std::getline(file, input)) {
					texts.push_back(text::convertTo<char32_t>(std::string_view(input)));
				}
			}
			std::ranges::shuffle(texts, twister);

			rectParent = scene::SceneManager::get()->currentScene()->newEntity();

			float posBegin = (windowWidth - rectSize.x * texts.size()) / 2.f;

			for (auto&& [i, t] : texts | std::views::enumerate) {
				auto rect = newRectWithText(
					{ rng(twister), rng(twister), rng(twister), 1.f },
					t,
					{ posBegin + i * rectSize.x, (windowHeight + rectSize.y) / 2.f, 0.f },
					rectSize,
					i
				);
				rect.setParent(rectParent);
				rectEntities.push_back(rect);
			}
			while (rectParent.childrenCount() < minRects) {
				for (auto&& [i, child] :
					 rectParent.children() | std::views::take(rectParent.childrenCount()) | std::views::enumerate) {
					newRectWithText(
						child.getComponent<RectSpriteComponent>().color,
						child.firstChild().getComponent<text::TextComponent>().string(),
						{ 0, (windowHeight + rectSize.y) / 2, 0 },
						rectSize,
						child.getComponent<RectSpriteComponent>().index,
						child.getComponent<RectSpriteComponent>().marginPercent
					)
						.setParent(rectParent);
				}
			}
			posBegin = (windowWidth - rectSize.x * rectParent.childrenCount()) / 2.f;
			for (auto [i, rect2] : rectParent.children() | std::views::enumerate) {
				/*moveRect(
				 rect2,
				 (rect2.getComponent<Transform>().position.x < windowWidth / 2) ? rectSize.x / 2 :
																				  -rectSize.x / 2,
				 (windowWidth - rectParent.childrenCount() * rectSize.x) / 2.f,
				 (windowWidth + rectParent.childrenCount() * rectSize.x) / 2.f
				);*/
				rect2.getComponent<Transform>().position.x = posBegin + i * rectSize.x;
				// updateTextInRect(rect);
			}
		}
	}

	void init() override {
		socket.bind(30'420);

		Ref<Scene> testScene = createRef<Scene>();

		_physics = createRef<physics::System>(testScene->domain());

		scene::SceneManager::get()->changeScene(testScene);

		std::vector<Vertex> vertices{
			{  { 0.f, 0.f, 0.1f }, { 0.f, 0.f } },
			{ { -1.f, 0.f, 0.1f }, { 1.f, 0.f } },
			{  { 0.f, 1.f, 0.1f }, { 0.f, 1.f } },
			{ { -1.f, 1.f, 0.1f }, { 1.f, 1.f } },
		};

		Ref<gfx::Renderer> renderer = gfx::Renderer::getCurrent();

		Ref<asset::mesh::Mesh> mesh = asset::mesh::Mesh::create<Vertex>(vertices, indices);

		auto uniformBuffer =
			renderer->getBufferManager()->createBuffer(gfx::BufferType::uniform, &ubo, sizeof(UniformBuffer));

		float fullCoverSize = 0.2;
		float fadeCoverSize = 0.2;
		{ // full covers
			auto coverPipeline = renderer->getPipelineManager()->create({
				.vertexShaderPath = "shaders/vertex_default.glsl",
				.fragmentShaderPath = "shaders/fragment_cover.glsl",
				.textures = {},
				.buffers = { uniformBuffer },
			});

			auto cover1 = testScene->newEntity();
			cover1.addComponent<Transform>({
				{ 0.f, windowHeight, -0.2f },
				{ 0, 0, 0, 1 },
				{ fullCoverSize * windowWidth, windowHeight, 0.f }
			   });
			cover1.addComponent<scene::components::MeshComponent>(mesh, coverPipeline);
			cover1.addComponent<DebugComponent<0>>(cover1.handle());
			auto cover2 = testScene->newEntity();
			cover2.addComponent<Transform>({
				{ windowWidth * (1.f - fullCoverSize), windowHeight, -0.2f },
				{ 0, 0, 0, 1 },
				{ fullCoverSize * windowWidth, windowHeight, 0.f }
			   });
			cover2.addComponent<scene::components::MeshComponent>(mesh, coverPipeline);
			cover2.addComponent<DebugComponent<0>>(cover2.handle());
		}
		{ // fades
			auto fadePipeline = renderer->getPipelineManager()->create({
				.vertexShaderPath = "shaders/vertex_default.glsl",
				.fragmentShaderPath = "shaders/fragment_gradient.glsl",
				.textures = {},
				.buffers = { uniformBuffer },
			});

			auto fade1 = testScene->newEntity();
			fade1.addComponent<Transform>({
				{ windowWidth * (fullCoverSize + fadeCoverSize), 0, -0.2f },
				glm::angleAxis(glm::radians(180.f), float3{ 0.f, 0.f, 1.f }
				   ) * Quat{ 0, 0, 0, 1 },
				{ fadeCoverSize * windowWidth, windowHeight, 0.f }
			   });
			fade1.addComponent<scene::components::MeshComponent>(mesh, fadePipeline);
			fade1.addComponent<DebugComponent<0>>(fade1.handle());
			auto fade2 = testScene->newEntity();
			fade2.addComponent<Transform>({
				{ windowWidth * (1.f - fullCoverSize - fadeCoverSize), windowHeight, -0.2f },
				{ 0, 0, 0, 1 },
				{ fadeCoverSize * windowWidth, windowHeight, 0.f }
			   });
			fade2.addComponent<scene::components::MeshComponent>(mesh, fadePipeline);
			fade2.addComponent<DebugComponent<0>>(fade2.handle());
		}
		{ // selection line
			Color color = { 1, 0, 0, 1 };
			auto selectionLinePipeline = renderer->getPipelineManager()->create({
				.vertexShaderPath = "shaders/vertex_default.glsl",
				.fragmentShaderPath = "shaders/fragment_selectionLine.glsl",
				.textures = { renderer->getTextureManager()->createTexture2D(1, 1, &color) },
				.buffers = { uniformBuffer },
			});

			const auto lineLength = windowHeight / 2;
			const auto lineThickness = 2.f;

			auto line = testScene->newEntity();
			line.addComponent<Transform>({
				{ (windowWidth - lineThickness) / 2, (windowHeight + lineLength) / 2, -0.3f },
				{ 0, 0, 0, 1 },
				{ lineThickness, lineLength, 0 }
			 });
			line.addComponent<scene::components::MeshComponent>(mesh, selectionLinePipeline);
			line.addComponent<DebugComponent<0>>(line.handle());
		}

		/*{
			auto gradient = testScene->newEntity();
			auto&& t = gradient.addComponent<scene::components::TransformComponent>({
				{ 0, windowHeight, -0.2f },
				{ 0, 0, 0, 1 },
				{ windowWidth, windowHeight, 0 }
			 });

			gradient.addComponent<scene::components::MeshComponent>({ gradientMesh, gradientPipeline });
		}*/

		initRects();

		InputHandler::get()._bindKey(VK_SPACE, [&]() {
			if (!finished && !speedup && !brake && !ongoing && !presentation) {
				// finished = true;
				speedup = true;
				ongoing = true;
				velocity = 0.01f;
			}
		});

		{
			auto text = testScene->newEntity();
			auto&& t = text.addComponent<Transform>({
				{ 0, 0, 0 },
				{ 0, 0, 0, 1 },
				{ 1, 1, 0 }
			});
			auto&& tc = text.addComponent<text::TextComponent>(
				U"Demo silnika Archimedes",
				std::vector{ uniformBuffer },
				"Arial"
			);
			text.addComponent<DebugComponent<1>>(text.handle());

			auto tmat = t.getTransformMatrix();
			auto bottomLeft = tc.bottomLeft(tmat);
			auto topRight = tc.topRight(tmat);
			auto size = topRight - bottomLeft;

			auto textWidth = 1200.f;
			auto fontSize = textWidth / size.x;

			size *= fontSize;
			t.scale *= fontSize;
			t.position = { (windowWidth - textWidth) / 2, windowHeight * (0.8), -0.3f };
		}

		{
			auto text = testScene->newEntity();
			auto&& t = text.addComponent<Transform>({
				{ 0, 0, 0 },
				{ 0, 0, 0, 1 },
				{ 1, 1, 0 }
			});
			auto&& tc = text.addComponent<text::TextComponent>(U"(działa)", std::vector{ uniformBuffer }, "Arial");
			text.addComponent<DebugComponent<1>>(text.handle());

			auto tmat = t.getTransformMatrix();
			auto bottomLeft = tc.bottomLeft(tmat);
			auto topRight = tc.topRight(tmat);
			auto size = topRight - bottomLeft;

			auto textWidth = 200.f;
			auto fontSize = textWidth / size.x;

			size *= fontSize;
			t.scale *= fontSize;
			t.position = { (windowWidth - textWidth) / 2, windowHeight * (0.7), -0.3f };
		}

		{
			auto text = testScene->newEntity();
			auto&& t = text.addComponent<Transform>({
				{ 0, 0, 0 },
				{ 0, 0, 0, 1 },
				{ 1, 1, 0 }
			});
			auto&& tc = text.addComponent<text::TextComponent>(
				U"\x2193Kliknij w spację\x2193",
				std::vector{ uniformBuffer },
				"Arial"
			);
			text.addComponent<DebugComponent<1>>(text.handle());

			auto tmat = t.getTransformMatrix();
			auto bottomLeft = tc.bottomLeft(tmat);
			auto topRight = tc.topRight(tmat);
			auto size = topRight - bottomLeft;

			auto textWidth = 800.f;
			auto fontSize = textWidth / size.x;

			size *= fontSize;
			t.scale *= fontSize;
			t.position = { (windowWidth - textWidth) / 2, windowHeight * (1.0f - 0.9), -0.3f };
			text.addComponent<std::pair<float, float>>((windowWidth - textWidth) / 2, size.x);
		}
	}

	bool brake = false;
	bool finished = false;
	bool presentation = false;
	bool ongoing = false;
	const int minRects = floor(windowWidth / rectSize.x) + 2;

	void update() {
		char buffer[128] = {};

		if (socket.dataAvalible()) {
			socket.recv(buffer, sizeof(buffer) - 1);
			if (std::string_view(buffer) == "SPIN!") {
				if (!finished && !speedup && !brake && !ongoing && !presentation) {
					// finished = true;
					speedup = true;
					ongoing = true;
					velocity = 0.01f;
				}
			}
		}
		if (!finished && !speedup && !brake && !ongoing && !presentation) {
			// finished = true;
			speedup = true;
			ongoing = true;
			velocity = 0.01f;
		}

		static float frame = 0;
		auto&& domain = scene::SceneManager::get()->currentScene()->domain();

		std::this_thread::sleep_for(std::chrono::milliseconds(16));

		if (speedup) {
			velocity *= 1.05f;
		} else if (velocity > 0.1f && brake) {
			if (velocity > 20.f) {
				velocity *= 0.99;
			} else if (velocity < 20.f && velocity > 5.f) {
				velocity *= 0.98;
			} else if (velocity < 5.f) {
				velocity *= 0.95;
			}
			if (velocity < 0.1f) {
				// FINISHED SPINNING
				velocity = 0;
				brake = false;
				ongoing = false;
				finished = true;
				// presentation = true;
			}
		}
		if (velocity > 200.f && speedup) {
			speedup = false;
			std::jthread([&]() {
				auto random = std::mt19937(std::random_device()());
				std::this_thread::sleep_for(
					std::chrono::milliseconds(std::uniform_int_distribution<int>(1'000, 3'000)(random))
				);
				brake = true;
			}).detach();
		}

		if (finished) {
			finished = false;
			for (auto rect : rectParent.children()) {
				auto&& t = rect.getComponent<Transform>();
				if (t.position.x > windowWidth / 2 - rectSize.x && t.position.x <= windowWidth / 2) {
					Logger::debug(
						"{}",
						text::convertTo<char>(
							std::u32string_view(rect.firstChild().getComponent<text::TextComponent>().string())
						)
					);

					present(
						rect.firstChild().getComponent<text::TextComponent>().string(),
						presentation,
						std::vector{ gfx::Renderer::getCurrent()
										 ->getBufferManager()
										 ->createBuffer(gfx::BufferType::uniform, &ubo, sizeof(UniformBuffer)) },
						toDeleteInUpdate,
						toDeleteInUpdateMutex
					);

					int index = rect.getComponent<RectSpriteComponent>().index;

					std::vector<Entity> toRemove;
					for (auto&& rect2 : rectParent.children()) {
						if (rect2.getComponent<RectSpriteComponent>().index == index) {
							toRemove.push_back(rect2.firstChild());
							toRemove.push_back(rect2);
						}
					}
					for (auto&& e : toRemove) {
						scene::SceneManager::get()->currentScene()->removeEntity(e);
					}

					while (rectParent.childrenCount() < minRects && rectParent.childrenCount() != 0) {
						for (auto&& [i, child] : rectParent.children() | std::views::take(rectParent.childrenCount()) |
								 std::views::enumerate) {
							newRectWithText(
								child.getComponent<RectSpriteComponent>().color,
								child.firstChild().getComponent<text::TextComponent>().string(),
								{ 0, (windowHeight + rectSize.y) / 2, 0 },
								rectSize,
								child.getComponent<RectSpriteComponent>().index,
								child.getComponent<RectSpriteComponent>().marginPercent
							)
								.setParent(rectParent);
						}
					}
					if (rectParent.childrenCount() != 0) {
						const float posBegin = (windowWidth - rectSize.x * rectParent.childrenCount()) / 2.f;
						for (auto [i, rect2] : rectParent.children() | std::views::enumerate) {
							/*moveRect(
							 rect2,
							 (rect2.getComponent<Transform>().position.x < windowWidth / 2) ? rectSize.x / 2 :
																							  -rectSize.x / 2,
							 (windowWidth - rectParent.childrenCount() * rectSize.x) / 2.f,
							 (windowWidth + rectParent.childrenCount() * rectSize.x) / 2.f
							);*/
							rect2.getComponent<Transform>().position.x = posBegin + i * rectSize.x;
							// updateTextInRect(rect);
						}
					}

					break;
				}
			}
		}

		if (rectParent.childrenCount() != 0) {
			for (auto rect : rectParent.children()) {
				moveRect(
					rect,
					-velocity,
					(windowWidth - rectParent.childrenCount() * rectSize.x) / 2.f,
					(windowWidth + rectParent.childrenCount() * rectSize.x) / 2.f
				);
			}
		}
		for (auto&& [entity, pair, transform, text] :
			 domain.view<std::pair<float, float>, Transform, text::TextComponent>().all()) {
			transform.scale = float3{ 50, 50, 0 } * (ongoing ? 0.f : (cos(frame) + 1) / 2.f) +
				float3{ 50, 50, 0 } * (ongoing ? 0.f : 1.f);

			auto tmat = transform.getTransformMatrix();
			auto bottomLeft = text.bottomLeft(tmat);
			auto topRight = text.topRight(tmat);
			auto size = topRight - bottomLeft;

			transform.position.x = pair.first + (pair.second - size.x) / 2;
		}
		frame += 0.025f;

		//_physics->update();

		{
			auto lock = std::lock_guard(toDeleteInUpdateMutex);
			for (auto&& e : toDeleteInUpdate) {
				scene::SceneManager::get()->currentScene()->removeEntity(e);
			}
			toDeleteInUpdate.clear();
		}

		{
			std::vector<ecs::Entity> toRemove;
			auto zAx = float3{ 0, 0, 1 };

			for (auto&& [entity, transform, particle, movable] :
				 domain.view<Transform, Particle, physics::Moveable>().all()) {
				transform.position.x += movable.velocity.x;
				transform.position.y += movable.velocity.y;
				movable.velocity.y -= 9.81 / 10;

				transform.rotation = glm::angleAxis(glm::eulerAngles(transform.rotation).z + particle.rotation, zAx) *
					transform.rotation;
				/*Logger::debug(
					"particle: ({}, {}, {})",
					transform.position.x,
					transform.position.y,
					transform.position.z
				);*/
				if (transform.position.y < -100) {
					toRemove.push_back(entity);
					// Logger::debug("particle- {}", entity);
				}
			}
			for (auto e : toRemove) {
				domain.kill(e);
			}
		}

		if (rectParent.childrenCount() == 0) {
			initRects();
		}
	}

	Ref<physics::System> _physics;
};
