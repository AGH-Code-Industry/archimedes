#pragma once

#include <cmath>
#include <numbers>

#include "../Helpers.h"

using namespace arch;

struct SpatialAudioTestApp: Application {
	f32 windowWidth = 1'200.f;
	f32 windowHeight = 600.f;

	const std::string soundFile = "wind.mp3";
	Ref<GraphicsManager> graphicsManager;

	float3 sourcePosition = { 450.0f, 200.0f, 0.0f };
	float3 sourceVelocity = { 1.0f, 0.0f, 0.0f };

	int circleStep = 0;
	const int stepsPerCircle = 1'000;
	const int stepsLimit = 1'000'000;

	const float radius = 150.0f;
	const float speed = 10.0f;
	float3 listenerPosition = { 300.0f, 200.0f, 0.0f };

	void createListener(Ref<Scene> testScene) {
		Entity e = testScene->newEntity();
		auto& domain = testScene->domain();
		auto& transform = e.addComponent<scene::components::TransformComponent>({
			listenerPosition,
			quaternion(0.0f),
			{ 100.0f, 50.0f, 0.0f },
		});
		e.addComponent<scene::components::MeshComponent>({ graphicsManager->mesh, graphicsManager->pipeline });
		auto& moveable = e.addComponent<physics::RigidBodyComponent>();
		moveable.linearVelocity = float3{ 0.0f, 0.0f, 0.0f };
		auto& listener = e.addComponent<audio::ListenerComponent>();
		auto&& soundManager = testScene->domain().global<SoundManager>();
		soundManager.audioManager->setListener(domain, listener, transform, moveable);
	}

	void createSource(Ref<Scene> testScene) {
		Entity e = testScene->newEntity();
		auto& transform = e.addComponent<scene::components::TransformComponent>({
			sourcePosition,
			quaternion(0.0f),
			{ 100.0f, 50.0f, 0.0f }
		});
		e.addComponent<scene::components::MeshComponent>({ graphicsManager->mesh, graphicsManager->pipeline2 });
		auto& moveable = e.addComponent<physics::RigidBodyComponent>();
		moveable.linearVelocity = sourceVelocity;
		auto& source = e.addComponent<audio::AudioSourceComponent>();
		source.path = soundFile;
		source.isLooped = true;
		source.rolloffFactor = 0.01f;
		auto&& soundManager = testScene->domain().global<SoundManager>();
		soundManager.audioManager->assignSource(source, transform, moveable);
		soundManager.audioManager->playSource(source);
	}

	void init() override {
		graphicsManager = createRef<GraphicsManager>();

		// initialize test scene
		Ref<Scene> testScene = arch::createRef<Scene>();
		scene::SceneManager::get()->changeScene(testScene);
		auto&& soundManager = testScene->domain().global<SoundManager>();
		soundManager.init({ soundFile });

		// add a "listening" triangle which will be in center of the scene
		// it resembles the Listener of the sound
		createListener(testScene);

		// add a "source" triangle - this resembles the sound's source
		// it will be moved while moving the source
		createSource(testScene);

		graphicsManager->clean();
	}

	void update() override {
		auto& domain = scene::SceneManager::get()->currentScene()->domain();

		auto view = domain.view<
			scene::components::TransformComponent,
			physics::RigidBodyComponent,
			audio::AudioSourceComponent>();

		for (auto [entity, transform, moveable, audioSource] : view.entityComps()) {
			float angle = circleStep * 2 * std::numbers::pi / stepsPerCircle;
			transform.position.x = listenerPosition.x + radius * std::cos(angle);
			transform.position.y = listenerPosition.y + radius * std::sin(angle);
			moveable.linearVelocity.x = radius * std::sin(angle);
			moveable.linearVelocity.y = radius * std::cos(angle);
		}
		circleStep = (circleStep + 1) % stepsLimit;
		auto&& soundManager = domain.global<SoundManager>();
		soundManager.audioManager->synchronize(domain);
	}
};
