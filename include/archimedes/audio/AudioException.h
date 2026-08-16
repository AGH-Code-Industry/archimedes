#pragma once
#include <string>

#include <archimedes/Exception.h>

namespace arch::audio {
/// @brief Exception thrown when an error occurs in the audio system.
/// @see Exception
///
class AudioException final: public Exception {
public:
	/// @brief Constructor.
	/// @param message Message of the exception.
	/// @param loc location of the exception.
	///
	AudioException(
		const std::string& message,
		const utils::SimpleSourceLocation& loc = utils::SimpleSourceLocation::current()
	);
};

} // namespace arch::audio
