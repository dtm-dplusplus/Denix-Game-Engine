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

class VertexArray;
class VertexBuffer;
class ShaderProgram;

class Lab2 final : public Scene
{
public:

	// We forward declare our GL classes so the ctor/dtor must be defined in the cpp file+
	Lab2();
	~Lab2() override;

	void Update() override;

	void Draw() override;

private:
	std::shared_ptr<VertexArray> Vao;
	std::shared_ptr<VertexBuffer> Vbo;
	std::shared_ptr<ShaderProgram> Program;

	GLint ColorUniformId;
	glm::vec4 Color;
};

class Lab2Sq final : public Scene
{
public:

	// We forward declare our GL classes so the ctor/dtor must be defined in the cpp file+
	Lab2Sq();
	~Lab2Sq() override;

	void Update() override;

	void Draw() override;

private:
	std::shared_ptr<VertexArray> Vao;
	std::shared_ptr<VertexBuffer> Vbo;
	std::shared_ptr<ShaderProgram> Program;

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
	std::shared_ptr<VertexArray> Vao;
	std::shared_ptr<VertexBuffer> Vbo;
	std::shared_ptr<ShaderProgram> Program;

	// Color
	GLint ColorUniformId;
	glm::vec4 Color = { 1.0f, 0.0f, 0.0f, 1.0f };
	float ColSpeed = 0.35f;

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