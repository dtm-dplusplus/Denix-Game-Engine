#include "Texture.h"

#include <filesystem>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

namespace Denix
{
	Texture::Texture(const std::string& _path, bool _load)
	{
		std::filesystem::path path = _path;
		m_TextureID = 0;
		m_TextureName = path.filename().string();
		m_Width = 0;
		m_Height = 0;
		m_BitDepth = 0;
		m_FilePath = _path;
		if(_load) LoadTexture();
	}

    bool Texture::LoadTexture()
	{
		unsigned char* texData = stbi_load(m_FilePath.c_str(), &m_Width, &m_Height, &m_BitDepth, 0);
		if (!texData)
		{
			DE_LOG(LogRender, Error, "Failed to find: {}", m_FilePath)
			return false;
		}

		glGenTextures(1, &m_TextureID);
		glBindTexture(GL_TEXTURE_2D, m_TextureID);
		GLint internalFormat;
		if (m_BitDepth == 4)
		{
			internalFormat = GL_RGBA;
			m_Target = GL_TEXTURE_2D;
		}
		else if (m_BitDepth == 3)
		{
			internalFormat = GL_RGB;
			m_Target = GL_TEXTURE_2D;
		}
		else
		{
			DE_LOG(LogRender, Error, "Unsupported Bit Depth: {}", m_BitDepth)
			return false;
		}
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_Width, m_Height, 0, internalFormat, GL_UNSIGNED_BYTE, texData);
		glGenerateMipmap(GL_TEXTURE_2D);

		// Apply Defualt Settings
		glTexParameteri(m_Target, GL_TEXTURE_WRAP_S, m_Settings.WrapMode);
		glTexParameteri(m_Target, GL_TEXTURE_WRAP_T, m_Settings.WrapMode);
		glTexParameteri(m_Target, GL_TEXTURE_MIN_FILTER, m_Settings.FilterMode);
		glTexParameteri(m_Target, GL_TEXTURE_MAG_FILTER, m_Settings.FilterMode);

		glBindTexture(GL_TEXTURE_2D, 0);

		stbi_image_free(texData);

		return true;
	}
}