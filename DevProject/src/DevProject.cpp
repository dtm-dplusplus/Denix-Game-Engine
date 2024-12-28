 #include "Denix.h"
#include "Scene/AudioScene.h"

#include "Scene/CPGScene.h"
#include "Scene/DevScene.h"
#include "Scene/ThreadScene.h"

 //using namespace Denix;

template<class T>
	class Test: public std::enable_shared_from_this<T>
{
	public:
		Test()
		{
		}

	
		virtual ~Test() = default;

	virtual void Start()
	{
		m_Instance = this->shared_from_this();
		DE_LOG(LogDevProject, Info, "Test Start");
	}

	
	static Ref<T> GetInstance() {return m_Instance;}

private:
	inline static Ref<T> m_Instance;
};

class Test2: public Test<Test2>
{
public:
	void Start() override { DE_LOG(LogDevProject, Info, "Test2 Start"); }

	void Test2Func()
	{
		DE_LOG(LogDevProject, Info, "Test2Func");
	}
};


class DevProject final: public Engine
{
public:

	DevProject()
	{
		DE_LOG_CREATE(LogDevProject)

		m_ProjectName = "DevProject";
		ReflectionSubsystem::Register<DevScene>();
		ReflectionSubsystem::Register<CPGScene>();
		ReflectionSubsystem::Register<ThreadScene>();
		ReflectionSubsystem::Register<AudioScene>();
		ReflectionSubsystem::Register<CPGCube>();

		
	}

	void Initialize() override
	{
		Engine::Initialize();

		// Add any client code once the engine has been initialized
		TestClass = MakeRef<Test2>();
		TestClass->Start();
		Test2::GetInstance()->Test2Func();
	}

	Ref<Test2> TestClass;
};

URef<Engine> Denix::CreateEngine()
{
	return MakeURef<DevProject>();
}