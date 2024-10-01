
#include "DevScene.h"

#include "imgui.h"
#include "Denix/Engine.h"
#include "Denix/Scene/SceneSubsystem.h"

using namespace Denix;

DevScene::DevScene(): Scene("Dev Scene")
{
	// Scrape all assets from content folder
	std::string contentPath = FileSubsystem::GetUserContentRoot();
	m_SceneAsset = MakeRef<Asset>(contentPath + "Scene\\DevScene.asset");
	for (const auto& entry : std::filesystem::recursive_directory_iterator(contentPath))
	{
		if (entry.is_regular_file())
		{
			std::string filePath = entry.path().string();
			Ref<Asset> asset = MakeRef<Asset>(filePath);
			m_Assets.push_back(asset);
		}
	}

	
}

void DevScene::Update(float _deltaTime)
{
	Scene::Update(_deltaTime);

	ImGui::Begin("Dev Scene");
	
	if (ImGui::CollapsingHeader("Engine Config"), ImGuiTreeNodeFlags_DefaultOpen)
	{
		ImGui::Text("Project Name: %s", FileSubsystem::GetProjectName().c_str());
		ImGui::Text("Engine Root: %s", FileSubsystem::GetEngineRoot().c_str());
		ImGui::Text("Project Root: %s", FileSubsystem::GetProjectRoot().c_str());
		ImGui::Text("User Content Root: %s", FileSubsystem::GetUserContentRoot().c_str());
		/*if(Engine::Get().m_EngineStartupScene)
		{
			ImGui::Text("Startup Scene: %s", Engine::Get().m_EngineStartupScene->GetAssetPath().c_str());
		}
		ImGui::Checkbox("Serialize Dev Scene", &Engine::Get().m_SerializeDevScene);*/

		/*if(ImGui::Button("Reload Scene"))
		{
			if (Engine::Get().m_SerializeDevScene)
			{
				if (const auto scene = MakeRef<DevScene>())
				{
					SceneSubsystem::LoadScene(scene);
					SceneSubsystem::OpenScene(scene->GetSceneName());
				}
			}
			else
			{
				// Load the scene from the asset
				
			}
		}*/
		if (ImGui::Button("Save Config"))
		{
			Engine::Get().SaveConfig();
		}
		if (ImGui::Button("Load Config"))
		{
			Engine::Get().LoadConfig();
		}
	}
	if (ImGui::CollapsingHeader("Assets", ImGuiTreeNodeFlags_DefaultOpen))
	{
		for (auto& asset : m_Assets)
		{
			ImGui::CollapsingHeader(asset->GetAssetName().c_str());
			ImGui::Text("Asset Name: %s", asset->GetAssetName().c_str());
			ImGui::Text("Asset Path: %s", asset->GetAssetPath().c_str());
			ImGui::Text("Asset Directory: %s", asset->GetAssetDirectory().c_str());
			ImGui::Text("Asset Extension: %s", asset->GetAssetExtension().c_str());
		}
	}
	if(ImGui::Button("Save Scene"))
	{
		YAML::Emitter Emitter;
		Serialize(Emitter);
	}

	if (ImGui::Button("Load Scene"))
	{
		YAML::Node sceneNode = YAML::LoadFile(m_SceneAsset->GetAssetPath());
		Deserialize(sceneNode);
	}

	
	ImGui::End();
}

bool DevScene::Load()
{
	//YAML::Node sceneNode = YAML::LoadFile("DevScene.yaml");
	//Deserialize(sceneNode);
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

	// Write to file
	std::string contentPath = FileSubsystem::GetUserContentRoot();
	FileSubsystem::WriteFile(m_SceneAsset->GetAssetPath(), _out.c_str());
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