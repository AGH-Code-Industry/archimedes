#include <concepts>
#include <execution>
#include <print>
#include <random>

#include <Application.h>
#include <Ecs.h>
#include <Font.h>
#include <Net.h>
#include <Scene.h>
#include <Text.h>
#include <audio/AudioManager.h>
#include <audio/SoundDevice.h>
#include <physics/System.h>
#include <physics/components/Colliding.h>
#include <stb_image.h>

using namespace arch;
namespace phy = physics;

using Transform = scene::components::TransformComponent;
using MeshComp = scene::components::MeshComponent;

namespace chr = std::chrono;
namespace rgs = std::ranges;
namespace vws = std::views;
using Clk = chr::high_resolution_clock;
using namespace std::chrono_literals;

using ClkNow = decltype(Clk::now());

const auto zAxis = float3{ 0, 0, 1 };

int key = -1;

struct Vertex {
	float3 position;
	float2 tex_coords;
};

struct Keyboard {
	static inline bool leftDown = false;
	static inline bool leftHold = false;

	static inline bool rightDown = false;
	static inline bool rightHold = false;

	static inline bool escDown = false;
	static inline bool escHold = false;

	static bool keyDown(int key) noexcept {
		if (key == GLFW_KEY_LEFT_SHIFT) {
			return leftDown;
		} else if (key == GLFW_KEY_ENTER) {
			return rightDown;
		} else if (key == GLFW_KEY_ESCAPE) {
			return escDown;
		}
		return false;
	}
};

std::vector<Vertex> defaultVertices{
	{  { 0.f, 0.f, 0.f }, { 0.f, 1.f } },
	{ { -1.f, 0.f, 0.f }, { 1.f, 1.f } },
	{  { 0.f, 1.f, 0.f }, { 0.f, 0.f } },
	{ { -1.f, 1.f, 0.f }, { 1.f, 0.f } },
};
std::vector<Vertex> particleVertices{
	{  { 0.5f, -0.5f, 0.f }, { 0.f, 0.f } },
	{ { -0.5f, -0.5f, 0.f }, { 1.f, 0.f } },
	{	  { 0.5f, 0.5f, 0.f }, { 0.f, 1.f } },
	{  { -0.5f, 0.5f, 0.f }, { 1.f, 1.f } },
};
std::vector<u32> indices{ 0, 1, 2, 2, 1, 3 };

inline Ref<gfx::texture::Texture> loadTexture(std::string_view filename) noexcept {
	auto renderer = gfx::Renderer::getCurrent();

	stbi_set_flip_vertically_on_load(true);
	int ignored;
	int width;
	int height;
	unsigned char* loadedTextureData = stbi_load(filename.data(), &width, &height, &ignored, STBI_rgb_alpha);

	if (loadedTextureData == nullptr) {
		throw std::exception("ni ma pliku :(");
	}

	auto textureData = std::vector<Color>(width * height);

	std::for_each(
		std::execution::par_unseq,
		textureData.begin(),
		textureData.end(),
		[begin = textureData.data(), &loadedTextureData](Color& color) {
			const auto i = &color - begin;
			color.r = loadedTextureData[4 * i + 0] / 255.f;
			color.g = loadedTextureData[4 * i + 1] / 255.f;
			color.b = loadedTextureData[4 * i + 2] / 255.f;
			color.a = loadedTextureData[4 * i + 3] / 255.f;
		}
	);
	stbi_image_free(loadedTextureData);

	return renderer->getTextureManager()->createTexture2D(width, height, textureData.data());
}

struct WulkanComponent {
	// net::UDPSocket socket;

	int particleCount;
	int key;
	std::uniform_real_distribution<float> explosionAngleDistrib;
	std::uniform_real_distribution<float> velocityDistrib;
	std::uniform_real_distribution<float> rotationSpeedDistrib;
	std::uniform_real_distribution<float> sizeDistrib;
	std::uniform_real_distribution<float> eDistrib;

