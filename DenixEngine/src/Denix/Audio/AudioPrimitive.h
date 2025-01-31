#pragma once
#include <cstdint>
#include <iostream>
#include <string>
#include <al/al.h>
#include <al/alc.h>
#include <SDL3/SDL_audio.h>

// https://indiegamedev.net/2020/02/15/the-complete-guide-to-openal-with-c-part-1-playing-a-sound/

#define alCall(function, ...) alCallImpl(function, __VA_ARGS__)
#define alcCall(function, device, ...) alcCallImpl(function, device, __VA_ARGS__)

namespace Denix
{
    inline static bool s_AudioLogging = false;

    inline bool check_al_errors()
    {
        ALenum error = alGetError();
        if (error != AL_NO_ERROR && s_AudioLogging)
        {
            switch (error)
            {
            case AL_INVALID_NAME:
                std::cerr << "AL_INVALID_NAME: a bad name (ID) was passed to an OpenAL function";
                break;
            case AL_INVALID_ENUM:
                std::cerr << "AL_INVALID_ENUM: an invalid enum value was passed to an OpenAL function";
                break;
            case AL_INVALID_VALUE:
                std::cerr << "AL_INVALID_VALUE: an invalid value was passed to an OpenAL function";
                break;
            case AL_INVALID_OPERATION:
                std::cerr << "AL_INVALID_OPERATION: the requested operation is not valid";
                break;
            case AL_OUT_OF_MEMORY:
                std::cerr << "AL_OUT_OF_MEMORY: the requested operation resulted in OpenAL running out of memory";
                break;
            default:
                std::cerr << "UNKNOWN AL ERROR: " << error;
            }
            std::cerr << std::endl;
            return false;
        }
        return true;
    }

    template <typename alFunction, typename... Params>
    auto alCallImpl(alFunction function,
                    Params... params)
        -> std::enable_if_t<!std::is_same_v<void, decltype(function(params...))>, decltype(function(params...))>
    {
        auto ret = function(std::forward<Params>(params)...);
        check_al_errors();
        return ret;
    }


    template <typename alFunction, typename... Params>
    auto alCallImpl(alFunction function,
                    Params... params)
        -> std::enable_if_t<std::is_same_v<void, decltype(function(params...))>, bool>
    {
        function(std::forward<Params>(params)...);
        return check_al_errors();
    }


    inline bool check_alc_errors(ALCdevice* device)
    {
        ALCenum error = alcGetError(device);
        if (error != ALC_NO_ERROR)
        {
            switch (error)
            {
            case ALC_INVALID_VALUE:
                std::cerr << "ALC_INVALID_VALUE: an invalid value was passed to an OpenAL function";
                break;
            case ALC_INVALID_DEVICE:
                std::cerr << "ALC_INVALID_DEVICE: a bad m_Device was passed to an OpenAL function";
                break;
            case ALC_INVALID_CONTEXT:
                std::cerr << "ALC_INVALID_CONTEXT: a bad context was passed to an OpenAL function";
                break;
            case ALC_INVALID_ENUM:
                std::cerr << "ALC_INVALID_ENUM: an unknown enum value was passed to an OpenAL function";
                break;
            case ALC_OUT_OF_MEMORY:
                std::cerr << "ALC_OUT_OF_MEMORY: an unknown enum value was passed to an OpenAL function";
                break;
            default:
                std::cerr << "UNKNOWN ALC ERROR: " << error;
            }
            std::cerr << std::endl;
            return false;
        }
        return true;
    }

    template <typename alcFunction, typename... Params>
    auto alcCallImpl(const char* filename,
                     const std::uint_fast32_t line,
                     alcFunction function,
                     ALCdevice* device,
                     Params... params)
        -> std::enable_if_t<std::is_same_v<void, decltype(function(params...))>, bool>
    {
        function(std::forward<Params>(params)...);
        return check_alc_errors(device);
    }

    template <typename alcFunction, typename ReturnType, typename... Params>
    auto alcCallImpl(const char* filename,
                     const std::uint_fast32_t line,
                     alcFunction function,
                     ReturnType& returnValue,
                     ALCdevice* device,
                     Params... params)
        -> std::enable_if_t<!std::is_same_v<void, decltype(function(params...))>, bool>
    {
        returnValue = function(std::forward<Params>(params)...);
        return check_alc_errors(device);
    }

    inline ALenum SDL_AL_Format(const SDL_AudioSpec& wavSpec)
    {
        if (wavSpec.format == SDL_AUDIO_U8)
        {
            if (wavSpec.channels == 1)
            {
                return AL_FORMAT_MONO8;
            }
            if (wavSpec.channels == 2)
            {
                return AL_FORMAT_STEREO8;
            }
        }
        else if (wavSpec.format == SDL_AUDIO_S16)
        {
            if (wavSpec.channels == 1)
            {
                return AL_FORMAT_MONO16;
            }
            if (wavSpec.channels == 2)
            {
                return AL_FORMAT_STEREO16;
            }
        }
        return AL_NONE; // Unsupported format
    }
}