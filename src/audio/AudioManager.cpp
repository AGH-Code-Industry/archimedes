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
	_mutex.lock();
	if(index < 0 || index >= _audioSources.size()) {
		throw AudioException("Audio system: invalid index");
	}
	SourceState state = _audioSources[index].getState();
	_mutex.unlock();
	return state;
}

bool AudioManager::_vectorEndReached() {
	_mutex.lock();
	bool endReached = _currentIndex >= _audioSources.size();
	_mutex.unlock();
	return endReached;
}

void AudioManager::play() {
	Logger::info("Audio system: audio manager started playing");
	while (isListening) {
		_mutex.lock();
		_currentIndex = 0;
		_mutex.unlock();
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
			_mutex.lock();
			_currentIndex++;
			_mutex.unlock();
		}
	}
	Logger::info("Audio system: audio manager stopped playing");
}

void AudioManager::addSource(const std::string& path,float pitch,float gain,float positionX,
							float positionY,float velocityX,float velocityY,bool isLooped
) {
	_mutex.lock();
	if(_audioSources.size() == maxSources) {
		throw AudioException("Too many audio sources");
	}
	_audioSources.emplace_back(_soundBank, path, pitch, gain, positionX, positionY, velocityX, velocityY, isLooped);
	int index = _audioSources.size() - 1;
	_audioSources[index].activate();
	Logger::info("Audio system: added AudioSource with path {} and index {}", path, std::to_string(index));
	_mutex.unlock();
}

void AudioManager::removeSource(int index) {
	_mutex.lock();
	if(index < 0 || index >= _audioSources.size()) {
		throw AudioException("Audio system: invalid index");
	}
	_audioSources[index].deactivate();
	_audioSources.erase(_audioSources.begin() + index);
	if (index < _currentIndex) {
		_currentIndex--;
	}
	_mutex.unlock();
	Logger::info("Audio system: removed AudioSource with index {}", std::to_string(index));
}

void AudioManager::pauseSource(int index) {
	_mutex.lock();
	if(index < 0 || index >= _audioSources.size()) {
		throw AudioException("Audio system: invalid index");
	}
	_audioSources[index].pausePlaying();
	_mutex.unlock();
	Logger::info("Audio system: paused AudioSource with index {}", std::to_string(index));
}

void AudioManager::continueSource(int index) {
	_mutex.lock();
	if(index < 0 || index >= _audioSources.size()) {
		throw AudioException("Audio system: invalid index");
	}
	_audioSources[index].continuePlaying();
	_mutex.unlock();
	Logger::info("Audio system: continued AudioSource with index {}", std::to_string(index));
}

void AudioManager::_playSource(int index) {
	_mutex.lock();
	if(index < 0 || index >= _audioSources.size()) {
		throw AudioException("Audio system: invalid index");
	}
	_audioSources[index].play();
	_mutex.unlock();
}

void AudioManager::_updateSource(int index) {
	_mutex.lock();
	if(index < 0 || index >= _audioSources.size()) {
		throw AudioException("Audio system: invalid index");
	}
	_audioSources[index].update();
	_mutex.unlock();
}

} // namespace arch::audio