	Ref<gfx::texture::Texture> texture;
	Ref<gfx::pipeline::Pipeline> particlePipeline;
	Ref<asset::mesh::Mesh> particleMesh;

	float3 particlePosition;
};

inline auto makeMesh(std::span<Vertex>&& vs, std::span<u32>&& is) noexcept {
	auto renderer = gfx::Renderer::getCurrent();
	return asset::mesh::Mesh::create(
		renderer->getBufferManager()->createVertexBuffer(std::move(vs)),
		renderer->getBufferManager()->createIndexBuffer(std::move(is))
	);
}

inline auto defaultUniformBuffer() noexcept {
	auto renderer = gfx::Renderer::getCurrent();
	auto windowSize = renderer->getWindow()->getSize();

	struct UBO {
		Mat4x4 projection;
	};

	static UBO ubo{ glm::ortho(0.f, (float)windowSize.x, 0.f, (float)windowSize.y) };

	return renderer->getBufferManager()->createBuffer(gfx::BufferType::uniform, &ubo, sizeof(ubo));
}

inline void setupWulkan(Entity wulkan, int key, bool right) noexcept {
	auto renderer = gfx::Renderer::getCurrent();
	auto wulkanTexture = loadTexture("textures/wulkan.png");
	auto particleTexture = loadTexture("textures/particle.png");
	auto texWidth = wulkanTexture->getWidth();
	auto texHeight = wulkanTexture->getHeight();

	auto&& mesh = makeMesh(std::span(defaultVertices), std::span(indices));
	auto&& pipeline = renderer->getPipelineManager()->create(gfx::pipeline::Pipeline::Desc{
		.vertexShaderPath = "shaders/vertex_default.glsl",
		.fragmentShaderPath = "shaders/fragment_default.glsl",
		.textures = { wulkanTexture },
		.buffers = { defaultUniformBuffer() } });
	auto&& particlePipeline = renderer->getPipelineManager()->create(gfx::pipeline::Pipeline::Desc{
		.vertexShaderPath = "shaders/vertex_default.glsl",
		.fragmentShaderPath = "shaders/fragment_default.glsl",
		.textures = { particleTexture },
		.buffers = { defaultUniformBuffer() } });

	float scale = 0.25f;
	float windowLeftSpace = (gfx::Renderer::getCurrent()->getWindow()->getSize().x - 2.f * texWidth * scale) / 3.f;
	auto&& transform = wulkan.addComponent(Transform{
		.position = float3{ windowLeftSpace + right * (windowLeftSpace + texWidth * scale), texHeight * scale, 0 },
		.rotation = { 0, 0, 0, 1 },
		.scale = float3{ texWidth, texHeight, 0 }
		  * scale
	  });

	wulkan.addComponent(MeshComp{ .mesh = mesh, .pipeline = pipeline });

	auto&& wulkanComp = wulkan.addComponent(WulkanComponent{
		//.socket = net::UDPSocket(30'420),
		.particleCount = 200,
		.key = key,
		.explosionAngleDistrib = std::uniform_real_distribution(glm::radians(-45.f), glm::radians(45.f)),
		.velocityDistrib = std::uniform_real_distribution(600.f, 1000.f),
		.rotationSpeedDistrib = std::uniform_real_distribution(glm::radians(-9.f), glm::radians(9.f)),
		.sizeDistrib = std::uniform_real_distribution(20.f, 40.f),
		.eDistrib = std::uniform_real_distribution(0.25f, 0.4f),
		.texture = wulkanTexture,
		.particlePipeline = particlePipeline,
		.particleMesh = makeMesh(std::span(particleVertices), std::span(indices)),
		.particlePosition = transform.position + float3{ texWidth * scale / 2.f, 0, 0 }
	});
}

template<bool Right>
struct ParticleComponent {
	glm::quat rotationQuaternion;
	float e;
	int collisionCounter = 0;
};

struct KillParticleFlag {
	static constexpr bool flagComponent = true;
};

