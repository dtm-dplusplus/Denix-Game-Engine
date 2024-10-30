#pragma once
#include "Denix/Scene/Object.h"

#include <chrono>

namespace Denix
{
    class Timer: public Object
    {
    public:
        Timer(const ObjectInitializer& _objInit);

        void Start();
        void Stop();

       template<typename Period = std::chrono::seconds>
        float GetDuration() const
        {
            return std::chrono::duration<float, Period>(m_EndTime - m_StartTime).count();
        }

        
        std::chrono::duration<float> m_Duration;
        std::chrono::time_point<std::chrono::system_clock> m_StartTime, m_EndTime;
    };
    
}
