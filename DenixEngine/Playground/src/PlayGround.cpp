#include "Denix.h"
#include "Scenes/PhysicsScene.h"
#include "Scenes/GraphicsGame.h"

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

		ResourceSubsystem::LoadTexture(FileSubsystem::GetUserContentRoot() + R"(Textures\Start.png)", "GameStart");
		ResourceSubsystem::LoadTexture(FileSubsystem::GetUserContentRoot() + R"(Textures\End.png)", "GameEnd");
		ResourceSubsystem::LoadModel("SM_Pipe", FileSubsystem::GetUserContentRoot() + R"(Models\Pipe.obj)");
		ResourceSubsystem::LoadModel("SM_Bird", FileSubsystem::GetUserContentRoot() + R"(Models\Bird.obj)");
		Ref<Material> red = MakeRef<Material>(ObjectInitializer("MAT_Red"));
		red->GetBaseParam().Color = {1.0f,0.0f,0.0f};
		ResourceSubsystem::AddMaterial(red);

		RestartScene();
	}

	void RestartScene() override
	{
		if (SceneSubsystem* sceneSys = SceneSubsystem::Get())
		{
			if (const auto scene = MakeRef<GraphicsGame>())
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