inline void addRedBox(Entity parent) noexcept {
	auto redBox = parent.addChild();
	auto&& collider = parent.getComponent<phy::Colliding>();
	redBox.addComponent(Transform{
		.position = { collider.box.topLeft, -0.05f },
		.rotation = { 0, 0, 0, 1 },
		.scale = { abs(collider.box.topLeft.x - collider.box.bottomRight.x),
					 abs(collider.box.topLeft.y - collider.box.bottomRight.y),
					 0 }
	   });

	auto color = Color(1, 0, 0, 1);
	auto texture = gfx::Renderer::getCurrent()->getTextureManager()->createTexture2D(1, 1, &color);

	auto mesh = asset::mesh::Mesh::create<Vertex>(defaultVertices, indices);

	auto pipeline =
		gfx::Renderer::getCurrent()->getPipelineManager()->create({ .vertexShaderPath = "shaders/vertex_default.glsl",
																	.fragmentShaderPath =
																		"shaders/fragment_default.glsl",
																	.textures = { texture },
																	.buffers = { defaultUniformBuffer() } });

	redBox.addComponent(MeshComp{ .mesh = mesh, .pipeline = pipeline });
}

inline void setupParticle(
	Scene& scene,
	Entity particle,
	float3 position,
	std::uniform_real_distribution<float>& explosionAngleDistrib,
	std::uniform_real_distribution<float>& velocityDistrib,
	std::uniform_real_distribution<float>& rotationSpeedDistrib,
	std::uniform_real_distribution<float>& sizeDistrib,
	std::uniform_real_distribution<float>& eDistrib,
	const Ref<gfx::pipeline::Pipeline>& pipeline,
	const Ref<asset::mesh::Mesh>& mesh,
	bool right
) noexcept {
	static auto rng = std::mt19937(std::random_device{}());
	static auto initialAngleDistrib = std::uniform_real_distribution(0.f, 360.f);
	static auto groundCollisionHandler = [&domain = scene.domain()](const ecs::Entity lhs, const ecs::Entity rhs) {
		auto colliderOpt = domain.tryGetComponent<phy::Colliding>(lhs);
		auto gBoxOpt = domain.tryGetComponent<const phy::BBox>(rhs);
		if (!colliderOpt || !gBoxOpt) {
			return;
		}
		bool right = domain.hasComponent<ParticleComponent<true>>(lhs);

		float e;
		if (right) {
			e = domain.getComponent<ParticleComponent<true>>(lhs).e;
			if (++domain.getComponent<ParticleComponent<true>>(lhs).collisionCounter >= 3) {
				domain.addComponent<KillParticleFlag>(lhs);
			}
		} else {
			e = domain.getComponent<ParticleComponent<false>>(lhs).e;
			if (++domain.getComponent<ParticleComponent<false>>(lhs).collisionCounter >= 3) {
				domain.addComponent<KillParticleFlag>(lhs);
			}
		}

		auto&& collider = *colliderOpt;
		auto&& gBox = *gBoxOpt;

		auto insideDistance = gBox.topLeft.y - collider.box.bottomRight.y;

		collider.body.velocity.x *= e;
		collider.body.velocity.y *= -e;

		collider.box.topLeft.y += insideDistance;
		collider.box.bottomRight.y += insideDistance;
		collider.body.center.position.y += insideDistance;
	};

	const float size = sizeDistrib(rng);
	auto&& transform = particle.addComponent(Transform{
		.position = position + float3{	   0,	  0, -0.1f },
		.rotation = glm::angleAxis(initialAngleDistrib(rng), zAxis),
		.scale = float3{ size, size,	 0 }
	 });
	particle.addComponent(MeshComp{ .mesh = mesh, .pipeline = pipeline });

	if (right) {
		particle.addComponent(ParticleComponent<true>{ .rotationQuaternion =
														   glm::angleAxis(rotationSpeedDistrib(rng), zAxis),
													   .e = std::sqrt(eDistrib(rng)) });
	} else {
		particle.addComponent(ParticleComponent<false>{ .rotationQuaternion =
															glm::angleAxis(rotationSpeedDistrib(rng), zAxis),
														.e = std::sqrt(eDistrib(rng)) });
	}

	auto angle = explosionAngleDistrib(rng);
	auto vel = velocityDistrib(rng);

	particle.addComponent(phy::Colliding{
		.action = groundCollisionHandler,
		.body = { .center = { .mass = 1.f, .position = position },
				  .force = float2{ 0, -9.81f * 100 },
				  .velocity = float2{ -sin(angle), cos(angle) } * vel },
		.box = { .topLeft = transform.position + float3{ -size, size, 0 } / 2.f,
				 .bottomRight = transform.position - float3{ -size, size, 0 } / 2.f }
	  });
	if (cos(angle) < 0.f) {
		Logger::debug("<0: cos({}) = {}", angle, cos(angle));
	}
	// addRedBox(particle);
}

