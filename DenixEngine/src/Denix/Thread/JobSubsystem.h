#pragma once
#include "Denix/System/Subsystem.h"

#include "Denix/Thread/Thread.h"
#include <queue>  

namespace Denix
{
    
    class JobSubsystem: public Subsystem, public std::enable_shared_from_this<JobSubsystem>
    {
    public:
        JobSubsystem()
        {
            s_ThreadSubsystem = this;
            m_JobsDone = 0;
        }
        ~JobSubsystem() override
        {
            s_ThreadSubsystem = nullptr;
        }

        /**
         * 
         * @tparam Func 
         * @tparam Args 
         * @param _name 
         * @param _priority 
         * @param _waitCounter
         * @param _func 
         * @param _args 
         */
        template <typename Func, typename... Args>
    static void AddJob(const std::string& _name, const Priority _priority, const Ref<Counter>& _waitCounter, Func&& _func, Args&&... _args)
    {
            Ref<JobDeclaration> job = MakeRef<JobDeclaration>();
            job->m_Name = _name;
            job->m_Priority = _priority;
            job->m_WaitCounter =  _waitCounter? _waitCounter : MakeRef<Counter>(1);
            job->m_EntryPoint = std::bind(std::forward<Func>(_func), std::forward<Args>(_args)...);
            
        s_ThreadSubsystem->m_Jobs.push(job);
    }
        


        static Ref<Thread> GetThread(std::thread::id _id)
        {
           for (const auto& thread: s_ThreadSubsystem->m_WorkerThreads)
               if(thread->m_ThreadID == _id) return thread;

            return nullptr;
        }


        Ref<Thread> m_ThreadScheduler;
        std::vector<Ref<Thread>> m_WorkerThreads;
        size_t m_SystemThreadCount = 0;

        struct JobComparator
        {
            bool operator() (const Ref<JobDeclaration>& _lhs, const Ref<JobDeclaration>& _rhs) const
            {
                return _lhs->m_Priority < _rhs->m_Priority;
            }
        };
        std::priority_queue<Ref<JobDeclaration>, std::vector<Ref<JobDeclaration>>, JobComparator> m_Jobs;
        size_t m_JobsDone;

        static Ref<JobSubsystem> Get() { return s_ThreadSubsystem->shared_from_this(); }
        static JobSubsystem* s_ThreadSubsystem;

    private:
        void Initialize() override;

        void Deinitialize() override;

        void ScheduleWork();

        
        friend class Engine;
    };
}
