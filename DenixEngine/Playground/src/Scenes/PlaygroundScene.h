#pragma once

#include "Denix/Scene/Scene.h"

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

		friend class PlaygroundScene;
	};

	class PlaygroundScene final : public Scene
	{
	public:
		PlaygroundScene(const ObjectInitializer& _objInit = ObjectInitializer::Get());
		~PlaygroundScene() override;

		bool Load() override;
		void Unload() override;

		void BeginScene() override;
		void EndScene() override;
		void Update(float _deltaTime) override;

	private:
		Ref<VertexArray> Vao;
		Ref<VertexBuffer> Vbo;
		Ref<GLShader> Program;

	};
}