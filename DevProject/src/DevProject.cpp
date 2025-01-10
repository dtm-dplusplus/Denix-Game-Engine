 #include "Denix.h"

#include "Scene/AudioScene.h"
#include "Scene/GACPScene.h"
#include "Scene/DevScene.h"
#include "Scene/CPG/CPGActor.h"

class DevProject final: public Engine
{
public:

	DevProject(const std::string& _projectName): Engine(_projectName){}

	void PreInitialize() override
	{
		Engine::PreInitialize();

		DE_LOG_CREATE(LogDevProject)
		ReflectionSubsystem::Register<DevScene>();
		ReflectionSubsystem::Register<GACPScene>();
		ReflectionSubsystem::Register<GACPScene>();
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
	return MakeRef<DevProject>(ReflectionHelper::GetClassNameDE<DevProject>());
}