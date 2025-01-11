#pragma once
#include <cmath>
#include <mutex>
#include <thread>

#include <Ecs.h>
#include <Logger.h>
#include <audio/AudioManager.h>
#include <audio/SoundDevice.h>

const std::string sounds = "/home/anon/dev/archimedes/archimedes_bin/sounds/";

namespace audio = arch::audio;
namespace ecs = arch::ecs;

inline void testSimpleSound() {
	ecs::Domain domain;

	// initialize OpenAL context
	audio::SoundDevice device;

	// initialize SoundBank
	audio::SoundBank soundBank;

	// audio file name
	const std::string filename = "Chiptone A4.wav";

	// load audio files that we can play
	soundBank.addClip(sounds + filename);
	soundBank.loadInitialGroups();

	// initialize and start the audioManager
	std::mutex mutex;
	audio::AudioManager audioManager(&soundBank, &domain, mutex);
	std::jthread audioThread(&audio::AudioManager::play, &audioManager);

	while (getchar() != 'q') {
		{
			auto lock = std::lock_guard(mutex);
			auto entity = domain.newEntity();
			auto source = &domain.addComponent<audio::AudioSourceComponent>(entity);

			source->path = sounds + filename;
			source->gain = 0.5;
			source->isLooped = false;
			//			source->play();
			domain.addComponent<audio::PlayAudioSourceComponent>(entity);
		}
	}

	// close the audioManager
	audioManager.stop();
}

inline void testControl() {
	ecs::Domain domain;

	// initialize OpenAL context
	audio::SoundDevice device;

	// initialize SoundBank
	audio::SoundBank soundBank;

	// audio file name
	const std::string filename = "rickroll.wav";

	// load audio files that we can play
	soundBank.addClip(sounds + filename);
	soundBank.loadInitialGroups();

	// initialize and start the audioManager
	std::mutex mutex;
	audio::AudioManager audioManager(&soundBank, &domain, mutex);
	std::jthread audioThread(&audio::AudioManager::play, &audioManager);

	auto entity = domain.newEntity();
	{
		auto lock = std::lock_guard(mutex);
		auto source = &domain.addComponent<audio::AudioSourceComponent>(entity);

		source->path = sounds + filename;
		source->gain = 0.5;
		source->isLooped = true;
		//			source->play();
		domain.addComponent<audio::PlayAudioSourceComponent>(entity);
	}

	char controlSign = 'x';
	bool isPlaying = true;
	while (controlSign != 'q') {
		controlSign = getchar();
		switch (controlSign) {
			case 'p':
				{
					std::lock_guard<std::mutex> lock(mutex);
					domain.removeComponent<audio::PlayAudioSourceComponent>(entity);
					domain.removeComponent<audio::StopAudioSourceComponent>(entity);
					domain.addComponent<audio::PauseAudioSourceComponent>(entity);
				}
				break;
				//				source->pause(); break;
			case 'c':
				{
					std::lock_guard<std::mutex> lock(mutex);
					domain.removeComponent<audio::PauseAudioSourceComponent>(entity);
					domain.addComponent<audio::PlayAudioSourceComponent>(entity);
				}
				break;
				//				source->play(); break;
			case 's':
				{
					std::lock_guard<std::mutex> lock(mutex);
					domain.removeComponent<audio::PlayAudioSourceComponent>(entity);
					domain.removeComponent<audio::PauseAudioSourceComponent>(entity);
					domain.addComponent<audio::StopAudioSourceComponent>(entity);
				}
				//				source->stop();
				isPlaying = false;
				break;
			case 'r':
				if (!isPlaying) {
					{
						std::lock_guard<std::mutex> lock(mutex);
						auto source = &domain.addComponent<audio::AudioSourceComponent>(entity);

						source->path = sounds + filename;
						source->gain = 0.5;
						source->isLooped = true;
						//			source->play();
						domain.addComponent<audio::PlayAudioSourceComponent>(entity);
					}
					//					source = createSource(mutex, domain, filename, true);
					//					source->play();
				}
				break;
			default: break;
		}
	}

	// close the audioManager
	audioManager.stop();
}

inline void testSpatialAudio() {
	ecs::Domain domain;

	// initialize OpenAL context
	audio::SoundDevice device;

	// initialize SoundBank
	audio::SoundBank soundBank;

	// audio file name
	const std::string filename = "wind.mp3";

	// load audio files that we can play
	soundBank.addClip(sounds + filename);
	soundBank.loadInitialGroups();

	std::mutex mutex;


	// create an entity and give it an audio source component
	auto entity = domain.newEntity();
	auto source = &domain.addComponent<audio::AudioSourceComponent>(entity);
	{
		auto lock = std::lock_guard(mutex);
		source->path = sounds + filename;
		source->gain = 0.5;
		source->isLooped = true;
		//			source->play();
		source->positionX = 0.0f;
		source->positionY = 1.0f;
		domain.addComponent<audio::PlayAudioSourceComponent>(entity);
	}

	// initialize and start the audioManager
	audio::AudioManager audioManager(&soundBank, &domain, mutex);
	std::jthread audioThread(&audio::AudioManager::play, &audioManager);

	// during playing the sound, update the source every 50 ms.
	// make the source hang around the listener while changing the distance from them.
	const int steps = 1'000;
	for (int i = 0; i < steps; i++) {
		const int stepsPerCircle = 100;
		const float distance = 5.0f + 5.0 * std::sin(i * 2 * std::numbers::pi / stepsPerCircle);
		{
			auto lock = std::lock_guard(mutex);
			{
				source->positionX = distance * std::cos(i * 2 * std::numbers::pi / stepsPerCircle);
				source->positionY = distance * std::sin(i * 2 * std::numbers::pi / stepsPerCircle);
			}
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(50));
	}

	// close the audioManager
	audioManager.stop();
}