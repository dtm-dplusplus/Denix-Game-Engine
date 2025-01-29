#include "JobSubsystem.h"

#include "Denix/Core/Time/TimerSubsystem.h"

Denix::JobSubsystem::JobSubsystem():
    m_JobMutex(MakeRef<Mutex>()),
    m_BatchUpdateThreshold(225),
    m_SystemThreads(0),
    m_AvailableWorkerThreads(0),
    m_ActiveWorkerThreads(0)
{
}

void Denix::JobSubsystem::Initialize()
{
    Subsystem::Initialize();

    DE_LOG(LogJob, Warn, "Thread Subsystem Initializing")

    // Get System Thread Information
    const int threadCount = std::thread::hardware_concurrency();
    DE_ASSERT(threadCount > 0, "Failed to get system thread count")

    m_SystemThreads = threadCount;
    m_AvailableWorkerThreads = m_SystemThreads - 1;
    m_ActiveWorkerThreads = m_AvailableWorkerThreads;

    // Initialize the worker threads - Subtract 1 for main thread
    for (int i = 0; i < m_ActiveWorkerThreads; i++)
    {
        m_WorkerThreads.emplace_back(MakeRef<Thread>(i));
    }

    DE_LOG(LogJob, Trace, "System threads: {}", m_SystemThreads)
    DE_LOG(LogJob, Trace, "Worker threads: {}", m_ActiveWorkerThreads)
    DE_LOG(LogJob, Info, "Thread Subsystem Initialized")
}

void Denix::JobSubsystem::Deinitialize()
{
    DE_LOG(LogJob, Trace, "JobSubsystem Deinitializing")

    for (const auto& thread : m_WorkerThreads) thread->m_Active = false;

    //m_Jobs.clear();
    m_WorkerThreads.clear();

    DE_LOG(LogJob, Trace, "Cleared Job Queue & Threads")

    Subsystem::Deinitialize();

    DE_LOG(LogJob, Trace, "JobSubsystem Deinitialized")
}

void Denix::JobSubsystem::UpdateActiveThreads()
{
    // Clamp the active threads to the system thread count
    s_Instance->m_ActiveWorkerThreads = std::clamp(s_Instance->m_ActiveWorkerThreads, 1,
                                                   s_Instance->m_AvailableWorkerThreads);

    // Update the worker threads
    for (int i = 0; i < s_Instance->m_AvailableWorkerThreads; i++)
    {
        if (i < s_Instance->m_ActiveWorkerThreads) s_Instance->m_WorkerThreads[i]->m_ShouldWork = true;
        else s_Instance->m_WorkerThreads[i]->m_ShouldWork = false;
    }

    DE_LOG(LogJob, Trace, "Set Active Worker Threads: {} of {}", s_Instance->m_ActiveWorkerThreads,
           s_Instance->m_AvailableWorkerThreads)
}

Denix::Ref<Denix::JobDeclaration> Denix::JobSubsystem::RequestJob()
{
    //std::lock_guard lock(s_Instance->m_RequestJobMutex);

    // Pop the next job from the queue
    //DE_LOG(LogJob, Trace, "Requesting Job")
    if (Ref<JobDeclaration> job; s_Instance->m_Jobs.try_pop(job) && job)
    {
        //DE_LOG(LogJob, Trace, "Job Found: {}", job->m_Name)
        return job;
    }

    //DE_LOG(LogJob, Trace, "No Job Found")
    return nullptr;
}

void Denix::JobSubsystem::StartThreadProfiling()
{
    // Setup Threads for profiling
    for (const auto& thread : s_Instance->m_WorkerThreads)
    {
        thread->m_JobExecCount = 0;
        thread->m_ThreadExecTime = 0.0f;
        thread->m_ThreadSleepTime = 0.0f;
    }

    // Enable internal profiling flag for the threads
    Thread::s_ShouldProfile = true;
}

void Denix::JobSubsystem::StopThreadProfiling()
{
    // Disable internal profiling flag for the threads
    Thread::s_ShouldProfile = false;

    // Submit the thread data to the active profile session
    if (const Ref<ProfileSession> session = ProfileSubsystem::GetActiveProfileSession())
    {
        // Submit the thread data to the active profile session
        for (const auto& thread : s_Instance->m_WorkerThreads)
        {
            session->m_ThreadData.push_back({
                .m_ThreadID = thread->m_ThreadID, .m_JobExecCount = thread->m_JobExecCount,
                .m_ThreadExecTime = thread->
                m_ThreadExecTime,
                .m_ThreadSleepTime = thread->m_ThreadSleepTime
            });
        }
    }
}
