#pragma once

#include <AL/al.h>
#include <AL/alc.h>

#include <concepts>
#include <sstream>
#include "exception.h"

namespace arch::audio{
    #define alCall(function, ...) alCallImplementation(__FILE__, __LINE__, function, __VA_ARGS__)

    #define alcCall(function, device, ...) alcCallImplementation(__FILE__, __LINE__, function, device, __VA_ARGS__)


    void checkAlErrors(const std::string& filename,
                        const std::uint_fast32_t line) {

        ALenum error = alGetError();
        if (error == AL_NO_ERROR){
            return;
        }
        std::stringstream errorStream;
        errorStream << "(" << filename << ": " << line << ") ";
        switch (error) {
            case AL_INVALID_NAME:
                errorStream << "AL_INVALID_NAME: a bad name (ID) was passed to "
                            "an OpenAL function";
                break;
            case AL_INVALID_ENUM:
                errorStream << "AL_INVALID_ENUM: an invalid enum value was "
                            "passed to an OpenAL function";
                break;
            case AL_INVALID_VALUE:
                errorStream << "AL_INVALID_VALUE: an invalid value was passed to "
                            "an OpenAL function";
                break;
            case AL_INVALID_OPERATION:
                errorStream << "AL_INVALID_OPERATION: the requested operation is "
                            "not valid";
                break;
            case AL_OUT_OF_MEMORY:
                errorStream << "AL_OUT_OF_MEMORY: the requested operation "
                            "resulted in OpenAL running out of memory";
                break;
            default:
                errorStream << "UNKNOWN AL ERROR: " << error;
        }
        std::string errorText = errorStream.str();
        throw AudioException(errorText);
    }

    void checkAlcErrors(const std::string& filename, const std::uint_fast32_t line, ALCdevice* device){
        ALCenum error = alcGetError(device);
        if(error == ALC_NO_ERROR){
            return;
        }
        std::stringstream errorStream;
        errorStream << "(" << filename << ": " << line << ") ";
        switch(error){
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
            default:
                errorStream << "UNKNOWN ALC ERROR: " << error;
        }
        std::string errorText = errorStream.str();
        throw AudioException(errorText);
    } 

    template<typename Function, typename... Params>
    concept voidReturn = requires(Function function, Params... params){
        { function(std::forward<Params>(params)...) } -> std::same_as<void>;
    };

    template<typename Function, typename... Params>
        concept normalReturn = not voidReturn<Function, Params...>;

    template<typename alFunction, typename... Params>
    requires voidReturn<alFunction, Params...>
    auto alCallImplementation(const char* filename, const std::uint_fast32_t line,
                            alFunction function, Params... params){
        function(std::forward<Params>(params)...);
        checkAlErrors(filename, line);
    };

    template<typename alFunction, typename... Params>
    requires normalReturn<alFunction, Params...>
    auto alCallImplementation(const char* filename, const std::uint_fast32_t line,
                            alFunction function, Params... params){
        auto returnValue = function(std::forward<Params>(params)...);
        checkAlErrors(filename, line);
        return returnValue;
    }

    template<typename alcFunction, typename... Params>
    requires voidReturn<alcFunction, Params...>
    auto alcCallImplementation(const char* filename, const std::uint_fast32_t line, 
                    alcFunction function, ALCdevice* device, Params... params){
        function(std::forward<Params>(params)...);
        checkAlcErrors(filename, line, device);
    }

    template<typename alcFunction, typename... Params>
    requires normalReturn<alcFunction, Params...>
    auto alcCallImplementation(const char* filename, const std::uint_fast32_t line,
                                alcFunction function, ALCdevice* device, Params... params){
        auto returnValue = function(std::forward<Params>(params)...);
        checkAlcErrors(filename, line, device);
        return returnValue;
    }
}
