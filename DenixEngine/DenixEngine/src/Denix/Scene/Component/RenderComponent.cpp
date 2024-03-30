#include "RenderComponent.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

namespace Denix
{
	void Texture::LoadTexture()
	{
		unsigned char* texData = stbi_load(m_FileLocation.c_str(), &m_Width, &m_Height, &m_BitDepth, 0);
		if (!texData)
		{
			DE_LOG(LogRender, Error, "Failed to find: {}", m_FileLocation)
			return;
		}
		DE_LOG(LogRender, Info, "Loaded Texture: {}", m_FileLocation)

		glGenTextures(1, &m_TextureID);
		glBindTexture(GL_TEXTURE_2D, m_TextureID);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, texData);
		glGenerateMipmap(GL_TEXTURE_2D);

		glBindTexture(GL_TEXTURE_2D, 0);

		stbi_image_free(texData);
	}

	void RenderComponent::BeginScene()
	{
		Component::BeginScene();

		//RegisterComponent();
	}

	void RenderComponent::EndScene()
	{
		//UnregisterComponent();

		Component::EndScene();
	}

	void RenderComponent::RegisterComponent()
	{
		Component::RegisterComponent();
	}

	void RenderComponent::UnregisterComponent()
	{
		Component::UnregisterComponent();
	}
}