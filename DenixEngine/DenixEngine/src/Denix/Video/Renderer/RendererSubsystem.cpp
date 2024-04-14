#include "depch.h"
#include "RendererSubsystem.h"

#include "Denix/Scene/Camera.h"
#include "Denix/Scene/Scene.h"
#include "Denix/Scene/Component/TransformComponent.h"
#include "Denix/Video/GL/MeshComponent.h"
#include "Denix/Video/Renderer/RenderComponent.h"

namespace Denix
{
	RendererSubsystem* RendererSubsystem::s_RendererSubSystem{ nullptr };

	// This Render function is far too complex and a lot of this stuff should be set prior to rendering the draw command.
	// Perhaps we call a render pass function, then call draw.
	void RendererSubsystem::DrawImmediate(const Ref<RenderComponent>& _render, const Ref<TransformComponent>& _transform, const Ref<MeshComponent>& _mesh)
	{
		// Global check to see if the renderer is active - Useful for debugging
		if(!m_Active) return;

		// Begin rendering
		if(_render && _transform && _mesh)
		{
			_render->m_Shader->Bind();
			_mesh->m_VAO->Bind();
			_mesh->m_IBO->Bind();

			// Upload the model matrix
			glUniformMatrix4fv(_render->m_Shader->GetUniform("u_Model"), 1, 
				GL_FALSE, glm::value_ptr(_transform->GetModel()));

			// Upload Affects Lighting bool
			glUniform1i(_render->m_Shader->GetUniform("u_AffectsLighting"), _render->m_AffectsLighting);
			glUniform1i(_render->m_Shader->GetUniform("u_BaseColorAsTexture"), _render->GetBaseColorAsTexture());


			// Upload the camera matrices relative to Object
			if (Ref<Camera> camera = m_ActiveScene->m_ActiveCamera)
			{
				glUniformMatrix4fv(_render->m_Shader->GetUniform("u_Projection"), 1, 
					GL_FALSE, glm::value_ptr(camera->GetProjectionMatrix()));

				glUniformMatrix4fv(_render->m_Shader->GetUniform("u_View"), 1, 
					GL_FALSE, glm::value_ptr(camera->GetViewMatrix()));

				glUniform3f(_render->m_Shader->GetUniform("u_CameraPosition"), 
					camera->GetTransformComponent()->GetPosition().x,
					camera->GetTransformComponent()->GetPosition().y,
					camera->GetTransformComponent()->GetPosition().z);
			}

			// TODO This is slow, but as the same texture can be applied to multiple objects, it may have different settigns
			// So we override the settings here
			if (_render->m_Texture)
			{
				_render->m_Texture->Bind();

				glTexParameteri(_render->m_Texture->GetTarget(), GL_TEXTURE_WRAP_S, _render->m_TextureSettings.WrapMode);
				glTexParameteri(_render->m_Texture->GetTarget(), GL_TEXTURE_WRAP_T, _render->m_TextureSettings.WrapMode);
				glTexParameteri(_render->m_Texture->GetTarget(), GL_TEXTURE_MIN_FILTER, _render->m_TextureSettings.FilterMode);
				glTexParameteri(_render->m_Texture->GetTarget(), GL_TEXTURE_MAG_FILTER, _render->m_TextureSettings.FilterMode);
			}


			if(const Ref<Material> mat = _render->m_Material)
			{
				glUniform3f(_render->m_Shader->GetUniform("u_Material.BaseColor"), 
					mat->GetBaseColor().r,mat->GetBaseColor().g,mat->GetBaseColor().b);
				glUniform1f(_render->m_Shader->GetUniform("u_Material.SpecularIntensity"), mat->GetSpecularIntensity());
				glUniform1f(_render->m_Shader->GetUniform("u_Material.SpecularPower"), mat->GetSpecularPower());
			}

			// Draw the indexed mesh
			glDrawElements(GL_TRIANGLES, _mesh->m_IBO->GetIndexCount(), GL_UNSIGNED_INT, 0);

			if (_render->m_Texture) _render->m_Texture->Unbind();
			IndexBuffer::Unbind();
			VertexArray::Unbind();
			GLShader::Unbind();
		}
	}

	void RendererSubsystem::SetActiveScene(const Ref<Scene>& _scene)
	{
		m_ActiveScene = _scene;
	}
}
