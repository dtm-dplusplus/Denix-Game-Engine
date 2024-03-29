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

	class TexObject : public GameObject
	{
		public:
		TexObject(): GameObject({ "TexObject" })
		{
			m_MeshComponent->CreateMesh(vertices, indices, 20, 12);

			//// Reset the state
			VertexBuffer::Unbind();
			VertexArray::Unbind();
			IndexBuffer::Unbind();
		}

		void Update(float _deltaTime) override
		{
						//brickTexture.UseTexture();
		}

		unsigned int indices[12] = {
				0, 3, 1,
				1, 3, 2,
				2, 3, 0,
				0, 1, 2
		};

		GLfloat vertices[20] = {
			//	x      y      z			u	  v
				-1.0f, -1.0f, 0.0f,		0.0f, 0.0f,
				0.0f, -1.0f, 1.0f,		0.5f, 0.0f,
				1.0f, -1.0f, 0.0f,		1.0f, 0.0f,
				0.0f, 1.0f, 0.0f,		0.5f, 1.0f
		};
	};
	class PlaygroundScene final : public Scene
	{
	public:
		PlaygroundScene(const ObjectInitializer& _objInit = ObjectInitializer("PlaygroundScene")) {}

		bool Load() override;
		void Update(float _deltaTime) override;

		Ref<TexObject> TextureObj;

		Texture brickTexture;

		GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0;

	};
}
