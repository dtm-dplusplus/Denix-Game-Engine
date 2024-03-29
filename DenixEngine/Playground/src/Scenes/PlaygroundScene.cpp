#include "PlaygroundScene.h"
#include "imgui.h"

#include <filesystem>

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

	Mesh::Mesh()
	{
		VAO = 0;
		VBO = 0;
		IBO = 0;
		indexCount = 0;
	}

	void Mesh::CreateMesh(GLfloat* vertices, unsigned int* indices, unsigned int numOfVertices, unsigned int numOfIndices)
	{
		indexCount = numOfIndices;

		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);

		glGenBuffers(1, &IBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * numOfIndices, indices, GL_STATIC_DRAW);

		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0]) * numOfVertices, vertices, GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertices[0]) * 5, 0);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(vertices[0]) * 5, (void*)(sizeof(vertices[0]) * 3));
		glEnableVertexAttribArray(1);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		glBindVertexArray(0);
	}

	void Mesh::RenderMesh()
	{
		glBindVertexArray(VAO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
		glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}

	void Mesh::ClearMesh()
	{
		if (IBO != 0)
		{
			glDeleteBuffers(1, &IBO);
			IBO = 0;
		}

		if (VBO != 0)
		{
			glDeleteBuffers(1, &VBO);
			VBO = 0;
		}

		if (VAO != 0)
		{
			glDeleteVertexArrays(1, &VAO);
			VAO = 0;
		}

		indexCount = 0;
	}


	Mesh::~Mesh()
	{
		ClearMesh();
	}




	bool PlaygroundScene::Load()
	{
		Scene::Load();

		vShader = std::filesystem::current_path().parent_path().string() + "\\Playground\\Content\\Shaders\\shader.vert";
		fShader = std::filesystem::current_path().parent_path().string() + "\\Playground\\Content\\Shaders\\shader.frag";

		CreateObjects();

		ShaderSubsystem* shaderSubSystem = ShaderSubsystem::Get();
		std::vector<std::pair<GLenum, std::string>> shaders;
		shaders.emplace_back(GL_VERTEX_SHADER, File::Read(vShader, true));
		shaders.emplace_back(GL_FRAGMENT_SHADER, File::Read(fShader, true));
		shaderSubSystem->LoadShader(shaders, "TextureShader");

		GLShader = shaderSubSystem->GetShader("TextureShader");

		// Check Uniforms
		GLShader->GetUniform("u_Model");
		GLShader->GetUniform("u_Projection");
		GLShader->GetUniform("u_View");

		std::string brick = std::filesystem::current_path().parent_path().string() + "\\Playground\\Content\\Textures\\brick.png";

		brickTexture = Texture(brick.c_str());
		brickTexture.LoadTexture();


		Square = MakeRef<Plane>();
		Square->GetTransformComponent()->SetScale(glm::vec3(30.0f));
		Square->GetRenderComponent()->SetIsVisible(false);
		m_SceneObjects.push_back(Square);
		return true;
	}

	void PlaygroundScene::Update(float _deltaTime)
	{
		Scene::Update(_deltaTime);

		ImGui::Begin("Playground Scene");
		
		ImGui::End();

		GLShader->Bind();

		glUniformMatrix4fv(GLShader->GetUniform("u_Model"), 1, GL_FALSE, glm::value_ptr(Square->GetTransformComponent()->GetModel()));
		glUniformMatrix4fv(GLShader->GetUniform("u_Projection"), 1, GL_FALSE, glm::value_ptr(m_Camera->GetProjectionMatrix()));
		glUniformMatrix4fv(GLShader->GetUniform("u_View"), 1, GL_FALSE, glm::value_ptr(m_Camera->GetViewMatrix()));

		brickTexture.UseTexture();
		Pyramid->RenderMesh();

		glUseProgram(0);
	}
}
