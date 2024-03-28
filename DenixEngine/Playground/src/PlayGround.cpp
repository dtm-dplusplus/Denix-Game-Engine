#include "Denix.h"
#include "Scenes/PlaygroundScene.h"

using namespace Denix;

class Playground: public Engine
{
public:

	Playground()
	{
		DE_LOG_CREATE(LogPlayground)
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

		RestartScene();
	}

	void RestartScene() override
	{
		if (SceneSubsystem* sceneSys = SceneSubsystem::Get())
		{
			if (const auto scene = MakeRef<PlaygroundScene>())
			{
				sceneSys->LoadScene(scene);
				sceneSys->OpenScene(scene->GetName());
			}
		}
	}
};

Denix::Engine* Denix::CreateEngine()
{
	return new Playground();
}