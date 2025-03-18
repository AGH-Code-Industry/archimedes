#pragma once
#include <cmath>
#include <mutex>
#include <thread>

#include <Ecs.h>
#include <Logger.h>
#include <audio/AudioManager.h>
#include <audio/SoundDevice.h>

using namespace arch;

inline void testSimpleSound() {
	ecs::Domain domain;

	// initialize OpenAL context
	audio::SoundDevice device;

	// initialize SoundBank
	audio::SoundBank soundBank;

	// audio file name
	const std::string filename = "Chiptone A4.wav";

	// load audio files that we can play
	soundBank.addClip(filename);
	soundBank.loadInitialGroups();

	// initialize and start the audioManager
	audio::AudioManager audioManager(&soundBank, &domain);
	std::jthread audioThread(&audio::AudioManager::play, &audioManager);

	while (getchar() != 'q') {
		auto entity = domain.newEntity();
		auto source = &domain.addComponent<audio::AudioSourceComponent>(entity);

		source->path = filename;
		source->gain = 0.5;
		source->isLooped = false;
		audioManager.play(source);
		audioManager.cleanSources(domain);
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
	soundBank.addClip(filename);
	soundBank.loadInitialGroups();


	// initialize and start the audioManager
	audio::AudioManager audioManager(&soundBank, &domain);
	std::jthread audioThread(&audio::AudioManager::play, &audioManager);

	auto entity = domain.newEntity();
	auto source = &domain.addComponent<audio::AudioSourceComponent>(entity);

	source->path = filename;
	source->gain = 0.5;
	source->isLooped = true;

	char controlSign = 'x';
	bool isPlaying = true;
	while (controlSign != 'q') {
		controlSign = getchar();
		switch (controlSign) {
			case 'p':
				if (isPlaying) {
					audioManager.pauseSource(source);
				}
				break;
			case 'c':
				if (isPlaying) {
					audioManager.playSource(source);
				}
				break;
			case 's':
				if (isPlaying) {
					audioManager.stopSource(source);
					isPlaying = false;
				}
				break;
			case 'r':
				if (!isPlaying) {
					auto source = &domain.addComponent<audio::AudioSourceComponent>(entity);
					source->path = filename;
					source->gain = 0.5;
					source->isLooped = true;
					audioManager.playSource(source);
					isPlaying = true;
				}
				break;
			default: break;
		}
		audioManager.cleanSources(domain);
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
	soundBank.addClip(filename);
	soundBank.loadInitialGroups();

	std::mutex mutex;


	// create an entity and give it an audio source component
	auto entity = domain.newEntity();
	auto source = &domain.addComponent<audio::AudioSourceComponent>(entity);
	source->path = filename;
	source->gain = 0.5;
	source->isLooped = true;
	source->positionX = 0.0f;
	source->positionY = 1.0f;

	// initialize and start the audioManager
	audio::AudioManager audioManager(&soundBank, &domain);
	std::jthread audioThread(&audio::AudioManager::play, &audioManager);

	audioManager.playSource(source);

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
		audioManager.cleanSources(domain);
		std::this_thread::sleep_for(std::chrono::milliseconds(50));
	}

	// close the audioManager
	audioManager.stop();
}