#pragma once
#include <glm/vec2.hpp>
#include <glm/vec4.hpp>

#include "Denix/Scene/Component.h"
#include "Denix/System/ShaderSubsystem.h"
#include "Denix/Video/GL/GLShader.h"



namespace Denix
{
	struct TextureSettings
	{
		GLint WrapMode = GL_REPEAT;
		int WrapValue = 0; // ImGui Combo Index

		GLint FilterMode = GL_LINEAR;
		int FilterValue = 1; // ImGui Combo Index
	};

	class Texture
	{
	public:
		Texture()
		{
			m_TextureID = 0;
			m_Width = 0;
			m_Height = 0;
			m_BitDepth = 0;
			m_FileLocation = "";
		}

		Texture(const std::string& _fileLoc)
		{
			m_TextureID = 0;
			m_Width = 0;
			m_Height = 0;
			m_BitDepth = 0;
			m_FileLocation = _fileLoc;
		}

		void LoadTexture();

		void Bind() const
		{
			glActiveTexture(GL_TEXTURE0); // TODO This unit may change in the future
			glBindTexture(m_Target, m_TextureID);
		}

		void Unbind() const
		{
			glBindTexture(m_Target, 0);
		}

		void ClearTexture()
		{
			glDeleteTextures(1, &m_TextureID);
			m_TextureID = 0;
			m_Width = 0;
			m_Height = 0;
			m_BitDepth = 0;
			m_FileLocation = "";
		}

		GLuint GetTextureID() const { return m_TextureID; }
		GLenum GetTarget() const { return m_Target; }

		int GetWidth() const { return m_Width; }
		int GetHeight() const { return m_Height; }
		glm::vec2 GetSize() const { return {m_Width, m_Height}; }

		std::string GetFileLocation() const { return m_FileLocation; }

	private:
		GLuint m_TextureID;
		GLenum m_Target = GL_TEXTURE_2D;
		int m_Width, m_Height, m_BitDepth;

		std::string m_FileLocation;
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
			m_Texture = MakeRef<Texture>(_path);
			m_Texture->LoadTexture();

			if (!m_Texture->GetTextureID()) return;

			m_Texture->Bind();

			glTexParameteri(m_Texture->GetTarget(), GL_TEXTURE_WRAP_S, m_TextureSettings.WrapMode);
			glTexParameteri(m_Texture->GetTarget(), GL_TEXTURE_WRAP_T, m_TextureSettings.WrapMode);
			glTexParameteri(m_Texture->GetTarget(), GL_TEXTURE_MIN_FILTER, m_TextureSettings.FilterMode);
			glTexParameteri(m_Texture->GetTarget(), GL_TEXTURE_MAG_FILTER, m_TextureSettings.FilterMode);

			m_Texture->Unbind();
		}

		Ref<Texture> GetTexture() const { return m_Texture; }

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
		
		TextureSettings& GetTextureSettings() { return m_TextureSettings; }

	private:
		bool m_IsVisible = true;

		glm::vec4 m_DebugColor = glm::vec4(0.98f, 1.f, 1.f, 1.f);
		Ref<GLShader> m_Shader;
		Ref<Texture> m_Texture;

		// Null effect atm as we are overriding with glPolygonMode globally
		int m_DrawMode = GL_TRIANGLES;

		// Texture
		TextureSettings m_TextureSettings;
		

		friend class SceneSubsystem;
		friend class RendererSubsystem;
	};
}
