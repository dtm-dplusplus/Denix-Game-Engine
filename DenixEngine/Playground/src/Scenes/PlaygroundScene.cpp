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

		CubeModel = MakeRef<ModelObject>();
		CubeModel->GetMeshComponent()->SetMesh(ResourceSubsystem::GetMesh("SM_Cube"));
		CubeModel->GetRenderComponent()->SetIsVisible(false);
		CubeModel->TestModel->LoadModel(FileSubsystem::GetContentRoot() + R"(Models\cube.obj)");
		m_SceneObjects.push_back(CubeModel);

		
		return true;
	}

	void PlaygroundScene::Update(float _deltaTime)
	{
		Scene::Update(_deltaTime);
		
		{
			Ref<RenderComponent> render = CubeModel->GetRenderComponent();
			Ref<TransformComponent> transform = CubeModel->GetTransformComponent();
			Ref<GLShader> shader = CubeModel->GetRenderComponent()->GetShader();
			if (render && transform && shader)
			{
				if (const Ref<Material> mat = render->GetMaterial())
				{
					glUniform3f(shader->GetUniform("u_Material.BaseColor"),
						mat->GetBaseColor().r, mat->GetBaseColor().g, mat->GetBaseColor().b);
					glUniform1f(shader->GetUniform("u_Material.SpecularIntensity"), mat->GetSpecularIntensity());
					glUniform1f(shader->GetUniform("u_Material.SpecularPower"), mat->GetSpecularPower());
				}

				ResourceSubsystem::GetShader("DefaultShader")->Bind();

				// Draw the indexed mesh
				for (unsigned int i = 0; i < CubeModel->TestModel->m_Meshes.size(); i++)
				{
					unsigned int materialIndex = CubeModel->TestModel->m_MeshToTex[i];

					if (materialIndex < CubeModel->TestModel->m_Textures.size() && CubeModel->TestModel->m_Textures[materialIndex])
					{
						CubeModel->TestModel->m_Textures[materialIndex]->Bind();
					}

					CubeModel->TestModel->m_Meshes[i]->GetVertexArray()->Bind();
					CubeModel->TestModel->m_Meshes[i]->GetIndexBuffer()->Bind();

					// Upload the model matrix
					glUniformMatrix4fv(render->GetShader()->GetUniform("u_Model"), 1,
						GL_FALSE, glm::value_ptr(transform->GetModel()));

					// Upload Affects Lighting bool
					glUniform1i(render->GetShader()->GetUniform("u_AffectsLighting"), render->AffectsLighting());
					glUniform1i(render->GetShader()->GetUniform("u_BaseColorAsTexture"), render->GetBaseColorAsTexture());


					// Upload the camera matrices relative to Object
					if (Ref<Camera> camera = m_ActiveCamera)
					{
						glUniformMatrix4fv(render->GetShader()->GetUniform("u_Projection"), 1,
							GL_FALSE, glm::value_ptr(camera->GetProjectionMatrix()));

						glUniformMatrix4fv(render->GetShader()->GetUniform("u_View"), 1,
							GL_FALSE, glm::value_ptr(camera->GetViewMatrix()));

						glUniform3f(render->GetShader()->GetUniform("u_CameraPosition"),
							camera->GetTransformComponent()->GetPosition().x,
							camera->GetTransformComponent()->GetPosition().y,
							camera->GetTransformComponent()->GetPosition().z);
					}

					glDrawElements(GL_TRIANGLES, CubeModel->TestModel->m_Meshes[i]->GetIndexBuffer()->GetIndexCount(), GL_UNSIGNED_INT, 0);
					glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
					glBindVertexArray(0);
				}

				IndexBuffer::Unbind();
				VertexArray::Unbind();
				GLShader::Unbind();
			}
		}
		

		ImGui::SetNextWindowPos(ImVec2(1000, 50));
		ImGui::Begin("Playground Tools");
		
		ImGui::End();
	}
}