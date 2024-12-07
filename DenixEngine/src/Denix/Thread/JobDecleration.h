#pragma once
#include "ThreadPrimitive.h"
#include "Denix/Profile/Profile.h"

namespace Denix
{
    struct JobDeclaration
    {
        JobDeclaration(const std::string& _name, const Priority _priority, Ref<Counter> _waitCounter, const Ref<class JobProfile>& _profile,
                       std::function<void()> _entryPoint);

        size_t m_ID;

        // size_t m_FrameID;

        /**
         * @brief Job name. Used for debugging purposes
         */
        std::string m_Name;

        /**
         * @brief Jobs entry point function
         */
        std::function<void()> m_EntryPoint;

        /**
         * @brief Job priority
         */
        Denix::Priority m_Priority;

        /**
         * @brief Counter to keep track of how many jobs we are waiting on before a job group is finished.
         * Should probably be moved to some kind of job buidler which sets up dependencies
         */
        Denix::Ref<Denix::Counter> m_WaitCounter;

        /**
         * @brief Timer to keep track of how long the job takes to execute
        */
        Denix::Ref<Denix::Profile> m_JobProfile;

        int m_ThreadIndex;
        
        inline static size_t s_IDCounter = 0;
    };

}
