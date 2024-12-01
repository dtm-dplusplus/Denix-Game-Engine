#include "C:/Users/Denis/Documents/Programming Projects/Denix-Game-Engine/Build/DenixEngine/CMakeFiles/DenixEngine.dir/Debug/cmake_pch.hxx"
#include "Thread.h"

#include "JobSubsystem.h"

int Denix::Thread::s_WaitForCounterSleepTime = 50;
int Denix::Thread::s_WaitForJobSleepTime = 100;

void Denix::Thread::Work()
{
    while (m_ShouldWork)
    {
        m_Job = JobSubsystem::GetJob();
        if (m_Job)
        {
            m_IsWorking = true;
            m_Job->m_Timer.Start();
            m_Job->m_EntryPoint();
            m_Job->m_Timer.Stop();
            m_Job->m_WaitCounter->Decrement();
            m_Job = nullptr;

            // Let the scheduler know that the job is done
            m_JobsDone++;
            m_IsWorking = false;
        }

        // Wait briefly to allow jobs to populate the queue and be prioritized
        std::this_thread::sleep_for(std::chrono::nanoseconds(s_WaitForJobSleepTime));
    }
}