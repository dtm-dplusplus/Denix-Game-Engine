#include "C:/Users/Denis/Documents/Programming Projects/Denix-Game-Engine/Build/DenixEngine/CMakeFiles/DenixEngine.dir/Debug/cmake_pch.hxx"
#include "AudioClip.h"
#include "al/al.h"
#include "al/alc.h"
#include "al/alext.h"
#include "Denix/Resource/Asset.h"

Denix::AudioClip::AudioClip(const Ref<Asset>& _audioClipAsset): Object({_audioClipAsset->GetAssetName()})
{
    m_AudioClpAsset = _audioClipAsset;
}

Denix::AudioClip::~AudioClip()
{
    m_AudioClpAsset.reset();
    
    alCall(alDeleteBuffers, 1, &m_Buffer);
}

bool Denix::AudioClip::Load()
{
    uint8_t* audioBuffer;

    auto audioClpAsset = m_AudioClpAsset.lock();
    
    if (!SDL_LoadWAV( audioClpAsset->GetAssetPath().c_str(), &m_ClipSpec, &audioBuffer, &m_WavLength)) {
        SDL_free(audioBuffer);
        DE_LOG(LogAudio, Error, "Failed to load WAV File: {}", audioClpAsset->GetAssetName())
        return false;
    }

    // Create OpenAL buffer
    alCall(alGenBuffers,1, &m_Buffer);

    // Validate buffer creation
    if (m_Buffer == 0)
    {
        SDL_free(audioBuffer);
        DE_LOG(LogAudio, Error, "Failed to create OpenAL buffer: {}", audioClpAsset->GetAssetName())
        return false;
    }
    
    // Copy audio data to OpenAL buffer & Free audio buffer
    alCall(alBufferData, m_Buffer, SDL_AL_Format(m_ClipSpec), audioBuffer, m_WavLength, m_ClipSpec.freq);
    SDL_free(audioBuffer);

    return true;
}
