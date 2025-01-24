 #include "Denix.h"

#include "Scene/AudioScene.h"
#include "Scene/GACP/GACPScene.h"
#include "Scene/DevScene.h"
#include "Scene/UIScene.h"
#include "Scene/GACP/GACPActor.h"
#include "Scene/GEP/GEPScene.h"

 class DevProject final: public Engine
{
public:

	DevProject() = default;
	~DevProject() override = default;
 	
	void PreInitialize() override
	{
		Engine::PreInitialize();

		// Register project specific classes - These are pre-registered for reflection
		DE_LOG_CREATE(LogDevProject)
		ReflectionSubsystem::Register<DevScene>();
		ReflectionSubsystem::Register<GACPScene>();
		ReflectionSubsystem::Register<UIScene>();
		ReflectionSubsystem::Register<GEPScene>();
		ReflectionSubsystem::Register<AudioScene>();
		ReflectionSubsystem::Register<GACPActor>();
		ReflectionSubsystem::Register<Button>();
		DE_LOG(LogReflection, Info, "Registered DevProject Classes")
	}
	
	void Initialize() override
	{
		Engine::Initialize();

		// Do any project specific initialization here
		DE_LOG(LogDevProject, Info, "DevProject Initialized")
	}
};

 Ref<Engine> Denix::MakeEngine()
{
	return MakeRef<DevProject>();
}