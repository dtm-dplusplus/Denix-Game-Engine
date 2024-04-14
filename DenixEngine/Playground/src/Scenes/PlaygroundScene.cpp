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

		PntLight = MakeRef<PointLight>();
		PntLight->GetTransformComponent()->SetPosition({ 0.0f, 7.0f, 0.0f });
		PntLight->SetLightColor({ 1.0f,0.0f,0.0f });
		m_SceneObjects.push_back(PntLight);

		Ref<Cube> red = MakeRef<Cube>();
		red->GetTransformComponent()->SetPosition({ 5.0f, 10.0f, 0.0f });
		red->GetRenderComponent()->SetMaterial(ResourceSubsystem::GetMaterial("MAT_Red"));

		SptLight = MakeRef<SpotLight>();
		SptLight->GetTransformComponent()->SetPosition({ -5.0f, 5.0f, 0.0f });
		m_SceneObjects.push_back(SptLight);

		/*TestModel = MakeRef<ModelObject>();
		TestModel->GetRenderComponent()->SetIsVisible(false);
		TestModel->TestModel->LoadModel(FileSubsystem::GetContentRoot() + R"(Models\cube.obj)");
		m_SceneObjects.push_back(TestModel);*/

		return true;
	}

	void PlaygroundScene::Update(float _deltaTime)
	{
		Scene::Update(_deltaTime);

		// Lighting
		{
			Ref<GLShader> program = ResourceSubsystem::GetShader("DefaultShader");
			program->Bind();
			glUniform1i(program->GetUniform("u_PointLightCount"), (int)m_PointLights.size());
			glUniform1i(program->GetUniform("u_SpotLightCount"), (int)m_SpotLights.size());


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

			for (int i = 0; i < (int)m_SpotLights.size(); i++)
			{
			const glm::vec3& lightCol = m_SpotLights[i]->GetLightColor();
			glUniform3f(program->GetUniform("u_SpotLight[" + std::to_string(i) + "].Base.Base.Color"), lightCol.r, lightCol.g, lightCol.b);
			glUniform1f(program->GetUniform("u_SpotLight[" + std::to_string(i) + "].Base.Base.AmbientIntensity"), m_SpotLights[i]->GetAmbientIntensity());
			glUniform1f(program->GetUniform("u_SpotLight[" + std::to_string(i) + "].Base.Base.DiffuseIntensity"), m_SpotLights[i]->GetDiffuseIntensity());

			const glm::vec3& pos = m_SpotLights[i]->GetTransformComponent()->GetPosition();
			glUniform3f(program->GetUniform("u_SpotLight[" + std::to_string(i) + "].Base.Position"), pos.x, pos.y, pos.z);
			glUniform1f(program->GetUniform("u_SpotLight[" + std::to_string(i) + "].Base.Constant"), m_SpotLights[i]->GetConstant());
			glUniform1f(program->GetUniform("u_SpotLight[" + std::to_string(i) + "].Base.Linear"), m_SpotLights[i]->GetLinear());
			glUniform1f(program->GetUniform("u_SpotLight[" + std::to_string(i) + "].Base.Exponent"), m_SpotLights[i]->GetExponent());

			const glm::vec3& dir = m_SpotLights[i]->GetDirection();
			glUniform3f(program->GetUniform("u_SpotLight[" + std::to_string(i) + "].Direction"), dir.x, dir.y, dir.z);
			glUniform1f(program->GetUniform("u_SpotLight[" + std::to_string(i) + "].Edge"), m_SpotLights[i]->GetProcessedEdge());
			}
			GLShader::Unbind();
		}
		
		// Model
		//{
		//	Ref<RenderComponent> render = TestModel->GetRenderComponent();
		//	Ref<TransformComponent> transform = TestModel->GetTransformComponent();
		//	if (render && transform)
		//	{
		//		

		//		


		//		/*if (const Ref<Material> mat = render->m_Material)
		//		{
		//			glUniform3f(render->m_Shader->GetUniform("u_Material.BaseColor"),
		//				mat->GetBaseColor().r, mat->GetBaseColor().g, mat->GetBaseColor().b);
		//			glUniform1f(render->m_Shader->GetUniform("u_Material.SpecularIntensity"), mat->GetSpecularIntensity());
		//			glUniform1f(render->m_Shader->GetUniform("u_Material.SpecularPower"), mat->GetSpecularPower());
		//		}*/

		//		//ResourceSubsystem::GetShader("DefaultShader")->Bind();

		//		//// Draw the indexed mesh
		//		//for (unsigned int i = 0; i < TestModel->TestModel->m_Meshes.size(); i++)
		//		//{
		//		//	unsigned int materialIndex = TestModel->TestModel->m_MeshToTex[i];

		//		//	if (materialIndex < TestModel->TestModel->m_Textures.size() && TestModel->TestModel->m_Textures[materialIndex])
		//		//	{
		//		//		TestModel->TestModel->m_Textures[materialIndex]->Bind();
		//		//	}

		//		//	TestModel->TestModel->m_Meshes[i]->GetVertexArray()->Bind();
		//		//	TestModel->TestModel->m_Meshes[i]->GetIndexBuffer()->Bind();

		//		//	// Upload the model matrix
		//		//	glUniformMatrix4fv(render->GetShader()->GetUniform("u_Model"), 1,
		//		//		GL_FALSE, glm::value_ptr(transform->GetModel()));

		//		//	// Upload Affects Lighting bool
		//		//	glUniform1i(render->GetShader()->GetUniform("u_AffectsLighting"), render->AffectsLighting());
		//		//	glUniform1i(render->GetShader()->GetUniform("u_BaseColorAsTexture"), render->GetBaseColorAsTexture());


		//		//	// Upload the camera matrices relative to Object
		//		//	if (Ref<Camera> camera = m_ActiveCamera)
		//		//	{
		//		//		glUniformMatrix4fv(render->GetShader()->GetUniform("u_Projection"), 1,
		//		//			GL_FALSE, glm::value_ptr(camera->GetProjectionMatrix()));

		//		//		glUniformMatrix4fv(render->GetShader()->GetUniform("u_View"), 1,
		//		//			GL_FALSE, glm::value_ptr(camera->GetViewMatrix()));

		//		//		glUniform3f(render->GetShader()->GetUniform("u_CameraPosition"),
		//		//			camera->GetTransformComponent()->GetPosition().x,
		//		//			camera->GetTransformComponent()->GetPosition().y,
		//		//			camera->GetTransformComponent()->GetPosition().z);
		//		//	}

		//		//	glDrawElements(GL_TRIANGLES, TestModel->TestModel->m_Meshes[i]->GetIndexBuffer()->GetIndexCount(), GL_UNSIGNED_INT, 0);
		//		//	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		//		//	glBindVertexArray(0);
		//		//}

		//		//IndexBuffer::Unbind();
		//		//VertexArray::Unbind();
		//		//GLShader::Unbind();
		//	}
		//}
		

		ImGui::SetNextWindowPos(ImVec2(1000, 50));
		ImGui::Begin("Playground Tools");
		
		ImGui::End();
	}
}