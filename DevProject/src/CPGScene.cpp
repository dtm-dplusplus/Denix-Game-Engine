#include "CPGScene.h"

#include "imgui.h"
#include "CPG/Ray.h"
#include "misc/cpp/imgui_stdlib.h"
#include "Denix/Engine.h"
#include "Denix/Resource/ResourceSubsystem.h"
#include "Denix/Scene/SceneSubsystem.h"
#include "yaml-cpp/yaml.h"
#include "Denix/Reflection/ReflectionSubsystem.h"
#include "Denix/Profile/ProfileSubsystem.h"
#include "Denix/Core/Timer.h"
#include "Denix/Core/TimerSubsystem.h"

using namespace Denix;


CPGScene::CPGScene(const Ref<Asset>& _sceneAsset): Scene(_sceneAsset)
{
	
}

void CPGScene::Update(float _deltaTime)
{
	Scene::Update(_deltaTime);

	// This should be done on a timer to check for new assets
	ShowEngineContent = false;
	
	if(ImGui::Begin(m_SceneName.c_str()))
	{
		 if(ImGui::Button("Spawn Cube"))
		 {
		 	for (int i = 0; i < 25; ++i)
		 	{
		 		for (int j = 0; j < 25; ++j)
		 		{
		 			// Calculate the position for each cube
		 			glm::vec3 position(i * 2.5, j * 2.5f, 0.0f);
		 			
		 			// Spawn the cube at the calculated position
		 			SpawnGameObject<Cube>(position);
		 		}
		 	}
		 }
		
		if(ImGui::CollapsingHeader("Profiler"))
		{
			ImGui::DragInt("Max FPS", &TimerSubsystem::GetMaxFPS(), 1, 0, 240);
			ImGui::SliderFloat("Game Speed", &TimerSubsystem::GetGameTimeSpeed(), 0.0f, 2.0f);
			ImGui::Text("Frame time: %fms", TimerSubsystem::GetFrameTimeMs());
			ImGui::Text("FPS: %d", TimerSubsystem::GetFPS());
		
			for (const auto& [name, profile] : ProfileSubsystem::Get()->GetProfiles())
			{
				//ImGui::SetNextItemOpen(true, ImGuiCond_Once);
				if(ImGui::TreeNode(name.c_str()))
				{
					ImGui::Text("Frame Percentage: %f", profile->m_FramePercentage);
					ImGui::Text("Duration: %fms", profile->GetDuration());
					ImGui::Text("Average Duration: %fms", profile->m_AverageDuration);
					if(ImGui::DragInt("Average Duration Count", &profile->m_AverageDurationCount, 1.0f, 3, 100))
					{
						profile->m_DurationRecords.resize(profile->m_AverageDurationCount);
					}
					ImGui::TreePop();
				}
			}
		}
	
		/*ImGui::SeparatorText("Ray Tracing");
		if (ImGui::Button("Spawn Ray"))
		{
			const glm::vec3& camPos = m_ActiveCamera->GetTransformComponent()->GetPosition();
			const glm::vec3& camForward = m_ActiveCamera->GetCameraFront();
			const glm::vec3& camRight = m_ActiveCamera->m_CameraRight;
			m_Ray = SpawnGameObject<Ray>(camPos, glm::degrees(camForward * camRight));
		}
		ImGui::DragFloat("Move Speed", &Ray::m_MoveSpeed);*/
		
		if(ImGui::CollapsingHeader("Reflection"))
		{
			for (const auto& key : ReflectionSubsystem::GetCreateFuncs() | std::views::keys)
			{
				ImGui::Text(key.c_str());
			}
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
		}

		if(ImGui::CollapsingHeader("Assets"))
		{
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
				for (const auto& scene : ResourceSubsystem::GetSceneStore())
				{
					ImGui::Text(scene->GetAssetName().c_str());

					if(ImGui::Button("Set as startup scene"))
					{
						Engine::Get().SetStartupScene(m_SceneAsset);
					}
					if (ImGui::Button("Open"))
					{
						SceneSubsystem::OpenScene(scene);
					}
					ImGui::Text("Asset Path: %s", scene->GetAssetPath().c_str());
				}
				ImGui::TreePop();
			}
		}

		ImGui::End();
	}
}