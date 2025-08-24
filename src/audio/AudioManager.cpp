#include <archimedes/Logger.h>
#include <archimedes/audio/AudioException.h>
#include <archimedes/audio/AudioManager.h>
#include <archimedes/ecs/View.h>

namespace arch::audio {

AudioManager::AudioManager(SoundBank* soundBank): _soundBank(soundBank) {
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
					if (_sources[source].run() && !_dontRemoveFinished[source]) {
						_sourceStates[source] = stopped;
					}
					break;
				case paused: _sources[source].pausePlaying(); break;
				case stopped:
					if (_sources[source].stopPlaying()) {
						_sources[source].clean();
						_sourceStates[source] = removed;
					}
					break;
				default: break;
			}
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
	}
	Logger::info("Audio system: audio manager stopped playing");
}

void AudioManager::stop() {
	_isListening = false;
}

void AudioManager::playSource(AudioSourceComponent& source) {
	if (source._id == -1) {
		throw AudioException("Audio manager can't play not registered source");
	}
	SourceState currentState = _sourceStates[source._id];
	if (currentState == paused || currentState == assigned) {
		_sourceStates[source._id] = playing;
	}
}

void AudioManager::rewindSource(AudioSourceComponent& source) {
	if (source._id == -1) {
		throw AudioException("Audio manager can't play rewind registered source");
	}
	SourceState currentState = _sourceStates[source._id];
	if (currentState == playing || (currentState == stopped && _dontRemoveFinished[source._id])) {
		_sources[source._id].rewindPlaying();
		_sourceStates[source._id] = playing;
	}
}

void AudioManager::pauseSource(const AudioSourceComponent& source) {
	if (source._id == -1) {
		throw AudioException("Audio manager can't pause not registered source");
	}
	SourceState currentState = _sourceStates[source._id];
	if (currentState == playing) {
		_sourceStates[source._id] = paused;
	}
}

void AudioManager::stopSource(const AudioSourceComponent& source) {
	if (source._id == -1) {
		throw AudioException("Audio manager can't stop not registered source");
	}
	SourceState currentState = _sourceStates[source._id];
	if (currentState == playing || currentState == paused) {
		_sourceStates[source._id] = stopped;
	}
}

void AudioManager::assignSource(AudioSourceComponent& source) {
	if (source._id != -1) {
		throw AudioException("Audio manager can't assign already assigned source");
	}
	int index = _findEmptyPlayer();
	if (index == -1) {
		throw AudioException("Audio manager can't find empty source slot");
	}
	source._id = index;
	_sources[index].setClipPath(source.path);
	_sources[index].update(source);
	_sourceStates[index] = assigned;
	Logger::info("Audio system: audio manager assigned Source with index {}", index);
}

void AudioManager::assignSource(
	AudioSourceComponent& source,
	const scene::components::TransformComponent& transform,
	const physics::Moveable& moveable
) {
	int index = _findEmptyPlayer();
	if (index == -1) {
		throw AudioException("Audio manager can't find empty source slot");
	}
	source._id = index;
	_sources[index].setClipPath(source.path);
	_sources[index].update(source, transform, moveable);
	_sourceStates[index] = assigned;
	Logger::info("Audio system: audio manager assigned Source with index {}", index);
}

void AudioManager::synchronize(ecs::Domain& domain) {
	// synchronize Sources
	auto sourcesView = domain.view<AudioSourceComponent>();
	for (auto [entity, audioSource] : sourcesView.all()) {
		if (audioSource._id == -1) {
			continue;
		}

		auto transform = domain.tryGetComponent<scene::components::TransformComponent>(entity);
		auto moveable = domain.tryGetComponent<physics::Moveable>(entity);

		if (_sourceStates[audioSource._id] == removed) {
			_sourceStates[audioSource._id] = unused;
			_dontRemoveFinished[audioSource._id] = false;
			Logger::info("Audio system: audio manager removed Source with index {}", audioSource._id);
			audioSource._id = -1;
		} else if (transform.hasValue() && moveable.hasValue()) {
			updateSource(audioSource, transform.get(), moveable.get());
		} else {
			updateSource(audioSource);
		}
	}
	for (int source = 0; source < 16; source++) {
		if (_sourceStates[source] == removed) {
			_sourceStates[source] = unused;
			_dontRemoveFinished[source] = false;
			Logger::info("Audio system: audio manager removed Source with index {}", source);
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
			auto transform = domain.tryGetComponent<scene::components::TransformComponent>(entity);
			auto moveable = domain.tryGetComponent<physics::Moveable>(entity);
			if (transform.hasValue() && moveable.hasValue()) {
				updateListener(listener, transform.get(), moveable.get());
			} else {
				updateListener(listener);
			}
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
	if (_sourceStates[source._id] == stopped || _sourceStates[source._id] == removed) {
		return;
	}
	_dontRemoveFinished[source._id] = source.dontRemoveFinished;
	_sources[source._id].update(source);
}

void AudioManager::updateSource(
	const AudioSourceComponent& source,
	const scene::components::TransformComponent& transform,
	const physics::Moveable& moveable
) {
	if (source._id == -1) {
		throw AudioException("Audio manager can't update not registered source");
	}
	if (_sourceStates[source._id] == stopped || _sourceStates[source._id] == removed) {
		return;
	}
	_dontRemoveFinished[source._id] = source.dontRemoveFinished;
	_sources[source._id].update(source, transform, moveable);
}

void AudioManager::updateListener(const ListenerComponent& listener) {
	_listener.update(listener);
}

void AudioManager::updateListener(
	const ListenerComponent& listener,
	const scene::components::TransformComponent& transform,
	const physics::Moveable& moveable
) {
	_listener.update(listener, transform, moveable);
}

void AudioManager::setListener(ecs::Domain& domain, ListenerComponent& listener) {
	auto view = domain.view<ListenerComponent>();
	for (auto [entity, listener] : view.all()) {
		listener._isActive = false;
	}
	listener._isActive = true;
	updateListener(listener);
}

void AudioManager::setListener(
	ecs::Domain& domain,
	ListenerComponent& listener,
	const scene::components::TransformComponent& transform,
	const physics::Moveable& moveable
) {
	auto view = domain.view<ListenerComponent>();
	for (auto [entity, listener] : view.all()) {
		listener._isActive = false;
	}
	listener._isActive = true;
	updateListener(listener, transform, moveable);
}

SourceState AudioManager::getState(const AudioSourceComponent& source) const {
	if (source._id == -1) {
		return unused;
	}
	return _sourceStates[source._id];
}

} // namespace arch::audio

