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

		// Hack to serialize the dev scene  until reflection is implemented
		std::string contentPath = FileSubsystem::GetContentRoot();
		if(Ref<Scene> newScene = SceneSubsystem::DeserializeScene<DevScene>(MakeRef<Asset>(contentPath + "Scene\\DevScene.asset")))
		{
			SceneSubsystem::OpenScene(newScene);
		}
	}
};

Engine* Denix::CreateEngine()
{
	return new DevProject();
}