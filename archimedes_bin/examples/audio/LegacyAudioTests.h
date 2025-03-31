// #pragma once
// #include <cmath>
// #include <thread>
// #include <numbers>
//
// #include <Ecs.h>
// #include <Logger.h>
// #include <audio/AudioManager.h>
// #include <audio/SoundDevice.h>
//
// using namespace arch;
//
//
// inline void testControl() {
// 	ecs::Domain domain;
//
// 	// initialize OpenAL context
// 	audio::SoundDevice device;
//
// 	// initialize SoundBank
// 	audio::SoundBank soundBank;
//
// 	// audio file name
// 	const std::string filename = "rickroll.wav";
//
// 	// load audio files that we can play
// 	soundBank.addClip(filename);
// 	soundBank.loadInitialGroups();
//
//
// 	// initialize and start the audioManager
// 	audio::AudioManager audioManager(&soundBank);
// 	std::jthread audioThread(&audio::AudioManager::play, &audioManager);
//
// 	auto entity = domain.newEntity();
// 	auto& source = domain.addComponent<audio::AudioSourceComponent>(entity);
//
// 	source.path = filename;
// 	source.gain = 0.5;
// 	source.isLooped = true;
// 	audioManager.playSource(source);
//
// 	char controlSign = 'x';
// 	bool isPlaying = true;
// 	while (controlSign != 'q') {
// 		controlSign = getchar();
// 		switch (controlSign) {
// 			case 'p':
// 				if (isPlaying) {
// 					audioManager.pauseSource(source);
// 				}
// 				break;
// 			case 'c':
// 				if (isPlaying) {
// 					audioManager.playSource(source);
// 				}
// 				break;
// 			case 's':
// 				if (isPlaying) {
// 					audioManager.stopSource(source);
// 					isPlaying = false;
// 				}
// 				break;
// 			case 'r':
// 				if (!isPlaying) {
// 					source = domain.addComponent<audio::AudioSourceComponent>(entity);
// 					source.path = filename;
// 					source.gain = 0.5;
// 					source.isLooped = true;
// 					audioManager.playSource(source);
// 					isPlaying = true;
// 				}
// 				break;
// 			default: break;
// 		}
// 		audioManager.synchronize(domain);
// 	}
//
// 	// close the audioManager
// 	audioManager.stop();
// }