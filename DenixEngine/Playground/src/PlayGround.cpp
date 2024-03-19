#include "Denix.h"
#include "Scenes/PlaygroundScene.h"

class Playground: public Denix::Engine
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
		if (Denix::SceneSubsystem* sceneSys = Denix::SceneSubsystem::Get())
		{
			if (const auto scene = 
				MakeRef<Denix::PlaygroundScene>(Denix::ObjectInitializer("Playground")))
			{
				sceneSys->LoadScene(scene);
				sceneSys->OpenScene("Playground");
			}
		}
		DE_LOG(LogEngine, Trace, "Playground RestartScene")
	}
};

Denix::Engine* Denix::CreateEngine()
{
	return new Playground();
}