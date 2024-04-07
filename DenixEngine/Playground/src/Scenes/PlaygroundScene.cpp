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
		m_SceneObjects.push_back(Pyramid);

		PntLight = MakeRef<PointLight>();
		PntLight->GetTransformComponent()->SetPosition({ 0.0f, 7.0f, 0.0f });
		PntLight->SetLightColor({ 1.0f,0.0f,0.0f });
		m_SceneObjects.push_back(PntLight);

		Ref<Cube> red = MakeRef<Cube>();
		red->GetTransformComponent()->SetPosition({ 5.0f, 10.0f, 0.0f });
		red->GetRenderComponent()->SetMaterial(ResourceSubsystem::GetMaterial("MAT_Red"));

		//m_SceneObjects.push_back(MakeRef<Camera>());

		return true;
	}

	void PlaygroundScene::Update(float _deltaTime)
	{
		Scene::Update(_deltaTime);

		Ref<GLShader> program = ResourceSubsystem::GetShader("DefaultShader");
		program->Bind();
		glUniform1i(program->GetUniform("u_PointLightCount"), (int)m_PointLights.size());

		for (int i = 0; i < (int)m_PointLights.size(); i++)
		{
			const glm::vec3& lightCol =	m_PointLights[i]->GetLightColor();
			glUniform3f(program->GetUniform("u_PointLight[" + std::to_string(i) + "].Base.Color"), lightCol.r, lightCol.g, lightCol.b);
			glUniform1f(program->GetUniform("u_PointLight[" + std::to_string(i) + "].Base.AmbientIntensity"), m_PointLights[i]->GetAmbientIntensity());
			glUniform1f(program->GetUniform("u_PointLight[" + std::to_string(i) + "].Base.DiffuseIntensity"), m_PointLights[i]->GetDiffuseIntensity());

			const glm::vec3& pos = m_PointLights[i]->GetTransformComponent()->GetPosition();
			glUniform3f(program->GetUniform("u_PointLight[" + std::to_string(i) + "].Position"), pos.x, pos.y, pos.z);
			glUniform1f(program->GetUniform("u_PointLight[" + std::to_string(i) + "].Constant"), m_PointLights[i]->GetConstant());
			glUniform1f(program->GetUniform("u_PointLight[" + std::to_string(i) + "].Linear"), m_PointLights[i]->GetLinear());
			glUniform1f(program->GetUniform("u_PointLight[" + std::to_string(i) + "].Exponent"), m_PointLights[i]->GetExponent());
		}

		GLShader::Unbind();

		ImGui::SetNextWindowPos(ImVec2(1000, 50));
		ImGui::Begin("Playground Tools");

		ImGui::SeparatorText("Scene Info");
		ImGui::Text("Point Lights: %d", m_PointLights.size());

		

		ImGui::SeparatorText("Directional Light Settings");
		ImGui::ColorEdit4("Base Color", &m_DirLight->GetRenderComponent()->GetTexture()->m_BaseColor[0]);
		ImGui::ColorEdit3("DirectionalLight Color", &m_DirLight->GetLightColor()[0]);
		ImGui::SliderFloat("Ambient Intensity", &m_DirLight->GetAmbientIntensity(),
			m_DirLight->m_AmbientConstraints.x, m_DirLight->m_AmbientConstraints.y);
		ImGui::SliderFloat("Diffuse Intensity", &m_DirLight->GetDiffuseIntensity(),
			m_DirLight->m_DiffuseConstraints.x, m_DirLight->m_DiffuseConstraints.y);
		ImGui::DragFloat3("DirectionalLight Position", &m_DirLight->GetLightDirection()[0], 0.1f);


		ImGui::PushID("defmat");
		
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