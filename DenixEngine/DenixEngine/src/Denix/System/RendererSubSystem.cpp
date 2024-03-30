#include "depch.h"
#include "RendererSubsystem.h"

#include "Denix/Scene/Camera.h"
#include "Denix/Scene/Component/TransformComponent.h"
#include "Denix/Scene/Component/MeshComponent.h"
#include "Denix/Scene/Component/RenderComponent.h"

namespace Denix
{
	RendererSubsystem* RendererSubsystem::s_RendererSubSystem{ nullptr };

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

			// Upload the model matrix
			glUniformMatrix4fv(_render->m_Shader->GetUniform("u_Model"), 1, 
				GL_FALSE, glm::value_ptr(_transform->GetModel()));

			if (m_ActiveCamera)
			{
				// Upload the projection matrix
				glUniformMatrix4fv(_render->m_Shader->GetUniform("u_Projection"), 1, 
					GL_FALSE, glm::value_ptr(m_ActiveCamera->GetProjectionMatrix()));

				// Upload the view matrix
				glUniformMatrix4fv(_render->m_Shader->GetUniform("u_View"), 1, 
					GL_FALSE, glm::value_ptr(m_ActiveCamera->GetViewMatrix()));
			}

			// Upload the color
			glUniform4fv(_render->m_Shader->GetUniform("u_Color"), 1, &_render->GetDebugColor()[0]);

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
