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
        float GetElapsedTime();

        float m_Duration;
       std::chrono::time_point<std::chrono::system_clock> m_StartTime, m_EndTime;
    };
    
}
