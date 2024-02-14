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