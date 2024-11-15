#pragma once
#include "Denix/Core.h"
#include <thread>
#include <functional>

#include <windows.h>

namespace Denix
{
    class Thread
    {
    public:
        Thread() = default;

        ~Thread()
        {
            if (IsJoinable())
            {
                Join();
            }

            DE_LOG(Log, Info, "Thread: {} destroyed", m_ThreadIDInt)
        }

        template <typename Func, typename... Args>
        explicit Thread(Func&& _func, Args&&... _args)
        {
            m_Thread = std::thread(std::forward<Func>(_func), std::forward<Args>(_args)...);
            m_ThreadID = m_Thread.get_id();
            SetThreadIDInt();
            
            DE_LOG(Log, Info, "Thread: {} created", m_ThreadIDInt)
        }

        /**
         * @brief Join the thread. Check if the thread is joinable before joining
         * Automatically called in the destructor
         */
        void Join()
        {
            if (m_Thread.joinable())
            {
                m_Thread.join();
                DE_LOG(Log, Info, "Thread {} joined", m_ThreadIDInt)
            }
            else
            {
                DE_LOG(Log, Error, "Thread {} not joinable", m_ThreadIDInt)
            }
        }

        bool IsJoinable() const
        {
            return m_Thread.joinable();
        }

        void Detach()
        {
            m_Thread.detach();
            DE_LOG(Log, Info, "Thread {} detached", m_ThreadIDInt)
        }
        
        void SetThreadIDInt()
        {
            std::stringstream ss;
            ss << m_Thread.get_id();
            m_ThreadIDInt = std::stoi(ss.str());
        }

    // private:
        std::thread m_Thread;
        std::thread::id m_ThreadID;
        int m_ThreadIDInt;
    };
}
