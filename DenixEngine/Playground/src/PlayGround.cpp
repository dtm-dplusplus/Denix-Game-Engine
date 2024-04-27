#include "Denix.h"
#include "Scenes/PlaygroundScene.h"
#include "Scenes/PlatformerScene.h"

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

		Ref<Material> red = MakeRef<Material>("MAT_Red");
		red->SetBaseColor({ 1.0f,0.0f,0.0f });

		Ref<Material> green = MakeRef<Material>("MAT_Green");
		red->SetBaseColor({ 0.0f,1.0f,0.0f });

		Ref<Material> blue = MakeRef<Material>("MAT_Blue");
		red->SetBaseColor({ 0.0f,0.0f,1.0f });

		ResourceSubsystem::LoadMaterial(red);
		ResourceSubsystem::LoadMaterial(green);
		ResourceSubsystem::LoadMaterial(blue);

		RestartScene();
	}

	void RestartScene() override
	{
		if (SceneSubsystem* sceneSys = SceneSubsystem::Get())
		{
			if (const auto scene = MakeRef<PlatformerScene>())
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