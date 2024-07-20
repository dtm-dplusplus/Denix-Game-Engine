#include "Engine.h"


class DevProject: public Denix::Engine
{
public:

    DevProject()
    {
        // DE_LOG_CREATE(LogPlayground)

        // m_ProjectName = "Playground";

        // DE_LOG(LogPlayground, Trace, "Playground Created")
    }

    ~DevProject() override
    {
        // DE_LOG(LogPlayground, Trace, "Playground Destroyed")
    }
};

Denix::Engine* Denix::CreateEngine()
{
    return new DevProject();
}