 #include "Denix.h"
#include "DevScene.h"

using namespace Denix;

class DevProject: public Engine
{
public:

	DevProject()
	{
		DE_LOG_CREATE(LogDevProject)

		m_ProjectName = "DevProject";
		Factory::Instance().Register("DevScene", []() { return std::make_unique<DevScene>(); });

		DE_LOG(LogDevProject, Trace, "DevProject Created")
	}

	~DevProject() override
	{
		DE_LOG(LogDevProject, Trace, "DevProject Destroyed")
	}

	void PreInitialize() override
	{
		Engine::PreInitialize();
	}

	void PostInitialize() override
	{
		Engine::PostInitialize();

		// Add any client code once the engine has been initialized
	}
};

Engine* Denix::CreateEngine()
{
	return new DevProject();
}