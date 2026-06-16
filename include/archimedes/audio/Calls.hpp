#pragma once

#include <concepts>
#include <source_location>
#include <sstream>

#include <AL/al.h>
#include <AL/alc.h>
#include <archimedes/audio/AudioException.h>

///@brief Calls an AL function and checks if it threw an error.
///@param function function to be called.
///@param ... parameters of the function.
#define alCall(function, ...) arch::audio::alCallImplementation(std::source_location::current(), function, __VA_ARGS__)

///@brief Calls an ALC function and checks if it threw an error.
///@param function function to be called.
///@param device ALCDevice returned by alcOpenDevice call.
///@param ... parameters of the function.
#define alcCall(function, device, ...) \
	alcCallImplementation(std::source_location::current(), function, device, __VA_ARGS__)

namespace arch::audio {
/// @brief Check for errors after "al" function call.
/// @param location location in the code where the original function was called.
/// @throws AudioException if an error is found.
void inline checkAlErrors(const std::source_location& location) {
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
	throw AudioException(errorText, location);
}

/// @brief Check for errors after "alc" function call.
/// @param device ALCDevice returned by alcOpenDevice call.
/// @param location location in the code where the original function was called.
/// @throws AudioException if an error is found.
void inline checkAlcErrors(ALCdevice* device, const std::source_location& location) {
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
	throw AudioException(errorText, location);
}

template<typename Function, typename... Params>
/// @brief Check if function returns void.
concept VoidReturn = requires(Function function, Params... params) {
	{ function(std::forward<Params>(params)...) } -> std::same_as<void>;
};

template<typename Function, typename... Params>
/// @brief Check if function returns something other than void.
concept NormalReturn = not VoidReturn<Function, Params...>;

/// @brief Wrapper for OpenAL "al" functions with void return type.
/// @tparam AlFunction function to be wrapped.
/// @tparam Params parameters of wrapped function.
/// @param location location in the code where the original function was called.
/// @param function wrapped function.
/// @param params wrapped function's parameters.
template<typename AlFunction, typename... Params>
requires VoidReturn<AlFunction, Params...>
auto alCallImplementation(const std::source_location location, AlFunction function, Params&&... params) {
	function(std::forward<Params>(params)...);
	checkAlErrors(location);
}

/// @brief Wrapper for OpenAL "al" functions with return type other than void.
/// @tparam AlFunction function to be wrapped.
/// @tparam Params parameters of wrapped function.
/// @param location location in the code where the original function was called.
/// @param function wrapped function.
/// @param params wrapped function's parameters.
/// @return return value of wrapped function.
template<typename AlFunction, typename... Params>
requires NormalReturn<AlFunction, Params...>
auto alCallImplementation(const std::source_location location, AlFunction function, Params&&... params) {
	auto returnValue = function(std::forward<Params>(params)...);
	checkAlErrors(location);
	return returnValue;
}

/// @brief Wrapper for OpenAL "alc" functions with void return type.
/// @tparam AlcFunction function to be wrapped.
/// @tparam Params parameters of wrapped function.
/// @param location location in the code where the original function was called.
/// @param function wrapped function.
/// @param device ALCDevice returned by alcOpenDevice call.
/// @param params wrapped function's parameters.
template<typename AlcFunction, typename... Params>
requires VoidReturn<AlcFunction, Params...>
auto alcCallImplementation(
	const std::source_location location,
	AlcFunction function,
	ALCdevice* device,
	Params&&... params
) {
	function(std::forward<Params>(params)...);
	checkAlcErrors(device, location);
}

/// @brief Wrapper for OpenAL "alc" functions with return type other than void.
/// @tparam AlcFunction function to be wrapped.
/// @tparam Params parameters of wrapped function.
/// @param location location in the code where the original function was called.
/// @param function wrapped function.
/// @param device ALCDevice returned by alcOpenDevice call.
/// @param params wrapped function's parameters.
/// @return return value of wrapped function.
template<typename AlcFunction, typename... Params>
requires NormalReturn<AlcFunction, Params...>
auto alcCallImplementation(
	const std::source_location location,
	AlcFunction function,
	ALCdevice* device,
	Params&&... params
) {
	auto returnValue = function(std::forward<Params>(params)...);
	checkAlcErrors(device, location);
	return returnValue;
}
} // namespace arch::audio
