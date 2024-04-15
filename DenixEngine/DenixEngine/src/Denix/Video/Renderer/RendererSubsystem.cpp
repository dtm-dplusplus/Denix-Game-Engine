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
	void RendererSubsystem::DrawImmediate(const Ref<RenderComponent>& _renderComp, const Ref<TransformComponent>& _transformComp, const Ref<MeshComponent>& _meshComp)
	{
		// Global check to see if the renderer is active - Useful for debugging
		if(!m_Enabled) return;

		// Check if the components are valid
		if (!_renderComp)

		// Check Render Component is visible
		if (!_renderComp->IsVisible()) return;

		// Check Other Components
		if(!_transformComp && !_meshComp) return;

		_renderComp->m_Shader->Bind();
		_meshComp->m_Mesh->m_VAO->Bind();
		_meshComp->m_Mesh->m_IBO->Bind();

			
		// Upload the model matrix
		glUniformMatrix4fv(_renderComp->m_Shader->GetUniform("u_Model"), 1, 
			GL_FALSE, glm::value_ptr(_transformComp->GetModel()));

		// Upload Affects Lighting bool
		glUniform1i(_renderComp->m_Shader->GetUniform("u_AffectsLighting"), _renderComp->m_AffectsLighting);
		glUniform1i(_renderComp->m_Shader->GetUniform("u_BaseColorAsTexture"), _renderComp->GetBaseColorAsTexture());


		// Upload the camera matrices relative to Object
		if (const Ref<Camera> camera = m_ActiveScene->m_ActiveCamera)
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

		// TODO This is slow, but as the same texture can be applied to multiple objects, it may have different settigns
		// So we override the settings here
		if (_renderComp->m_Texture)
		{
			_renderComp->m_Texture->Bind();

			glTexParameteri(_renderComp->m_Texture->GetTarget(), GL_TEXTURE_WRAP_S, _renderComp->m_TextureSettings.WrapMode);
			glTexParameteri(_renderComp->m_Texture->GetTarget(), GL_TEXTURE_WRAP_T, _renderComp->m_TextureSettings.WrapMode);
			glTexParameteri(_renderComp->m_Texture->GetTarget(), GL_TEXTURE_MIN_FILTER, _renderComp->m_TextureSettings.FilterMode);
			glTexParameteri(_renderComp->m_Texture->GetTarget(), GL_TEXTURE_MAG_FILTER, _renderComp->m_TextureSettings.FilterMode);
		}

		if(const Ref<Material> mat = _renderComp->m_Material)
		{
			glUniform3f(_renderComp->m_Shader->GetUniform("u_Material.BaseColor"), 
				mat->GetBaseColor().r,mat->GetBaseColor().g,mat->GetBaseColor().b);
			glUniform1f(_renderComp->m_Shader->GetUniform("u_Material.SpecularIntensity"), mat->GetSpecularIntensity());
			glUniform1f(_renderComp->m_Shader->GetUniform("u_Material.SpecularPower"), mat->GetSpecularPower());
		}

		glDrawElements(GL_TRIANGLES, _meshComp->m_Mesh->m_IBO->GetIndexCount(), GL_UNSIGNED_INT, 0);

			
		if (_renderComp->m_Texture) _renderComp->m_Texture->Unbind();
		IndexBuffer::Unbind();
		VertexArray::Unbind();
		GLShader::Unbind();
	}

	void RendererSubsystem::SetActiveScene(const Ref<Scene>& _scene)
	{
		m_ActiveScene = _scene;
	}
}
