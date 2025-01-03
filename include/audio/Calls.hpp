#pragma once

#include <concepts>
#include <sstream>

#include <AL/al.h>
#include <AL/alc.h>
#include <audio/AudioException.h>

namespace arch::audio {
/// @brief Check for errors after "al" function call.
/// @throws AudioException if an error is found
void inline checkAlErrors() {
	const ALenum error = alGetError();
	if (error == AL_NO_ERROR) {
		return;
	}
	std::stringstream errorStream;
	switch (error) {
		case AL_INVALID_NAME: errorStream << "AL_INVALID_NAME: a bad name (ID) was passed to an OpenAL function"; break;
		case AL_INVALID_ENUM:
			errorStream << "AL_INVALID_ENUM: an invalid enum value was passed to an OpenAL function";
			break;
		case AL_INVALID_VALUE:
			errorStream << "AL_INVALID_VALUE: an invalid value was passed to an OpenAL function";
			break;
		case AL_INVALID_OPERATION: errorStream << "AL_INVALID_OPERATION: the requested operation is not valid"; break;
		case AL_OUT_OF_MEMORY:
			errorStream << "AL_OUT_OF_MEMORY: the requested operation resulted in OpenAL running out of memory";
			break;
		default: errorStream << "UNKNOWN AL ERROR: " << error;
	}
	const std::string errorText = errorStream.str();
	throw AudioException(errorText);
}

/// @brief Check for errors after "alc" function call
/// @param device ALCDevice returned by alcOpenDevice call
/// @throws AudioException if an error is found
void inline checkAlcErrors(ALCdevice* device) {
	const ALCenum error = alcGetError(device);
	if (error == ALC_NO_ERROR) {
		return;
	}
	std::stringstream errorStream;
	switch (error) {
		case ALC_INVALID_VALUE:
			errorStream << "ALC_INVALID_VALUE: an invalid value was passed to an OpenAL function";
			break;
		case ALC_INVALID_DEVICE:
			errorStream << "ALC_INVALID_DEVICE: a bad device was passed to an OpenAL function";
			break;
		case ALC_INVALID_CONTEXT:
			errorStream << "ALC_INVALID_CONTEXT: a bad context was passed to an OpenAL function";
			break;
		case ALC_INVALID_ENUM:
			errorStream << "ALC_INVALID_ENUM: an unknown enum value was passed to an OpenAL function";
			break;
		case ALC_OUT_OF_MEMORY:
			errorStream << "ALC_OUT_OF_MEMORY: an unknown enum value was passed to an OpenAL function";
			break;
		default: errorStream << "UNKNOWN ALC ERROR: " << error;
	}
	const std::string errorText = errorStream.str();
	throw AudioException(errorText);
}

template<typename Function, typename... Params>
/// @brief Check if function returns void
concept voidReturn = requires(Function function, Params... params) {
	{ function(std::forward<Params>(params)...) } -> std::same_as<void>;
};

template<typename Function, typename... Params>
/// @brief Check if function returns something other than void
concept normalReturn = not voidReturn<Function, Params...>;

/// @brief Wrapper for OpenAL "al" functions with void return type
/// @tparam AlFunction function to be wrapped
/// @tparam Params parameters of wrapped function
/// @param function wrapped function
/// @param params wrapped function's parameters
template<typename AlFunction, typename... Params>
requires voidReturn<AlFunction, Params...>
auto alCall(AlFunction function, Params&&... params) {
	function(std::forward<Params>(params)...);
	checkAlErrors();
}

/// @brief Wrapper for OpenAL "al" functions with return type other than void
/// @tparam AlFunction function to be wrapped
/// @tparam Params parameters of wrapped functio
/// @param function wrapped function
/// @param params wrapped function's parameters
/// @return return value of wrapped function
template<typename AlFunction, typename... Params>
requires normalReturn<AlFunction, Params...>
auto alCall(AlFunction function, Params&&... params) {
	auto returnValue = function(std::forward<Params>(params)...);
	checkAlErrors();
	return returnValue;
}

/// @brief Wrapper for OpenAL "alc" functions with void return type
/// @tparam AlcFunction function to be wrapped
/// @tparam Params parameters of wrapped function
/// @param function wrapped function
/// @param device ALCDevice returned by alcOpenDevice call
/// @param params wrapped function's parameters
template<typename AlcFunction, typename... Params>
requires voidReturn<AlcFunction, Params...>
auto alcCall(AlcFunction function, ALCdevice* device, Params&&... params) {
	function(std::forward<Params>(params)...);
	checkAlcErrors(device);
}

/// @brief Wrapper for OpenAL "alc" functions with return type other than void
/// @tparam AlcFunction function to be wrapped
/// @tparam Params parameters of wrapped function
/// @param function wrapped function
/// @param device ALCDevice returned by alcOpenDevice call
/// @param params wrapped function's parameters
/// @return return value of wrapped function
template<typename AlcFunction, typename... Params>
requires normalReturn<AlcFunction, Params...>
auto alcCall(AlcFunction function, ALCdevice* device, Params&&... params) {
	auto returnValue = function(std::forward<Params>(params)...);
	checkAlcErrors(device);
	return returnValue;
}
} // namespace arch::audio
