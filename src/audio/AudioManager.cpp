#include <Logger.h>
#include <audio/AudioException.h>
#include <audio/AudioManager.h>
#include <ecs/Entity.h>
#include <ecs/View.h>

namespace arch::audio {

AudioManager::AudioManager(SoundBank* soundBank, ecs::Domain* domain, std::mutex& ecsMutex):
	_soundBank(soundBank),
	_domain(domain),
	_ecsMutex(ecsMutex) {
	for (int i = 0; i < 16; i++) {
		_sources[i].initialize(soundBank);
	}
	Logger::info("Audio system: opened audio manager");
}

AudioManager::~AudioManager() {
	Logger::info("Audio system: closed audio manager");
}

int AudioManager::_findEmptyPlayer() {
	for (int i = 0; i < 16; i++) {
		if (!_sourceUsed[i]) {
			return i;
		}
	}
	return -1;
}

bool AudioManager::_hasMultipleStates(const ecs::Entity& entity) {
	int states = 0;
	if(_domain->hasComponent<PlayAudioSourceComponent>(entity)) {
		states++;
	}
	if(_domain->hasComponent<StopAudioSourceComponent>(entity)) {
		states++;
	}
	if(_domain->hasComponent<PauseAudioSourceComponent>(entity)) {
		states++;
	}
	return states > 1;
}


void AudioManager::_checkPlayingComponents() {
	auto lock = std::lock_guard(_ecsMutex);
	auto view = _domain->view<AudioSourceComponent, PlayAudioSourceComponent>();
	for (auto&& [entity, source, playState] : view.all()) {
		if (source.sourceIndex < -1 || source.sourceIndex >= 16) {
			throw AudioException("Audio system: source index out of range");
		}
		if(_hasMultipleStates(entity)) {
			throw AudioException("AudioSourceComponent has multiple states");
		}
		if (source.path == "") {
			throw AudioException("AudioSourceComponent has no path");
			//TODO: I don't know why it shouldn't throw an error, should be documented better
			//(applies to 2 other functions)
			continue;
		}
		if (source.sourceIndex == -1) {
			_assignSource(source);
		}
		_updateSource(source);
		_runSource(source, entity);
	}
}

void AudioManager::_checkPausingComponents() {
	auto lock = std::lock_guard(_ecsMutex);
	auto view = _domain->view<AudioSourceComponent, PauseAudioSourceComponent>();
	for (auto&& [entity, source, pauseState] : view.all()) {
		if (source.sourceIndex < -1 || source.sourceIndex >= 16) {
			throw AudioException("Audio system: source index out of range");
		}
		if(_hasMultipleStates(entity)) {
			throw AudioException("AudioSourceComponent has multiple states");
		}
		if (source.path == "") {
			throw AudioException("AudioSourceComponent has no path");
		}
		_updateSource(source);
		_pauseSource(source);
	}
}

void AudioManager::_checkStoppingComponents() {
	auto lock = std::lock_guard(_ecsMutex);
	auto view = _domain->view<AudioSourceComponent, StopAudioSourceComponent>();
	for (auto&& [entity, source, stopState] : view.all()) {
		if (source.sourceIndex < -1 || source.sourceIndex >= 16) {
			throw AudioException("Audio system: source index out of range");
		}
		if(_hasMultipleStates(entity)) {
			throw AudioException("AudioSourceComponent has multiple states");
		}
		if (source.path == "") {
			throw AudioException("AudioSourceComponent has no path");
		}
		if (_stopSource(source)) {
			_removeSource(source, entity);
		}
	}
}


void AudioManager::play() {
	Logger::info("Audio system: audio manager started playing");
	while (_isListening) {
		_updateListener();
		_checkPlayingComponents();
		_checkPausingComponents();
		_checkStoppingComponents();
		// {
		// 	auto lock = std::lock_guard(_ecsMutex);
		// 	for (auto&& [source] : _domain->view<AudioSourceComponent>().components()) {
		// 		if (source.sourceIndex < -1 || source.sourceIndex >= 16) {
		// 			throw AudioException("Audio system: source index out of range");
		// 		}
		// 		if (source.path == "") {
		// 			continue;
		// 		}
		// 		switch (source.getState()) {
		// 			case playing:
		// 				if (source.sourceIndex == -1) {
		// 					_assignSource(source);
		// 				}
		// 				_updateSource(source);
		// 				_runSource(source);
		// 				break;
		// 			case pausing:
		// 				_updateSource(source);
		// 				_pauseSource(source);
		// 				break;
		// 			case stopping:
		// 				if (_stopSource(source)) {
		// 					_removeSource(source);
		// 				}
		// 				break;
		// 			case ignoring: break;
		// 		}
		// 	}
		// }
	}
	Logger::info("Audio system: audio manager stopped playing");
}

void AudioManager::stop() {
	_isListening = false;
}

void AudioManager::_assignSource(AudioSourceComponent& source) {
	int index = _findEmptyPlayer();
	if (index == -1) {
		throw AudioException("Cannot find empty source");
	}
	_sourceUsed[index] = true;
	source.sourceIndex = index;
	_sources[index].setClipPath(source.path);
	Logger::info("Audio system: assigned Source with index {}", std::to_string(source.sourceIndex));
}

void AudioManager::_removeSource(AudioSourceComponent& source, ecs::Entity& entity) {
	// std::lock_guard<std::mutex> lock(_ecsMutex);

	_sourceUsed[source.sourceIndex] = false;
	int index = source.sourceIndex;
	source.sourceIndex = -1;
	_domain->removeComponent<StopAudioSourceComponent>(entity);
	// source.ignore();
	Logger::info("Audio system: removed Source with index {}", std::to_string(index));
}

void AudioManager::_pauseSource(AudioSourceComponent& source) {
	_sources[source.sourceIndex].pausePlaying();
}

void AudioManager::_runSource(AudioSourceComponent& source, ecs::Entity& entity) {
	// std::lock_guard<std::mutex> lock(_ecsMutex);
	if(_sources[source.sourceIndex].run()) {
		//source.stop() equivalent
		_domain->removeComponent<PauseAudioSourceComponent>(entity);
		_domain->removeComponent<PlayAudioSourceComponent>(entity);
		_domain->addComponent<StopAudioSourceComponent>(entity);
	}
}

bool AudioManager::_stopSource(AudioSourceComponent& source) {
	return _sources[source.sourceIndex].stopPlaying();
}

void AudioManager::_updateSource(AudioSourceComponent& source) {
	_sources[source.sourceIndex].update(source);
}

void AudioManager::_updateListener() {
	_listener.update();
}

} // namespace arch::audio

