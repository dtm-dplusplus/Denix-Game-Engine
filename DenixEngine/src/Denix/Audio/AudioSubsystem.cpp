#include "AudioSubsystem.h"

#include <SDL3/SDL_events.h>

#include "AudioSource.h"

Denix::AudioSubsystem::AudioSubsystem(): m_Device(nullptr), m_Context(nullptr)
{
}

void Denix::AudioSubsystem::Initialize()
{
    DE_LOG(LogAudio, Warn, "Initializing Audio Subsystem")
    Subsystem::Initialize();

    // Initialize OpenAL
    m_Device = alcOpenDevice(nullptr);
    DE_ASSERT(m_Device, "Failed to open OpenAL m_Device")

    m_Context = alcCreateContext(m_Device, nullptr);
    DE_ASSERT(m_Context, "Failed to create OpenAL context")
    alcMakeContextCurrent(m_Context);

    // Determine the number of sources supported by the audio hardware
    int monoSources, stereoSources;
    alcGetIntegerv(m_Device, ALC_MONO_SOURCES, 1, &monoSources);
    alcGetIntegerv(m_Device, ALC_STEREO_SOURCES, 1, &stereoSources);

    // Check we have audio sources. Resume without audio if we don't
    if (monoSources <= 0 && stereoSources <= 0)
    {
        DE_LOG(LogAudio, Critical, "No audio sources supported by audio hardware")
        DE_LOG(LogAudio, Error, "Audio Subsystem Disabled")
        m_Enabled = false;
        DE_LOG(LogAudio, Info, "Audio Subsystem Initialized")
        return;
    }

    AudioSource::s_MaxMonoSources = monoSources;
    AudioSource::s_MaxStereoSources = stereoSources;
    DE_LOG(LogAudio, Info, "Mono Sources: {}, Stereo Sources: {}", monoSources, stereoSources)
    DE_LOG(LogAudio, Info, "Max Sources: {}", monoSources + stereoSources)
    m_GlobalAudioSource = MakeRef<AudioSource>();

    // Check we have global audio sources. Resume without if we don't
    if (!m_GlobalAudioSource)
    {
        DE_LOG(LogAudio, Critical, "Failed to create global audio source")
        DE_LOG(LogAudio, Error, "Audio Subsystem Disabled")
        m_Enabled = false;
        return;
    }

    DE_LOG(LogAudio, Info, "Audio Subsystem Initialized")
}

void Denix::AudioSubsystem::PlayAudioClipSingle(const Ref<AudioClip>& _audioClip)
{
    if (!_audioClip)
    {
        DE_LOG(LogAudio, Error, "Audio Clip is null")
        return;
    }

    s_Instance->m_GlobalAudioSource->SetAudioClip(_audioClip);
    s_Instance->m_GlobalAudioSource->Play();
    DE_LOG(LogAudio, Info, "Playing Audio Clip: {}", _audioClip->GetAssetName())
}

Denix::Ref<Denix::AudioSource> Denix::AudioSubsystem::CreateNewAudioSource()
{
    if (AudioSource::s_MonoSources + 1 >= AudioSource::s_MaxMonoSources)
    {
        DE_LOG(LogAudio, Error, "Failed to create audio source. No available sources")
        return nullptr;
    }

    if (Ref<AudioSource> audioSource = MakeRef<AudioSource>())
    {
        AudioSource::s_MonoSources++;
        return audioSource;
    }

    DE_LOG(LogAudio, Error, "Failed to create audio source")

    return nullptr;
}

void Denix::AudioSubsystem::Deinitialize()
{
    DE_LOG(LogAudio, Trace, "Audio Subsystem Deinitializing")
    m_GlobalAudioSource.reset();
    alcMakeContextCurrent(nullptr);
    alcDestroyContext(m_Context);
    alcCloseDevice(m_Device);
    DE_LOG(LogAudio, Info, "Audio Subsystem Deinitialized")
    Subsystem::Deinitialize();
}

void Denix::AudioSubsystem::ProcessAudioEvent(const SDL_Event& _event)
{
    switch (_event.type)
    {
    case SDL_EVENT_AUDIO_DEVICE_ADDED:
        DE_LOG(LogInput, Trace, "Audio Device Added Event");
        break;
    case SDL_EVENT_AUDIO_DEVICE_REMOVED:
        DE_LOG(LogInput, Trace, "Audio Device Removed Event");
        break;
    case SDL_EVENT_AUDIO_DEVICE_FORMAT_CHANGED:
        DE_LOG(LogInput, Trace, "Audio Device Format Changed Event");
        break;
    default:
        DE_LOG(LogInput, Error, "Unknown Audio Event");
    }
}
