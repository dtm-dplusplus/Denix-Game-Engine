#pragma once
#include "Denix/Scene/Object.h"
#include "Denix/Core/Timer.h"

namespace Denix
{
    class Timer;

    
    /**
     * Profile class for profiling code.
     * Millisecond precision.
     */
    class Profile: public Object
    {
    public:
        Profile(const ObjectInitializer& _objInit);

        Ref<Timer> m_Timer;

        void Start();
        void End();

        float GetAverageDuration() const { return m_AverageDuration; }
        float GetDuration() const;

        std::vector<float> m_DurationRecords;
        
        /**
         * The average duration of the profile.
         */
        float m_AverageDuration;

        /**
         * Number of durations to average.
         */
        int m_AverageDurationCount;

        /**
        * Percentage of the frame time the profile took.
        * Updated by the ProfileSubsystem.
        */
        float m_FramePercentage;
        
    private:
        /**
         * The number of durations recorded before averaging.
         * This is reset to 0 when the average is calculated.
         */
        int m_DurationCount;

       
        
        friend class ProfileSubsystem;
    };
}