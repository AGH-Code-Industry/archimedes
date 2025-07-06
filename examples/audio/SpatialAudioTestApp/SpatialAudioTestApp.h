#pragma once

#include <cmath>
#include <numbers>

#include "../Helpers.h"

using namespace arch;

struct SpatialAudioTestApp: Application {
	const std::string soundFile = "wind.mp3";
	Ref<GraphicsManager> graphicsManager;
	SoundManager soundManager;

	float3 sourcePosition = { 450.0f, 200.0f, 0.0f };
	float2 sourceVelocity = { 1.0f, 0.0f };

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
			{ 0.0f, 0.0f, 0.0f, 0.0f },
			{ 100.0f, 50.0f, 0.0f },
		});
		e.addComponent<scene::components::MeshComponent>({ graphicsManager->mesh, graphicsManager->pipeline });
		auto& moveable = e.addComponent<physics::MoveableComponent>();
		moveable.velocity = float2{ 0.0f, 0.0f };
		auto& listener = e.addComponent<audio::ListenerComponent>();
		soundManager.audioManager->setListener(domain, listener, transform, moveable);
	}

	void createSource(Ref<Scene> testScene) {
		Entity e = testScene->newEntity();
		auto& transform = e.addComponent<scene::components::TransformComponent>({
			sourcePosition,
			{ 0.0f, 0.0f, 0.0f, 0.0f },
			{ 100.0f, 50.0f, 0.0f }
		});
		e.addComponent<scene::components::MeshComponent>({ graphicsManager->mesh, graphicsManager->pipeline2 });
		auto& moveable = e.addComponent<physics::MoveableComponent>();
		moveable.velocity = sourceVelocity;
		auto& source = e.addComponent<audio::AudioSourceComponent>();
		source.path = soundFile;
		source.isLooped = true;
		source.rolloffFactor = 0.01f;
		soundManager.audioManager->assignSource(source, transform, moveable);
		soundManager.audioManager->playSource(source);
	}

	void init() override {
		graphicsManager = createRef<GraphicsManager>();
		soundManager.init({ soundFile });

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

		auto view =
			domain.view<scene::components::TransformComponent, physics::MoveableComponent, audio::AudioSourceComponent>();

		for (auto [entity, transform, moveable, audioSource] : view.all()) {
			float angle = circleStep * 2 * std::numbers::pi / stepsPerCircle;
			transform.position.x = listenerPosition.x + radius * std::cos(angle);
			transform.position.y = listenerPosition.y + radius * std::sin(angle);
			moveable.velocity.x = radius * std::sin(angle);
			moveable.velocity.y = radius * std::cos(angle);
		}
		circleStep = (circleStep + 1) % stepsLimit;
		soundManager.audioManager->synchronize(domain);
	}
};
