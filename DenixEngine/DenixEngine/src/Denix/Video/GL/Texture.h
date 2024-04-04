#pragma once

#include "Denix/Core.h"
#include <string>
#include <GL/glew.h>
#include <glm/vec2.hpp>
#include <glm/vec4.hpp>

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
			m_TextureName = "";
			m_Width = 0;
			m_Height = 0;
			m_BitDepth = 0;
			m_FilePath = "";
		}

		Texture(const std::string& _path, const std::string& _name)
		{
			m_TextureID = 0;
			m_TextureName = _name;
			m_Width = 0;
			m_Height = 0;
			m_BitDepth = 0;
			m_FilePath = _path;
		}

        bool LoadTexture();

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
			m_FilePath = "";
		}

		GLuint GetTextureID() const { return m_TextureID; }
		GLenum GetTarget() const { return m_Target; }

		std::string GetTextureName() const { return m_TextureName; }

		TextureSettings GetSettings() const { return m_Settings; }
		int GetWidth() const { return m_Width; }
		int GetHeight() const { return m_Height; }
		glm::vec2 GetSize() const { return { m_Width, m_Height }; }

		std::string GetFileLocation() const { return m_FilePath; }

		glm::vec4 m_BaseColor = glm::vec4(1.f, 1.f, 1.f, 1.f);

	private:
		GLuint m_TextureID;
		std::string m_TextureName;

		GLenum m_Target = GL_TEXTURE_2D;
		int m_Width, m_Height, m_BitDepth;

		TextureSettings m_Settings;

		std::string m_FilePath;
	};
}

