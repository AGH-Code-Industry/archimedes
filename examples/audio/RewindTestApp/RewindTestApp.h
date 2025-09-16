#pragma once

#include "../Helpers.h"
#include <archimedes/Input.h>

using namespace arch;

struct RewindTestApp: Application {
	const std::string soundFile = "Chiptone A4.wav";
	SoundManager soundManager;

	std::mutex mutex;

	bool playSound = false;

	Entity entity;

	bool played = false;

	void init() override {
		soundManager.init({ soundFile });

		Ref<Scene> testScene = arch::createRef<Scene>();

		entity = testScene->newEntity();
		auto& source = entity.addComponent<audio::AudioSourceComponent>();
		source.path = soundFile;
		source.isLooped = false;
		source.dontRemoveFinished = true;
		soundManager.audioManager->assignSource(source);

		scene::SceneManager::get()->changeScene(testScene);
	}

	void update() override {
		auto& domain = scene::SceneManager::get()->currentScene()->domain();
		{
			auto lock = std::lock_guard(mutex);
			auto& source = entity.getComponent<audio::AudioSourceComponent>();
			if (playSound) {
				if (played) {
					soundManager.audioManager->rewindSource(source);
				} else {
					soundManager.audioManager->playSource(source);
					played = true;
				}
			}
			playSound = false;
		}
		soundManager.audioManager->synchronize(domain);

		if (Keyboard::space.pressed()) {
			auto lock = std::lock_guard(mutex);
			if (!playSound) {
				playSound = true;
			}
		}
	}
};
