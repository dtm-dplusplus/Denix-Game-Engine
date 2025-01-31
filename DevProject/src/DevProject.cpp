 #include "Denix.h"

#include "Game/Scene/DevScene.h"
#include "Game/Scene/GEPScene.h"
#include "Game/Actor/BallActor.h"

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
		ReflectionSubsystem::Register<GEPScene>();
		//ReflectionSubsystem::Register<BallActor>();
		//ReflectionSubsystem::Register<TargetActor>();
		//ReflectionSubsystem::Register<Character>();

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