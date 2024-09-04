
#include "DevScene.h"

#include <fstream>

#include "imgui.h"

using namespace Denix;

DevScene::DevScene(): Scene("Dev Scene")
{
}

void DevScene::Update(float _deltaTime)
{
    Scene::Update(_deltaTime);

    if(ImGui::Button("Serialize"))
    {
		YAML::Emitter Emitter;
    	Serialize(Emitter);
    	
    	// Open file stream
		std::ofstream fout("DevScene.yaml");

		// Write emitter content to file
		fout << Emitter.c_str();

		// Close file stream
		fout.close();
    	DE_LOG(LogDevProject, Info, "Serialize")
    	m_SceneObjects.clear();
    }

	if(ImGui::Button("Deserialize"))
	{
		YAML::Node n = YAML::LoadFile("DevScene.yaml");
		//DE_LOG(LogDevProject, Info, n["m_SceneName"].as<std::string>())
		DE_LOG(LogDevProject, Info, "Deserialize")
	}
}

bool DevScene::Load()
{
    Scene::Load();


	// Load node from file
	YamlObj= MakeRef<Cube>();
	SpawnSceneObject(YamlObj);
	return true;
}

void DevScene::Unload()
{
	Scene::Unload();

	//Serialize(Emitter);
}

 void DevScene::Serialize(YAML::Emitter& _out)
 {
	_out << YAML::Comment("DE_ASSET: Scene");
	_out << YAML::Newline << YAML::Comment( m_SceneName + " Scene Data");
	_out << YAML::BeginMap;
	_out << YAML::Key << "m_SceneName" << YAML::Value << m_SceneName;
	_out << YAML::Key << "m_Gravity" << YAML::Value << m_Gravity;
	_out << YAML::BeginSeq;

	_out << YAML::Comment("Scene Objects");
	for (const auto& gameObject : m_SceneObjects)
	{
		_out << YAML::BeginMap;
		gameObject->Serialize(_out);
		_out << YAML::EndMap;
	}

	_out << YAML::EndSeq;
	_out << YAML::EndMap;
 }

 void DevScene::Deserialize(YAML::Node& _in)
 {
	
 }
