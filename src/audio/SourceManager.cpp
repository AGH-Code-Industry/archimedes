#include <Logger.h>
#include <audio/AudioException.h>
#include <audio/SourceManager.h>

namespace arch::audio {

SourceManager::SourceManager(SoundBank* soundBank, ecs::Domain* domain):
	_soundBank(soundBank),
	_domain(domain){
	for(int i=0; i<16; i++) {
		_sources[i].initialize(soundBank);
	}
	Logger::info("Audio system: opened audio manager");
}

SourceManager::~SourceManager() {
	Logger::info("Audio system: closed audio manager");
}

int SourceManager::_findEmptySource() {
	for(int i=0; i<16; i++) {
		if(!_sourceUsed[i]) {
			return i;
		}
	}
	return -1;
}

void SourceManager::play() {
	Logger::info("Audio system: audio manager started playing");
	while (isListening) {
		_updateListener();
		// Logger::debug("Audio system: audio manager update started");
		for (auto&& [entity, source] : _domain->components<SourceComponent>()) {
			if(source.sourceIndex < -1 || source.sourceIndex >= 16) {
				throw AudioException("Audio system: source index out of range");
			}
			if(source.path == "") {
				continue;
			}
			switch (source.getState()) {
				case playing:
					Logger::debug("Audio system: source is playing");
					if(source.sourceIndex == -1) {
						_assignSource(source);
					}
					_updateSource(source);
					_playSource(source); //flagged
					Logger::debug("Audio system: afterplay");
					break;
				case paused:
					// Logger::debug("Audio system: source is paused");
					_updateSource(source);
					_pauseSource(source);
					break;
				case stopped:
					// Logger::debug("Audio system: source is stopped");
					_stopSource(source);
					_removeSource(source);
					break;
				case ignored:
					// Logger::debug("Audio system: source is ignored");
					break;
			}
		}
		// Logger::debug("Audio system: audio manager update ended");
	}
	Logger::info("Audio system: audio manager stopped playing");
}

void SourceManager::stop() {
	isListening = false;
}

void SourceManager::_assignSource(SourceComponent& component) {
	int index = _findEmptySource();
	if (index == -1) {
		throw AudioException("Cannot find empty source");
	}
	Logger::debug("{}", index);
	_sourceUsed[index] = true;
	component.sourceIndex = index;
	Logger::info("Audio system: assigned Source with index {}", std::to_string(component.sourceIndex));
}

void SourceManager::_removeSource(SourceComponent& component) {
	_sourceUsed[component.sourceIndex] = false;
	int index = component.sourceIndex;
	component.sourceIndex = -1;
	component.ignore();
	Logger::info("Audio system: removed Source with index {}", std::to_string(index));
}

void SourceManager::_pauseSource(SourceComponent& component) {
	_sources[component.sourceIndex].pausePlaying();
	Logger::info("Audio system: paused Source with index {}", std::to_string(component.sourceIndex));
}


void SourceManager::_playSource(SourceComponent& component) {
	_sources[component.sourceIndex].play(component); //flagged .play()
}

void SourceManager::_stopSource(SourceComponent& component) {
	_sources[component.sourceIndex].stopPlaying();
	Logger::info("Audio system: stopped Source with index {}", std::to_string(component.sourceIndex));
}

void SourceManager::_updateSource(SourceComponent& component) {
	_sources[component.sourceIndex].update(component);
}

void SourceManager::_updateListener() {
	_listener.update();
}

} // namespace arch::audio

