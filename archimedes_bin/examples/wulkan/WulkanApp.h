#include <concepts>
#include <execution>
#include <random>

#include <Application.h>
#include <Ecs.h>
#include <Net.h>
#include <Scene.h>
#include <Text.h>
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

const auto zAxis = float3{ 0, 0, 1 };

struct Vertex {
	float3 position;
	float2 tex_coords;
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
	net::UDPSocket socket;

	int particleCount;
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

inline void setupWulkan(Entity wulkan) noexcept {
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

	float scale = 0.2f;
	auto&& transform = wulkan.addComponent(Transform{
		.position = float3{ (gfx::Renderer::getCurrent()->getWindow()->getSize().x - texWidth * scale) / 2.f,
							texHeight * scale,
							0 },
		.rotation = { 0, 0, 0, 1 },
		.scale = float3{ texWidth, texHeight, 0 }
		  * scale
	  });
	wulkan.addComponent(MeshComp{ .mesh = mesh, .pipeline = pipeline });
	auto&& wulkanComp = wulkan.addComponent(WulkanComponent{
		.socket = net::UDPSocket(30'420),
		.particleCount = 50,
		.explosionAngleDistrib = std::uniform_real_distribution(glm::radians(-45.f), glm::radians(45.f)),
		.velocityDistrib = std::uniform_real_distribution(600.f, 1000.f),
		.rotationSpeedDistrib = std::uniform_real_distribution(glm::radians(-4.5f), glm::radians(4.5f)),
		.sizeDistrib = std::uniform_real_distribution(20.f, 40.f),
		.eDistrib = std::uniform_real_distribution(0.25f, 0.4f),
		.texture = wulkanTexture,
		.particlePipeline = particlePipeline,
		.particleMesh = makeMesh(std::span(particleVertices), std::span(indices)),
		.particlePosition = transform.position + float3{ texWidth * scale / 2.f, 0, 0 }
	});
}

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
	const Ref<asset::mesh::Mesh>& mesh
) noexcept {
	static auto rng = std::mt19937(std::random_device{}());
	static auto initialAngleDistrib = std::uniform_real_distribution(0.f, 360.f);
	static auto groundCollisionHandler = [&domain = scene.domain()](const ecs::Entity lhs, const ecs::Entity rhs) {
		auto colliderOpt = domain.tryGetComponent<phy::Colliding>(lhs);
		auto gBoxOpt = domain.tryGetComponent<const phy::BBox>(rhs);
		if (!colliderOpt || !gBoxOpt) {
			return;
		}

		auto e = domain.getComponent<ParticleComponent>(lhs).e;
		if (++domain.getComponent<ParticleComponent>(lhs).collisionCounter >= 3) {
			domain.addComponent<KillParticleFlag>(lhs);
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

	particle.addComponent(ParticleComponent{ .rotationQuaternion = glm::angleAxis(rotationSpeedDistrib(rng), zAxis),
											 .e = std::sqrt(eDistrib(rng)) });

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
			wulkanComp.particleMesh
		);
	}
}

inline void wulkanSystem(Scene& scene) noexcept {
	auto view = scene.domain().view<WulkanComponent>();

	auto wulkan = Entity(scene, view.front());
	auto&& [wulkanComp] = view.get(wulkan);

	if (wulkanComp.socket.dataAvalible()) {
		char buf[64]{};
		wulkanComp.socket.recv(buf, sizeof(buf));

		if (std::string_view(buf) == "BOOM!") {
			explosion(scene, wulkan);
		}
	}
}

inline void setupGround(Entity ground) noexcept {
	ground.addComponent(phy::BBox{
		.topLeft = float2{														-1'000'000,	   0 },
		.bottomRight = { gfx::Renderer::getCurrent()->getWindow()->getSize().x + 1'000'000, -1'000 }
	 });
}

class WulkanApp: public Application {
public:

	Ref<phy::System> physicsSystem;

	void init() {
		scene::SceneManager::get()->changeScene(createRef<Scene>());
		physicsSystem = createRef<phy::System>(scene::SceneManager::get()->currentScene()->domain());

		auto wulkan = scene::SceneManager::get()->currentScene()->newEntity();
		setupWulkan(wulkan);

		auto ground = scene::SceneManager::get()->currentScene()->newEntity();
		setupGround(ground);
	}

	void update() {
		// static auto lastT = Clk::now();
		// auto now = Clk::now();
		std::this_thread::sleep_for(16ms);
		// lastT = now;
		physicsSystem->update();

		auto scene = scene::SceneManager::get()->currentScene();

		auto&& domain = scene->domain();
		auto view = domain.view<phy::Colliding, Transform, ParticleComponent>(exclude<KillParticleFlag>);
		bool first = true;

		for (auto&& [entity, collider, transform, particle] : view.all()) {
			transform.position = float3{ collider.body.center.position, -0.1f };

			transform.rotation = particle.rotationQuaternion * transform.rotation;
		}

		std::vector<ecs::Entity> toRemove;
		for (auto&& entity : domain.view<KillParticleFlag>()) {
			toRemove.push_back(entity);
		}
		for (auto&& e : toRemove) {
			scene->removeEntity(e);
		}

		wulkanSystem(*scene);
	}
};
