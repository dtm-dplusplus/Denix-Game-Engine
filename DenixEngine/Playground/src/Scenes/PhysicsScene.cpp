#include "PhysicsScene.h"
#include "imgui.h"
#include "Denix/Core/TimerSubsystem.h"

#include "Denix/Scene/Object/Shapes/Shapes.h"
#include "Denix/Video/Renderer/RendererSubsystem.h"
#include "Denix/Core/Math.h"

namespace Denix
{
	ModelObject::ModelObject(const ObjectInitializer& _objInit) : GameObject(_objInit)
	{
		ModelObj = MakeRef<Model>();
		//ModelObj->LoadModel(FileSubsystem::GetContentRoot() + "Models/Cube.obj");
		ModelObj->LoadModel(FileSubsystem::GetContentRoot() + "Models/Sphere.obj");

		m_RenderComponent->SetIsVisible(false);
		m_RenderComponent->GetBaseColorAsTexture() = true;
		m_RenderComponent->SetMaterial(ResourceSubsystem::GetMaterial("MAT_Red"));
	}

	bool PhysicsScene::Load()
	{
		Scene::Load();

		Ref <Plane> floor = MakeRef<Plane>();
		floor->GetTransformComponent()->SetRotation({90.0f, 0.0f,0.0f});
		floor->GetTransformComponent()->SetScale(glm::vec3(50.0f, 50.0f, 1.0f));
		m_SceneObjects.push_back(floor);

		/*PntLight = MakeRef<PointLight>();
		PntLight->GetTransformComponent()->SetPosition({ 0.0f, 7.0f, 0.0f });
		PntLight->SetLightColor({ 1.0f,0.0f,0.0f });
		m_SceneObjects.push_back(PntLight);

		Ref<Cube> red = MakeRef<Cube>();
		red->GetTransformComponent()->SetPosition({ 5.0f, 10.0f, 0.0f });
		red->GetRenderComponent()->SetMaterial(ResourceSubsystem::GetMaterial("MAT_Red"));

		SptLight = MakeRef<SpotLight>();
		SptLight->GetTransformComponent()->SetPosition({ -5.0f, 5.0f, 0.0f });
		m_SceneObjects.push_back(SptLight);*/

		PhysicsObject = MakeRef<PhysObject>(ObjectInitializer("Cube 1"));
		PhysicsObject->GetTransformComponent()->SetPosition({ 0.0f, 10.0f, 0.0f });
		PhysicsObject->GetTransformComponent()->SetMoveability(Moveability::Dynamic);
		PhysicsObject->GetPhysicsComponent()->SimulatePhysics() = true;
		PhysicsObject->GetCollider() = MakeRef<CubeCollider>();
		PhysicsObject->GetCollider()->GetRenderComponent()->SetIsVisible(true);

		PhysicsObject->GetMeshComponent()->SetMesh(ResourceSubsystem::GetMesh("SM_Cube"));
		m_SceneObjects.push_back(PhysicsObject);

		Ref<PhysObject> other = MakeRef<PhysObject>(ObjectInitializer("Cube 2"));
		other->GetTransformComponent()->SetPosition({ 5.0f, 10.0f, 0.0f });
		other->GetTransformComponent()->SetMoveability(Moveability::Dynamic);
		other->GetPhysicsComponent()->SimulatePhysics() = false;
		other->GetMeshComponent()->SetMesh(ResourceSubsystem::GetMesh("SM_Cube"));
		other->GetCollider() = MakeRef<CubeCollider>();
		other->GetCollider()->GetRenderComponent()->SetIsVisible(true);

		m_SceneObjects.push_back(other);

		m_DirLight->SetAmbientIntensity(0.7f);
		m_ViewportCamera->SetFov(100.0f);
		m_ViewportCamera->GetTransformComponent()->SetPosition({ 0.0f,5.0f, 10.0f });


		TestModel = MakeRef<ModelObject>();
		m_SceneObjects.push_back(TestModel);

		return true;
	}

