#include <cstring>

#include <AL/al.h>
#include <archimedes/audio/AudioException.h>
#include <archimedes/audio/Clip.h>
#include <archimedes/audio/AudioFile.h>

namespace arch::audio {

ClipFormat Clip::getFormat() const {
	if (_channelsNumber == 1) {
		return mono;
	}
	if (_channelsNumber == 2) {
		return stereo;
	}
	throw AudioException("Clip can't get format for file: " + _filePath);
}

std::size_t Clip::getBufferElements() const {
	return _sampleItems * _channelsNumber;
}

bool Clip::fillBuffer(std::vector<short>& buffer, std::size_t& cursor, bool isLooped) const {
	const std::size_t bufferElements = getBufferElements();
	std::size_t sizeToCopy = bufferElements;

	if (!_isLoaded) {
		throw AudioException("Clip can't get data from file: " + _filePath + " - it's not loaded");
	}

	if (cursor + bufferElements > _soundData.size()) {
		sizeToCopy = _soundData.size() - cursor;
	}

	std::memcpy(buffer.data(), &_soundData.data()[cursor], sizeToCopy * sizeof(short));
	cursor += sizeToCopy;

	if (sizeToCopy < bufferElements) {
		const std::size_t lastChunkSize = bufferElements - sizeToCopy;
		if (isLooped) {
			cursor = 0;
			std::memcpy(&buffer.data()[sizeToCopy], &_soundData.data()[cursor], lastChunkSize * sizeof(short));
			cursor = lastChunkSize;
		} else {
			std::memset(&buffer.data()[sizeToCopy], 0, lastChunkSize * sizeof(short));
		}
		return true;
	}
	return false;
}

void Clip::load() {
	if (_isLoaded) {
		throw AudioException("Can't load Clip: " + _filePath + " - it's already loaded");
	}
	AudioFile file(_filePath);
	file.load(_soundData);
	_channelsNumber = file.getChannels();
	_dataSize = file.getDataSize();
	_sampleRate = file.getSampleRate();
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
}

int Clip::getSampleRate() const {
	return _sampleRate;
}

bool Clip::getIsLoaded() const {
	return _isLoaded;
}

} // namespace arch::audio
