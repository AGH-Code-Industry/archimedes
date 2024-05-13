#pragma once

#include <AL/alc.h>
#include <AL/al.h>
#include <string>
#include <vector>


namespace arch::audio{

	class Clip{
		ALint _sampleRate, _dataSize;
		std::vector<ALshort> _data;
		std::string _filePath;
		ALenum _format;

		ALenum _chooseFormat(const ALushort& channelsNumber) const;

		public:
			Clip(const std::string& path) : _filePath(path) {}
			void load();
			void unload();
			~Clip();
			const ALshort* getData() const;
			ALenum getFormat() const;
			ALint getSize() const;
			ALint getSampleRate() const;
	};
}