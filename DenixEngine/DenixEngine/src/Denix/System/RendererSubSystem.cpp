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
		if(_render && _transform && _mesh)
		{
			_render->m_Shader->Bind();
			_mesh->m_VAO->Bind();

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

			glDrawArrays(_render->GetDrawMode(), 0, _mesh->m_VBO->GetCount());

			//m_RendererComponents.emplace_back(_render, _transform, _mesh);

			VertexArray::Unbind();
			GLShader::Unbind();
		}
	}

	void RendererSubsystem::SetActiveCamera(const Ref<Camera>& _camera)
	{
		m_ActiveCamera = _camera;
	}
}
