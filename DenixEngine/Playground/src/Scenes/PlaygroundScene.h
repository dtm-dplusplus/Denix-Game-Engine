#pragma once

#include <filesystem>

#include "Denix/Scene/Scene.h"

namespace Denix
{
	

	class TexObject : public GameObject
	{
		public:
		TexObject(): GameObject({ "TexObject" })
		{
			m_MeshComponent->CreateMesh(vertices, indices, 20, 12);

			std::string brick = std::filesystem::current_path().parent_path().string() + "\\Playground\\Content\\Textures\\brick.png";
			std::string def = std::filesystem::current_path().parent_path().string() + "\\DenixEngine\\res\\Textures\\DefaultTexture.png";
			m_RenderComponent->LoadTexture(def);

			m_RenderComponent->SetShader(ShaderSubsystem::Get()->GetShader("TextureShader"));
			//// Reset the state
			VertexBuffer::Unbind();
			VertexArray::Unbind();
			IndexBuffer::Unbind();
		}

		void Update(float _deltaTime) override
		{
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
		Ref<Plane> BrickWall;
	};
}
