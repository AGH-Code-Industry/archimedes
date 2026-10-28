#include <cstring>

#include <AL/al.h>
#include <archimedes/audio/AudioException.h>
#include <archimedes/audio/Clip.h>
#include <archimedes/audio/AudioFile.h>
#include <archimedes/audio/Calls.hpp>

namespace arch::audio {

ALenum Clip::_getFormat() const {
	if (_channelsNumber == 1) {
		return AL_FORMAT_MONO16;
	}
	if (_channelsNumber == 2) {
		return AL_FORMAT_STEREO16;
	}
	throw AudioException("Clip can't get format for file: " + _filePath);
}


void Clip::load() {
	if (_isLoaded) {
		throw AudioException("Can't load Clip: " + _filePath + " - it's already loaded");
	}
	std::vector<i16> soundData;
	AudioFile file(_filePath);
	file.load(soundData);
	_channelsNumber = file.getChannels();
	_sampleRate = file.getSampleRate();
	alCall(alBufferData, _alBuffer, _getFormat(), soundData.data(), soundData.size(), _sampleRate);
	_isLoaded = true;
}

void Clip::unload() {
	if (!_isLoaded) {
		throw AudioException("Can't unload Clip: " + _filePath + " - it's not loaded");
	}
	_soundData.clear();
}

Clip::~Clip() {
	if (_isLoaded) {
		unload();
		_isLoaded = false;
	}
	alCall(alDeleteBuffers, 1, &_alBuffer);
}

int Clip::getSampleRate() const {
	return _sampleRate;
}

bool Clip::getIsLoaded() const {
	return _isLoaded;
}

Clip::Clip(std::string_view path) : _filePath(path) {
	alCall(alGenBuffers, 1, &_alBuffer);
}

} // namespace arch::audio
