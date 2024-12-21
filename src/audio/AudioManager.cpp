#include <Logger.h>
#include <audio/AudioException.h>
#include <audio/AudioManager.h>
#include <ecs/View.h>


namespace arch::audio {

AudioManager::AudioManager(SoundBank* soundBank, ecs::Domain* domain, std::mutex& ecsMutex):
	_soundBank(soundBank),
	_domain(domain),
	_ecsMutex(ecsMutex){
	for(int i=0; i<16; i++) {
		_sources[i].initialize(soundBank);
	}
	Logger::info("Audio system: opened audio manager");
}

AudioManager::~AudioManager() {
	Logger::info("Audio system: closed audio manager");
}

int AudioManager::_findEmptyPlayer() {
	for(int i=0; i<16; i++) {
		if(!_sourceUsed[i]) {
			return i;
		}
	}
	return -1;
}

void AudioManager::play() {
	Logger::info("Audio system: audio manager started playing");
	while (_isListening) {
		_updateListener();
		{
			auto lock = std::lock_guard(_ecsMutex);
			for (auto&& [source] : _domain->view<AudioSource>().components()) {
				if(source.sourceIndex < -1 || source.sourceIndex >= 16) {
					throw AudioException("Audio system: source index out of range");
				}
				if(source.path == "") {
					continue;
				}
				switch (source.getState()) {
					case playing:
						if(source.sourceIndex == -1) {
							_assignSource(source);
						}
						_updateSource(source);
						_runSource(source);
						break;
					case pausing:
						_updateSource(source);
						_pauseSource(source);
						break;
					case stopping:
						if(_stopSource(source)) {
							_removeSource(source);
						}
						break;
					case ignoring:
						break;
				}
			}
		}
	}
	Logger::info("Audio system: audio manager stopped playing");
}

void AudioManager::stop() {
	_isListening = false;
}

void AudioManager::_assignSource(AudioSource& source) {
	int index = _findEmptyPlayer();
	if (index == -1) {
		throw AudioException("Cannot find empty source");
	}
	_sourceUsed[index] = true;
	source.sourceIndex = index;
	Logger::info("Audio system: assigned Source with index {}", std::to_string(source.sourceIndex));
}

void AudioManager::_removeSource(AudioSource& source) {
	_sourceUsed[source.sourceIndex] = false;
	int index = source.sourceIndex;
	source.sourceIndex = -1;
	source.ignore();
	Logger::info("Audio system: removed Source with index {}", std::to_string(index));
}

void AudioManager::_pauseSource(AudioSource& source) {
	_sources[source.sourceIndex].pausePlaying();
}

void AudioManager::_runSource(AudioSource& source) {
	_sources[source.sourceIndex].run(source);
}

bool AudioManager::_stopSource(AudioSource& source) {
	return _sources[source.sourceIndex].stopPlaying();
}

void AudioManager::_updateSource(AudioSource& source) {
	_sources[source.sourceIndex].update(source);
}

void AudioManager::_updateListener() {
	_listener.update();
}

} // namespace arch::audio

