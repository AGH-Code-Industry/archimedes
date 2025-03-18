#include <Logger.h>
#include <audio/AudioException.h>
#include <audio/AudioManager.h>
#include <ecs/View.h>

namespace arch::audio {

AudioManager::AudioManager(SoundBank* soundBank): _soundBank(soundBank){
	for (int i = 0; i < 16; i++) {
		_sources[i].initialize(soundBank);
	}
	Logger::info("Audio system: opened audio manager");
}

AudioManager::~AudioManager() {
	Logger::info("Audio system: closed audio manager");
}

int AudioManager::_findEmptyPlayer() const {
	for (int i = 0; i < 16; i++) {
		if (_sourceStates[i] == unused) {
			return i;
		}
	}
	return -1;
}

void AudioManager::play() {
	Logger::info("Audio system: audio manager started playing");
	while (_isListening) {
		_updateListener();
		for (int source = 0; source < 16; source++) {
			switch (_sourceStates[source]) {
				case playing:
					if (_sources[source].run()) {
						_sourceStates[source] = stopped;
					}
					break;
				case paused:
					_sources[source].pausePlaying();
					break;
				case stopped:
					if (_sources[source].stopPlaying()) {
						_sourceStates[source] = removed;
						_sources[source].cleanClipPath();
					}
					break;
				case unused:
					break;
				case removed:
					break;
			}
		}
	}
	Logger::info("Audio system: audio manager stopped playing");
}

void AudioManager::stop() {
	_isListening = false;
}

void AudioManager::playSource(AudioSourceComponent& source) {
	if (source._id == -1) {
		_assignSource(source);
	}
	updateSource(source);
	SourceState currentState = _sourceStates[source._id];
	if (currentState == unused || currentState == paused) {
		_sourceStates[source._id] = playing;
	}
}

void AudioManager::pauseSource(const AudioSourceComponent& source) {
	if (source._id == -1) {
		throw AudioException("Audio manager can't pause not registered source");
	}
	updateSource(source);
	SourceState currentState = _sourceStates[source._id];
	if (currentState == playing) {
		_sourceStates[source._id] = paused;
	}
}

void AudioManager::stopSource(const AudioSourceComponent& source) {
	if (source._id == -1) {
		throw AudioException("Audio manager can't stop not registered source");
	}
	updateSource(source);
	SourceState currentState = _sourceStates[source._id];
	if (currentState == playing || currentState == paused) {
		_sourceStates[source._id] = stopped;
	}
}

void AudioManager::_assignSource(AudioSourceComponent& source) {
	int index = _findEmptyPlayer();
	if (index == -1) {
		throw AudioException("Audio manager can't find empty source slot");
	}
	source._id = index;
	_sources[index].setClipPath(source.path);
	_sources[index].update(source);
	Logger::info("Audio system: audio manager assigned Source with index {}", std::to_string(index));
}

void AudioManager::cleanSources(const ecs::Domain& domain) {
	auto view = domain.view<AudioSourceComponent>();
	for (auto [entity, audioSource] : view.all()) {
		if (audioSource._id == -1) {
			continue;
		}
		if (_sourceStates[audioSource._id] == removed) {
			_sourceStates[audioSource._id] = unused;
			audioSource._id = -1;
		}
	}
	for (int source = 0; source < 16; source++) {
		if (_sourceStates[source] == removed) {
			_sourceStates[source] = unused;
		}
	}
}


void AudioManager::updateSource(const AudioSourceComponent& source) {
	if (source._id == -1) {
		throw AudioException("Audio manager can't update not registered source");
	}
	_sources[source._id].update(source);
}

void AudioManager::_updateListener() {
	_listener.update();
}

} // namespace arch::audio

