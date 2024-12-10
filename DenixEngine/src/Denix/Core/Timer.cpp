#include "C:/Users/Denis/Documents/Programming Projects/Denix-Game-Engine/Build/DenixEngine/CMakeFiles/DenixEngine.dir/Debug/cmake_pch.hxx"
#include "Timer.h"

#include "TimerSubsystem.h"

namespace Denix
{
    Timer::Timer(const ObjectInit& _objInit, bool _start): Object(_objInit)
    {
        m_Duration = 0.0f;
        m_StartTime = 0.0f;
        m_EndTime = 0.0f;
        
        if (_start) Start();
    }

    void Timer::Start()
    {
        m_StartTime =   TimerSubsystem::GetElapsedTime();
    }

    void Timer::Stop()
    {
        m_EndTime = TimerSubsystem::GetElapsedTime();
        m_Duration = m_EndTime - m_StartTime;
    }

    float Timer::GetElapsed() const
    {
        return TimerSubsystem::GetElapsedTime() - m_StartTime;
    }
}