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
	Vao->AttribPtr(Vbo->GetAttribPerVert(), GL_FLOAT);

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




Lab3::Lab3()
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
	Vao->AttribPtr(Vbo->GetAttribPerVert(), GL_FLOAT);

	// Reset the state
	VertexBuffer::Unbind();
	VertexArray::Unbind();

	// Create ShaderProgram
	Program = std::make_unique<ShaderProgram>();
	Program->CreateProgram();
	Program->CompileShader(GL_VERTEX_SHADER, File::Read("res/shaders/UniVert.glsl"));
	Program->CompileShader(GL_FRAGMENT_SHADER, File::Read("res/shaders/UniFrag.glsl"));
	Program->AttachShaders();

	Program->BindAttrib(0, "a_Position");

	if (!Program->LinkProgram()) DE_LOG(LogGL, Error, "Link program failed")
	else DE_LOG(LogGL, Info, "Link program success")

	// Check Color Uniform
	ColorUniformId = glGetUniformLocation(Program->GetID(), "u_Color");

	if (ColorUniformId == -1) DE_LOG(LogGL, Error, "Uniform not found")
	else DE_LOG(LogGL, Info, "Uniform found")

	// Check Model Uniform
	ModelUniformId = glGetUniformLocation(Program->GetID(), "u_Model");

	if (ModelUniformId == -1) DE_LOG(LogGL, Error, "Uniform not found")
	else DE_LOG(LogGL, Info, "Uniform found")
}

Lab3::~Lab3()
{
}

void Lab3::Update()
{
	// Update UI
	ImGui::Begin("Lab2 Settings");
	ImGui::SeparatorText("Color");
	ImGui::ColorEdit4("Color", &Color[0]);

	ImGui::SeparatorText("Camera");
	ImGui::DragFloat3("CamPos", &CamPos[0]);
	ImGui::Checkbox("Perspec Proj", &IsPerspective);
	ImGui::DragFloat("FOV", &FOV);
	ImGui::DragFloat("Near Plane", &NearPlane);
	ImGui::DragFloat("Far Plane", &FarPlane);

	ImGui::SeparatorText("Rotation");
	if(ImGui::Checkbox("Rotate", &IsRotating)) Angle = 0.f;
	ImGui::DragFloat("RotSpeed", &RotSpeed);
	ImGui::End();

	// Prepare the projection matrix
	if (!IsPerspective)
	{
		Projection = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, NearPlane, FarPlane);
	}
	else
	{
		Projection = glm::perspective(glm::radians(FOV),800.f / 600.f, NearPlane, FarPlane);
	}
	
	// Prepare the model matrix
	Model = glm::mat4(1.0f);
	Model = glm::translate(Model, CamPos);

	// Rotate the model matrix
	if (IsRotating)
	{
		Model = glm::rotate(Model, glm::radians(Angle), glm::vec3(0, 1, 0));
	}

	// Increase the float angle so next frame the triangle rotates further
	Angle += RotSpeed;

}

void Lab3::Draw()
{
	
	// Move this to renderer in the future
	Program->Bind();
	Vao->Bind();

	// Upload the model matrix
	glUniformMatrix4fv(ModelUniformId, 1, GL_FALSE, glm::value_ptr(Model));

	// Upload the projection matrix
	glUniformMatrix4fv(ProjectionUniformId, 1, GL_FALSE, glm::value_ptr(Projection));
		

	glUniform4fv(ColorUniformId, 1, &Color[0]);
	glDrawArrays(GL_TRIANGLES, 0, 3);

	// Draw another tri with a different model matrix
	Model = glm::mat4(1.0f);
	Model = glm::translate(Model, CamPos);
	Model = glm::translate(Model, glm::vec3(-0.5f, 0.0f, 0.f));
	Model = glm::rotate(Model, glm::radians(Angle), glm::vec3(0, 1, 0));
	Model = glm::scale(Model, glm::vec3(0.25f, 0.25f, 0.25f));

	glUniformMatrix4fv(ModelUniformId, 1, GL_FALSE, glm::value_ptr(Model));
	glDrawArrays(GL_TRIANGLES, 0, 3);

	VertexArray::Unbind();
	ShaderProgram::Unbind();
}
