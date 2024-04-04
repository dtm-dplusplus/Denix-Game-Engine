#include "PlaygroundScene.h"
#include "imgui.h"

namespace Denix
{
	bool PlaygroundScene::Load()
	{
		Scene::Load();

		DirLight = MakeRef<DirectionalLight>();
		DirLight->GetTransformComponent()->SetPosition(glm::vec3(0.0f, 3.0f, 0.0f));
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

		ImGui::ColorEdit4("Background Color", &DirLight->GetRenderComponent()->GetTexture()->m_BaseColor[0]);

		ImGui::Checkbox("Affects Lighting", &DirLight->GetRenderComponent()->AffectsLighting());

		ImGui::ColorEdit3("DirectionalLight Color", &DirLight->m_LightColor[0]);

		ImGui::DragFloat("Ambient Intensity", &DirLight->m_AmbientIntensity, _deltaTime,
			DirLight->m_AmbientConstraints.x, DirLight->m_AmbientConstraints.y);

		ImGui::DragFloat("Diffuse Intensity", &DirLight->m_DiffuseIntensity, _deltaTime,
			DirLight->m_DiffuseConstraints.x, DirLight->m_DiffuseConstraints.y);

		ImGui::DragFloat3("DirectionalLight Position", &DirLight->m_LightDirection[0], 0.1f);
		ImGui::End();

		DirLight->UseLight();
	}
}