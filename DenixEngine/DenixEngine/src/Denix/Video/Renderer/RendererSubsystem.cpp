#include "depch.h"
#include "RendererSubsystem.h"

#include "Denix/Resource/ResourceSubsystem.h"
#include "Denix/Scene/Camera.h"
#include "Denix/Scene/Scene.h"
#include "Denix/Scene/GameObject.h"
#include "Denix/Physics/Collider.h"

namespace Denix
{
	RendererSubsystem* RendererSubsystem::s_RendererSubSystem{ nullptr };

	void RendererSubsystem::RenderScene()
	{
		if (!s_RendererSubSystem->m_Enabled) return;

		// Render the scene based on the viewport mode
		switch (static_cast<ViewportMode>(m_ViewportMode))
		{
		case ViewportMode::Default: RenderDefaultViewport(); break;
		
		case ViewportMode::Unlit: RenderUnlitViewport(); break;

		case ViewportMode::Wireframe: RenderWireframeViewport(); break;
		
		case ViewportMode::Collision: RenderCollisionViewport(); break;
		}

		// Will skip if viewport mode is not default
		RenderLighting();
	}

	void RendererSubsystem::RenderDefaultViewport()
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		for (const Ref<GameObject>& object : s_RendererSubSystem->m_ActiveScene->m_SceneObjects)
		{
			const Ref<RenderComponent> renderComp = object->GetRenderComponent();
			const Ref<TransformComponent> transformComp = object->GetTransformComponent();
			const Ref<MeshComponent> meshComp = object->GetMeshComponent();

			if (!renderComp || !transformComp || !meshComp) continue;

			if (const Ref<Material> mat = renderComp->m_Material; renderComp->IsVisible())
			{
				mat->m_Shader->Bind();

				// Upload the material
				const BaseMatParam& base = mat->GetBaseParam();
				glUniform1i(renderComp->m_Shader->GetUniform("u_Material.Base.IsTexture"), base.IsTexture);

				if (base.IsTexture && base.Texture)
				{
					base.Texture->Bind();

					// Texture Settings need to move to the material/texture
					glTexParameteri(base.Texture->GetTarget(), GL_TEXTURE_WRAP_S, renderComp->m_TextureSettings.WrapMode);
					glTexParameteri(base.Texture->GetTarget(), GL_TEXTURE_WRAP_T, renderComp->m_TextureSettings.WrapMode);
					glTexParameteri(base.Texture->GetTarget(), GL_TEXTURE_MIN_FILTER, renderComp->m_TextureSettings.FilterMode);
					glTexParameteri(base.Texture->GetTarget(), GL_TEXTURE_MAG_FILTER, renderComp->m_TextureSettings.FilterMode);
				}
				else
				{
					glUniform3f(renderComp->m_Shader->GetUniform("u_Material.Base.Color"),
						base.Color.r, base.Color.g, base.Color.b);


					glUniform1f(renderComp->m_Shader->GetUniform("u_Material.SpecularIntensity"), mat->GetSpecularIntensity());
					glUniform1f(renderComp->m_Shader->GetUniform("u_Material.SpecularPower"), mat->GetSpecularPower());
				}
				
				// Upload the camera matrices relative to Object
				if (const Ref<Camera> camera = s_RendererSubSystem->m_ActiveScene->m_ActiveCamera)
				{
					glUniformMatrix4fv(renderComp->m_Shader->GetUniform("u_Projection"), 1,
						GL_FALSE, glm::value_ptr(camera->GetProjectionMatrix()));

					glUniformMatrix4fv(renderComp->m_Shader->GetUniform("u_View"), 1,
						GL_FALSE, glm::value_ptr(camera->GetViewMatrix()));

					glUniform3f(renderComp->m_Shader->GetUniform("u_CameraPosition"),
						camera->GetTransformComponent()->GetPosition().x,
						camera->GetTransformComponent()->GetPosition().y,
						camera->GetTransformComponent()->GetPosition().z);
				}

				// Upload the model matrix
				glUniformMatrix4fv(renderComp->m_Shader->GetUniform("u_Model"), 1,
					GL_FALSE, glm::value_ptr(transformComp->GetModel()));

				// Upload Affects Lighting bool
				glUniform1i(renderComp->m_Shader->GetUniform("u_AffectsLighting"), renderComp->m_AffectsLighting);

				// Draw Call
				if (const Ref<Model> model = meshComp->GetModel())
				{
					for (unsigned int i = 0; i < model->m_Meshes.size(); i++)
					{
						if (model->m_Meshes[i]->m_VAO && model->m_Meshes[i]->m_IBO)
						{
							model->m_Meshes[i]->m_VAO->Bind();
							model->m_Meshes[i]->m_IBO->Bind();
							glDrawElements(GL_TRIANGLES, model->m_Meshes[i]->m_IBO->GetIndexCount(), GL_UNSIGNED_INT, 0);
						}
					}
				}
			}

			// Draw Collision over gameobject if set to visible
			if (object->GetPhysicsComponent()->IsColliderVisible()) RenderCollider(object->GetPhysicsComponent());
		}

