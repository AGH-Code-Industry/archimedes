#pragma once

#include <AL/alc.h>
#include <AL/al.h>
#include <string>
#include <vector>


namespace arch::audio{

	enum ClipType{
		WAV,
		OGG,
		MP3
	};

	class Clip{
		ClipType type;
		ALint sampleRate, dataSize;
		std::vector<ALchar> data;
		std::string filePath;
		ALenum format;

		void readFirstWAVChunk(std::ifstream& input, ALchar buffer[4]);
		void readSecondWAVChunk(std::ifstream& input, ALchar buffer[4],
								ALushort& channelsNumber, ALushort& bitsPerSample);
		void readThirdWAVChunk(std::ifstream& input, ALchar buffer[4]);
		void readListWAVChunk(std::ifstream& input, ALchar buffer[4]);
		ALenum getFormat(const ALushort& channelsNumber, const ALushort& bitsPerSample);
		void loadWAV();

		public:
			Clip(ClipType type, const std::string& path) : type(type), filePath(path) {}
			void load();
			void unload();
			~Clip();
			const ALchar* getData();
			ALenum getFormat();
			ALint getSize();
			ALint getSampleRate();
	};
}