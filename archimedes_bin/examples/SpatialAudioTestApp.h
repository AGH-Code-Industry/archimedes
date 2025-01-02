#pragma once

#include <cmath>
#include <mutex>
#include <thread>

#include <Ecs.h>
#include <Engine.h>
#include <Scene.h>
#include <audio/AudioManager.h>
#include <audio/SoundDevice.h>

namespace audio = arch::audio;
namespace ecs = arch::ecs;
namespace scene = arch::scene;



class SpatialAudioTestApp: public arch::Application {

	const std::string _soundFile = "./sounds/wind.mp3";

	//initialize OpenAL context
	audio::SoundDevice _device;

	//initialize SoundBank
	audio::SoundBank _soundBank;

	//AudioManager should be initialized after launching the app
	std::mutex _mutex;
	audio::AudioManager* _audioManager{};
	std::jthread* _audioThread{};

	// counter variable using in sound's source's movement calculation
	int _currentStep{};

	void init() override {
		//initialize test scene
		arch::Ref<scene::Scene> testScene = arch::createRef<scene::Scene>();

		//initialize AudioManager
		ecs::Domain* domain = &testScene->domain();
		_audioManager = new audio::AudioManager(&_soundBank, domain, _mutex);
		_audioThread = new std::jthread(&audio::AudioManager::play, &_audioManager);

		//add a sound to SoundBank and load it
		_soundBank.addClip(_soundFile);
		_soundBank.loadInitialGroups();

		// add a "listening" triangle which will be in center of the scene
		// it resembles the Listener of the sound
		{
			ecs::Entity e = testScene->newEntity();
			testScene->domain().addComponent<scene::components::TransformComponent>(
				e,
				{
					{ 0.0f, 0.0f, 0.0f },
					{ 0.0f, 0.0f, 0.0f, 1.0f },
					{ .5f, .5f, .5f },
			}
			);
			testScene->domain().addComponent<scene::components::MeshComponent>(e, { /*mesh*/ });
		}

		// add a "source" triangle - this resembles the sound's source
		// it will be moved while moving the source
		{
			ecs::Entity e = testScene->newEntity();
			testScene->domain().addComponent<scene::components::TransformComponent>(
				e,
				{
					{ 0.0f, 2.0f, 0.0f },
					{ 0.0f, 0.0f, 0.0f, 1.0f },
					arch::float3(0.2f)
			}
			);
			testScene->domain().addComponent<scene::components::MeshComponent>(e, { /*mesh*/ });
			auto source = &testScene->domain().addComponent<audio::AudioSource>(e);
			source->path = _soundFile;
			source->gain = 0.5f;
			source->isLooped = true;
			source->positionX = 0.0f;
			source->positionY = 2.0f;
			source->play();
		}

		scene::SceneManager::get()->changeScene(testScene);
	}

	void update() override {
		auto view = scene::SceneManager::get()
						->currentScene()
						->domain()
						.view<scene::components::TransformComponent, audio::AudioSource>();

		for (auto [entity, transform, audioSource] : view.all()) {
			// tells how many steps a triangle needs to take to do a full circle
			const int stepsPerCircle = 100;
			const float distance = 2.0f + 2.0f * std::sin(_currentStep * 2 * std::numbers::pi / stepsPerCircle);
			float positionX = distance * std::cos(_currentStep * 2 * std::numbers::pi / stepsPerCircle);
			float positionY = distance * std::sin(_currentStep * 2 * std::numbers::pi / stepsPerCircle);
			{
				auto lock = std::lock_guard(_mutex);
				audioSource.positionX = positionX;
				audioSource.positionY = positionY;
			}
			transform.position = {positionX, positionY, 0.0f};
		}
		_currentStep++;
	}

	~SpatialAudioTestApp() override {
		delete _audioManager;
	}

};
