
#include "DevScene.h"

#include "imgui.h"
#include "misc/cpp/imgui_stdlib.h"
#include "Denix/Engine.h"
#include "Denix/Resource/ResourceSubsystem.h"
#include "Denix/Scene/SceneSubsystem.h"
#include "yaml-cpp/yaml.h"

using namespace Denix;

void ShaderEditor::Update()
{
	ImGui::Begin("Shader Editor", &IsOpen);
	ImGui::BeginTabBar("Shader Editor Tabs");
	if (EditShader)
	{
		for (auto& shaderSource : EditShader->GetShaderSources())
		{
			if (ImGui::BeginTabItem(shaderSource.FileName.c_str()))
			{
				//static ImGuiChildFlags cflags = ImGuiChildFlags_AlwaysAutoResize | ImGuiChildFlags_AutoResizeX | ImGuiChildFlags_AutoResizeY;
				//
				static ImGuiInputTextFlags flags = ImGuiInputTextFlags_AllowTabInput |
					ImGuiInputTextFlags_CtrlEnterForNewLine ;
				//ImGui::SameLine();
				if(ImGui::Button("Recompile Shader"))
				{
					ResourceSubsystem::ReloadShader(EditShader);
				}
				ImGui::BeginChild(shaderSource.FileName.c_str());
				ImGui::InputTextMultiline("##source", &shaderSource.Source, ImGui::GetWindowSize(), flags);
				ImGui::EndChild();
				ImGui::EndTabItem();
			}
		}
	}
	ImGui::EndTabBar();
	ImGui::End();
}

DevScene::DevScene(): Scene("Dev Scene")
{
	// Scrape all assets from content folder
	// This should be done on a timer to check for new assets
	ShowEngineContent = false;
	std::string contentPath = FileSubsystem::GetContentRoot();
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

	m_ShaderEditor = MakeRef<ShaderEditor>(ResourceSubsystem::GetShader("DefaultShader"));
}

void DevScene::Update(float _deltaTime)
{
	Scene::Update(_deltaTime);

	ImGui::Begin("Dev Scene");
	
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
	if (ImGui::CollapsingHeader("Assets", ImGuiTreeNodeFlags_DefaultOpen))
	{
		// Stupidly slow way to display assets
		for (auto& asset : m_Assets)
		{
			if(!ShowEngineContent &&
				asset->GetAssetDirectory().find(FileSubsystem::GetEngineContentRoot()) != std::string::npos)
			{
				continue;
			}
			
			ImGui::CollapsingHeader(asset->GetAssetName().c_str());
			ImGui::Text("Asset Name: %s", asset->GetAssetName().c_str());
			ImGui::Text("Asset Path: %s", asset->GetAssetPath().c_str());
			ImGui::Text("Asset Directory: %s", asset->GetAssetDirectory().c_str());
			ImGui::Text("Asset Extension: %s", asset->GetAssetExtension().c_str());
		}
	}
	
	if(ImGui::Button("Save Scene"))
	{
		SceneSubsystem::SerializeScene(this);
	}

	if (ImGui::Button("Load Scene"))
	{
		if(Ref<Scene> newScene = SceneSubsystem::DeserializeScene(m_SceneAsset))
		{
			SceneSubsystem::OpenScene(newScene);
		}
	}

	if (ImGui::CollapsingHeader("Shader Editor", ImGuiTreeNodeFlags_DefaultOpen))
	{
		m_ShaderEditor->Update();
	}
	
	ImGui::End();
}

bool DevScene::Load()
{
	return Scene::Load();
}

void DevScene::Unload()
{
	Scene::Unload();
}
