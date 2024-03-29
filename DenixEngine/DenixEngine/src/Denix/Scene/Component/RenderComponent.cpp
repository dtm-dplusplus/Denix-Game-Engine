#include "RenderComponent.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

namespace Denix
{
	void Texture::LoadTexture()
	{
		unsigned char* texData = stbi_load(fileLocation, &width, &height, &bitDepth, 0);
		if (!texData)
		{
			DE_LOG(LogRender, Error, "Failed to find: {}", fileLocation)
			return;
		}
		DE_LOG(LogRender, Info, "Loaded Texture: {}", fileLocation)

		glGenTextures(1, &textureID);
		glBindTexture(GL_TEXTURE_2D, textureID);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, texData);
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