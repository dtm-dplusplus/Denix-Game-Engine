#include "Denix.h"
#include "Scenes/PhysicsScene.h"

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
		ResourceSubsystem::LoadMaterial(red);

		RestartScene();
	}

	void RestartScene() override
	{
		if (SceneSubsystem* sceneSys = SceneSubsystem::Get())
		{
			if (const auto scene = MakeRef<PhysicsScene>())
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