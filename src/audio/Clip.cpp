#include <audio/AudioException.h>
#include <audio/Clip.h>
#include <sndfile.h>
#include <AL/al.h>

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
	SF_INFO soundInfo;
	SNDFILE* soundFile; // TODO: can be wrapped to own class
	ALint samples;
	if (_isLoaded) {
		throw AudioException("Can't load Clip: " + _filePath + " - it's already loaded");
	}
	soundInfo.format = 0;
	soundFile = sf_open(_filePath.c_str(), SFM_READ, &soundInfo);
	if (soundFile == nullptr) {
		std::string errorMessage = std::string(sf_strerror(soundFile));
		throw AudioException(_filePath + " - " + errorMessage);
	}
	samples = soundInfo.channels * soundInfo.frames;
	_soundData.reserve(samples);
	for (int i = 0; i < samples; i++) {
		_soundData.push_back(0);
	}
	sf_read_short(soundFile, _soundData.data(), samples);
	sf_close(soundFile);
	_channelsNumber = soundInfo.channels;
	_dataSize = samples * sizeof(short);
	_sampleRate = soundInfo.samplerate;
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
