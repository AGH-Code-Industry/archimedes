#pragma once

#include <cmath>
#include <numbers>
#include <thread>

#include <Ecs.h>
#include <Engine.h>
#include <Scene.h>
#include <audio/AudioManager.h>
#include <audio/SoundDevice.h>
#include <physics/components/Velocity.h>

struct GraphicsManager {
	struct Vertex {
		float3 position;
		float2 tex_coords;
	};

	std::vector<Vertex> vertices{
		{ { -.25f, -.25f, 0.1f }, { 0.f, 0.f } },
		{	  { 0.f, .25f, 0.1f }, { .5f, 1.f } },
		{  { .25f, -.25f, 0.1f }, { 1.f, 0.f } },
	};

	std::vector<u32> indices{ 0, 1, 2 };

	Color pixels[1] = {
		Color{ 1, .5, 1, 1 }
	};

	Ref<gfx::Renderer> renderer;

	Ref<gfx::texture::Texture> texture;

	struct UniformBuffer {
		Mat4x4 projection;
	};

	Ref<gfx::Buffer> uniformBuffer;

	UniformBuffer ubo{ glm::ortho(0.f, 640.f, 0.f, 400.f) };

	Ref<gfx::pipeline::Pipeline> pipeline, pipeline2;

	Ref<asset::mesh::Mesh> mesh;

	GraphicsManager(){
		renderer = gfx::Renderer::getCurrent();
		texture = renderer->getTextureManager()->createTexture2D(1, 1, pixels);
		uniformBuffer = renderer->getBufferManager()->createBuffer(gfx::BufferType::uniform, &ubo, sizeof(UniformBuffer));
		pipeline = renderer->getPipelineManager()->create(
		{
			.vertexShaderPath = "shaders/vertex_default.glsl",
			.fragmentShaderPath = "shaders/fragment_default.glsl",
			.textures = { texture },
			.buffers = { uniformBuffer },
		}
		);
		pipeline2 = renderer->getPipelineManager()->create(
		{
			.vertexShaderPath = "shaders/vertex_default.glsl",
			.fragmentShaderPath = "shaders/fragment_default2.glsl",
			.textures = {},
			.buffers = { uniformBuffer },
		}
		);
		mesh = asset::mesh::Mesh::create<Vertex>(vertices, indices);
	}

	void clean() {
		mesh = nullptr;
		pipeline = nullptr;
		pipeline2 = nullptr;
		uniformBuffer = nullptr;
		texture = nullptr;
		renderer = nullptr;
	}
};

struct SoundManager {
	std::string soundFile;
	audio::SoundDevice device;
	audio::SoundBank soundBank;
	audio::AudioManager* audioManager{};
	std::jthread* audioThread{};

	void init(const std::string& sound){
		soundFile = sound;
		audioManager = new audio::AudioManager(&soundBank);
	 	audioThread = new std::jthread(&audio::AudioManager::play, audioManager);
	 	soundBank.addClip(soundFile);
	 	soundBank.loadInitialGroups();
	}

	~SoundManager(){
		audioManager->stop();
		audioThread->join();
		delete audioManager;
	}

};

struct SpatialAudioTestApp: Application {

	const std::string soundFile = "wind.mp3";
	Ref<GraphicsManager> graphicsManager;
	SoundManager soundManager;

	float3 sourcePosition = {450.0f, 200.0f, 0.0f};
	float2 sourceVelocity = {1.0f, 0.0f};

	int circleStep = 0;
	const int stepsPerCircle = 1'000;
	const int stepsLimit = 1'000'000;

	const float radius = 150.0f;
	const float speed = 10.0f;
	float3 listenerPosition = {300.0f, 200.0f, 0.0f};


	void createListener(Ref<Scene> testScene) {
		Entity e = testScene->newEntity();
		auto& domain = testScene->domain();
	 	auto& transform = e.addComponent<scene::components::TransformComponent>(
	 		{
	 			listenerPosition,
	 			{ 0.0f, 0.0f, 0.0f, 1.0f },
	 			{ 100.0f, 50.0f, 0.0f },
	 	}
	 	);
		e.addComponent<scene::components::MeshComponent>({ graphicsManager->mesh, graphicsManager->pipeline});
		auto& velocity = e.addComponent<physics::Velocity>(float2{ 0.0f, 0.0f });
		auto& listener = e.addComponent<audio::ListenerComponent>();
		soundManager.audioManager->setListener(domain, listener, transform, velocity);
	}

	void createSource(Ref<Scene> testScene) {
		Entity e = testScene->newEntity();
	 	auto& transform = e.addComponent<scene::components::TransformComponent>(
	 		{
	 			sourcePosition,
	 			{ 0.0f, 0.0f, 0.0f, 1.0f },
	 			{100.0f, 50.0f, 0.0f}
	 	   }
	 	);
		e.addComponent<scene::components::MeshComponent>({ graphicsManager->mesh, graphicsManager->pipeline2 });
		auto& velocity = e.addComponent<physics::Velocity>(sourceVelocity);
	 	auto& source = e.addComponent<audio::AudioSourceComponent>();
	 	source.path = soundFile;
	 	source.isLooped = true;
		source.rolloffFactor = 0.01f;
		soundManager.audioManager->assignSource(source, transform, velocity);
		soundManager.audioManager->playSource(source);
	}

	void init() override {
		graphicsManager = createRef<GraphicsManager>();
		soundManager.init(soundFile);

	 	// initialize test scene
	 	Ref<Scene> testScene = arch::createRef<Scene>();

	 	// add a "listening" triangle which will be in center of the scene
	 	// it resembles the Listener of the sound
	 	createListener(testScene);

	 	// add a "source" triangle - this resembles the sound's source
	 	// it will be moved while moving the source
	 	createSource(testScene);

		graphicsManager->clean();

	 	scene::SceneManager::get()->changeScene(testScene);
	 }

	void update() override {

		auto& domain = scene::SceneManager::get()->currentScene()->domain();

		auto view = domain.view<scene::components::TransformComponent, physics::Velocity, audio::AudioSourceComponent>();

		for (auto [entity, transform, velocity, audioSource] : view.all()) {
			float angle = circleStep * 2 * std::numbers::pi / stepsPerCircle;
			transform.position.x = listenerPosition.x + radius * std::cos(angle);
			transform.position.y = listenerPosition.y + radius * std::sin(angle);
			velocity.value.x = radius * std::sin(angle);
			velocity.value.y = radius * std::cos(angle);
		}
		circleStep = (circleStep + 1) % stepsLimit;
		soundManager.audioManager->synchronize(domain);
}
};
