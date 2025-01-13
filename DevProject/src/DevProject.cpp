 #include "Denix.h"

#include "Scene/AudioScene.h"
#include "Scene/GACP/GACPScene.h"
#include "Scene/DevScene.h"
#include "Scene/GACP/GACPActor.h"
#include "Scene/GEP/GEPScene.h"

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
		ReflectionSubsystem::Register<GEPScene>();
		ReflectionSubsystem::Register<AudioScene>();
		ReflectionSubsystem::Register<GACPActor>();
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