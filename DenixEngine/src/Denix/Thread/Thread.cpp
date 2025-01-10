#include "C:/Users/Denis/Documents/Programming Projects/Denix-Game-Engine/Build/DenixEngine/CMakeFiles/DenixEngine.dir/Debug/cmake_pch.hxx"
#include "Thread.h"

#include "JobSubsystem.h"
#include "Denix/Profile/ProfileSubsystem.h"

Denix::Thread::Thread(const int _index): m_ThreadIDInt(0),
                                         m_ThreadIndex(_index),
                                         m_JobExecCount(0),
                                         m_ThreadExecTime(0.0f),
                                         m_ThreadSleepTime(0.0f)
{
    m_Thread = std::thread(&Thread::Work, this);
    std::stringstream ss;
    ss << m_Thread.get_id();
    m_ThreadIDInt = std::stoi(ss.str());
    DE_LOG(LogThread, Trace, "Thread Index: {} ID: {}", m_ThreadIndex, m_ThreadIDInt)
}

Denix::Thread::~Thread()
{
    if (m_Thread.joinable())
    {
        m_Thread.join();
        DE_LOG(LogThread, Trace, "Thread {} joined", m_ThreadIDInt)
    }
    else
    {
        DE_LOG(LogThread, Error, "Thread {} not joinable", m_ThreadIDInt)
    }
}

void Denix::Thread::Work()
{
    while (m_Active)
    {
        // Request a job from the job queue
        if (m_ShouldWork) m_Job = JobSubsystem::RequestJob();

        // If a job was found in the queue, execute it
        if (m_Job)
        {
            if (s_ShouldProfile)
            {
                m_Job->m_ThreadIndex = m_ThreadIndex;
                DE_PROFILE_JOB(m_Job)
            }
            m_Job->m_EntryPoint();
             if (s_ShouldProfile) DE_PROFILE_JOB_END(m_Job)
            m_Job->m_WaitCounter->Decrement();

            // Profile the thread
            if (s_ShouldProfile)
            {
                m_JobExecCount++;
                m_ThreadExecTime += m_Job->m_JobTime.Duration;
                m_ThreadSleepTime += static_cast<float>(s_WaitForJobSleepTime) * 0.000000001f; // Convert to seconds. We sleep right after this loop so we can add the sleep time here
            }

            m_Job = nullptr;
        }

        // Wait briefly to allow jobs to populate the queue and be prioritized & Reduce CPU usage
        std::this_thread::sleep_for(std::chrono::nanoseconds(s_WaitForJobSleepTime));
    }
}