	void PhysicsScene::Update(float _deltaTime)
	{
		Scene::Update(_deltaTime);
		
		ImGui::SetNextWindowPos(ImVec2(1000, 50));
		ImGui::Begin("Physics Scene Tools");
		ImGui::SliderFloat("Game Speed", &TimerSubsystem::GetGameTimeSpeed(), 0.0f, 2.0f);
		ImGui::Text("Frame time: %fms", TimerSubsystem::GetFrameTime());
		ImGui::Text("FPS: %d", TimerSubsystem::GetFPS());
		ImGui::End();

		//TestModel->RenderModel();
		if (Ref<RenderComponent> rendComp = TestModel->GetRenderComponent())
		{
			Ref<GLShader> shader = rendComp->GetShader();
			shader->Bind();

			for (unsigned int i = 0; i < TestModel->ModelObj->m_Meshes.size(); i++)
			{
				unsigned int materialIndex = TestModel->ModelObj->m_MeshToTex[i];

				/*if (materialIndex < m_Textures.size() && m_Textures[materialIndex])
				{
				m_Textures[materialIndex]->Bind();
				}*/

				/*m_Meshes[i]->GetVertexArray()->Bind();
				m_Meshes[i]->GetIndexBuffer()->Bind();
				glDrawElements(GL_TRIANGLES, m_Meshes[i]->GetIndexBuffer()->GetIndexCount(), GL_UNSIGNED_INT, 0);
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
				glBindVertexArray(0);*/

				TestModel->ModelObj->m_Meshes[i]->GetVertexArray()->Bind();
				TestModel->ModelObj->m_Meshes[i]->GetIndexBuffer()->Bind();


				glUniformMatrix4fv(shader->GetUniform("u_Model"), 1,
					GL_FALSE, glm::value_ptr(TestModel->GetTransformComponent()->GetModel()));

				// Upload Affects Lighting bool
				glUniform1i(shader->GetUniform("u_AffectsLighting"), rendComp->AffectsLighting());
				glUniform1i(shader->GetUniform("u_BaseColorAsTexture"), rendComp->GetBaseColorAsTexture());

				// Upload the texture
				/*if (!renderComp->GetBaseColorAsTexture() && renderComp->m_Texture)
				{
					renderComp->m_Texture->Bind();

					glTexParameteri(renderComp->m_Texture->GetTarget(), GL_TEXTURE_WRAP_S, renderComp->m_TextureSettings.WrapMode);
					glTexParameteri(renderComp->m_Texture->GetTarget(), GL_TEXTURE_WRAP_T, renderComp->m_TextureSettings.WrapMode);
					glTexParameteri(renderComp->m_Texture->GetTarget(), GL_TEXTURE_MIN_FILTER, renderComp->m_TextureSettings.FilterMode);
					glTexParameteri(renderComp->m_Texture->GetTarget(), GL_TEXTURE_MAG_FILTER, renderComp->m_TextureSettings.FilterMode);
				}*/

				// Upload the material
				if (const Ref<Material> mat = TestModel->GetRenderComponent()->GetMaterial())
				{
					if (TestModel->GetRenderComponent()->GetBaseColorAsTexture())
					{
						const glm::vec3& baseColor = mat->GetBaseColor();
						glUniform3f(shader->GetUniform("u_Material.BaseColor"),
							baseColor.r, baseColor.g, baseColor.b);
					}

					glUniform1f(shader->GetUniform("u_Material.SpecularIntensity"), mat->GetSpecularIntensity());
					glUniform1f(shader->GetUniform("u_Material.SpecularPower"), mat->GetSpecularPower());
				}

				// Draw Call
				glDrawElements(GL_TRIANGLES, TestModel->ModelObj->m_Meshes[i]->GetIndexBuffer()->GetIndexCount(), GL_UNSIGNED_INT, 0);
			}
		}
		
		
	}
}