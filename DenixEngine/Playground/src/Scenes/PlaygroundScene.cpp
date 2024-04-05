#include "PlaygroundScene.h"
#include "imgui.h"

namespace Denix
{
	bool PlaygroundScene::Load()
	{
		Scene::Load();

		Ref <Plane> floor = MakeRef<Plane>();
		floor->GetTransformComponent()->SetRotation({90.0f, 0.0f,0.0f});
		floor->GetTransformComponent()->SetScale(glm::vec3(50.0f, 50.0f, 1.0f));
		m_SceneObjects.push_back(floor);

		DirLight = MakeRef<DirectionalLight>();
		DirLight->GetTransformComponent()->SetPosition(glm::vec3(0.0f, 100.0f, 0.0f));
		m_SceneObjects.push_back(DirLight);

		WeirdoCube = MakeRef<WeirdCube>();
		WeirdoCube->GetRenderComponent()->SetTexture(ResourceSubsystem::GetTexture("brick"));
		m_SceneObjects.push_back(WeirdoCube);
		return true;
	}

	void PlaygroundScene::Update(float _deltaTime)
	{
		Scene::Update(_deltaTime);

		ImGui::SetNextWindowPos(ImVec2(1000, 50));
		ImGui::Begin("Playground Tools");

		ImGui::SeparatorText("Directional Light Settings");
		ImGui::ColorEdit4("Base Color", &DirLight->GetRenderComponent()->GetTexture()->m_BaseColor[0]);


		ImGui::ColorEdit3("DirectionalLight Color", &DirLight->m_LightColor[0]);

		ImGui::SliderFloat("Ambient Intensity", &DirLight->m_AmbientIntensity,
			DirLight->m_AmbientConstraints.x, DirLight->m_AmbientConstraints.y);

		ImGui::SliderFloat("Diffuse Intensity", &DirLight->m_DiffuseIntensity,
			DirLight->m_DiffuseConstraints.x, DirLight->m_DiffuseConstraints.y);

		ImGui::DragFloat3("DirectionalLight Position", &DirLight->m_LightDirection[0], 0.1f);

		ImGui::SeparatorText("Werid Object Settings");

		
		ImGui::End();

		DirLight->UseLight();
	}
}