#include "C:/Users/Denis/Documents/Programming Projects/Denix-Game-Engine/Build/DenixEngine/CMakeFiles/DenixEngine.dir/Debug/cmake_pch.hxx"
#include "AudioClip.h"
#include "al/al.h"
#include "al/alc.h"
#include "al/alext.h"
#include "Denix/Asset/Asset.h"

Denix::AudioClip::AudioClip(const AssetInit& _assetInit): Asset(_assetInit)
{
}

Denix::AudioClip::~AudioClip()
{
    alCall(alDeleteBuffers, 1, &m_Buffer);
}

bool Denix::AudioClip::Load()
{
    uint8_t* audioBuffer;

    if (!SDL_LoadWAV( GetAssetPath().c_str(), &m_ClipSpec, &audioBuffer, &m_WavLength)) {
        SDL_free(audioBuffer);
        DE_LOG(LogAudio, Error, "Failed to load WAV File: {}", GetAssetName())
        return false;
    }

    // Create OpenAL buffer
    alCall(alGenBuffers,1, &m_Buffer);

    // Validate buffer creation
    if (m_Buffer == 0)
    {
        SDL_free(audioBuffer);
        DE_LOG(LogAudio, Error, "Failed to create OpenAL buffer: {}", GetAssetName())
        return false;
    }
    
    // Copy audio data to OpenAL buffer & Free audio buffer
    alCall(alBufferData, m_Buffer, SDL_AL_Format(m_ClipSpec), audioBuffer, m_WavLength, m_ClipSpec.freq);
    SDL_free(audioBuffer);

    return true;
}
