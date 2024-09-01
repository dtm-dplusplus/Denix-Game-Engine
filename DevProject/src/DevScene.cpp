
#include "DevScene.h"

#include <fstream>

#include "imgui.h"

namespace YAML {
	template<>
	struct convert<glm::vec3> {
		static Node encode(const glm::vec3& rhs) {
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			return node;
		}

		static bool decode(const Node& node, glm::vec3& rhs) {
			if(!node.IsSequence() || node.size() != 3) {
				return false;
			}

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			rhs.z = node[2].as<float>();
			return true;
		}
	};
}

using namespace Denix;

DevScene::DevScene(): Scene("Dev Scene")
{
	Emitter.SetBoolFormat(YAML::Literal);
}

void DevScene::Update(float _deltaTime)
{
    Scene::Update(_deltaTime);

    if(ImGui::Button("Save Scene"))
    {
    	Serialize(Emitter);
    	DE_LOG(LogDevProject, Info, "Scene Saved")
    }

}

bool DevScene::Load()
{
    Scene::Load();


	// Load node from file
	YamlObj= MakeRef<Cube>();
	Deserialize(YAML::LoadFile("object.yaml"));
	SpawnSceneObject(YamlObj);
	return true;
}

void DevScene::Unload()
{
	Scene::Unload();

	Serialize(Emitter);
}

 void DevScene::Serialize(YAML::Emitter& _out)
 {
 	// Render Component
 	Node["m_IsVisible"] = YamlObj->GetRenderComponent()->IsVisible();
 	Node["m_AffectsLighting"] = YamlObj->GetRenderComponent()->AffectsLighting();

 	// Transform Component
 	Node["m_Position"] = YamlObj->GetTransformComponent()->GetPosition();
 	Node["m_Rotation"] = YamlObj->GetTransformComponent()->GetRotation();
 	Node["m_Scale"] = YamlObj->GetTransformComponent()->GetScale();
 	Node["m_Moveability"] = static_cast<int>(YamlObj->GetTransformComponent()->GetMoveability());

 	// Save node to emitter
 	_out << Node;

 	// Open file stream
 	std::ofstream fout("object.yaml");

 	// Write emitter content to file
 	fout << Emitter.c_str();

 	// Close file stream
 	fout.close();
 }

 void DevScene::Deserialize(const YAML::Node& _in)
 {
 	// Update Game Object
 	YamlObj->GetRenderComponent()->SetIsVisible(_in["m_IsVisible"].as<bool>());
 	YamlObj->GetRenderComponent()->SetAffectsLighting(_in["m_AffectsLighting"].as<bool>());
 	YamlObj->GetTransformComponent()->SetPosition(_in["m_Position"].as<glm::vec3>());
 	YamlObj->GetTransformComponent()->SetRotation(_in["m_Rotation"].as<glm::vec3>());
 	YamlObj->GetTransformComponent()->SetScale(_in["m_Scale"].as<glm::vec3>());
 	YamlObj->GetTransformComponent()->SetMoveability(static_cast<Moveability>(_in["m_Moveability"].as<int>()));
 }
