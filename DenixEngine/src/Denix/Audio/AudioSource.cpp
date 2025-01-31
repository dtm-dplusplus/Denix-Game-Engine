#include "AudioSource.h"

#include "AudioClip.h"
#include "AudioPrimitive.h"
#include "AudioSubsystem.h"

#include "Denix/Core/Log/Logger.h"

Denix::AudioSource::AudioSource(): BaseObject({"AudioSource"})
{
    // Check we have available sources
    if (s_MonoSources == s_MaxMonoSources)
    {
        DE_LOG(LogAudio, Error, "Failed to create audio source. No available sources")
        return;
    }

    // Create OpenAL source
    alCall(alGenSources, 1, &m_Source);

    // Validate source creation
    if (m_Source == 0)
    {
        DE_LOG(LogAudio, Error, "Failed to create OpenAL source")
        return;
    }

    // Set source properties
    alCall(alSourcef, m_Source, AL_PITCH, m_Pitch);
    alCall(alSourcef, m_Source, AL_GAIN, m_Gain);
    alCall(alSource3f, m_Source, AL_POSITION, m_Position.x, m_Position.y, m_Position.z);
    alCall(alSource3f, m_Source, AL_VELOCITY, m_Velocity.x, m_Velocity.y, m_Velocity.z);
    alCall(alSourcei, m_Source, AL_LOOPING, m_Looping);

    // Increment source count. 
    s_MonoSources++;
}

Denix::AudioSource::~AudioSource()
{
    // Delete OpenAL source
    alCall(alDeleteSources, 1, &m_Source);
    s_MonoSources--;

    m_AudioClip.reset();
}

void Denix::AudioSource::SetAudioClip(const Ref<AudioClip>& _audioClip)
{
    if (!_audioClip)
    {
        DE_LOG(LogAudio, Error, "Failed to set audio clip. Invalid audio clip. {}", GetName())
        return;
    }

    // Attach buffer to source
    m_AudioClip = _audioClip;
    alCall(alSourcei, m_Source, AL_BUFFER, m_AudioClip.lock()->m_Buffer);
}

void Denix::AudioSource::Play() const
{
    if (!m_AudioClip.lock())
    {
        DE_LOG(LogAudio, Error, "Failed to play audio source. No audio clip attached. {}", GetName())
        return;
    }

    // Enable looping
    if(m_Looping) alCall(alSourcei, m_Source, AL_LOOPING, AL_TRUE);

    alCall(alSourcePlay, m_Source);
    m_State = SourceState::Playing;
}

void Denix::AudioSource::Stop() const
{
    alCall(alSourceStop, m_Source);
    m_State = SourceState::Stopped;
}

void Denix::AudioSource::Pause() const
{
    alCall(alSourcePause, m_Source);
    m_State = SourceState::Paused;
}
