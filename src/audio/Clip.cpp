#include <audio/Clip.h>
#include <audio/AudioException.h>
#include <sndfile.h>
#include <iostream>

namespace arch::audio{

	ALenum Clip::getFormat() const {
		if(_channelsNumber == 1){
			return AL_FORMAT_MONO16;
		}
		if(_channelsNumber == 2){
			return AL_FORMAT_STEREO16;
		}
		throw AudioException(_filePath + " - wrong format");
	}

	std::size_t Clip::getBufferSize() const {
		return _buffersItems * _channelsNumber;
	}

	bool Clip::fillBuffer(std::vector<ALshort>& buffer, std::size_t& cursor, bool isLooped) const {
		const std::size_t bufferSize = getBufferSize();
		std::size_t sizeToCopy = bufferSize;

		if(cursor + bufferSize > _soundData.size()) {
			sizeToCopy = _soundData.size() - cursor;
		}

		std::memcpy(buffer.data(), &_soundData.data()[cursor], sizeToCopy * sizeof(short));
		cursor += sizeToCopy;

		if(sizeToCopy < bufferSize) {
			const std::size_t lastChunkSize = bufferSize - sizeToCopy;
			if(isLooped) {
				cursor = 0;
				std::memcpy(&buffer.data()[sizeToCopy], &_soundData.data()[cursor], lastChunkSize * sizeof(short));
				cursor = lastChunkSize;
			}
			else {
				std::memset(&buffer.data()[sizeToCopy], 0, lastChunkSize * sizeof(short));
			}
			return true;
		}
		return false;
	}



	void Clip::load(){
		SF_INFO soundInfo;
		SNDFILE *soundFile;  // TODO: can be wrapped to own class
		ALint samples;
		soundInfo.format = 0;
		soundFile = sf_open(_filePath.c_str(), SFM_READ, &soundInfo);
		samples = soundInfo.channels * soundInfo.frames;
		_soundData.reserve(samples);
		for(int i=0; i<samples; i++) {
			_soundData.push_back(0);
		}
		sf_read_short(soundFile, _soundData.data(), samples);
		sf_close(soundFile);
		_channelsNumber = soundInfo.channels;
		_dataSize = samples * sizeof(short);
		_sampleRate = soundInfo.samplerate;
	}

	void Clip::unload(){
		_soundData.clear();
	}

	Clip::~Clip(){
		unload();
	}

	const ALshort* Clip::getData() const {
		return _soundData.data();
	}

	ALint Clip::getSize() const {
		return _dataSize;
	}

	ALint Clip::getSampleRate() const {
		return _sampleRate;
	}
}