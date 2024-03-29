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

	Shader::Shader()
	{
		shaderID = 0;
		uniformModel = 0;
		uniformProjection = 0;
	}

	void Shader::CreateFromString(const char* vertexCode, const char* fragmentCode)
	{
		CompileShader(vertexCode, fragmentCode);
	}

	void Shader::CreateFromFiles(const char* vertexLocation, const char* fragmentLocation)
	{
		std::string vertexString = ReadFile(vertexLocation);
		std::string fragmentString = ReadFile(fragmentLocation);
		const char* vertexCode = vertexString.c_str();
		const char* fragmentCode = fragmentString.c_str();

		CompileShader(vertexCode, fragmentCode);
	}

	std::string Shader::ReadFile(const char* fileLocation)
	{
		std::string content;
		std::ifstream fileStream(fileLocation, std::ios::in);

		if (!fileStream.is_open()) {
			printf("Failed to read %s! File doesn't exist.", fileLocation);
			return "";
		}

		std::string line = "";
		while (!fileStream.eof())
		{
			std::getline(fileStream, line);
			content.append(line + "\n");
		}

		fileStream.close();
		return content;
	}

	void Shader::CompileShader(const char* vertexCode, const char* fragmentCode)
	{
		shaderID = glCreateProgram();

		if (!shaderID)
		{
			printf("Error creating shader program!\n");
			return;
		}

		AddShader(shaderID, vertexCode, GL_VERTEX_SHADER);
		AddShader(shaderID, fragmentCode, GL_FRAGMENT_SHADER);

		GLint result = 0;
		GLchar eLog[1024] = { 0 };

		glLinkProgram(shaderID);
		glGetProgramiv(shaderID, GL_LINK_STATUS, &result);
		if (!result)
		{
			glGetProgramInfoLog(shaderID, sizeof(eLog), NULL, eLog);
			printf("Error linking program: '%s'\n", eLog);
			return;
		}

		glValidateProgram(shaderID);
		glGetProgramiv(shaderID, GL_VALIDATE_STATUS, &result);
		if (!result)
		{
			glGetProgramInfoLog(shaderID, sizeof(eLog), NULL, eLog);
			printf("Error validating program: '%s'\n", eLog);
			return;
		}

		uniformProjection = glGetUniformLocation(shaderID, "projection");
		uniformModel = glGetUniformLocation(shaderID, "model");
		uniformView = glGetUniformLocation(shaderID, "view");
	}

	GLuint Shader::GetProjectionLocation()
	{
		return uniformProjection;
	}
	GLuint Shader::GetModelLocation()
	{
		return uniformModel;
	}
	GLuint Shader::GetViewLocation()
	{
		return uniformView;
	}

	void Shader::UseShader()
	{
		glUseProgram(shaderID);
	}

	void Shader::ClearShader()
	{
		if (shaderID != 0)
		{
			glDeleteProgram(shaderID);
			shaderID = 0;
		}

		uniformModel = 0;
		uniformProjection = 0;
	}


	void Shader::AddShader(GLuint theProgram, const char* shaderCode, GLenum shaderType)
	{
		GLuint theShader = glCreateShader(shaderType);

		const GLchar* theCode[1];
		theCode[0] = shaderCode;

		GLint codeLength[1];
		codeLength[0] = strlen(shaderCode);

		glShaderSource(theShader, 1, theCode, codeLength);
		glCompileShader(theShader);

		GLint result = 0;
		GLchar eLog[1024] = { 0 };

		glGetShaderiv(theShader, GL_COMPILE_STATUS, &result);
		if (!result)
		{
			glGetShaderInfoLog(theShader, sizeof(eLog), NULL, eLog);
			printf("Error compiling the %d shader: '%s'\n", shaderType, eLog);
			return;
		}

		glAttachShader(theProgram, theShader);
	}

	Shader::~Shader()
	{
		ClearShader();
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
		CreateShaders();

		std::string brick = std::filesystem::current_path().parent_path().string() + "\\Playground\\Content\\Textures\\brick.png";
		std::string dirt = std::filesystem::current_path().parent_path().string() + "\\Playground\\Content\\Textures\\dirt.png";

		brickTexture = Texture(brick.c_str());
		brickTexture.LoadTexture();
		dirtTexture = Texture(dirt.c_str());
		dirtTexture.LoadTexture();

		

		return true;
	}

	void PlaygroundScene::Update(float _deltaTime)
	{
		Scene::Update(_deltaTime);

		ImGui::Begin("Playground Scene");
		
		ImGui::End();


		shaderList[0].UseShader();
		uniformModel = shaderList[0].GetModelLocation();
		uniformProjection = shaderList[0].GetProjectionLocation();
		uniformView = shaderList[0].GetViewLocation();

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(m_Camera->GetTransformComponent()->GetModel()));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(m_Camera->GetProjectionMatrix()));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(m_Camera->GetViewMatrix()));
		brickTexture.UseTexture();
		meshList[0]->RenderMesh();

		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 1.0f, -50.0f));
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		dirtTexture.UseTexture();
		meshList[1]->RenderMesh();

		glUseProgram(0);

	}
}
