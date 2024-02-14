#include "DePch.h"
#include "Scene.h"

#include "imgui.h"
#include "File.h"
#include "Video/GL/VertexBuffer.h"
#include "Video/GL/VertexArray.h"
#include "Video/GL/Shader.h"

TriangleScene::TriangleScene(): Scene("Triangle Uniform")
	
{
	const GLfloat positions[] = {
		0.0f, 0.5f, 0.0f,
		-0.5f,-0.5f, 0.0f,
		0.5f,-0.5f, 0.0f
	};

	// VBO creation
	Vbo = std::make_unique<VertexBuffer>(positions, sizeof(positions), 3, GL_FLOAT);

	// Create a new VAO on the GPU and bind it
	Vao = std::make_unique<VertexArray>();
	Vao->GenVertexArray();
	Vao->Bind();

	// Bind the vbos & attrivbs
	Vbo->Bind();
	Vao->AttribPtr(0,Vbo->GetAttribPerVert(), GL_FLOAT);

	// Reset the state
	VertexBuffer::Unbind();
	VertexArray::Unbind();

	// Create ShaderProgram
	Program = std::make_unique<ShaderProgram>();
	Program->CreateProgram();
	Program->CompileShader(GL_VERTEX_SHADER, File::Read("res/shaders/Vert.glsl"));
	Program->CompileShader(GL_FRAGMENT_SHADER, File::Read("res/shaders/UniFrag.glsl"));
	Program->AttachShaders();

	Program->BindAttrib(0, "a_Position");

	if (!Program->LinkProgram()) DE_LOG(LogGL, Error, "Link program failed")
	else DE_LOG(LogGL, Info, "Link program success")

	// Check if the uniform exists
	ColorUniformId = glGetUniformLocation(Program->GetID(), "u_Color");

	if (ColorUniformId == -1) DE_LOG(LogGL, Error, "Uniform not found")
	else DE_LOG(LogGL, Info, "Uniform found")

	Color = { 1.0f, 0.0f, 0.0f, 1.0f };
}

TriangleScene::~TriangleScene()
{
	
}

void TriangleScene::Update()
{
	// Update UI
	ImGui::Begin("Triangle Color");
	ImGui::ColorEdit4("RGBA", &Color[0]);
	ImGui::End();
}

void TriangleScene::Draw()
{
	// Move this to renderer in the future
	Program->Bind();
	Vao->Bind();

	glUniform4fv(ColorUniformId, 1, &Color[0]);
	glDrawArrays(GL_TRIANGLES, 0, 3);

	VertexArray::Unbind();
	ShaderProgram::Unbind();
}