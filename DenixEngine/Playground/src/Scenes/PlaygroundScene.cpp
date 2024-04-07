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


		Pyramid = MakeRef<WeirdCube>();
		Pyramid->GetTransformComponent()->SetPosition({ 0.0f,5.0f,0.0f });
		Pyramid->GetRenderComponent()->SetTexture(ResourceSubsystem::GetTexture("brick"));
		Pyramid->GetRenderComponent()->SetMaterial(MakeRef<Material>(0.5f, 4));
		m_SceneObjects.push_back(Pyramid);

		TestCube = MakeRef<Cube>();
		TestCube->GetTransformComponent()->SetPosition({ 5.0f,5.0f,0.0f });
		TestCube->GetRenderComponent()->SetTexture(ResourceSubsystem::GetTexture("brick"));
		TestCube->GetRenderComponent()->SetMaterial(MakeRef<Material>(0.5f, 4));
		m_SceneObjects.push_back(TestCube);

		PntLight = MakeRef<PointLight>();
		m_SceneObjects.push_back(PntLight);
		return true;
	}

	void PlaygroundScene::Update(float _deltaTime)
	{
		Scene::Update(_deltaTime);

		ImGui::SetNextWindowPos(ImVec2(1000, 50));
		ImGui::Begin("Playground Tools");

		ImGui::SeparatorText("Point Light Settings");
		ImGui::DragFloat3("PointLight Position", &PntLight->GetTransformComponent()->GetPosition()[0], 0.1f);
		ImGui::ColorEdit3("PointLight Color", &PntLight->GetLightColor()[0]);
		ImGui::SliderFloat("PointLight Intensity", &PntLight->GetDiffuseIntensity(), 0.0f, 3.0f);
		ImGui::SliderFloat("PointLight Ambient Intensity", &PntLight->GetAmbientIntensity(), 0.0f, 1.0f);
		ImGui::SliderFloat("PointLight Constant", &PntLight->GetConstant(), 0.0f, 1.0f);
		ImGui::SliderFloat("PointLight Linear", &PntLight->GetLinear(), 0.0f, 1.0f);
		ImGui::SliderFloat("PointLight Exponent", &PntLight->GetExponent(), 0.0f, 1.0f);

		ImGui::SeparatorText("Directional Light Settings");
		ImGui::ColorEdit4("Base Color", &m_DirLight->GetRenderComponent()->GetTexture()->m_BaseColor[0]);
		ImGui::ColorEdit3("DirectionalLight Color", &m_DirLight->GetLightColor()[0]);
		ImGui::SliderFloat("Ambient Intensity", &m_DirLight->GetAmbientIntensity(),
			m_DirLight->m_AmbientConstraints.x, m_DirLight->m_AmbientConstraints.y);
		ImGui::SliderFloat("Diffuse Intensity", &m_DirLight->GetDiffuseIntensity(),
			m_DirLight->m_DiffuseConstraints.x, m_DirLight->m_DiffuseConstraints.y);
		ImGui::DragFloat3("DirectionalLight Position", &m_DirLight->GetLightDirection()[0], 0.1f);


		ImGui::PushID("defmat");
		Ref<Material> defmat = ResourceSubsystem::GetMaterial("DefaultMaterial");
		ImGui::SeparatorText("Default Material Settings");
		ImGui::DragFloat("Specular Intensity", &defmat->GetSpecularIntensity());
		ImGui::DragFloat("Specular Power", &defmat->GetSpecularPower());
		ImGui::PopID();


		ImGui::PushID("mat");
		Ref<Material> mat = Pyramid->GetRenderComponent()->GetMaterial();
		ImGui::SeparatorText("Werid Object Settings");
		ImGui::DragFloat("Specular Intensity", &mat->GetSpecularIntensity());
		ImGui::DragFloat("Specular Power", &mat->GetSpecularPower());
		ImGui::PopID();
		ImGui::End();

	}
}