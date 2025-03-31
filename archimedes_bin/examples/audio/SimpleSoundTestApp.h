#pragma once

#include <InputHandler.h>
#include <examples/audio/Helpers.h>

struct SimpleSoundTestApp: Application {

	const std::string soundFile = "Chiptone A4.wav";
	SoundManager soundManager;

	std::mutex mutex;

	bool playSound = false;

	void init() override {
		soundManager.init({soundFile});

		InputHandler::get().bindKey(VK_SPACE, [&](int action) {
			if (action == GLFW_PRESS) {
				auto lock = std::lock_guard(mutex);
				if (!playSound) {
					playSound = true;
				}
			}
		});

	 	Ref<Scene> testScene = arch::createRef<Scene>();

	 	scene::SceneManager::get()->changeScene(testScene);
	}

	void addSound(ecs::Domain& domain) {
		auto entity = domain.newEntity();
		auto& source = domain.addComponent<audio::AudioSourceComponent>(entity);

		source.path = soundFile;
		source.isLooped = false;
		soundManager.audioManager->assignSource(source);
		soundManager.audioManager->playSource(source);
	}

	void removeInactive(ecs::Domain& domain) {
		std::vector<ecs::Entity> toRemove;
		auto view = domain.view<audio::AudioSourceComponent>();
		for(auto [entity, source] : view.all()) {
			if(soundManager.audioManager->getState(source) == audio::SourceState::unused){
				toRemove.push_back(entity);
			}
		}
		for(auto entity : toRemove) {
			domain.kill(entity);
		}
	}

	void update() override {
		auto& domain = scene::SceneManager::get()->currentScene()->domain();
		{
			auto lock = std::lock_guard(mutex);
			if(playSound) {
				addSound(domain);
			}
			playSound = false;
		}
		soundManager.audioManager->synchronize(domain);
		removeInactive(domain);
	}
};
