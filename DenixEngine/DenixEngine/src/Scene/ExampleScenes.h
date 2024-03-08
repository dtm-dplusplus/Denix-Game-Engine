#pragma once

#include "Scene/Scene.h"

#include <GL/glew.h>
#include "glm/glm.hpp"
#include <memory>

namespace Denix
{
	class VertexArray;
	class VertexBuffer;
	class GLShader;

	class TestObject : public GameObject
	{
	public:
		TestObject();
		~TestObject() override;

		void BeginScene() override;
		void EndScene() override;

		void Update(float _deltaTime) override;
	private:
		Ref<VertexArray> Vao;
		Ref<VertexBuffer> Vbo;
		Ref<GLShader> Program;

	

		friend class DefaultScene;
	};

	class DefaultScene final : public Scene
	{
	public:
		DefaultScene();
		~DefaultScene() override;

		bool Load() override;
		void Unload() override;

		void BeginScene() override;
		void EndScene() override;
		void Update(float _deltaTime) override;

	private:
		Ref<VertexArray> Vao;
		Ref<VertexBuffer> Vbo;
		Ref<GLShader> Program;

		//// Rotation
		//bool IsRotating = false;
		//float RotSpeed = 0.01f;
	};
}