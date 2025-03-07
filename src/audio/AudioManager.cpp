#include <Logger.h>
#include <audio/AudioException.h>
#include <audio/AudioManager.h>
#include <ecs/Entity.h>
#include <ecs/View.h>

namespace arch::audio {

AudioManager::AudioManager(SoundBank* soundBank, ecs::Domain* domain): _soundBank(soundBank), _domain(domain){
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
		if (!_sourceUsed[i]) {
			return i;
		}
	}
	return -1;
}

void AudioManager::_performAction(ecs::Entity& entity, AudioSourceComponent& source) {
	Logger::debug("Found action");
	auto actionComponent = _domain->getComponent<AudioSourceActionComponent>(entity);
	switch (actionComponent.action){
		case SourceAction::play:
			Logger::debug("Playing action");
			break;
		case SourceAction::pause:
			Logger::debug("Pausing action");
			break;
		case SourceAction::stop:
			Logger::debug("Stopping action");
			break;
	}
	if (!_sourceComponents.contains(&source)) {
		if (actionComponent.action != SourceAction::play) {
			throw AudioException("Can't pause/stop unregistered source");
		}
		_assignSource(source);
	}
	_changeState(source, actionComponent.action);
	_domain->removeComponent<AudioSourceActionComponent>(entity);
}


void AudioManager::play() {
	Logger::info("Audio system: audio manager started playing");
	while (_isListening) {
		_updateListener();
		auto view = _domain->view<AudioSourceComponent>();
		for (auto&& [entity, source] : view.all()) {
			if (_domain->hasComponent<AudioSourceActionComponent>(entity)) {
				_performAction(entity, source);
			}
			if (_sourceComponents.contains(&source)) {
				_updateSource(source);
			}
		}
	}
	Logger::info("Audio system: audio manager stopped working");
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
	_sourceComponents.try_emplace(&source, playing, index);
	_sources[index].setClipPath(source.path);
	Logger::info("Audio system: assigned Source with index {}", std::to_string(index));
}

void AudioManager::_removeSource(AudioSourceComponent& source) {
	SourceData& data = _sourceComponents[&source];
	_sourceUsed[data.index] = false;
	_sources[data.index].cleanClipPath();
	Logger::info("Audio system: removed Source with index {}", std::to_string(data.index));
	_sourceComponents.erase(&source);
}

void AudioManager::_changeState(AudioSourceComponent& source, SourceAction action) {
	SourceData& data = _sourceComponents[&source];
	if (action == SourceAction::stop) {
		data.state = stopped;
	}
	else if (action == pause && data.state == playing) {
		data.state = paused;
	}
	else if (action == SourceAction::play && data.state == paused) {
		data.state = playing;
	}
}

void AudioManager::_updateSource(AudioSourceComponent& source) {
	SourceData& data = _sourceComponents[&source];
	_sources[data.index].update(source);
	switch (data.state) {
		case playing:
			if (_sources[data.index].run()) {
				_changeState(source, SourceAction::stop);
			}
			break;
		case paused:
			_sources[data.index].pausePlaying();
			break;
		case stopped:
			if (_sources[data.index].stopPlaying()) {
				_removeSource(source);
			}
			break;
	}
}

void AudioManager::_updateListener() {
	_listener.update();
}

} // namespace arch::audio

