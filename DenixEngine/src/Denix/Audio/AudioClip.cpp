#include "C:/Users/Denis/Documents/Programming Projects/Denix-Game-Engine/Build/DenixEngine/CMakeFiles/DenixEngine.dir/Debug/cmake_pch.hxx"
#include "AudioClip.h"
#include "al/al.h"
#include "al/alc.h"
#include "al/alext.h"

void Denix::AudioBuffer::GenBuffer()
{
    alGenBuffers(1, &m_Buffer);
}

void Denix::AudioBuffer::BufferData(ALenum format, const ALvoid* data, ALsizei size, ALsizei samplerate)
{
    alBufferData( m_Buffer, format, data, size, samplerate );
}

void Denix::AudioSource::GenSource()
{
    alGenSources(1, &m_Source);
}


Denix::AudioClip::AudioClip()
{
    m_Buffer.GenBuffer();
    m_Source.GenSource();
    //alSourcei(m_Source.m_Source, AL_BUFFER, m_Buffer.m_Buffer);
}

Denix::AudioClip::AudioClip(const ObjectInit& _objInit): Object(_objInit)
{}

Denix::AudioClip::~AudioClip()
{
    alDeleteBuffers(1, &m_Buffer.m_Buffer);
    alDeleteSources(1, &m_Source.m_Source);
}

void Denix::AudioClip::Play() const
{
    alSourcePlay(m_Source.m_Source);
}

void Denix::AudioClip::Stop() const
{
    alSourceStop(m_Source.m_Source);
}