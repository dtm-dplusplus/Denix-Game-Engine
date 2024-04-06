#include "depch.h"
#include "RendererSubsystem.h"

#include "Denix/Scene/Camera.h"
#include "Denix/Scene/Component/TransformComponent.h"
#include "Denix/Scene/Component/MeshComponent.h"
#include "Denix/Scene/Component/RenderComponent.h"

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
			if (!_render->m_AffectsLighting)
			{
				const glm::vec4& baseCol = _render->m_Texture->m_BaseColor;
				glUniform4f(_render->m_Shader->GetUniform("u_BaseColor"), baseCol.r, baseCol.g, baseCol.b, baseCol.a);

			}

			// Upload the camera matrices relative to Object
			if (m_ActiveCamera)
			{
				glUniformMatrix4fv(_render->m_Shader->GetUniform("u_Projection"), 1, 
					GL_FALSE, glm::value_ptr(m_ActiveCamera->GetProjectionMatrix()));

				glUniformMatrix4fv(_render->m_Shader->GetUniform("u_View"), 1, 
					GL_FALSE, glm::value_ptr(m_ActiveCamera->GetViewMatrix()));

				glUniform3f(_render->m_Shader->GetUniform("u_CameraPosition"), 
										m_ActiveCamera->GetTransformComponent()->GetPosition().x,
										m_ActiveCamera->GetTransformComponent()->GetPosition().y,
										m_ActiveCamera->GetTransformComponent()->GetPosition().z);	
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


			if(_render->m_Material)
			{
				_render->m_Material->UseMaterial(_render->m_Shader->GetUniform("u_Material.SpecularPower"), _render->m_Shader->GetUniform("u_Material.SpecularIntensity"));
			}

			// Draw the indexed mesh
			glDrawElements(GL_TRIANGLES, _mesh->m_IBO->GetIndexCount(), GL_UNSIGNED_INT, 0);

			if (_render->m_Texture) _render->m_Texture->Unbind();
			IndexBuffer::Unbind();
			VertexArray::Unbind();
			GLShader::Unbind();
		}
	}

	void RendererSubsystem::SetActiveCamera(const Ref<Camera>& _camera)
	{
		m_ActiveCamera = _camera;
	}
}
