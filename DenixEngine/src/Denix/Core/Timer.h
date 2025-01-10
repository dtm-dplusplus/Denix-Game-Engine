#pragma once
#include "Denix/Scene/Object.h"
#include "Denix/Core/TimePrimitive.h"

#include <chrono>

namespace Denix
{
    class Timer
    {
    public:
        Timer() = default;
        ~Timer() = default;
        
        void Reset()
        {
            m_TimeEvent.Duration = 0.0f;
            m_TimeEvent.EndTime = 0.0f;
            m_TimeEvent.StartTime = GetProgramElaspedTime();
        }
        void Start()
        {
            m_TimeEvent.StartTime = GetProgramElaspedTime();
        }

        void Stop()
        {
            m_TimeEvent.EndTime = GetProgramElaspedTime();
            m_TimeEvent.Duration = m_TimeEvent.EndTime - m_TimeEvent.StartTime;
        }

        float GetElapsed() const
        {
            return GetProgramElaspedTime() - m_TimeEvent.StartTime;
        }
        
        float GetElapsedMs() const  {return GetElapsed() * 1000.0f;}

        /**
     * 
     * @return the time elapsed since the start of the program (seconds)
     */
        static float GetProgramElaspedTime()
        {
            return std::chrono::duration<float>(
                std::chrono::high_resolution_clock::now() - m_ProgramStartTimePoint).count();
        }

        
        float GetDuration() const
        {
            if (m_TimeEvent.EndTime == 0.0f) return GetElapsed();
            return m_TimeEvent.Duration;
        }

        float GetDurationMs() const {  return GetDuration() * 1000.0f;}

        float GetStartTime() const { return m_TimeEvent.StartTime; }
        float GetEndTime() const { return m_TimeEvent.EndTime; }

    private:
        TimeEvent m_TimeEvent;

        inline static std::chrono::time_point<std::chrono::high_resolution_clock> m_ProgramStartTimePoint;
        
        friend class TimerSubsystem;
        friend class Profile;
    };
}

