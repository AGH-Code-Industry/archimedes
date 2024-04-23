#include <audio/Clip.h>
#include <audio/AudioException.h>
#include <iostream>
#include <fstream>
#include <cstring>
#include <iterator>

namespace arch::audio{

	void Clip::readFirstWAVChunk(std::ifstream& input, ALchar buffer[5]){
		if(not input.read(buffer, 4)){
			throw AudioException(filePath + " - couldn't read ChunkID");
		}

		if(strcmp(buffer, "RIFF") != 0){
			throw AudioException(filePath + " - invalid RIFF file");
		}

		if(not input.read(buffer, 4)){
			throw AudioException(filePath + " - couldn't read ChunkSize");
		}
	}

	void Clip::readSecondWAVChunk(std::ifstream& input, ALchar buffer[5],
								ALushort& channelsNumber, ALushort& bitsPerSample){
		if(not input.read(buffer, 4)){
			throw AudioException(filePath + " - couldn't read Format");
		}

		if(strcmp(buffer, "WAVE") != 0){
			throw AudioException(filePath + " - invalid WAVE file");
		}

		if(not input.read(buffer, 4)){
			throw AudioException(filePath + " - couldn't read Subchunk1ID");
		}

		if(strcmp(buffer, "fmt ") != 0){
			throw AudioException(filePath + " - invalid Subchunk1ID");
		}

		if(not input.read(buffer, 4)){
			throw AudioException(filePath + " - couldn't read Subchunk1Size");
		}

		if(not input.read(buffer, 2)){
			throw AudioException(filePath + " - couldn't read AudioFormat");
		}

		if(not input.read(buffer, 2)){
			throw AudioException(filePath + " - couldn't read NumChannels");
		}

		memcpy(&channelsNumber, buffer, sizeof(ALushort));

		if(not input.read(buffer, 4)){
			throw AudioException(filePath + " - couldn't read SampleRate");
		}

		memcpy(&sampleRate, buffer, sizeof(ALint));

		if(not input.read(buffer, 4)){
			throw AudioException(filePath + " - couldn't read ByteRate");
		}

		if(not input.read(buffer, 2)){
			throw AudioException(filePath + " - couldn't read BlockAlign");
		}

		if(not input.read(buffer, 2)){
			throw AudioException(filePath + " - couldn't read BitsPerSample");
		}

		memcpy(&bitsPerSample, buffer, sizeof(ALushort));

	}

	void Clip::readListWAVChunk(std::ifstream& input, ALchar buffer[5]) {
		if(not input.read(buffer, 4)) {
			throw AudioException(filePath + " - couldn't read ListSize");
		}
		ALint size;
		memcpy(&size, buffer, sizeof(ALint));
		if(not input.read(buffer, size - 8)) {
			throw AudioException(filePath + " - couldn't skip List chunk");
		}
	}

	void Clip::readThirdWAVChunk(std::ifstream& input, ALchar buffer[5]){
		if(not input.read(buffer, 4)){
			throw AudioException(filePath + " - couldn't read Subchunk2ID");
		}

		if(strcmp(buffer, "data") != 0) {
			if(strcmp(buffer, "LIST") == 0) {
				readListWAVChunk(input, buffer);
			}
			else {
				throw AudioException(filePath + " - invalid Subchunk2ID");
			}
		}

		if(not input.read(buffer, 4)){
			throw AudioException(filePath + " - couldn't read Subchunk2Size");
		}

		memcpy(&dataSize, buffer, sizeof(ALint));

		if(input.eof()){
			throw AudioException(filePath + " - reached EOF");
		}

		if(input.fail()){
			throw AudioException(filePath + " - bad state set on file");
		}
		dataSize = dataSize - dataSize % 4;
		data.reserve(dataSize);
		data.assign(std::istreambuf_iterator<ALchar>(input), std::istreambuf_iterator<ALchar>());
	}

	ALenum Clip::getFormat(const ALushort& channelsNumber, const ALushort& bitsPerSample){
		if(channelsNumber == 1){
			if(bitsPerSample == 8){
				return AL_FORMAT_MONO8;
			}
			if(bitsPerSample == 16){
				return AL_FORMAT_MONO16;
			}
		}
		if(channelsNumber == 2){
			if(bitsPerSample == 8){
				return AL_FORMAT_STEREO8;
			}
			if(bitsPerSample == 16){
				return AL_FORMAT_STEREO16;
			}
		}
		std::cout<<channelsNumber<< " " << bitsPerSample << std::endl;
		throw AudioException(filePath + " - wrong format");
	}

	void Clip::loadWAV(){
		ALushort channelsNumber, bitsPerSample;
		std::ifstream input(filePath, std::ios::binary);
		if(not input.is_open()){
			throw AudioException("Couldn't open file " + filePath);
		}
		ALchar buffer[90] = "buff";
		readFirstWAVChunk(input, buffer);
		readSecondWAVChunk(input, buffer, channelsNumber, bitsPerSample);
		readThirdWAVChunk(input, buffer);
		format = getFormat(channelsNumber, bitsPerSample);
		input.close();
	}

	void Clip::load(){
		switch(type){
			case WAV:
				loadWAV();
				break;
			case OGG:
				throw AudioException("Audio format not supported: .ogg");
			case MP3:
				throw AudioException("Audio format not supported: .mp3");
		}
	}

	void Clip::unload(){
		data.clear();
	}

	Clip::~Clip(){
		unload();
	}

	const ALchar* Clip::getData(){
		return data.data();
	}

	ALenum Clip::getFormat(){
		return format;
	}

	ALint Clip::getSize(){
		return dataSize;
	}

	ALint Clip::getSampleRate(){
		return sampleRate;
	}
}