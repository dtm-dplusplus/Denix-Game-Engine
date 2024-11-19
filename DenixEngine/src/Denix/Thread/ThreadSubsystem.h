#pragma once
#include "Denix/System/Subsystem.h"

#include "Denix/Thread/Thread.h"

namespace Denix
{
    
    class ThreadSubsystem: public Subsystem, public std::enable_shared_from_this<ThreadSubsystem>
    {
    public:
        ThreadSubsystem()
        {
            s_ThreadSubsystem = this;
            m_JobsDone = 0;
        }
        ~ThreadSubsystem() override
        {
            s_ThreadSubsystem = nullptr;
        }

       
        template <typename Func, typename... Args>
       static  void AddJob(Func&& _func, Args&&... _args)
        {
            s_ThreadSubsystem->m_Jobs.emplace(std::bind(std::forward<Func>(_func), std::forward<Args>(_args)...));
        }

        std::vector<Thread> m_Threads;
        size_t m_SystemThreadCount = 0;
        
        std::queue<std::function<void()>> m_Jobs;
        size_t m_JobsDone;

        static Ref<ThreadSubsystem> Get() { return s_ThreadSubsystem->shared_from_this(); }
        static ThreadSubsystem* s_ThreadSubsystem;

    private:
        void Initialize() override;

        void Deinitialize() override;

        void ScheduleWork();

        
        friend class Engine;
    };
}
