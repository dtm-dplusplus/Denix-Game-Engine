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
            JoinCheck();

            DE_LOG(Log, Info, "Thread: {} destroyed", m_ThreadIDInt)
        }

        template <typename Func, typename... Args>
        explicit Thread(Func&& _func, Args&&... _args)
        {
            m_Thread = std::thread(std::forward<Func>(_func), std::forward<Args>(_args)...);
            m_ThreadID = m_Thread.get_id();
            SetThreadIDInt();
            m_IsWorking = true;
            DE_LOG(Log, Info, "Thread: {} created", m_ThreadIDInt)
        }

        /**
         * @brief Join the thread. 
         */
        void Join()
        {
            if (m_Thread.joinable())
            {
                m_Thread.join();
                m_IsWorking = false;
                DE_LOG(Log, Info, "Thread {} joined", m_ThreadIDInt)
            }
        }

        /**
         * @brief Join the thread. Check if the thread is joinable before joining
         * Automatically called in the destructor
         */
        void JoinCheck()
        {
            if (m_Thread.joinable())
            {
                m_Thread.join();
                m_IsWorking = false;
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

        /**
         * @brief Detach the thread from the main thread.
         *  Allowis the thread to continue execution independently.
         *  Once detached, the thread becomes non-joinable, meaning you cannot wait for it to finish using join.
         *  The detached thread will run until it completes, and its resources will be released automatically.
         */
        void Detach()
        {
            if (m_Thread.joinable())
            {
                m_Thread.detach();
                DE_LOG(Log, Info, "Thread {} detached", m_ThreadIDInt)
            }
            else
            {
                DE_LOG(Log, Error, "Thread {} failed to detach. Not joinable", m_ThreadIDInt)
            }
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
        size_t m_ThreadIDInt;

        bool m_IsWorking;
        bool m_StopFlag = false;
    };
}
