
#include "DevScene.h"

#include "imgui.h"
#include "misc/cpp/imgui_stdlib.h"
#include "Denix/Engine.h"
#include "Denix/Resource/ResourceSubsystem.h"
#include "Denix/Scene/SceneSubsystem.h"
#include "yaml-cpp/yaml.h"
#include "Denix/Editor/EditorSubsystem.h"

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
		static std::string matPath = FileSubsystem::GetEngineContentRoot() + "Material\\MAT_Default.asset";
		if(ImGui::Button("Serialize Material"))
		{
			Ref<Material> mat = m_SceneObjects[0]->GetRenderComponent()->GetMaterial();
			m_SceneObjects[0]->GetRenderComponent()->GetMaterial()->SetAsset(MakeRef<Asset>(matPath));
			YAML::Emitter out;

			out << YAML::BeginMap;
			out << YAML::Comment("DE_ASSET: Material");
			mat->Serialize(out);
			out << YAML::EndMap;

			FileSubsystem::WriteFile(matPath, out.c_str());
			DE_LOG(LogScene, Info, "Serialized Material");
		}
		if (ImGui::Button("Deserialize Material"))
		{
			Ref<Asset> asset = MakeRef<Asset>(matPath);
		
			Ref<Material> mat = MakeRef<Material>(asset);
			m_SceneObjects[0]->GetRenderComponent()->SetMaterial(mat);
			DE_LOG(LogScene, Info, "Deserialized Material");
		}
		if (ImGui::CollapsingHeader("Engine Config"), ImGuiTreeNodeFlags_DefaultOpen)
		{
			ImGui::Text("Project Name: %s", FileSubsystem::GetProjectName().c_str());
			ImGui::Text("Project Root: %s", FileSubsystem::GetProjectRoot().c_str());
			ImGui::Text("User Content Root: %s", FileSubsystem::GetContentRoot().c_str());
			ImGui::Checkbox("Show engine content", &ShowEngineContent);
		
			if (ImGui::Button("Save Config"))
			{
				Engine::Get().SaveConfig();

			}
			if (ImGui::Button("Load Config"))
			{
				Engine::Get().LoadConfig();
			}
		}

		if (ImGui::CollapsingHeader("Materials", ImGuiTreeNodeFlags_DefaultOpen))
		{
			for (const auto& mat : ResourceSubsystem::GetMaterialStore())
			{
				ImGui::TreeNode(mat.second->GetAsset()->GetAssetName().c_str());
			}
		}

		if (ImGui::CollapsingHeader("Scenes", ImGuiTreeNodeFlags_DefaultOpen))
		{
			for (const auto& scene : ResourceSubsystem::GetSceneStore())
			{
				ImGui::TreeNode(scene->GetAssetName().c_str());
				ImGui::Text("Asset Path: %s", scene->GetAssetPath().c_str());
			}
		}
	
		if (ImGui::CollapsingHeader("Assets", ImGuiTreeNodeFlags_DefaultOpen))
		{
			// Stupidly slow way to display assets
			for (const auto& asset : ResourceSubsystem::GetAssetStore())
			{
				if(!ShowEngineContent &&
					asset->GetAssetDirectory().find(FileSubsystem::GetEngineContentRoot()) != std::string::npos)
				{
					continue;
				}
			
				ImGui::TreeNode(asset->GetAssetName().c_str());
				ImGui::Text("Asset Name: %s", asset->GetAssetName().c_str());
				ImGui::Text("Asset Path: %s", asset->GetAssetPath().c_str());
				ImGui::Text("Asset Directory: %s", asset->GetAssetDirectory().c_str());
				ImGui::Text("Asset Extension: %s", asset->GetAssetExtension().c_str());
			}
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
