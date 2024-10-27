#include <Logger.h>
#include <audio/AudioException.h>
#include <audio/SourceManager.h>

namespace arch::audio {

SourceManager::SourceManager(SoundBank* soundBank) : _soundBank(soundBank) {
	mixer.initialize(&_sources, &_listener, &_mutex);
	Logger::info("Audio system: opened audio manager");
}

SourceManager::~SourceManager() {
	Logger::info("Audio system: closed audio manager");
}

SourceState SourceManager::_getState(int index) {
	_mutex.lock();
	if(index < 0 || index >= _sources.size()) {
		throw AudioException("Invalid index");
	}
	SourceState state = _sources[index].getState();
	_mutex.unlock();
	return state;
}

bool SourceManager::_vectorEndReached() {
	_mutex.lock();
	bool endReached = _currentIndex >= _sources.size();
	_mutex.unlock();
	return endReached;
}

void SourceManager::play() {
	Logger::info("Audio system: audio manager started playing");
	while (isListening) {
		_updateListener();
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

	while(true) {
		_mutex.lock();
		int size = _sources.size();
		_mutex.unlock();
		if (size == 0) {
			break;
		}
		removeSource(0);
	}
	Logger::info("Audio system: audio manager stopped playing");
}

void SourceManager::addSource(const std::string& path,float pitch,float gain,float positionX,
							float positionY,float velocityX,float velocityY,bool isLooped
) {
	_mutex.lock();
	if(_sources.size() == maxSources) {
		throw AudioException("Too many audio sources");
	}
	_sources.emplace_back(_soundBank, path, pitch, gain, positionX, positionY, velocityX, velocityY, isLooped);
	int index = _sources.size() - 1;
	_sources[index].activate();
	Logger::info("Audio system: added Source with path {} and index {}", path, std::to_string(index));
	_mutex.unlock();
}

void SourceManager::removeSource(int index) {
	_mutex.lock();
	if(index < 0 || index >= _sources.size()) {
		throw AudioException("Invalid index");
	}
	_sources[index].deactivate();
	_sources.erase(_sources.begin() + index);
	if (index < _currentIndex) {
		_currentIndex--;
	}
	_mutex.unlock();
	Logger::info("Audio system: removed Source with index {}", std::to_string(index));
}

void SourceManager::pauseSource(int index) {
	_mutex.lock();
	if(index < 0 || index >= _sources.size()) {
		throw AudioException("Invalid index");
	}
	_sources[index].pausePlaying();
	_mutex.unlock();
	Logger::info("Audio system: paused Source with index {}", std::to_string(index));
}

void SourceManager::continueSource(int index) {
	_mutex.lock();
	if(index < 0 || index >= _sources.size()) {
		throw AudioException("Invalid index");
	}
	_sources[index].continuePlaying();
	_mutex.unlock();
	Logger::info("Audio system: continued Source with index {}", std::to_string(index));
}

void SourceManager::_playSource(int index) {
	_mutex.lock();
	if(index < 0 || index >= _sources.size()) {
		throw AudioException("Invalid index");
	}
	_sources[index].play();
	_mutex.unlock();
}

void SourceManager::_updateSource(int index) {
	_mutex.lock();
	if(index < 0 || index >= _sources.size()) {
		throw AudioException("Invalid index");
	}
	_sources[index].update();
	_mutex.unlock();
}

void SourceManager::_updateListener() {
	_mutex.lock();
	_listener.update();
	_mutex.unlock();
}

} // namespace arch::audio

