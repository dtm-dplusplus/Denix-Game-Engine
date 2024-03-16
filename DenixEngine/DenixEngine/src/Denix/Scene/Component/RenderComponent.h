#pragma once
#include <glm/vec4.hpp>

#include "Denix/Scene/Component.h"
#include "Denix/System/ShaderSubSystem.h"
#include "Denix/Video/GL/GLShader.h"


namespace Denix
{
	class RenderComponent : public Component
	{
	public:
		RenderComponent() : Component(ObjectInitializer("Render Component"))
		{
			if (!m_Shader)
			{
				if (const Ref<GLShader> shader = ShaderSubSystem::Get()->GetShader("DebugShader"))
				{
					m_Shader = shader;
				}
			}
		}

		RenderComponent(const GLint _parentID) : Component(_parentID, ObjectInitializer("Render Component"))
		{
			if (!m_Shader)
			{
				if (const Ref<GLShader> shader = ShaderSubSystem::Get()->GetShader("DebugShader"))
				{
					m_Shader = shader;
				}
			}
		}

		~RenderComponent() override = default;

		void BeginScene() override;

		void EndScene() override;

		void RegisterComponent() override;
		void UnregisterComponent() override;
		Ref<GLShader> GetShader() const { return m_Shader; }
	public:
		glm::vec4 GetDebugColor() const { return m_DebugColor; }
		glm::vec4& GetDebugColor() { return m_DebugColor; }
		glm::vec4 SetDebugColor(const glm::vec4& _color) { return m_DebugColor = _color; }

	private:
		glm::vec4 m_DebugColor = glm::vec4(0.98f, 1.f, 1.f, 1.f);
		Ref<GLShader> m_Shader;

		friend class SceneSubSystem;
		friend class RendererSubSystem;
	};
}
