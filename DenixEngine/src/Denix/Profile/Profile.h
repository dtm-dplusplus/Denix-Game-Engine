#pragma once

#include "Denix/Scene/Object.h"
#include "Denix/Core/Timer.h"
#include <glm/vec3.hpp>

namespace Denix
{
    class Timer;

    struct DurationBuffer
    {
        std::vector<TimeEvent> ProfileResults;

        DurationBuffer()
        {
            // Reserve to reduce reallocations during profiling
            ProfileResults.reserve(5000);
        }

        /**
         * Save the profile result. Duration is calculated from the start and end time as the 3rd element in the vector.
         * @param _result
         */
        void SaveResult(const TimeEvent _result) { ProfileResults.push_back(_result);}

        
        /**
         * Returns the last completed  profile result
         * @return glm::vec2<float,float> Profile Start Time, Profile Duration
         */
        TimeEvent GetLastResult() const
        {
            if (!ProfileResults.empty()) return ProfileResults.back();
            return {};
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
        Profile()
        {
            m_Timer = MakeRef<Timer>();
            m_AverageDuration = 0.0f;
            m_MinimumDuration = 0.0f;
            m_MaximumDuration = 0.0f;
        }
        Profile(const ObjectInit& _objInit);

        Ref<Timer> m_Timer;

        virtual void Start();
        virtual void End();

        float GetMinDuration() const { return m_MinimumDuration; }
        float GetMinDurationMs() const { return m_MinimumDuration * 1000.0f; }

        float GetMaxDuration() const { return m_MaximumDuration; }
        float GetMaxDurationMs() const { return m_MaximumDuration * 1000.0f; }
        
        float GetAverageDuration() const { return m_AverageDuration; }
        float GetAverageDurationMs() const { return m_AverageDuration * 1000.0f; }

        float GetLastProfileDuration() const { return GetLastProfileResult().Duration;}
        float GetLastPorifleDurationMs() const { return GetLastProfileResult().Duration * 1000.0f; }
        TimeEvent GetLastProfileResult() const { return m_DurationBuffer.GetLastResult(); }

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

        DurationBuffer m_DurationBuffer;
        
        friend class ProfileSubsystem;
    };
}
