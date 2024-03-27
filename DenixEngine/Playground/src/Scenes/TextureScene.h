#pragma once

#include "Denix/Scene/Scene.h"

using namespace Denix;

class TexturePlane : public GameObject
{
public:
	TexturePlane(const ObjectInitializer& _objInit = { "TexturePlane" }) {}

	float Vertices[12] = {
	 0.5f,  0.5f, 0.0f,  // top right
	 0.5f, -0.5f, 0.0f,  // bottom right
	-0.5f, -0.5f, 0.0f,  // bottom left
	-0.5f,  0.5f, 0.0f   // top left 
	};

	unsigned int Indices[6] = {  // note that we start from 0!
		0, 1, 3,   // first triangle
		1, 2, 3    // second triangle
	};

	float TexCoord[8] = {
		0.0f, 0.0f,
		1.0f, 0.0f,
		1.0f, 1.0f,
		0.0f, 1.0f
	};

	unsigned int VBO;
	unsigned int IBO;
	unsigned int TCBO;

	unsigned int VAO;
	unsigned int texture;

	Ref<GLShader> m_Shader;
};

class TextureScene : public Scene
{
public:
	TextureScene(const ObjectInitializer& _objInit = ObjectInitializer("TextureScene")) {}
	~TextureScene() override {}

	bool Load() override;
	void Unload() override;

	void BeginScene() override;
	void EndScene() override;
	void Update(float _deltaTime) override;

	Ref<TexturePlane> TexPlane;
};

