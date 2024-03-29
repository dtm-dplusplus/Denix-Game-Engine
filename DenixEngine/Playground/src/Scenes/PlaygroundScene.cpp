#include "PlaygroundScene.h"
#include "imgui.h"

#include <filesystem>
#include "Denix/System/ShaderSubSystem.h"

#define STB_IMAGE_IMPLEMENTATION
#include <fstream>
#include <stb_image.h>
#include <iostream>

namespace Denix
{
	Texture::Texture()
	{
		textureID = 0;
		width = 0;
		height = 0;
		bitDepth = 0;
		fileLocation = "";
	}

	Texture::Texture(const char* fileLoc)
	{
		textureID = 0;
		width = 0;
		height = 0;
		bitDepth = 0;
		fileLocation = fileLoc;
	}

	void Texture::LoadTexture()
	{
		unsigned char* texData = stbi_load(fileLocation, &width, &height, &bitDepth, 0);
		if (!texData)
		{
			printf("Failed to find: %s\n", fileLocation);
			return;
		}

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

	void Texture::UseTexture()
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureID);
	}

	void Texture::ClearTexture()
	{
		glDeleteTextures(1, &textureID);
		textureID = 0;
		width = 0;
		height = 0;
		bitDepth = 0;
		fileLocation = "";
	}


	Texture::~Texture()
	{
		ClearTexture();
	}


	bool PlaygroundScene::Load()
	{
		Scene::Load();

		std::string brick = std::filesystem::current_path().parent_path().string() + "\\Playground\\Content\\Textures\\brick.png";

		brickTexture = Texture(brick.c_str());
		brickTexture.LoadTexture();


		TextureObj = MakeRef<TexObject>();
		TextureObj->GetTransformComponent()->SetScale(glm::vec3(30.0f));
		TextureObj->GetRenderComponent()->SetIsVisible(false);
		TextureObj->GetRenderComponent()->SetShader(ShaderSubsystem::Get()->GetShader("TextureShader"));
		m_SceneObjects.push_back(TextureObj);
		return true;
	}

	void PlaygroundScene::Update(float _deltaTime)
	{
		Scene::Update(_deltaTime);

		ImGui::Begin("Playground Scene");
		
		ImGui::End();

		//GLShader->Bind();
		const Ref<GLShader> shader = TextureObj->GetRenderComponent()->GetShader();
		shader->Bind();
		glUniformMatrix4fv(shader->GetUniform("u_Model"), 1, GL_FALSE, glm::value_ptr(TextureObj->GetTransformComponent()->GetModel()));
		glUniformMatrix4fv(shader->GetUniform("u_Projection"), 1, GL_FALSE, glm::value_ptr(m_Camera->GetProjectionMatrix()));
		glUniformMatrix4fv(shader->GetUniform("u_View"), 1, GL_FALSE, glm::value_ptr(m_Camera->GetViewMatrix()));
		brickTexture.UseTexture();
		TextureObj->GetMeshComponent()->RenderMesh();

		GLShader::Unbind();
	}
}