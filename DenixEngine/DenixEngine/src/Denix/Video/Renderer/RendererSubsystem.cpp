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

		default: DE_LOG(LogScene, Error, "Invalid Viewport");
		}

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

			if (renderComp->IsVisible())
			{
				renderComp->m_Shader->Bind();
				meshComp->m_Mesh->m_VAO->Bind();
				meshComp->m_Mesh->m_IBO->Bind();

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
				glUniform1i(renderComp->m_Shader->GetUniform("u_BaseColorAsTexture"), renderComp->GetBaseColorAsTexture());

				// Upload the texture
				if (!renderComp->GetBaseColorAsTexture() && renderComp->m_Texture)
				{
					renderComp->m_Texture->Bind();

					glTexParameteri(renderComp->m_Texture->GetTarget(), GL_TEXTURE_WRAP_S, renderComp->m_TextureSettings.WrapMode);
					glTexParameteri(renderComp->m_Texture->GetTarget(), GL_TEXTURE_WRAP_T, renderComp->m_TextureSettings.WrapMode);
					glTexParameteri(renderComp->m_Texture->GetTarget(), GL_TEXTURE_MIN_FILTER, renderComp->m_TextureSettings.FilterMode);
					glTexParameteri(renderComp->m_Texture->GetTarget(), GL_TEXTURE_MAG_FILTER, renderComp->m_TextureSettings.FilterMode);
				}

				// Upload the material
				if (const Ref<Material> mat = renderComp->m_Material)
				{
					if (renderComp->GetBaseColorAsTexture())
					{
						const glm::vec3& baseColor = mat->GetBaseColor();
						glUniform3f(renderComp->m_Shader->GetUniform("u_Material.BaseColor"),
							baseColor.r, baseColor.g, baseColor.b);
					}

					glUniform1f(renderComp->m_Shader->GetUniform("u_Material.SpecularIntensity"), mat->GetSpecularIntensity());
					glUniform1f(renderComp->m_Shader->GetUniform("u_Material.SpecularPower"), mat->GetSpecularPower());
				}

				glDrawElements(GL_TRIANGLES, meshComp->m_Mesh->m_IBO->GetIndexCount(), GL_UNSIGNED_INT, 0);
			}

			// Draw Collision over gameobject if set to visible
			if (const Ref<Collider> collider = object->GetCollider())
			{
				if(const Ref<RenderComponent> rendComp = collider->GetRenderComponent(); 
					collider->GetRenderComponent()->IsVisible())
				{
					rendComp->m_Shader->Bind();
					collider->GetMeshComponent()->m_Mesh->m_VAO->Bind();
					collider->GetMeshComponent()->m_Mesh->m_IBO->Bind();


					// Upload the model matrix
						glUniformMatrix4fv(rendComp->m_Shader->GetUniform("u_Model"), 1,
							GL_FALSE, glm::value_ptr(collider->GetTransformComponent()->GetModel()));

							// Upload Affects Lighting bool
						glUniform1i(rendComp->m_Shader->GetUniform("u_AffectsLighting"), false);
					glUniform1i(rendComp->m_Shader->GetUniform("u_BaseColorAsTexture"), true);

					if (const Ref<Material> mat = rendComp->m_Material)
					{
						if (const Ref<PhysicsComponent> physComp = object->GetPhysicsComponent())
						{
							if (physComp->IsColliding())
							{
								glUniform3f(rendComp->m_Shader->GetUniform("u_Material.BaseColor"), 1.0f, 0.0f, 0.0f);

							}
							else
							{
								glUniform3f(rendComp->m_Shader->GetUniform("u_Material.BaseColor"), 0.0f, 1.0f, 0.0f);

							}
						}
					}

					glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
					glDrawElements(GL_TRIANGLES, collider->GetMeshComponent()->m_Mesh->m_IBO->GetIndexCount(), GL_UNSIGNED_INT, 0);
					glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
				}
			}
		}
	}

	void RendererSubsystem::RenderUnlitViewport()
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		if (const Ref<DirectionalLight> dirLight = m_ActiveScene->m_DirLight)
		{
			Ref<GLShader> program = ResourceSubsystem::GetShader("DefaultShader");
			program->Bind();

			const glm::vec3& lightColor = dirLight->GetLightColor();
			glUniform3f(program->GetUniform("u_DirLight.Base.Color"), 1.0f,1.0f,1.0f);
		}

		for (const Ref<GameObject>& object : s_RendererSubSystem->m_ActiveScene->m_SceneObjects)
		{
			const Ref<RenderComponent> renderComp = object->GetRenderComponent();
			const Ref<TransformComponent> transformComp = object->GetTransformComponent();
			const Ref<MeshComponent> meshComp = object->GetMeshComponent();

			if (!renderComp || !transformComp || !meshComp) continue;

			if (object->GetRenderComponent()->IsVisible())
			{
				renderComp->m_Shader->Bind();
				meshComp->m_Mesh->m_VAO->Bind();
				meshComp->m_Mesh->m_IBO->Bind();

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
				glUniform1i(renderComp->m_Shader->GetUniform("u_AffectsLighting"), false);
				glUniform1i(renderComp->m_Shader->GetUniform("u_BaseColorAsTexture"), renderComp->GetBaseColorAsTexture());

				// Upload the texture
				if (!renderComp->GetBaseColorAsTexture() && renderComp->m_Texture)
				{
					renderComp->m_Texture->Bind();

					glTexParameteri(renderComp->m_Texture->GetTarget(), GL_TEXTURE_WRAP_S, renderComp->m_TextureSettings.WrapMode);
					glTexParameteri(renderComp->m_Texture->GetTarget(), GL_TEXTURE_WRAP_T, renderComp->m_TextureSettings.WrapMode);
					glTexParameteri(renderComp->m_Texture->GetTarget(), GL_TEXTURE_MIN_FILTER, renderComp->m_TextureSettings.FilterMode);
					glTexParameteri(renderComp->m_Texture->GetTarget(), GL_TEXTURE_MAG_FILTER, renderComp->m_TextureSettings.FilterMode);
				}

				// Upload the material
				if (const Ref<Material> mat = renderComp->m_Material)
				{
					glUniform3f(renderComp->m_Shader->GetUniform("u_Material.BaseColor"),
						1.0f, 1.0f, 1.0f);
				}

				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
				glDrawElements(GL_TRIANGLES, meshComp->m_Mesh->m_IBO->GetIndexCount(), GL_UNSIGNED_INT, 0);
			}

			// Draw Collision over gameobject if set to visible
			if (const Ref<Collider> collider = object->GetCollider(); collider->GetRenderComponent()->IsVisible())
			{
				Ref<RenderComponent> colRenderComp = collider->GetRenderComponent();
				if (object->GetRenderComponent()->IsVisible())
				{
					colRenderComp->m_Shader->Bind();
					meshComp->m_Mesh->m_VAO->Bind();
					meshComp->m_Mesh->m_IBO->Bind();


					// Upload the model matrix
					glUniformMatrix4fv(colRenderComp->m_Shader->GetUniform("u_Model"), 1,
						GL_FALSE, glm::value_ptr(transformComp->GetModel()));

					// Upload Affects Lighting bool
					glUniform1i(colRenderComp->m_Shader->GetUniform("u_AffectsLighting"), false);
					glUniform1i(colRenderComp->m_Shader->GetUniform("u_BaseColorAsTexture"), true);

					if (const Ref<Material> mat = colRenderComp->m_Material)
					{
						glUniform3f(colRenderComp->m_Shader->GetUniform("u_Material.BaseColor"),
							mat->GetBaseColor().r, mat->GetBaseColor().g, mat->GetBaseColor().b);
					}

					glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
					glDrawElements(GL_TRIANGLES, meshComp->m_Mesh->m_IBO->GetIndexCount(), GL_UNSIGNED_INT, 0);
					glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
				}
			}
		}
	}

	void RendererSubsystem::RenderWireframeViewport()
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

		for (const Ref<GameObject>& object : s_RendererSubSystem->m_ActiveScene->m_SceneObjects)
		{
			const Ref<RenderComponent> _renderComp = object->GetRenderComponent();
			const Ref<TransformComponent> _transformComp = object->GetTransformComponent();
			const Ref<MeshComponent> _meshComp = object->GetMeshComponent();

			if (!_renderComp || !_transformComp || !_meshComp) continue;

			_renderComp->m_Shader->Bind();
			_meshComp->m_Mesh->m_VAO->Bind();
			_meshComp->m_Mesh->m_IBO->Bind();

			// Upload the camera matrices relative to Object
			if (const Ref<Camera> camera = s_RendererSubSystem->m_ActiveScene->m_ActiveCamera)
			{
				glUniformMatrix4fv(_renderComp->m_Shader->GetUniform("u_Projection"), 1,
					GL_FALSE, glm::value_ptr(camera->GetProjectionMatrix()));

				glUniformMatrix4fv(_renderComp->m_Shader->GetUniform("u_View"), 1,
					GL_FALSE, glm::value_ptr(camera->GetViewMatrix()));

				glUniform3f(_renderComp->m_Shader->GetUniform("u_CameraPosition"),
					camera->GetTransformComponent()->GetPosition().x,
					camera->GetTransformComponent()->GetPosition().y,
					camera->GetTransformComponent()->GetPosition().z);
			}

			// Upload the model matrix
			glUniformMatrix4fv(_renderComp->m_Shader->GetUniform("u_Model"), 1,
				GL_FALSE, glm::value_ptr(_transformComp->GetModel()));

			// Upload Affects Lighting bool
			glUniform1i(_renderComp->m_Shader->GetUniform("u_AffectsLighting"), false);
			glUniform1i(_renderComp->m_Shader->GetUniform("u_BaseColorAsTexture"), true);

			glUniform3f(_renderComp->m_Shader->GetUniform("u_Material.BaseColor"),
				m_WireframeColor.r, m_WireframeColor.g, m_WireframeColor.b);

			glDrawElements(GL_TRIANGLES, _meshComp->m_Mesh->m_IBO->GetIndexCount(), GL_UNSIGNED_INT, 0);
		}
	}

	void RendererSubsystem::RenderCollisionViewport()
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		for (const Ref<GameObject>& object : s_RendererSubSystem->m_ActiveScene->m_SceneObjects)
		{

			const Ref<RenderComponent> renderComp = object->GetCollider()->GetRenderComponent();
			const Ref<TransformComponent> transformComp = object->GetTransformComponent();
			const Ref<MeshComponent> meshComp = object->GetCollider()->GetMeshComponent();

			if (!renderComp || !transformComp || !meshComp) continue;

			renderComp->m_Shader->Bind();
			meshComp->m_Mesh->m_VAO->Bind();
			meshComp->m_Mesh->m_IBO->Bind();

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
			glUniform1i(renderComp->m_Shader->GetUniform("u_AffectsLighting"), true);
			glUniform1i(renderComp->m_Shader->GetUniform("u_BaseColorAsTexture"), true);

			if (const Ref<PhysicsComponent> physComp = object->GetPhysicsComponent())
			{
				if (!physComp->IsColliding())
				{
					switch (object->GetTransformComponent()->GetMoveability())
					{
					case Moveability::Static:
					{
						glUniform3f(renderComp->m_Shader->GetUniform("u_Material.BaseColor"),
							m_StaticColliderColor.r, m_StaticColliderColor.g, m_StaticColliderColor.b);
					} break;

					case Moveability::Dynamic:
					{
						glUniform3f(renderComp->m_Shader->GetUniform("u_Material.BaseColor"),
							m_DynamicColliderColor.r, m_DynamicColliderColor.g, m_DynamicColliderColor.b);
					} break;

					default: DE_LOG(LogScene, Error, "Invalid Moveability") break;
					}
				}
				else
				{
					glUniform3f(renderComp->m_Shader->GetUniform("u_Material.BaseColor"), 1.0f, 0.0f, 0.0f);
				}
			}

			glDrawElements(GL_TRIANGLES, meshComp->m_Mesh->m_IBO->GetIndexCount(), GL_UNSIGNED_INT, 0);
		}
	}

	void RendererSubsystem::RenderLighting()
	{
		if (((ViewportMode)m_ViewportMode != ViewportMode::Default)) return;

		Ref<GLShader> program = ResourceSubsystem::GetShader("DefaultShader");
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

	void RendererSubsystem::SetActiveScene(const Ref<Scene>& _scene)
	{
		s_RendererSubSystem->m_ActiveScene = _scene;
	}
}