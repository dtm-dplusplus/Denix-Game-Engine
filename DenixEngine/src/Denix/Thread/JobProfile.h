#pragma once
#include "ThreadPrimitive.h"
#include "Denix/Core.h"
#include "Denix/Profile/Profile.h"


namespace Denix
{
    struct JobDeclaration;

    class JobProfile: public Profile
    {
    public:
        JobProfile(const ObjectInit& _objInit)
            : Profile(_objInit)
        {
        }

        void End() override;

        std::vector<Ref<JobDeclaration>> m_Jobs;
        int m_LastThreadIndex = 0;
        float m_ExecTime = 0.0f;
    };
}
