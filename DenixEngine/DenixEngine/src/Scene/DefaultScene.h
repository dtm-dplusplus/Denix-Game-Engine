#pragma once

#include "Scene/Scene.h"

#include <GL/glew.h>
#include "glm/glm.hpp"
#include <memory>

class VertexArray;
class VertexBuffer;
class ShaderProgram;

class TestObject : public GameObject
{
public:
	TestObject();
	~TestObject() override;

	void Update() override;
private:
	Ref<VertexArray> Vao;
	Ref<VertexBuffer> Vbo;
	Ref<ShaderProgram> Program;

	// Color
	GLint ColorUniformId;
	glm::vec4 Color = { 1.0f, 0.0f, 0.0f, 1.0f };

	// Matrix
	GLint ModelUniformId;
	GLint ViewUniformId;
	GLint ProjectionUniformId;

	// Rotation
	bool IsRotating = true;
	float RotSpeed = 0.01f;

	friend class DefaultScene;
};


class DefaultScene final : public Scene
{
public:
	DefaultScene();
	~DefaultScene() override;

	bool Load() override;
	void Unload() override;

	void Update() override;

private:
	Ref<VertexArray> Vao;
	Ref<VertexBuffer> Vbo;
	Ref<ShaderProgram> Program;

	Ref<TestObject> Obj;
};