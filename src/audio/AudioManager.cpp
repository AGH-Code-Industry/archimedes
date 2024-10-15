#include <Logger.h>
#include <audio/AudioException.h>
#include <audio/AudioManager.h>

namespace arch::audio {

AudioManager::AudioManager(SoundBank* soundBank) : _soundBank(soundBank) {
	Logger::info("Audio system: opened audio manager");
}

AudioManager::~AudioManager() {
	Logger::info("Audio system: closed audio manager");
}

SourceState AudioManager::_getState(int index) {
	mutex.lock();
	if(index < 0 || index >= audioSources.size()) {
		throw AudioException("Invalid index");
	}
	SourceState state = audioSources[index].getState();
	mutex.unlock();
	return state;
}

bool AudioManager::_vectorEndReached() {
	mutex.lock();
	bool endReached = _currentIndex >= audioSources.size();
	mutex.unlock();
	return endReached;
}

void AudioManager::play() {
	Logger::info("Audio system: audio manager started playing");
	while (isListening) {
		_updateListener();
		mutex.lock();
		_currentIndex = 0;
		mutex.unlock();
		while (true) {
			if(_vectorEndReached()) {
				break;
			}
			switch (_getState(_currentIndex)) {
				case SourceState::waiting:
					_playSource(_currentIndex);
					break;
				case SourceState::playing:
					_updateSource(_currentIndex);
					break;
				case SourceState::stopped:
					removeSource(_currentIndex);
				case SourceState::paused:
					continue;
			}
			mutex.lock();
			_currentIndex++;
			mutex.unlock();
		}
	}

	while(true) {
		mutex.lock();
		int size = audioSources.size();
		mutex.unlock();
		if (size == 0) {
			break;
		}
		removeSource(0);
	}
	Logger::info("Audio system: audio manager stopped playing");
}

void AudioManager::addSource(const std::string& path,float pitch,float gain,float positionX,
							float positionY,float velocityX,float velocityY,bool isLooped
) {
	mutex.lock();
	if(audioSources.size() == maxSources) {
		throw AudioException("Too many audio sources");
	}
	audioSources.emplace_back(_soundBank, path, pitch, gain, positionX, positionY, velocityX, velocityY, isLooped);
	int index = audioSources.size() - 1;
	audioSources[index].activate();
	Logger::info("Audio system: added AudioSource with path {} and index {}", path, std::to_string(index));
	mutex.unlock();
}

void AudioManager::removeSource(int index) {
	mutex.lock();
	if(index < 0 || index >= audioSources.size()) {
		throw AudioException("Invalid index");
	}
	audioSources[index].deactivate();
	audioSources.erase(audioSources.begin() + index);
	if (index < _currentIndex) {
		_currentIndex--;
	}
	mutex.unlock();
	Logger::info("Audio system: removed AudioSource with index {}", std::to_string(index));
}

void AudioManager::pauseSource(int index) {
	mutex.lock();
	if(index < 0 || index >= audioSources.size()) {
		throw AudioException("Invalid index");
	}
	audioSources[index].pausePlaying();
	mutex.unlock();
	Logger::info("Audio system: paused AudioSource with index {}", std::to_string(index));
}

void AudioManager::continueSource(int index) {
	mutex.lock();
	if(index < 0 || index >= audioSources.size()) {
		throw AudioException("Invalid index");
	}
	audioSources[index].continuePlaying();
	mutex.unlock();
	Logger::info("Audio system: continued AudioSource with index {}", std::to_string(index));
}

void AudioManager::_playSource(int index) {
	mutex.lock();
	if(index < 0 || index >= audioSources.size()) {
		throw AudioException("Invalid index");
	}
	audioSources[index].play();
	mutex.unlock();
}

void AudioManager::_updateSource(int index) {
	mutex.lock();
	if(index < 0 || index >= audioSources.size()) {
		throw AudioException("Invalid index");
	}
	audioSources[index].update();
	mutex.unlock();
}

void AudioManager::_updateListener() {
	mutex.lock();
	_listener.update();
	mutex.unlock();
}

} // namespace arch::audio

