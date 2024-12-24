#include "C:/Users/Denis/Documents/Programming Projects/Denix-Game-Engine/Build/DenixEngine/CMakeFiles/DenixEngine.dir/Debug/cmake_pch.hxx"
#include "AudioClip.h"
#include "al/al.h"
#include "al/alc.h"
#include "al/alext.h"
#include "Denix/Resource/Asset.h"

Denix::AudioClip::AudioClip(const Ref<Asset>& _audioClipAsset): Object({_audioClipAsset->GetAssetName()})
{
    m_AudioClpAsset = _audioClipAsset;

    uint8_t* audioBuffer;
    if (!SDL_LoadWAV( m_AudioClpAsset->GetAssetPath().c_str(), &m_ClipSpec, &audioBuffer, &m_WavLength)) {
        alCall(alDeleteBuffers, 1, &m_Buffer);
        alCall(alDeleteSources, 1, &m_Source);
        SDL_free(audioBuffer);
        DE_LOG(LogAudio, Error, "Failed to load WAV File: {}", _audioClipAsset->GetAssetName())
        return;
    }

    // Create OpenAL buffer and source
    alCall(alGenBuffers,1, &m_Buffer);
    alCall(alGenSources, 1, &m_Source);
    
    // Copy audio data to OpenAL buffer
    alCall(alBufferData, m_Buffer, SDL_AL_Format(m_ClipSpec), audioBuffer, m_WavLength, m_ClipSpec.freq);
  
    
    // alCall(alSourcef, source, AL_PITCH, 1);
    // alCall(alSourcef, source, AL_GAIN, 1.0f);
    // alCall(alSource3f, source, AL_POSITION, 0, 0, 0);
    // alCall(alSource3f, source, AL_VELOCITY, 0, 0, 0);
    alCall(alSourcei, m_Source, AL_LOOPING, AL_TRUE);
    SDL_free(audioBuffer);

    // Attach buffer to source
    alCall(alSourcei, m_Source, AL_BUFFER, m_Buffer);

    m_State = AudioClipState::Stopped;
}

Denix::AudioClip::~AudioClip()
{
    alCall(alDeleteBuffers, 1, &m_Buffer);
    alCall(alDeleteSources, 1, &m_Source);
}

void Denix::AudioClip::Play() const
{
    alCall(alSourcePlay, m_Source);
    m_State = AudioClipState::Playing;
}

void Denix::AudioClip::Stop() const
{
    alCall(alSourceStop, m_Source);
    m_State = AudioClipState::Stopped;
}

void Denix::AudioClip::Pause() const
{
    alCall(alSourcePause, m_Source);
    m_State = AudioClipState::Paused;
}