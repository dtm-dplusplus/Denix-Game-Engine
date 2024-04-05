#include "Denix.h"
#include "Scenes/PlaygroundScene.h"

using namespace Denix;

class Playground: public Engine
{
public:

	Playground()
	{
		DE_LOG_CREATE(LogPlayground)

		m_ProjectName = "Playground";

		DE_LOG(LogPlayground, Trace, "Playground Created")
	}

	~Playground() override
	{
		DE_LOG(LogPlayground, Trace, "Playground Destroyed")
	}

	void PreInitialize() override
	{
		Engine::PreInitialize();
	}

	void PostInitialize() override
	{
		Engine::PostInitialize();

		ResourceSubsystem::LoadTexture(FileSubsystem::GetProjectRoot() + R"(Content\Textures\brick.png)", "brick");

		RestartScene();
	}

	void RestartScene() override
	{
		if (SceneSubsystem* sceneSys = SceneSubsystem::Get())
		{
			if (const auto scene = MakeRef<PlaygroundScene>())
			{
				sceneSys->LoadScene(scene);
				sceneSys->OpenScene(scene->GetSceneName());
			}
		}
	}
};

Engine* Denix::CreateEngine()
{
	return new Playground();
}