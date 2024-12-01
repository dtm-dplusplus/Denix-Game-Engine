#pragma once
#include "Denix/Scene/Object.h"

#include <chrono>

namespace Denix
{
    class Timer: public Object
    {
    public:
        Timer() = default;
        Timer(const ObjectInit& _objInit, bool _start = false);

        void Start();
        void Stop();

       //template<typename T = std::chrono::seconds>
        float GetDuration() const
        {
            return std::chrono::duration<float>(m_EndTime - m_StartTime).count();
        }

      template<typename Period = std::chrono::seconds>
        float GetElapsed() const
        {
            if (m_StartTime == std::chrono::time_point<std::chrono::system_clock>())
            {
                DE_LOG(LogCore, Error, "{} start time not set", GetName())
                return 0.0f;
            }

           std::chrono::time_point<std::chrono::system_clock> endTime = std::chrono::system_clock::now();
            return std::chrono::duration<float>( endTime- m_StartTime).count();
        }
        
        std::chrono::duration<float> m_Duration;
        std::chrono::time_point<std::chrono::system_clock> m_StartTime, m_EndTime;
    };
    
}
