 #include "Denix.h"
#include "Scene/AudioScene.h"

#include "Scene/CPGScene.h"
#include "Scene/DevScene.h"
#include "Scene/ThreadScene.h"

 //using namespace Denix;
class DevProject final: public Engine
{
public:

	DevProject(const std::string& _projectName): Engine(_projectName){}

	void PreInitialize() override
	{
		Engine::PreInitialize();

		DE_LOG_CREATE(LogDevProject)
		ReflectionSubsystem::Register<DevScene>();
		ReflectionSubsystem::Register<CPGScene>();
		ReflectionSubsystem::Register<ThreadScene>();
		ReflectionSubsystem::Register<AudioScene>();
		ReflectionSubsystem::Register<CPGActor>();
	}
	
	void Initialize() override
	{
		Engine::Initialize();
	}
};

 Ref<Engine> Denix::MakeEngine()
{
	return MakeRef<DevProject>(ReflectionHelper::GetDEClassName<DevProject>());
}