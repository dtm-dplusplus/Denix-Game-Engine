#pragma once

#include "Denix/Scene/Scene.h"

namespace Denix
{
	class Texture
	{
	public:
		Texture();
		Texture(const char* fileLoc);

		void LoadTexture();
		void UseTexture();
		void ClearTexture();

		~Texture();

	private:
		GLuint textureID;
		int width, height, bitDepth;

		const char* fileLocation;
	};


	class Shader
	{
	public:
		Shader();

		void CreateFromString(const char* vertexCode, const char* fragmentCode);
		void CreateFromFiles(const char* vertexLocation, const char* fragmentLocation);

		std::string ReadFile(const char* fileLocation);

		GLuint GetProjectionLocation();
		GLuint GetModelLocation();
		GLuint GetViewLocation();

		void UseShader();
		void ClearShader();

		~Shader();

	private:
		GLuint shaderID, uniformProjection, uniformModel, uniformView;

		void CompileShader(const char* vertexCode, const char* fragmentCode);
		void AddShader(GLuint theProgram, const char* shaderCode, GLenum shaderType);
	};

	class Mesh
	{
	public:
		Mesh();

		void CreateMesh(GLfloat* vertices, unsigned int* indices, unsigned int numOfVertices, unsigned int numOfIndices);
		void RenderMesh();
		void ClearMesh();

		~Mesh();

	private:
		GLuint VAO, VBO, IBO;
		GLsizei indexCount;
	};



	class PlaygroundScene final : public Scene
	{
	public:
		PlaygroundScene(const ObjectInitializer& _objInit = ObjectInitializer("PlaygroundScene")) {}

		bool Load() override;
		void Update(float _deltaTime) override;

		//Ref<EqualTriangle> EqualTri;
		//Ref<RightTriangle> RightTri;
		//Ref<Plane> Square;

		std::vector<Mesh*> meshList;
		std::vector<Shader> shaderList;

		Texture brickTexture;
		Texture dirtTexture;

		// Vertex Shader
		std::string vShader;

		// Fragment Shader
		std::string fShader;

		GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0;

		void CreateObjects()
		{
			unsigned int indices[] = {
				0, 3, 1,
				1, 3, 2,
				2, 3, 0,
				0, 1, 2
			};

			GLfloat vertices[] = {
				//	x      y      z			u	  v
					-1.0f, -1.0f, 0.0f,		0.0f, 0.0f,
					0.0f, -1.0f, 1.0f,		0.5f, 0.0f,
					1.0f, -1.0f, 0.0f,		1.0f, 0.0f,
					0.0f, 1.0f, 0.0f,		0.5f, 1.0f
			};

			Mesh* obj1 = new Mesh();
			obj1->CreateMesh(vertices, indices, 20, 12);
			meshList.push_back(obj1);

			Mesh* obj2 = new Mesh();
			obj2->CreateMesh(vertices, indices, 20, 12);
			meshList.push_back(obj2);
		}

		void CreateShaders()
		{
			Shader* shader1 = new Shader();
			shader1->CreateFromFiles(vShader.c_str(), fShader.c_str());
			shaderList.push_back(*shader1);
		}

	};
}
