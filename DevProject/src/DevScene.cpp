
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

	if(ImGui::Button("Save Scene"))
	{
		YAML::Emitter Emitter;
		Serialize(Emitter);
	}
}

bool DevScene::Load()
{
	YAML::Node sceneNode = YAML::LoadFile("DevScene.yaml");
	Deserialize(sceneNode);
	return Scene::Load();
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
	_out << YAML::Key << "m_SceneObjects" << YAML::BeginSeq;

	for(auto& gameObject : m_SceneObjects)
	{
		_out << YAML::BeginMap;
		gameObject->Serialize(_out);
		_out << YAML::EndMap;
	}

	_out << YAML::EndMap;

	// Open file stream
	std::ofstream fout("DevScene.yaml");

	//// Write emitter content to file
	fout << _out.c_str();

	//// Close file stream
	fout.close();
	DE_LOG(LogDevProject, Info, "Serialize")
 }

 void DevScene::Deserialize(YAML::Node& _in)
 {
	DE_LOG(LogDevProject, Info, "Deserialize")
	YAML::Node sequenceNode = _in["m_SceneObjects"];

	m_SceneObjects.clear();
	for (YAML::const_iterator it = sequenceNode.begin(); it != sequenceNode.end(); ++it)
	{
		Ref<GameObject> gameObj = MakeRef<GameObject>();
		gameObj->Deserialize(*it);
		m_SceneObjects.push_back(gameObj);
	}
 }
