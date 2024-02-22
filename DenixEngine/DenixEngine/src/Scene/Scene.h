#pragma once

#include <string>

// Super Basic Scene class so I can start creating and testing features
// Will be intergrated into ECS later on
class Scene
{
public:
	Scene() =  default;
	Scene(std::string _name) : SceneName(std::move(_name)) {}
	virtual ~Scene() = default;

	virtual void Update() {}
	virtual void Draw() {}

protected:
	std::string SceneName;
};



#include <GL/glew.h>
#include "glm/glm.hpp"
#include <memory>

class TriangleScene final : public Scene
{
public:

	// We forward declare our GL classes so the ctor/dtor must be defined in the cpp file+
	TriangleScene();
	~TriangleScene() override;

	void Update() override;

	void Draw() override;

private:

	std::unique_ptr<class VertexArray> Vao;
	std::unique_ptr<class VertexBuffer> Vbo;
	std::unique_ptr<class ShaderProgram> Program;

	GLint ColorUniformId;
	glm::vec4 Color;
};

class Lab3 final : public Scene
{
public:
	// We forward declare our GL classes so the ctor/dtor must be defined in the cpp file+
	Lab3();
	~Lab3() override;

	void Update() override;

	void Draw() override;

private:
	std::unique_ptr<class VertexArray> Vao;
	std::unique_ptr<class VertexBuffer> Vbo;
	std::unique_ptr<class ShaderProgram> Program;

	// Color
	GLint ColorUniformId;
	glm::vec4 Color = { 1.0f, 0.0f, 0.0f, 1.0f };
	float ColSpeed = 0.35f;

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
	float FOV = 45.f;
	float NearPlane = 0.1f;
	float FarPlane = 100.f;

	glm::vec3 CamPos = { 0.f, 0.f, -2.5f };
};

class Lab4 final : public Scene
{
public:
	// We forward declare our GL classes so the ctor/dtor must be defined in the cpp file+
	Lab4();
	~Lab4() override;

	void Update() override;

	void Draw() override;

private:
	std::unique_ptr<class VertexArray> Vao;
	std::unique_ptr<class VertexBuffer> VboPosColor;
	std::unique_ptr<class ShaderProgram> Program;

	// Texture
	GLuint VboTex;
	GLint TextureUniformId;

	// Color
	GLint ColorUniformId;
	glm::vec4 Color = { 1.0f, 0.0f, 0.0f, 1.0f };
	float ColSpeed = 0.35f;

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
	float FOV = 45.f;
	float NearPlane = 0.1f;
	float FarPlane = 100.f;

	glm::vec3 CamPos = { 0.f, 0.f, -2.5f };
};