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

		enum class DrawMode
		{
			POINTS = GL_POINTS,
			LINES = GL_LINES,
			LINE_LOOP = GL_LINE_LOOP,
			LINE_STRIP = GL_LINE_STRIP,
			TRIANGLES = GL_TRIANGLES
			/*TRIANGLE_STRIP = GL_TRIANGLE_STRIP,
			TRIANGLE_FAN = GL_TRIANGLE_FAN*/
		};

		Ref<GLShader> GetShader() const { return m_Shader; }

		glm::vec4 GetDebugColor() const { return m_DebugColor; }
		glm::vec4& GetDebugColor() { return m_DebugColor; }
		glm::vec4 SetDebugColor(const glm::vec4& _color) { return m_DebugColor = _color; }

		GLenum GetDrawMode() const { return static_cast<GLenum>(m_DrawMode); }
		int& GetDrawMode() { return m_DrawMode; }
		void SetDrawMode(const GLenum _mode) 
		{
			m_DrawMode = _mode;
		}	

		bool IsVisible() const { return m_IsVisible; }
		bool& IsVisible() { return m_IsVisible; }
		void SetIsVisible(const bool _visible) { m_IsVisible = _visible; }
	public:
		void BeginScene() override;

		void EndScene() override;

		void RegisterComponent() override;
		void UnregisterComponent() override;
		

	private:
		bool m_IsVisible = true;

		glm::vec4 m_DebugColor = glm::vec4(0.98f, 1.f, 1.f, 1.f);
		Ref<GLShader> m_Shader;
		int m_DrawMode = GL_TRIANGLES;
		friend class SceneSubSystem;
		friend class RendererSubSystem;
	};
}
