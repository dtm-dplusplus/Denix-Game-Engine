#include "C:/Users/Denis/Documents/Programming Projects/Denix-Game-Engine/Build/DenixEngine/CMakeFiles/DenixEngine.dir/Debug/cmake_pch.hxx"
#include "AudioSubsystem.h"

#include "AudioSource.h"

void Denix::AudioSubsystem::Initialize()
{
    Subsystem::Initialize();
    DE_LOG(LogAudio, Warn, "Initializing Audio Subsystem")

    // Initialize OpenAL
    m_Device = alcOpenDevice(nullptr);
    if (!m_Device) {
        DE_LOG(LogAudio, Error, "Failed to open OpenAL m_Device")
    }

    m_Context = alcCreateContext(m_Device, nullptr);
    if (!m_Context) {
        DE_LOG(LogAudio, Error, "Failed to create OpenAL context")
    }
    alcMakeContextCurrent(m_Context);

    // Determine the number of sources supported by the audio hardware
    int monoSources, stereoSources;
    alcGetIntegerv(m_Device, ALC_MONO_SOURCES, 1, &monoSources);
    alcGetIntegerv(m_Device, ALC_STEREO_SOURCES, 1, &stereoSources);

    // Check we have audio sources. Resume without audio if we don't
    if (monoSources <= 0 && stereoSources <= 0) {
        DE_LOG(LogAudio, Critical, "No audio sources supported by audio hardware")
        DE_LOG(LogAudio, Error, "Audio Subsystem Disabled")
        m_Enabled = false;
        return;
    }
    
    AudioSource::s_MaxMonoSources = monoSources;
    AudioSource::s_MaxStereoSources = stereoSources;
    DE_LOG(LogAudio, Info, "Mono Sources: {}, Stereo Sources: {}", monoSources, stereoSources)
    DE_LOG(LogAudio, Info, "Max Sources: {}", monoSources + stereoSources)
    
    DE_LOG(LogAudio, Info, "Audio Subsystem Initialized")
}

void Denix::AudioSubsystem::Deinitialize()
{
    DE_LOG(LogAudio, Trace, "Deinitializing Audio Subsystem")
    alcMakeContextCurrent(nullptr);
    alcDestroyContext(m_Context);
    alcCloseDevice(m_Device);
    DE_LOG(LogAudio, Info, "Audio Subsystem Deinitialized")
    Subsystem::Deinitialize();
}