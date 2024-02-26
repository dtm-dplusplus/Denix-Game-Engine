#pragma once

#include "Scene/Scene.h"

#include <GL/glew.h>
#include "glm/glm.hpp"
#include <memory>

class VertexArray;
class VertexBuffer;
class ShaderProgram;


class ObjectScene final : public Scene
{
public:
	// We forward declare our GL classes so the ctor/dtor must be defined in the cpp file+
	ObjectScene();
	~ObjectScene() override;

	void Update() override;

	void Draw() override;

private:
	std::shared_ptr<VertexArray> Vao;
	std::shared_ptr<VertexBuffer> Vbo;
	std::shared_ptr<ShaderProgram> Program;

	// Color
	GLint ColorUniformId;
	glm::vec4 Color = { 1.0f, 0.0f, 0.0f, 1.0f };

	// Matrix
	GLint ModelUniformId;
	glm::mat4 Model;

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

	glm::vec3 CamPos = { 0.f, 0.f, -2.5f };
};

class TextureScene final : public Scene
{
public:
	// We forward declare our GL classes so the ctor/dtor must be defined in the cpp file+
	TextureScene();
	~TextureScene() override;

	void Update() override;

	void Draw() override;

private:
	std::shared_ptr<VertexArray> Vao;
	std::shared_ptr<VertexBuffer> VboTexCoord;
	std::shared_ptr<VertexBuffer> VboPos;
	std::shared_ptr<ShaderProgram> Program;

	// Texture
	GLuint TextureID;
	GLint TextureUniformId;

	// Matrix
	GLint ModelUniformId;
	glm::mat4 Model;

	GLint ProjectionUniformId;
	glm::mat4 Projection;

	// Rendering
	// bool IsCulled = true;

	// Rotation
	bool IsRotating = true;
	float Angle = 0.f;
	float RotSpeed = 0.5f;

	// Camera
	bool IsPerspective = true;
	float Fov = 45.f;
	float NearPlane = 0.1f;
	float FarPlane = 100.f;

	glm::vec3 CamPos = { 0.f, 0.f, -2.5f };
};