#pragma once

#include "Denix/Scene/Object.h"
#include "Denix/Core/Timer.h"
#include <glm/vec2.hpp>

namespace Denix
{
    class Timer;

    struct ProfileData
    {
        std::vector<glm::vec2> ProfileResults;

        ProfileData()
        {
            // Reserve to reduce reallocations during profiling
            ProfileResults.reserve(5000);
        }

        /**
         * 
         * @param x The start time of the profile
         * @param y The duration of the profile
         */
        void SaveResult(float x, float y) { ProfileResults.emplace_back(x, y); }

        
        /**
         * Returns the last completed  profile result
         * @return glm::vec2<float,float> Profile Start Time, Profile Duration
         */
        glm::vec2 GetLastResult() const
        {
            if (!ProfileResults.empty()) return ProfileResults.back();
            return {0.0f, 0.0f};
        }
        
        void Erase() { if (!ProfileResults.empty()) ProfileResults.resize(0);}
    };
    
    /**
     * Profile class for profiling code.
     * Millisecond precision.
     */
    class Profile: public Object
    {
    public:
        Profile() = default;
        Profile(const ObjectInit& _objInit);

        Ref<Timer> m_Timer;

        float m_CurrentProfileStartTime;
        
        virtual void Start();
        virtual void End();

        float GetAverageDuration() const { return m_AverageDuration; }

        float GetLastProfileDuration() const { return m_Timer->GetDuration();}
        glm::vec2 GetLastProfileResult() const { return m_Buffer.GetLastResult(); }

        /**
         * The average duration of the profile.
         */
        float m_AverageDuration;

        float m_MinimumDuration;

        float m_MaximumDuration;
        
        /**
         * Number of durations to average.
         */
        inline static int s_AverageDurationCount = 30;

        ProfileData m_Buffer;
        
        friend class ProfileSubsystem;
    };
}