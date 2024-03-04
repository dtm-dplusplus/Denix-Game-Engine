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

	GLint ProjectionUniformId;
	glm::mat4 Projection;

	// Rotation
	bool IsRotating = true;
	float Angle = 0.f;
	float RotSpeed = 5.f;

	// Camera
	bool IsPerspective = true;
	float Fov = 45.f;
	float NearPlane = 0.1f;
	float FarPlane = 100.f;

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