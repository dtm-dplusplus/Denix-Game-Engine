#pragma once
#include <cstdint>
#include <GLM/vec3.hpp>

#include "Denix/Scene/BaseObject.h"
#include "Denix/Scene/Object.h"

namespace Denix
{
    class AudioClip;

    enum class SourceState: uint8_t
    {
        Stopped = 0,
        Playing = 1,
        Paused = 2
    };
    
    class AudioSource: public BaseObject, public std::enable_shared_from_this<AudioSource>
    {
    public:
        AudioSource();
        ~AudioSource() override;
        void Play() const;
        void Stop() const;
        void Pause() const;

        SourceState GetState() const { return m_State; }
        
        static int GetMaxMonoSources() { return s_MaxMonoSources; }
        static int GetMaxStereoSources() { return s_MaxStereoSources; }

        void SetAudioClip(const Ref<AudioClip>& _audioClip);
        float m_Pitch = 1.0f;
        float m_Gain = 1.0f;
        glm::vec3 m_Position = {0.0f, 0.0f, 0.0f};
        glm::vec3 m_Velocity = {0.0f, 0.0f, 0.0f};
        bool m_Looping = false;

        WRef<AudioClip> m_AudioClip;
    private:
        uint32_t m_Source;
        
        mutable SourceState m_State = SourceState::Stopped;


        inline static uint32_t s_MonoSources = 0;
        inline static int s_MaxMonoSources = -1;

        inline static uint32_t s_StereoSources = 0;
        inline static int s_MaxStereoSources = -1;

        friend class AudioSubsystem;
    };
}
