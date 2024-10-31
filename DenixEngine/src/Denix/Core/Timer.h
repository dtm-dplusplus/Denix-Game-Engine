#pragma once
#include "Denix/Scene/Object.h"

#include <chrono>

namespace Denix
{
    class Timer: public Object
    {
    public:
        Timer(const ObjectInitializer& _objInit, bool _start = false);

        void Start();
        void Stop();

       template<typename Period = std::chrono::seconds>
        float GetDuration() const
        {
           // Return 0 if the end time is not set
           if(m_EndTime == std::chrono::time_point<std::chrono::system_clock>())
           {
               DE_LOG(LogCore, Error, "{} end time not set", GetName())
                return 0.0f;
           }
               
           
            return std::chrono::duration<float, Period>(m_EndTime - m_StartTime).count();
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