struct SoundManager {
	std::string soundFile;
	audio::SoundDevice device;
	audio::SoundBank soundBank;
	audio::AudioManager* audioManager{};
	std::jthread* audioThread{};

	void init(const std::string& sound) {
		soundFile = sound;
		audioManager = new audio::AudioManager(&soundBank);
		audioThread = new std::jthread(&audio::AudioManager::play, audioManager);
		soundBank.addClip(soundFile);
		soundBank.loadInitialGroups();
	}

	~SoundManager() {
		audioManager->stop();
		audioThread->join();
		delete audioManager;
	}
};

inline void explosionText(Scene& scene) noexcept {
	static auto rng = std::mt19937(std::random_device{}());
	static auto fontSizeDistrib = std::uniform_real_distribution(75.f, 150.f);
	static auto angleDistrib = std::uniform_real_distribution(glm::radians(-45.f), glm::radians(45.f));
	static const auto margin = 100.f;
	static auto xDistrib =
		std::uniform_real_distribution(0.f, (float)gfx::Renderer::getCurrent()->getWindow()->getSize().x - margin);
	static auto yDistrib =
		std::uniform_real_distribution(0.f, (float)gfx::Renderer::getCurrent()->getWindow()->getSize().y - margin);

	auto boomText = scene.newEntity();
	boomText.addComponent(Transform{
		.position = float3{ xDistrib(rng), yDistrib(rng), -0.2f },
		.rotation = glm::angleAxis(angleDistrib(rng), zAxis) * Quat{ 0, 0, 0, 1 },
		.scale = float3{ 1, 1, 0 }
		   * fontSizeDistrib(rng)
	  });
	boomText.addComponent(
		text::TextComponent(U"BOOM!", { defaultUniformBuffer() }, *font::FontDB::get()["Arial"]->bold())
	);
	boomText.addComponent<ClkNow>(Clk::now());
}

inline void explosion(Scene& scene, Entity wulkan) noexcept {
	auto&& wulkanComp = wulkan.getComponent<WulkanComponent>();

	for (auto i = 0u; i != wulkanComp.particleCount; ++i) {
		setupParticle(
			scene,
			wulkan.addChild(),
			wulkanComp.particlePosition,
			wulkanComp.explosionAngleDistrib,
			wulkanComp.velocityDistrib,
			wulkanComp.rotationSpeedDistrib,
			wulkanComp.sizeDistrib,
			wulkanComp.eDistrib,
			wulkanComp.particlePipeline,
			wulkanComp.particleMesh,
			wulkanComp.key == GLFW_KEY_ENTER
		);
	}

	auto&& sm = scene.domain().global<SoundManager>();
	auto&& ac = wulkan.getComponent<audio::AudioSourceComponent>();

	explosionText(scene);

	/*try {
		sm.audioManager->stopSource(ac);
	} catch (...) {}
	sm.audioManager->synchronize(scene.domain());*/
	sm.audioManager->playSource(ac);
}

