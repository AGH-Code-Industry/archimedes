#include <Logger.h>
#include <audio/AudioException.h>
#include <audio/AudioManager.h>
#include <ecs/View.h>

namespace arch::audio {

AudioManager::AudioManager(SoundBank* soundBank): _soundBank(soundBank){
	for (int i = 0; i < 16; i++) {
		_sources[i].initialize(soundBank);
		_sourceStates[i] = unused;
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

void AudioManager::synchronize(ecs::Domain& domain) {
	// synchronize Sources
	auto sourcesView = domain.view<AudioSourceComponent>();
	for (auto [entity, audioSource] : sourcesView.all()) {
		if (audioSource._id == -1) {
			continue;
		}
		if (_sourceStates[audioSource._id] == removed) {
			_sourceStates[audioSource._id] = unused;
			Logger::info("Audio system: audio manager removed Source with index {}", std::to_string(audioSource._id));
			audioSource._id = -1;
		}
		else {
			updateSource(audioSource);
		}
	}
	for (int source = 0; source < 16; source++) {
		if (_sourceStates[source] == removed) {
			_sourceStates[source] = unused;
			Logger::info("Audio system: audio manager removed Source with index {}", std::to_string(source));

		}
	}

	// synchronize Listener
	auto listenersView = domain.view<ListenerComponent>();
	bool activeListenerFound = false;
	for (auto [entity, listener] : listenersView.all()) {
		if (listener._isActive) {
			if (activeListenerFound) {
				throw AudioException("Audio system: there are two active Listeners");
			}
			activeListenerFound = true;
			updateListener(listener);
			if (!_listenerSet) {
				_listenerSet = true;
				Logger::info("Audio system: audio manager set the listener");
			}
		}
	}
	if (!activeListenerFound && _listenerSet) {
		_listenerSet = false;
		Logger::info("Audio system: audio manager unset the listener");
	}
}


void AudioManager::updateSource(const AudioSourceComponent& source) {
	if (source._id == -1) {
		throw AudioException("Audio manager can't update not registered source");
	}
	_sources[source._id].update(source);
}

void AudioManager::updateListener(const ListenerComponent& listener) {
	_listener.update(listener);
}

void AudioManager::setListener(ListenerComponent& listener, ecs::Domain& domain) {
	auto view = domain.view<ListenerComponent>();
	for (auto [entity, listener] : view.all()) {
		listener._isActive = false;
	}
	listener._isActive = true;
	updateListener(listener);
}

} // namespace arch::audio

