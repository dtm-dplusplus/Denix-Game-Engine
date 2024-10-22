 #include "Denix.h"
#include "DevScene.h"

using namespace Denix;

class DevProject final: public Engine
{
public:

	DevProject()
	{
		DE_LOG_CREATE(LogDevProject)

		m_ProjectName = "DevProject";
		ReflectionSubsystem::Register<DevScene>();
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