#include "C:/Users/Denis/Documents/Programming Projects/Denix-Game-Engine/Build/DenixEngine/CMakeFiles/DenixEngine.dir/Debug/cmake_pch.hxx"
#include "ThreadSubsystem.h"

#include "Denix/Core/TimerSubsystem.h"

Denix::ThreadSubsystem* Denix::ThreadSubsystem::s_ThreadSubsystem = nullptr;

void Denix::ThreadSubsystem::Initialize()
{
    Subsystem::Initialize();

    DE_LOG(Log, Warn, "Thread Subsystem Initializing")
    // Get Thread Information
    m_SystemThreadCount = std::thread::hardware_concurrency();
    
    SYSTEM_INFO sysinfo;
    GetSystemInfo(&sysinfo);
    DE_LOG(Log, Trace, "Number of cores: {}", sysinfo.dwNumberOfProcessors)
    DE_LOG(Log, Trace, "Number of threads: {}", m_SystemThreadCount)
    DE_LOG(Log, Trace, "Page size: {}", sysinfo.dwPageSize)
    DE_LOG(Log, Trace, "Processor type: {}", sysinfo.dwProcessorType)
    DE_LOG(Log, Trace, "Minimum application address: {}", sysinfo.lpMinimumApplicationAddress)
    DE_LOG(Log, Trace, "Maximum application address: {}", sysinfo.lpMaximumApplicationAddress)
    DE_LOG(Log, Trace, "Active processor mask: {}", sysinfo.dwActiveProcessorMask)
    DE_LOG(Log, Trace, "Allocation granularity: {}", sysinfo.dwAllocationGranularity)
    DE_LOG(Log, Trace, "Processor architecture: {}", sysinfo.wProcessorArchitecture)

    // Initialize the scheduler thread
    m_Threads.emplace_back();
    m_Threads.back().InitThread(&ThreadSubsystem::ScheduleWork, this);

    // Initialize the worker threads
    for (size_t i = 0; i < 4; i++)
    {
        m_Threads.emplace_back();
        m_Threads.back().InitWorkerThread();
    }
    DE_LOG(Log, Info, "Thread Subsystem Initialized")
}

void Denix::ThreadSubsystem::Deinitialize()
{
    Subsystem::Deinitialize();

    DE_LOG(Log, Trace, "Thread Subsystem Deinitializing")
    DE_LOG(Log, Trace, "Thread Subsystem Deinitialized")
}

void Denix::ThreadSubsystem::ScheduleWork()
{
    while (m_Enabled)
    {
        // Process jobs
        // Find a thread that is not working
        for (auto& thread : m_Threads)
        {
            if (!thread.m_IsWorking && !m_Jobs.empty())
            {
                
                thread.m_Job = m_Jobs.front();
                m_Jobs.pop();
            }
        }
        DE_LOG(Log, Info, "Thread Subsystem Working")
        //  std::this_thread::sleep_for(std::chrono::milliseconds(10)); // Adjust sleep duration as needed
    }
}