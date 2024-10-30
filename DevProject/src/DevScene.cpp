
#include "DevScene.h"

#include "imgui.h"
#include "misc/cpp/imgui_stdlib.h"
#include "Denix/Engine.h"
#include "Denix/Resource/ResourceSubsystem.h"
#include "Denix/Scene/SceneSubsystem.h"
#include "yaml-cpp/yaml.h"
#include "Denix/Editor/EditorSubsystem.h"
#include "Denix/Reflection/ReflectionSubsystem.h"

using namespace Denix;


DevScene::DevScene(): Scene("Dev Scene")
{
	// Scrape all assets from content folder
	// This should be done on a timer to check for new assets
	ShowEngineContent = false;
	std::string contentPath = FileSubsystem::GetContentRoot();
	m_SceneAsset = MakeRef<Asset>(contentPath + "Scene\\DevScene.asset");
}

DevScene::DevScene(const Ref<Asset>& _sceneAsset): Scene(_sceneAsset)
{
	// Scrape all assets from content folder
	// This should be done on a timer to check for new assets
	ShowEngineContent = false;
	std::string contentPath = FileSubsystem::GetContentRoot();
	m_SceneAsset = MakeRef<Asset>(contentPath + "Scene\\DevScene.asset");

	
}

void DevScene::Update(float _deltaTime)
{
	Scene::Update(_deltaTime);

	if(ImGui::Begin("Dev Scene"))
	{
		ImGui::SeparatorText("Reflection");
		for (const auto& key : ReflectionSubsystem::GetCreateFuncs() | std::views::keys)
		{
			ImGui::Text(key.c_str());
		}
		
		if (ImGui::CollapsingHeader("Engine Config"), ImGuiTreeNodeFlags_DefaultOpen)
		{
			ImGui::Text("Project Name: %s", FileSubsystem::GetProjectName().c_str());
			ImGui::Text("Project Root: %s", FileSubsystem::GetProjectRoot().c_str());
			ImGui::Text("User Content Root: %s", FileSubsystem::GetContentRoot().c_str());
			ImGui::Checkbox("Show engine content", &ShowEngineContent);
		}

		ImGui::SeparatorText("Assets");
		if (ImGui::TreeNode("Materials"))
		{
			for (const auto& mat : ResourceSubsystem::GetMaterialStore())
			{
				ImGui::Text(mat.second->GetAsset()->GetAssetName().c_str());
			}
			ImGui::TreePop();
		}

		if (ImGui::TreeNode("Scenes"))
		{
			for (const auto& scene : ResourceSubsystem::GetSceneStore())
			{
				ImGui::Text(scene->GetAssetName().c_str());
				ImGui::Text("Asset Path: %s", scene->GetAssetPath().c_str());
			}
			ImGui::TreePop();
		}
	
		if(ImGui::Button("Save Scene"))
		{
			SceneSubsystem::SerializeScene(this);

			for(const auto& mat: ResourceSubsystem::GetMaterialStore())
			{
				// Save Changes to asset - This should be done in the editor
				YAML::Emitter matAsssetEmitter;
				matAsssetEmitter << YAML::Comment("DE_ASSET: Material");
				matAsssetEmitter << YAML::BeginMap;
				mat.second->Serialize(matAsssetEmitter);
				matAsssetEmitter << YAML::EndMap;
                
				FileSubsystem::WriteFile(mat.second->GetAsset()->GetAssetPath(), matAsssetEmitter.c_str());
				DE_LOG(LogScene, Info, "Serialized Material");
			}
		}

		if (ImGui::Button("Load Scene"))
		{
			if(Ref<Scene> newScene = SceneSubsystem::DeserializeScene<DevScene>(m_SceneAsset))
			{
				SceneSubsystem::OpenScene(newScene);
			}
		}
	
		ImGui::End();
	}
}

bool DevScene::Load()
{
	return Scene::Load();
}

void DevScene::Unload()
{
	Scene::Unload();
}