inline void wulkanSystem(Scene& scene) noexcept {
	for (auto&& [entity, wulkan] : scene.domain().view<WulkanComponent>().all()) {
		if (Keyboard::keyDown(wulkan.key)) {
			explosion(scene, Entity(scene, entity));
		}
	}

	// auto wulkan = Entity(scene, view.front());
	// auto&& [wulkanComp] = view.get(wulkan);

	///*if (wulkanComp.socket.dataAvalible()) {
	//	char buf[64]{};
	//	wulkanComp.socket.recv(buf, sizeof(buf));

	//	if (std::string_view(buf) == "BOOM!") {
	//		explosion(scene, wulkan);
	//	}
	//}*/

	// if (Keyboard::keyDown(wulkanComp.key)) {
	//	// Keyboard::keyDown = false;
	//	explosion(scene, wulkan);
	// }
}

inline void setupGround(Entity ground) noexcept {
	ground.addComponent(phy::BBox{
		.topLeft = float2{														-1'000'000,	   0 },
		.bottomRight = { gfx::Renderer::getCurrent()->getWindow()->getSize().x + 1'000'000, -1'000 }
	 });
}

template<bool Right>
struct CompetitionMaxFlag {
	static constexpr bool flagComponent = true;
};

template<bool Right>
struct CompetitionCurrentFlag {
	static constexpr bool flagComponent = true;
};

inline void setupCompetition(Scene& scene) noexcept {
	auto buffer = defaultUniformBuffer();
	auto windowWidth = gfx::Renderer::getCurrent()->getWindow()->getSize().x;
	auto windowHeight = gfx::Renderer::getCurrent()->getWindow()->getSize().y;

	{
		auto max = scene.newEntity();
		max.addComponent(Transform{
			.position = { 10.f, windowHeight - 100.f, -0.2f },
			.rotation = { 0, 0, 0, 1 },
			.scale = { 100, 100, 0 }
		 });
		// max.addComponent<text::TextComponent>(text::TextComponent(U"", { buffer }, "Arial"));
		max.addComponent<CompetitionMaxFlag<false>>();

		auto curr = scene.newEntity();
		curr.addComponent(Transform{
			.position = { 10.f, windowHeight - 200.f, -0.2f },
			.rotation = { 0, 0, 0, 1 },
			.scale = { 100, 100, 0 }
		 });
		// curr.addComponent<text::TextComponent>(text::TextComponent(U"", { buffer }, "Arial"));
		curr.addComponent<CompetitionCurrentFlag<false>>();
	}

	{
		auto max = scene.newEntity();
		max.addComponent(Transform{
			.position = { 10.f + windowWidth / 2.f, windowHeight - 100.f, -0.2f },
			.rotation = { 0, 0, 0, 1 },
			.scale = { 100, 100, 0 }
		 });
		// max.addComponent<text::TextComponent>(text::TextComponent(U"", { buffer }, "Arial"));
		max.addComponent<CompetitionMaxFlag<true>>();

		auto curr = scene.newEntity();
		curr.addComponent(Transform{
			.position = { 10.f + windowWidth / 2.f, windowHeight - 200.f, -0.2f },
			.rotation = { 0, 0, 0, 1 },
			.scale = { 100, 100, 0 }
		 });
		// curr.addComponent<text::TextComponent>(text::TextComponent(U"", { buffer }, "Arial"));
		curr.addComponent<CompetitionCurrentFlag<true>>();
	}

	gfx::Renderer::getCurrent()->getWindow()->get();
}

static u32 maxParticlesLeft = 0;
static u32 maxParticlesRight = 0;

inline void competitionSystem(Scene& scene) noexcept {
	static bool firstTime = true;
	/*static ClkNow lastTime = Clk::now();

	auto now = Clk::now();
	if (now - lastTime < 1s) {
		return;
	}*/

	if (Keyboard::escDown) {
		maxParticlesLeft = 0;
		maxParticlesRight = 0;
	}

	if (firstTime) {
		firstTime = false;
		setupCompetition(scene);
	}

	{
		auto max = scene.entitiesWith<CompetitionMaxFlag<false>>().front();
		auto curr = scene.entitiesWith<CompetitionCurrentFlag<false>>().front();

		u32 currParticles = scene.domain().components<ParticleComponent<false>>().base().count();
		maxParticlesLeft = std::max(maxParticlesLeft, currParticles);

		auto buffer = defaultUniformBuffer();

		max.removeComponent<text::TextComponent>();
		max.addComponent(text::TextComponent(
			text::convertTo<char32_t>(std::format("Max: {}", maxParticlesLeft)),
			{ buffer },
			"Arial"
		));

		curr.removeComponent<text::TextComponent>();
		curr.addComponent(text::TextComponent(
			text::convertTo<char32_t>(std::format("Current: {}", currParticles)),
			{ buffer },
			"Arial"
		));
	}
	{
		auto max = scene.entitiesWith<CompetitionMaxFlag<true>>().front();
		auto curr = scene.entitiesWith<CompetitionCurrentFlag<true>>().front();

		u32 currParticles = scene.domain().components<ParticleComponent<true>>().base().count();
		maxParticlesRight = std::max(maxParticlesRight, currParticles);

		auto buffer = defaultUniformBuffer();

		max.removeComponent<text::TextComponent>();
		max.addComponent(text::TextComponent(
			text::convertTo<char32_t>(std::format("Max: {}", maxParticlesRight)),
			{ buffer },
			"Arial"
		));

		curr.removeComponent<text::TextComponent>();
		curr.addComponent(text::TextComponent(
			text::convertTo<char32_t>(std::format("Current: {}", currParticles)),
			{ buffer },
			"Arial"
		));
	}
}

class WulkanApp: public Application {
public:

	std::string explosionPath = "Explosion1.ogg";
	Ref<phy::System> physicsSystem;

	void init() {
		font::FontDB::get()["Arial"]->bold()->assure();

		auto scene = createRef<Scene>();
		scene::SceneManager::get()->changeScene(scene);

		physicsSystem = createRef<phy::System>(scene->domain());

		auto&& soundManager = scene->domain().global<SoundManager>();
		soundManager.init(explosionPath);

		auto wulkanLeft = scene->newEntity();
		setupWulkan(wulkanLeft, GLFW_KEY_LEFT_SHIFT, false);
		auto&& wulkanLeftComp = wulkanLeft.getComponent<WulkanComponent>();

		auto wulkanRight = scene->newEntity();
		setupWulkan(wulkanRight, GLFW_KEY_ENTER, true);
		auto&& wulkanRightComp = wulkanRight.getComponent<WulkanComponent>();

		auto listener = scene->newEntity();
		auto&& listenerComp = listener.addComponent<audio::ListenerComponent>();
		listenerComp.position.x = (wulkanLeftComp.particlePosition.x + wulkanRightComp.particlePosition.x) / 2.f;
		listenerComp.position.y = (wulkanLeftComp.particlePosition.y + wulkanRightComp.particlePosition.y) / 2.f;
		soundManager.audioManager->setListener(listenerComp, scene->domain());

		{
			auto&& source = wulkanLeft.addComponent<audio::AudioSourceComponent>();
			source.path = explosionPath;
			source.isLooped = false;
			source.position.x = wulkanLeftComp.particlePosition.x;
			source.position.y = wulkanLeftComp.particlePosition.y;
			source.velocity.x = 0;
			source.velocity.y = 0;
			source.rolloffFactor = 0.f;
			source.dontRemoveFinished = true;
		}
		{
			auto&& source = wulkanRight.addComponent<audio::AudioSourceComponent>();
			source.path = explosionPath;
			source.isLooped = false;
			source.position.x = wulkanRightComp.particlePosition.x;
			source.position.y = wulkanRightComp.particlePosition.y;
			source.velocity.x = 0;
			source.velocity.y = 0;
			source.rolloffFactor = 0.f;
			source.dontRemoveFinished = true;
		}

		auto ground = scene->newEntity();
		setupGround(ground);

		// auto window = gfx::Renderer::getCurrent()->getWindow()->get();
		// glfwSetKeyCallback(window, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
		//	if (key == ::key) {
		//		if (action == GLFW_PRESS) {
		//			Keyboard::keyDown = true;
		//			// Logger::debug("keyDown callback");
		//		} else if (action == GLFW_RELEASE) {
		//			Keyboard::keyDown = false;
		//			// Logger::debug("spaceUp callback");
		//		}
		//	}
		// });
	}

	void update() {
		// static auto lastT = Clk::now();
		// auto now = Clk::now();
		// std::this_thread::sleep_for(16ms);
		// lastT = now;
		physicsSystem->update();

		{
			auto keyState = glfwGetKey(gfx::Renderer::getCurrent()->getWindow()->get(), GLFW_KEY_LEFT_SHIFT);
			if (keyState == GLFW_PRESS && !Keyboard::leftHold) {
				Keyboard::leftDown = true;
				Keyboard::leftHold = true;
			} else if (keyState == GLFW_RELEASE) {
				Keyboard::leftDown = false;
				Keyboard::leftHold = false;
			}
		}
		{
			auto keyState = glfwGetKey(gfx::Renderer::getCurrent()->getWindow()->get(), GLFW_KEY_ENTER);
			if (keyState == GLFW_PRESS && !Keyboard::rightHold) {
				Keyboard::rightDown = true;
				Keyboard::rightHold = true;
			} else if (keyState == GLFW_RELEASE) {
				Keyboard::rightDown = false;
				Keyboard::rightHold = false;
			}
		}
		{
			auto keyState = glfwGetKey(gfx::Renderer::getCurrent()->getWindow()->get(), GLFW_KEY_ESCAPE);
			if (keyState == GLFW_PRESS && !Keyboard::rightHold) {
				Keyboard::escDown = true;
				Keyboard::escHold = true;
			} else if (keyState == GLFW_RELEASE) {
				Keyboard::escDown = false;
				Keyboard::escHold = false;
			}
		}

		auto&& scene = scene::SceneManager::get()->currentScene();

		auto&& domain = scene->domain();
		{
			auto view = domain.view<phy::Colliding, Transform, ParticleComponent<false>>(exclude<KillParticleFlag>);
			bool first = true;

			for (auto&& [entity, collider, transform, particle] : view.all()) {
				transform.position = float3{ collider.body.center.position, -0.1f };

				transform.rotation = particle.rotationQuaternion * transform.rotation;
			}
		}
		{
			auto view = domain.view<phy::Colliding, Transform, ParticleComponent<true>>(exclude<KillParticleFlag>);
			bool first = true;

			for (auto&& [entity, collider, transform, particle] : view.all()) {
				transform.position = float3{ collider.body.center.position, -0.1f };

				transform.rotation = particle.rotationQuaternion * transform.rotation;
			}
		}

		std::vector<ecs::Entity> toRemove;
		auto now = Clk::now();
		for (auto&& [entity, text, timePoint] : domain.view<text::TextComponent, ClkNow>().all()) {
			if (now - timePoint >= 1s) {
				toRemove.push_back(entity);
			}
		}

		auto view2 = domain.view<KillParticleFlag>();
		for (auto&& entity : view2) {
			// toRemove.push_back(entity);
			// if (!ecs::_details::EntityTraits::Version::hasNull(entity)) {
			toRemove.push_back(entity);
			//} else {
			//
			// WIDOKI COS ZLE DZIALAJA, GDY JEST JEDEN FLAG
			//
			//}
		}
		for (auto&& e : toRemove) {
			scene->removeEntity(e);
		}

		wulkanSystem(*scene);
		competitionSystem(*scene);
		domain.global<SoundManager>().audioManager->synchronize(domain);

		Keyboard::leftDown = false;
		Keyboard::rightDown = false;
		Keyboard::escDown = false;
	}
};
