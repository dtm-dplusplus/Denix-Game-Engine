#pragma once

#include "Denix/Scene/Object.h"
#include "Denix/Core/Timer.h"
#include <glm/vec2.hpp>

namespace Denix
{
    class Timer;

    struct ProfileBuffer
    {
        int MaxSize;
        int Offset;
        std::vector<glm::vec2> Data;

        ProfileBuffer(int max_size = 2000)
        {
            MaxSize = max_size;
            Offset = 0;
            Data.reserve(MaxSize);
        }

        void AddPoint(float x, float y)
        {
            if (Data.size() < MaxSize)
                Data.emplace_back(x, y);
            else
            {
                Data[Offset] = glm::vec2(x, y);
                Offset = (Offset + 1) % MaxSize;
            }
        }

        void Erase()
        {
            if (Data.size() > 0)
            {
                Data.resize(0);
                Offset = 0;
            }
        }
    };
    
    /**
     * Profile class for profiling code.
     * Millisecond precision.
     */
    class Profile: public Object
    {
    public:
        Profile(const ObjectInit& _objInit);

        Ref<Timer> m_Timer;

        virtual void Start();
        virtual void End();

        float GetAverageDuration() const { return m_AverageDuration; }
        float GetDuration() const;

        std::vector<float> m_DurationRecords;
        
        /**
         * The average duration of the profile.
         */
        float m_AverageDuration;

        float m_MinimumDuration;

        float m_MaximumDuration;
        /**
         * Number of durations to average.
         */
        int m_AverageDurationCount;

        /**
        * Percentage of the frame time the profile took.
        * Updated by the ProfileSubsystem.
        */
        float m_FramePercentage;

        ProfileBuffer m_Buffer;

        /**
         * Lets the profile know if it should visualize itself in the ProfileSubsystem.
         */
        bool m_Visualize = false;
    protected:
        /**
         * The number of durations recorded before averaging.
         * This is reset to 0 when the average is calculated.
         */
        int m_DurationCount;

       
        
        friend class ProfileSubsystem;
    };
}