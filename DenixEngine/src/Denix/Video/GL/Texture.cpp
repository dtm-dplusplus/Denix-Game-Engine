#include "Texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

namespace Denix
{
    bool Texture::LoadTexture()
	{
		unsigned char* texData = stbi_load(m_FilePath.c_str(), &m_Width, &m_Height, &m_BitDepth, 0);
		if (!texData)
		{
			DE_LOG(LogRenderer, Error, "Failed to find: {}", m_FilePath)
			return false;
		}
		DE_LOG(LogRenderer, Info, "Loaded Texture: {}", m_FilePath)

		glGenTextures(1, &m_TextureID);
		glBindTexture(GL_TEXTURE_2D, m_TextureID);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, texData);
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