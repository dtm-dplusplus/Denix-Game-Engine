#include "Denix/Core.h"
#include <thread>
#include <functional>
#include <sstream>

#include <windows.h>

namespace Denix
{
    class Thread
    {
    public:
        Thread() = default;
        ~Thread()
        {
            if (m_Thread.joinable())
            {
                m_Thread.join();
                DE_LOG(Log, Info, "Thread joined");
            }
            else
            {
                DE_LOG(Log, Error, "Thread not joinable");
            }
        }

        template<typename CallFunc, typename... Args>
            Thread(CallFunc&& _func, Args&&... _args)
            {
                DE_LOG(Log, Info, "Thread created with static function");
                m_Thread = std::thread(_func, _args...);
                m_ThreadID = std::this_thread::get_id();
            }

        template<typename T>
            Thread(void(T::*_memberFunc)(), T* _instance)
            {
                m_Thread = std::thread(std::bind(_memberFunc, _instance));
                m_ThreadID = m_Thread.get_id();        

                DE_LOG(Log, Info, "Thread created with member function. Thread ID: {}", m_ThreadID);
            std::cout << m_Thread.get_id();
            }

        void Join()
        {
            if (m_Thread.joinable())
            {
                m_Thread.join();
            }
        }

        bool IsJoinable() const
        {
            return m_Thread.joinable();
        }

        unsigned long GetThreadID() const
        {
            return m_ThreadID;
        }

    private:
        std::thread m_Thread;
       unsigned long  m_ThreadID;
    };
}