#include "Denix.h"
#include "Scenes/PlaygroundScene.h"

#include <filesystem>
#include "Denix/System/ResourceSubsystem.h"

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

		std::string brick = std::filesystem::current_path().parent_path().string() + "\\Playground\\Content\\Textures\\brick.png";
		ResourceSubsystem::LoadTexture(brick, "brick");	

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

Denix::Engine* Denix::CreateEngine()
{
	return new Playground();
}