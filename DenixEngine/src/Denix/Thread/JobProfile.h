#pragma once
#include "ThreadPrimitive.h"
#include "Denix/Core.h"
#include "Denix/Profile/Profile.h"


namespace Denix
{
    struct JobDeclaration;

    struct JobBuffer
    {
        std::vector<Ref<JobDeclaration>> JobResults;

        JobBuffer()
        {
            // Reserve to reduce reallocations during profiling
            JobResults.reserve(5000);
        }

        /**
         * 
         */
        void SaveJobResult(const Ref<JobDeclaration>& _job) { JobResults.push_back(_job); }
    };
    
    class JobProfile: public Profile
    {
    public:
        JobProfile() = default;
        
        JobProfile(const ObjectInit& _objInit)
            : Profile(_objInit)
        {
        }

        void End() override;

        JobBuffer m_JobBuffer;
    };
}