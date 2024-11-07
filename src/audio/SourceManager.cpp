#include <Logger.h>
#include <audio/AudioException.h>
#include <audio/SourceManager.h>

namespace arch::audio {

SourceManager::SourceManager(SoundBank* soundBank, ecs::Domain<ecs::e64> *domain) : _soundBank(soundBank), _domain(domain) {
	mixer.initialize();
	Logger::info("Audio system: opened audio manager");
}

SourceManager::~SourceManager() {
	Logger::info("Audio system: closed audio manager");
}

int SourceManager::_findEmptySource() {
	for(int i=0; i<16; i++) {
		if(!_sources[i].isActive) {
			return i;
		}
	}
	return -1;
}



void SourceManager::play() {
	Logger::info("Audio system: audio manager started playing");
	while (isListening) {
		_updateListener();
		for (auto&& [entity, pos] : _domain->components<SourceComponent>()) {
			_updateSource(pos);
		}
	}
	Logger::info("Audio system: audio manager stopped playing");
}

void SourceManager::addSource(SourceComponent& component){
	int index = _findEmptySource();
	if(index == -1) {
		throw AudioException("Could not find empty source");
	}
	component.sourceIndex = index;
	_sources[index].update(component);
	_sources[index].activate(_soundBank);
	Logger::info("Audio system: added Source with path {} and index {}", component.path, std::to_string(index));
}

void SourceManager::removeSource(SourceComponent& component) {\
	if(!_sources[component.sourceIndex].isActive) {
		throw AudioException("Source not active");
	}
	if(_sources[component.sourceIndex].isPlaying) {
		throw AudioException("Source is still playing");
	}
	_sources[component.sourceIndex].deactivate();
	Logger::info("Audio system: removed Source with index {}", std::to_string(component.sourceIndex));
}

void SourceManager::pauseSource(SourceComponent& component) {
	if(!_sources[component.sourceIndex].isActive) {
		throw AudioException("Source not active");
	}
	_sources[component.sourceIndex].pausePlaying();
	Logger::info("Audio system: paused Source with index {}", std::to_string(component.sourceIndex));
}

void SourceManager::continueSource(SourceComponent& component) {
	if(!_sources[component.sourceIndex].isActive) {
		throw AudioException("Source not active");
	}
	_sources[component.sourceIndex].continuePlaying();
	Logger::info("Audio system: continued Source with index {}", std::to_string(component.sourceIndex));
}

void SourceManager::_playSource(SourceComponent& component) {
	if(!_sources[component.sourceIndex].isActive) {
		throw AudioException("Source not active");
	}
	_sources[component.sourceIndex].play();
}

void SourceManager::_updateSource(SourceComponent& component) {
	if(!_sources[component.sourceIndex].isActive) {
		throw AudioException("Source not active");
	}
	_sources[component.sourceIndex].update(component);
}

void SourceManager::_updateListener() {
	_listener.update();
}

} // namespace arch::audio

