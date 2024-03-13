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

	void PostInitialize() override
	{
		DE_LOG(LogEngine, Trace, "Playground Post-Initialized")

		using namespace Denix;
		
		if (SceneSubSystem* sceneSys = SceneSubSystem::Get())
		{
			if (auto scene = MakeRef<PlaygroundScene>(ObjectInitializer("Playground")))
			{
				sceneSys->LoadScene(scene);
				sceneSys->OpenScene("Playground");
			}
		}
	}
};

Denix::Engine* Denix::CreateEngine()
{
	return new Playground();
}