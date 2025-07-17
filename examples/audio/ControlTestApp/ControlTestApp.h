#pragma once

#include "../Helpers.h"
#include <Input.h>

using namespace arch;

enum ControlAction {
	pauseSound,
	continueSound,
	stopSound,
	startSound,
	rewindSound,
	none
};

struct ControlTestApp: Application {
	// const std::string soundFile = "rickroll.wav";
	const std::string soundFile = "Chiptone A4.wav";
	SoundManager soundManager;
	std::mutex mutex;
	ControlAction controlAction = none;
	bool isPlaying = false;
	Entity entity;

	void init() override {
		soundManager.init({ soundFile });

		Ref<Scene> testScene = arch::createRef<Scene>();

		entity = testScene->newEntity();
		auto& source = entity.addComponent<audio::AudioSourceComponent>();
		source.path = soundFile;
		source.isLooped = false;
		source.dontRemoveFinished = true;

		scene::SceneManager::get()->changeScene(testScene);
	}

	void update() override {
		if (Keyboard::P.pressed()) {
			auto lock = std::lock_guard(mutex);
			if (controlAction == none) {
				controlAction = pauseSound;
			}
		}
		if (Keyboard::C.pressed()) {
			auto lock = std::lock_guard(mutex);
			if (controlAction == none) {
				controlAction = continueSound;
			}
		}
		if (Keyboard::S.pressed()) {
			auto lock = std::lock_guard(mutex);
			if (controlAction == none) {
				controlAction = stopSound;
			}
		}
		if (Keyboard::N.pressed()) {
			auto lock = std::lock_guard(mutex);
			if (controlAction == none) {
				controlAction = startSound;
			}
		}
		if (Keyboard::R.pressed()) {
			auto lock = std::lock_guard(mutex);
			if (controlAction == none) {
				controlAction = rewindSound;
			}
		}

		auto& domain = scene::SceneManager::get()->currentScene()->domain();
		{
			auto lock = std::lock_guard(mutex);
			auto& source = entity.getComponent<audio::AudioSourceComponent>();
			switch (controlAction) {
				case pauseSound:
					if (isPlaying) {
						soundManager.audioManager->pauseSource(source);
					}
					break;
				case continueSound:
					if (isPlaying) {
						soundManager.audioManager->playSource(source);
					}
					break;
				case stopSound:
					if (isPlaying) {
						soundManager.audioManager->stopSource(source);
						isPlaying = false;
					}
					break;
				case startSound:
					if (!isPlaying) {
						soundManager.audioManager->assignSource(source);
						soundManager.audioManager->playSource(source);
						isPlaying = true;
					}
					break;
				case rewindSound:
					if (isPlaying) {
						soundManager.audioManager->rewindSource(source);
					}
					break;
				case none: break;
			}
			controlAction = none;
		}
		soundManager.audioManager->synchronize(domain);
	}
};
