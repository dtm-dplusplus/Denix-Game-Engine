#include "C:/Users/Denis/Documents/Programming Projects/Denix-Game-Engine/Build/DenixEngine/CMakeFiles/DenixEngine.dir/Debug/cmake_pch.hxx"
#include "Timer.h"

namespace Denix
{
    Timer::Timer(const ObjectInitializer& _objInit, bool _start): Object(_objInit)
    {
        if (_start) Start();
    }

    void Timer::Start()
    {
        m_StartTime = std::chrono::system_clock::now();
    }

    void Timer::Stop()
    {
        m_EndTime = std::chrono::system_clock::now();
       m_Duration = std::chrono::duration<float>(m_EndTime - m_StartTime);
    }
}