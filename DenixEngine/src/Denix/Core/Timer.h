#pragma once
#include "Denix/Scene/Object.h"

#include <chrono>

namespace Denix
{
    class Timer: public Object
    {
    public:
        Timer()
        {
            m_Duration = 0.0f;
            m_StartTime = 0.0f;
            m_EndTime = 0.0f;
        }
        
        Timer(const ObjectInit& _objInit, bool _start = false);

        void Start();

        void Stop();

        float GetElapsed() const;
        float GetElapsedMs() const  {return GetElapsed() * 1000.0f;}

        float GetDuration() const
        {
            if (m_EndTime == 0.0f) return GetElapsed();
            return m_Duration;
        }

        float GetDurationMs() const {  return GetDuration() * 1000.0f;}

        float GetStartTime() const { return m_StartTime; }
        float GetEndTime() const { return m_EndTime; }

    private:
        float m_Duration;
       
        float m_StartTime, m_EndTime;

        friend class TimerSubsystem;
    };
}
