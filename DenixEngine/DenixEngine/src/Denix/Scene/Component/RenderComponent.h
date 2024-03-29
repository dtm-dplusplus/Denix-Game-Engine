#pragma once
#include <glm/vec4.hpp>

#include "Denix/Scene/Component.h"
#include "Denix/System/ShaderSubsystem.h"
#include "Denix/Video/GL/GLShader.h"



namespace Denix
{
	class Texture
	{
	public:
		Texture()
		{
			textureID = 0;
			width = 0;
			height = 0;
			bitDepth = 0;
			fileLocation = "";
		}

		Texture(const char* fileLoc)
		{
			textureID = 0;
			width = 0;
			height = 0;
			bitDepth = 0;
			fileLocation = fileLoc;
		}

		void LoadTexture();

		void Bind()
		{
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, textureID);
		}

		void Unbind()
		{
			glBindTexture(GL_TEXTURE_2D, 0);
		}
		void ClearTexture()
		{
			glDeleteTextures(1, &textureID);
			textureID = 0;
			width = 0;
			height = 0;
			bitDepth = 0;
			fileLocation = "";
		}

		GLuint GetID() const { return textureID; }

		GLuint textureID;
		int width, height, bitDepth;

		const char* fileLocation;
	};

	class RenderComponent : public Component
	{
	public:
		RenderComponent() : Component(ObjectInitializer("Render Component"))
		{
			if (!m_Shader)
			{
				if (const Ref<GLShader> shader = ShaderSubsystem::Get()->GetShader("DebugShader"))
				{
					m_Shader = shader;
				}
			}
		}

		RenderComponent(const GLint _parentID) : Component(_parentID, ObjectInitializer("Render Component"))
		{
			if (!m_Shader)
			{
				if (const Ref<GLShader> shader = ShaderSubsystem::Get()->GetShader("DebugShader"))
				{
					m_Shader = shader;
				}
			}
		}

		~RenderComponent() override = default;

		void LoadTexture(const std::string& _path)
		{
			m_Texture = MakeRef<Texture>(_path.c_str());
			m_Texture->LoadTexture();
		}

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
		void SetShader(const Ref<GLShader>& _shader) { m_Shader = _shader; }

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
		Ref<Texture> m_Texture;

		// Null effect atm as we are overriding with glPolygonMode globally
		int m_DrawMode = GL_TRIANGLES;

		friend class SceneSubsystem;
		friend class RendererSubsystem;
	};
}
