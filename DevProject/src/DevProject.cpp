 #include "Denix.h"
#include "Game/Canvas.h"

#include "Game/Scene/DevScene.h"
#include "Game/Scene/GEPScene.h"
#include "Game/Scene/AudioScene.h"
#include "Game/Scene/UIScene.h"
#include "Game/Actor/Character.h"
#include "Game/Actor/BallActor.h"
#include "Game/Actor/TargetActor.h"
#include "Game/Scene/MainMenu.h"

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
		ReflectionSubsystem::Register<UIScene>();
		ReflectionSubsystem::Register<GEPScene>();
		ReflectionSubsystem::Register<AudioScene>();
		ReflectionSubsystem::Register<MainMenu>();
		//ReflectionSubsystem::Register<BallActor>();
		//ReflectionSubsystem::Register<TargetActor>();
		//ReflectionSubsystem::Register<Character>();

		//ReflectionSubsystem::Register<Canvas>();
		//ReflectionSubsystem::Register<Button>();
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