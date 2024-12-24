 #include "Denix.h"
#include "Scene/AudioScene.h"

#include "Scene/CPGScene.h"
#include "Scene/DevScene.h"
#include "Scene/PhysicsScene.h"
#include "Scene/ThreadScene.h"

 //using namespace Denix;


class DevProject final: public Engine
{
public:

	DevProject()
	{
		DE_LOG_CREATE(LogDevProject)

		m_ProjectName = "DevProject";
		ReflectionSubsystem::Register<DevScene>();
		ReflectionSubsystem::Register<CPGScene>();
		ReflectionSubsystem::Register<PhysicsScene>();
		ReflectionSubsystem::Register<ThreadScene>();
		ReflectionSubsystem::Register<AudioScene>();
		ReflectionSubsystem::Register<CPGCube>();
	}

	void Initialize() override
	{
		Engine::Initialize();

		// Add any client code once the engine has been initialized
	}
};

URef<Engine> Denix::CreateEngine()
{
	return MakeURef<DevProject>();
}