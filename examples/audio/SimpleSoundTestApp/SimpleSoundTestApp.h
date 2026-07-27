#pragma once

#include "../Helpers.h"
#include <archimedes/Input.h>

using namespace arch;

struct SimpleSoundTestApp: Application {
	const std::string soundFile = "Chiptone A4.wav";

	std::mutex mutex;

	bool playSound = false;

	void init() override {
		Ref<Scene> testScene = arch::createRef<Scene>();
		scene::SceneManager::get()->changeScene(testScene);
		auto&& soundManager = testScene->domain().global<SoundManager>();
		soundManager.init({ soundFile });
	}

	void addSound(ecs::Domain& domain) {
		auto entity = domain.newEntity();
		auto& source = domain.addComponent<audio::AudioSourceComponent>(entity);

		source.path = soundFile;
		source.isLooped = false;
		auto&& soundManager = scene::SceneManager::get()->currentScene()->domain().global<SoundManager>();
		soundManager.audioManager->assignSource(source);
		soundManager.audioManager->playSource(source);
	}

	void removeInactive(ecs::Domain& domain) {
		auto&& soundManager = scene::SceneManager::get()->currentScene()->domain().global<SoundManager>();
		std::vector<ecs::Entity> toRemove;
		auto view = domain.view<audio::AudioSourceComponent>();
		for (auto [entity, source] : view.entityComps()) {
			if (soundManager.audioManager->getState(source) == audio::SourceState::unused) {
				toRemove.push_back(entity);
			}
		}
		for (auto entity : toRemove) {
			domain.kill(entity);
		}
	}

	void update() override {
		auto& domain = scene::SceneManager::get()->currentScene()->domain();
		{
			auto lock = std::lock_guard(mutex);
			if (playSound) {
				addSound(domain);
			}
			playSound = false;
		}
		auto&& soundManager = scene::SceneManager::get()->currentScene()->domain().global<SoundManager>();
		soundManager.audioManager->synchronize(domain);
		removeInactive(domain);

		if (Keyboard::space.pressed()) {
			auto lock = std::lock_guard(mutex);
			if (!playSound) {
				playSound = true;
			}
		}
	}
};
