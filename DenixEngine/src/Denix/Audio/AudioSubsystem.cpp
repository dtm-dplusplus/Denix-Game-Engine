#include "C:/Users/Denis/Documents/Programming Projects/Denix-Game-Engine/Build/DenixEngine/CMakeFiles/DenixEngine.dir/Debug/cmake_pch.hxx"
#include "AudioSubsystem.h"

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
