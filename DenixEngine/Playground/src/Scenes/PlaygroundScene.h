#pragma once

#include "Denix/Scene/Scene.h"
#include "Denix/System/ShaderSubSystem.h"

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
		Ref<Plane> Square;

		Mesh* Pyramid;
		Ref<GLShader> GLShader;

		Texture brickTexture;

		std::string vShader;
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

			Pyramid = new Mesh();
			Pyramid->CreateMesh(vertices, indices, 20, 12);
		}
	};
}
