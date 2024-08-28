#include "Denix.h"

using namespace Denix;

class DevProject: public Engine
{
public:

	DevProject()
	{
		DE_LOG_CREATE(LogDevProject)

		m_ProjectName = "DevProject";

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

		if (const auto scene = MakeRef<Scene>("Dev Scene"))
		{
			SceneSubsystem::LoadScene(scene);
			SceneSubsystem::OpenScene(scene->GetSceneName());
		}
	}
};

Engine* Denix::CreateEngine()
{
	return new DevProject();
}