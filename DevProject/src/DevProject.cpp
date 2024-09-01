#include "Denix.h"

using namespace Denix;

#define YAML_CPP_STATIC_DEFINE
#include <fstream>

#include "yaml-cpp/yaml.h"

class DevProject: public Engine
{
public:

	DevProject()
	{
		DE_LOG_CREATE(LogDevProject)

		m_ProjectName = "DevProject";

		DE_LOG(LogDevProject, Trace, "DevProject Created")
	}

	~DevProject() override
	{
		DE_LOG(LogDevProject, Trace, "DevProject Destroyed")
	}

	void PreInitialize() override
	{
		Engine::PreInitialize();
	}

	void PostInitialize() override
	{
		Engine::PostInitialize();

		if (const auto scene = MakeRef<Scene>("Dev Scene"))
		{
			SceneSubsystem::LoadScene(scene);
			SceneSubsystem::OpenScene(scene->GetSceneName());
		}

		// Create node
		YAML::Node node;
		node["Class"] = "Denix";
		node["ID"] = "01FAD234";
		node["Name"] = "Denix Is Boss";

		// Save node to file
		std::ofstream fout("denix.yaml");
		fout << node;
		fout.close();

		// Load node from file
		YAML::Node loadedNode = YAML::LoadFile("denix.yaml");
		DE_LOG(LogDevProject, Info, loadedNode["Class"].as<std::string>());
		DE_LOG(LogDevProject, Info, loadedNode["ID"].as<std::string>());
		DE_LOG(LogDevProject, Info, loadedNode["Name"].as<std::string>());

	}
};

Engine* Denix::CreateEngine()
{
	return new DevProject();
}