		if (const Ref<UICamera> cam = s_RendererSubSystem->m_ActiveScene->m_UICamera)
		{
			Ref<Shader> shader = ResourceSubsystem::GetShader("TextShader");

			for (const TextUI& text : cam->m_Texts)
			{
				cam->RenderText(shader, "This is sample text", 25.0f, 25.0f, 1.0f, glm::vec3(0.5, 0.8f, 0.2f));
            }
		}
	}

	void RendererSubsystem::RenderUnlitViewport()
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		ResourceSubsystem::GetShader("UnlitShader")->Bind();

		for (const Ref<GameObject>& object : s_RendererSubSystem->m_ActiveScene->m_SceneObjects)
		{
			const Ref<RenderComponent> renderComp = object->GetRenderComponent();
			const Ref<TransformComponent> transformComp = object->GetTransformComponent();
			const Ref<MeshComponent> meshComp = object->GetMeshComponent();

			if (!renderComp || !transformComp || !meshComp) continue;
			//if (!renderComp->m_Material) continue;
			//if (!renderComp->m_Material->GetShader()) continue;

			if (const Ref<Material> mat = renderComp->m_Material; renderComp->IsVisible())
			{
				mat->m_Shader->Bind();

				// Upload the material
				const BaseMatParam& base = mat->GetBaseParam();

				if (base.IsTexture && base.Texture)
				{
					base.Texture->Bind();

					// Texture Settings need to move to the material/texture
					glTexParameteri(base.Texture->GetTarget(), GL_TEXTURE_WRAP_S, renderComp->m_TextureSettings.WrapMode);
					glTexParameteri(base.Texture->GetTarget(), GL_TEXTURE_WRAP_T, renderComp->m_TextureSettings.WrapMode);
					glTexParameteri(base.Texture->GetTarget(), GL_TEXTURE_MIN_FILTER, renderComp->m_TextureSettings.FilterMode);
					glTexParameteri(base.Texture->GetTarget(), GL_TEXTURE_MAG_FILTER, renderComp->m_TextureSettings.FilterMode);
				}
				else
				{
					glUniform3f(renderComp->m_Shader->GetUniform("u_Material.Base.Color"),
						base.Color.r, base.Color.g, base.Color.b);

					glUniform1i(renderComp->m_Shader->GetUniform("u_Material.Base.IsTexture"), base.IsTexture);

					glUniform1f(renderComp->m_Shader->GetUniform("u_Material.SpecularIntensity"), mat->GetSpecularIntensity());
					glUniform1f(renderComp->m_Shader->GetUniform("u_Material.SpecularPower"), mat->GetSpecularPower());
				}

				// Upload the camera matrices relative to Object
				if (const Ref<Camera> camera = s_RendererSubSystem->m_ActiveScene->m_ActiveCamera)
				{
					glUniformMatrix4fv(renderComp->m_Shader->GetUniform("u_Projection"), 1,
						GL_FALSE, glm::value_ptr(camera->GetProjectionMatrix()));

					glUniformMatrix4fv(renderComp->m_Shader->GetUniform("u_View"), 1,
						GL_FALSE, glm::value_ptr(camera->GetViewMatrix()));

					glUniform3f(renderComp->m_Shader->GetUniform("u_CameraPosition"),
						camera->GetTransformComponent()->GetPosition().x,
						camera->GetTransformComponent()->GetPosition().y,
						camera->GetTransformComponent()->GetPosition().z);
				}

				// Upload the model matrix
				glUniformMatrix4fv(renderComp->m_Shader->GetUniform("u_Model"), 1,
					GL_FALSE, glm::value_ptr(transformComp->GetModel()));

				// Upload Affects Lighting bool
				glUniform1i(renderComp->m_Shader->GetUniform("u_AffectsLighting"), renderComp->m_AffectsLighting);

				// Draw Call
				if (const Ref<Model> model = meshComp->GetModel())
				{
					for (unsigned int i = 0; i < model->m_Meshes.size(); i++)
					{
						if (model->m_Meshes[i]->m_VAO && model->m_Meshes[i]->m_IBO)
						{
							model->m_Meshes[i]->m_VAO->Bind();
							model->m_Meshes[i]->m_IBO->Bind();
							glDrawElements(GL_TRIANGLES, model->m_Meshes[i]->m_IBO->GetIndexCount(), GL_UNSIGNED_INT, 0);
						}
					}
				}
			}

			// Draw Collision over gameobject if set to visible
			if (object->GetPhysicsComponent()->IsColliderVisible()) RenderCollider(object->GetPhysicsComponent());
		}
	}

	void RendererSubsystem::RenderWireframeViewport()
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

		Ref<Shader> shader = ResourceSubsystem::GetShader("UnlitShader");
		shader->Bind();

		// Upload the camera matrices
		if (const Ref<Camera> camera = s_RendererSubSystem->m_ActiveScene->m_ActiveCamera)
		{
			glUniformMatrix4fv(shader->GetUniform("u_Projection"), 1,
				GL_FALSE, glm::value_ptr(camera->GetProjectionMatrix()));

			glUniformMatrix4fv(shader->GetUniform("u_View"), 1,
				GL_FALSE, glm::value_ptr(camera->GetViewMatrix()));

			glUniform3f(shader->GetUniform("u_CameraPosition"),
				camera->GetTransformComponent()->GetPosition().x,
				camera->GetTransformComponent()->GetPosition().y,
				camera->GetTransformComponent()->GetPosition().z);
		}

		for (const Ref<GameObject>& object : s_RendererSubSystem->m_ActiveScene->m_SceneObjects)
		{
			const Ref<RenderComponent> renderComp = object->GetRenderComponent();
			const Ref<TransformComponent> transformComp = object->GetTransformComponent();
			const Ref<MeshComponent> meshComp = object->GetMeshComponent();

			if (!renderComp || !transformComp || !meshComp) continue;

			// Upload the model matrix
			glUniformMatrix4fv(shader->GetUniform("u_Model"), 1,
				GL_FALSE, glm::value_ptr(transformComp->GetModel()));

			// Upload the material
			glUniform3f(renderComp->m_Shader->GetUniform("u_Material.BaseColor"),1.0f, 1.0f, 1.0f);

			// Draw Call
			if (const Ref<Model> model = meshComp->GetModel())
			{
				for (unsigned int i = 0; i < model->m_Meshes.size(); i++)
				{
					if (model->m_Meshes[i]->m_VAO && model->m_Meshes[i]->m_IBO)
					{
						model->m_Meshes[i]->m_VAO->Bind();
						model->m_Meshes[i]->m_IBO->Bind();
						glDrawElements(GL_TRIANGLES, model->m_Meshes[i]->m_IBO->GetIndexCount(), GL_UNSIGNED_INT, 0);
					}
				}
			}
		}
	}

	void RendererSubsystem::RenderCollisionViewport()
	{
		

		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		Ref<Shader> shader = ResourceSubsystem::GetShader("DefaultShader");
		shader->Bind();

		// Upload the camera matrices relative to Object
		if (const Ref<Camera> camera = m_ActiveScene->m_ActiveCamera)
		{
			glUniformMatrix4fv(shader->GetUniform("u_Projection"), 1,
				GL_FALSE, glm::value_ptr(camera->GetProjectionMatrix()));

			glUniformMatrix4fv(shader->GetUniform("u_View"), 1,
				GL_FALSE, glm::value_ptr(camera->GetViewMatrix()));

			glUniform3f(shader->GetUniform("u_CameraPosition"),
				camera->GetTransformComponent()->GetPosition().x,
				camera->GetTransformComponent()->GetPosition().y,
				camera->GetTransformComponent()->GetPosition().z);
		}

		for (const Ref<GameObject>& object : m_ActiveScene->m_SceneObjects)
		{
			if (!object->GetPhysicsComponent()->GetCollider()) continue;

			const Ref<TransformComponent> transformComp = object->GetTransformComponent();
			const Ref<MeshComponent> meshComp = object->GetCollider()->GetMeshComponent();
			const Ref<PhysicsComponent> physComp = object->GetPhysicsComponent();

			// Upload the model matrix
			glUniformMatrix4fv(shader->GetUniform("u_Model"), 1,
				GL_FALSE, glm::value_ptr(transformComp->GetModel()));

			// Upload Affects Lighting bool
			glUniform1i(shader->GetUniform("u_AffectsLighting"), true);
			glUniform1i(shader->GetUniform("u_BaseColorAsTexture"), true);

			if (physComp->IsColliding())
			{
				glUniform3f(shader->GetUniform("u_Material.Base.Color"), 1.0f, 0.0f, 0.0f);
			}
			else
			{
				switch (object->GetTransformComponent()->GetMoveability())
				{
				case Moveability::Static:
				{
					glUniform3f(shader->GetUniform("u_Material.Base.Color"),
						m_StaticColliderColor.r, m_StaticColliderColor.g, m_StaticColliderColor.b);
				} break;

				case Moveability::Dynamic:
				{
					glUniform3f(shader->GetUniform("u_Material.Base.Color"),
						m_DynamicColliderColor.r, m_DynamicColliderColor.g, m_DynamicColliderColor.b);
				} break;
				}
			}

			// Draw Call
			if (const Ref<Model> model = meshComp->GetModel())
			{
				for (unsigned int i = 0; i < model->m_Meshes.size(); i++)
				{
					if (model->m_Meshes[i]->m_VAO && model->m_Meshes[i]->m_IBO)
					{
						model->m_Meshes[i]->m_VAO->Bind();
						model->m_Meshes[i]->m_IBO->Bind();
						glDrawElements(GL_TRIANGLES, model->m_Meshes[i]->m_IBO->GetIndexCount(), GL_UNSIGNED_INT, 0);
					}
				}
			}
		}

	}

	void RendererSubsystem::RenderLighting()
	{
		if (((ViewportMode)m_ViewportMode != ViewportMode::Default)) return;

		Ref<Shader> program = ResourceSubsystem::GetShader("DefaultShader");
		program->Bind();

		if (const Ref<DirectionalLight> dirLight = m_ActiveScene->m_DirLight)
		{
			const glm::vec3& lightDir = dirLight->GetLightDirection();
			const glm::vec3& lightColor = dirLight->GetLightColor();
			glUniform3f(program->GetUniform("u_DirLight.Base.Color"), lightColor.r, lightColor.g, lightColor.b);
			glUniform1f(program->GetUniform("u_DirLight.Base.AmbientIntensity"), dirLight->GetAmbientIntensity());
			glUniform1f(program->GetUniform("u_DirLight.Base.DiffuseIntensity"), dirLight->GetDiffuseIntensity());
			glUniform3f(program->GetUniform("u_DirLight.Direction"), lightDir.x, lightDir.y, lightDir.z);
		}

		glUniform1i(program->GetUniform("u_PointLightCount"), (int)m_ActiveScene->m_PointLights.size());
		glUniform1i(program->GetUniform("u_SpotLightCount"), (int)m_ActiveScene->m_SpotLights.size());


		for (int i = 0; i < (int)m_ActiveScene->m_PointLights.size(); i++)
		{
			const glm::vec3& lightCol = m_ActiveScene->m_PointLights[i]->GetLightColor();
			glUniform3f(program->GetUniform("u_PointLight[" + std::to_string(i) + "].Base.Color"), lightCol.r, lightCol.g, lightCol.b);
			glUniform1f(program->GetUniform("u_PointLight[" + std::to_string(i) + "].Base.AmbientIntensity"), m_ActiveScene->m_PointLights[i]->GetAmbientIntensity());
			glUniform1f(program->GetUniform("u_PointLight[" + std::to_string(i) + "].Base.DiffuseIntensity"), m_ActiveScene->m_PointLights[i]->GetDiffuseIntensity());

			const glm::vec3& pos = m_ActiveScene->m_PointLights[i]->GetTransformComponent()->GetPosition();
			glUniform3f(program->GetUniform("u_PointLight[" + std::to_string(i) + "].Position"), pos.x, pos.y, pos.z);
			glUniform1f(program->GetUniform("u_PointLight[" + std::to_string(i) + "].Constant"), m_ActiveScene->m_PointLights[i]->GetConstant());
			glUniform1f(program->GetUniform("u_PointLight[" + std::to_string(i) + "].Linear"), m_ActiveScene->m_PointLights[i]->GetLinear());
			glUniform1f(program->GetUniform("u_PointLight[" + std::to_string(i) + "].Exponent"), m_ActiveScene->m_PointLights[i]->GetExponent());
		}

		for (int i = 0; i < (int)m_ActiveScene->m_SpotLights.size(); i++)
		{
			const glm::vec3& lightCol = m_ActiveScene->m_SpotLights[i]->GetLightColor();
			glUniform3f(program->GetUniform("u_SpotLight[" + std::to_string(i) + "].Base.Base.Color"), lightCol.r, lightCol.g, lightCol.b);
			glUniform1f(program->GetUniform("u_SpotLight[" + std::to_string(i) + "].Base.Base.AmbientIntensity"), m_ActiveScene->m_SpotLights[i]->GetAmbientIntensity());
			glUniform1f(program->GetUniform("u_SpotLight[" + std::to_string(i) + "].Base.Base.DiffuseIntensity"), m_ActiveScene->m_SpotLights[i]->GetDiffuseIntensity());

			const glm::vec3& pos = m_ActiveScene->m_SpotLights[i]->GetTransformComponent()->GetPosition();
			glUniform3f(program->GetUniform("u_SpotLight[" + std::to_string(i) + "].Base.Position"), pos.x, pos.y, pos.z);
			glUniform1f(program->GetUniform("u_SpotLight[" + std::to_string(i) + "].Base.Constant"), m_ActiveScene->m_SpotLights[i]->GetConstant());
			glUniform1f(program->GetUniform("u_SpotLight[" + std::to_string(i) + "].Base.Linear"), m_ActiveScene->m_SpotLights[i]->GetLinear());
			glUniform1f(program->GetUniform("u_SpotLight[" + std::to_string(i) + "].Base.Exponent"), m_ActiveScene->m_SpotLights[i]->GetExponent());

			const glm::vec3& dir = m_ActiveScene->m_SpotLights[i]->GetDirection();
			glUniform3f(program->GetUniform("u_SpotLight[" + std::to_string(i) + "].Direction"), dir.x, dir.y, dir.z);
			glUniform1f(program->GetUniform("u_SpotLight[" + std::to_string(i) + "].Edge"), m_ActiveScene->m_SpotLights[i]->GetProcessedEdge());
		}
	}

	void RendererSubsystem::RenderCollider(const Ref<PhysicsComponent>& _component)
	{
		if (const Ref<Collider> collider = _component->GetCollider())
		{
			Ref<Shader> shader = ResourceSubsystem::GetShader("DefaultShader");
			shader->Bind();

			// Upload the model matrix
			glUniformMatrix4fv(shader->GetUniform("u_Model"), 1,
				GL_FALSE, glm::value_ptr(collider->GetTransformComponent()->GetModel()));

			// Upload Affects Lighting bool
			glUniform1i(shader->GetUniform("u_AffectsLighting"), false);
			glUniform1i(shader->GetUniform("u_Material.Base.IsTexture"), false);

			constexpr static glm::vec3 colColor = { 1.0f, 0.0f, 0.0f };
			constexpr static glm::vec3 noColColor = { 0.0f, 1.0f, 0.0f };

			if (_component->IsColliding())
			{
				glUniform3f(shader->GetUniform("u_Material.Base.Color"), colColor[0], colColor[1], colColor[2]);
			}
			else
			{
				glUniform3f(shader->GetUniform("u_Material.Base.Color"), noColColor[0], noColColor[1], noColColor[2]);
			}

			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			// Draw Call
			if (const Ref<Model> model = _component->GetCollider()->GetMeshComponent()->GetModel())
			{
				for (unsigned int i = 0; i < model->m_Meshes.size(); i++)
				{
					if (model->m_Meshes[i]->m_VAO && model->m_Meshes[i]->m_IBO)
					{
						model->m_Meshes[i]->m_VAO->Bind();
						model->m_Meshes[i]->m_IBO->Bind();
						glDrawElements(GL_TRIANGLES, model->m_Meshes[i]->m_IBO->GetIndexCount(), GL_UNSIGNED_INT, 0);
					}
				}
			}
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}
	}

	void RendererSubsystem::SetActiveScene(const Ref<Scene>& _scene)
	{
		s_RendererSubSystem->m_ActiveScene = _scene;
	}
}