#include "DePch.h"
#include "Scene.h"

#define IMGUI_DEFINE_MATH_OPERATORS
#include "imgui.h"
#include "File.h"
#include "imgui_internal.h"
#include "Video/GL/VertexBuffer.h"
#include "Video/GL/VertexArray.h"
#include "Video/GL/Shader.h"
#define STB_IMAGE_IMPLEMENTATION 
#include <stb_image.h>


////////////// Triangle Scene ////////////////////////
TriangleScene::TriangleScene(): Scene("Triangle Uniform")
{
	const GLfloat positions[] = {
		0.0f, 0.5f, 0.0f,
		-0.5f,-0.5f, 0.0f,
		0.5f,-0.5f, 0.0f
	};

	// VBO creation
	Vbo = std::make_unique<VertexBuffer>(positions, sizeof(positions), 3, GL_FLOAT, GL_ARRAY_BUFFER);

	// Create a new VAO on the GPU and bind it
	Vao = std::make_unique<VertexArray>();
	Vao->GenVertexArray();
	Vao->Bind();

	// Bind the vbos & attrivbs
	Vbo->Bind(GL_ARRAY_BUFFER);
	Vao->AttribPtr(Vbo->GetAttribPerVert(), GL_FLOAT);

	// Reset the state
	VertexBuffer::Unbind(GL_ARRAY_BUFFER);
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



////////////// Lab3 Matrices ////////////////////////
Lab3::Lab3()
{
	const GLfloat positions[] = {
		0.0f, 0.5f, 0.0f,
		-0.5f,-0.5f, 0.0f,
		0.5f,-0.5f, 0.0f
	};

	// VBO creation
	Vbo = std::make_unique<VertexBuffer>(positions, sizeof(positions), 3, GL_FLOAT, GL_ARRAY_BUFFER);

	// Create a new VAO on the GPU and bind it
	Vao = std::make_unique<VertexArray>();
	Vao->GenVertexArray();
	Vao->Bind();

	// Bind the vbos & attrivbs
	Vbo->Bind(GL_ARRAY_BUFFER);
	Vao->AttribPtr(Vbo->GetAttribPerVert(), GL_FLOAT);

	// Reset the state
	VertexBuffer::Unbind(GL_ARRAY_BUFFER);
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
	glDrawArrays(GL_TRIANGLES, 0, Vbo->GetSize());

	// Draw another tri with a different model matrix
	Model = glm::mat4(1.0f);
	Model = glm::translate(Model, CamPos);
	Model = glm::translate(Model, glm::vec3(-0.5f, 0.0f, 0.f));
	Model = glm::rotate(Model, glm::radians(Angle), glm::vec3(0, 1, 0));
	Model = glm::scale(Model, glm::vec3(0.25f, 0.25f, 0.25f));

	glUniformMatrix4fv(ModelUniformId, 1, GL_FALSE, glm::value_ptr(Model));
	glDrawArrays(GL_TRIANGLES, 0, Vbo->GetSize());

	VertexArray::Unbind();
	ShaderProgram::Unbind();
}


////////////// Textures ////////////////////////
Lab4::Lab4()
{
	const GLfloat positions[] = {
		0.0f, 0.5f, 0.0f,
		-0.5f,-0.5f, 0.0f,
		0.5f,-0.5f, 0.0f
	};

	VboPosColor = std::make_unique<VertexBuffer>(positions, sizeof(positions), 3, GL_FLOAT, GL_ARRAY_BUFFER);

	// Create and bind a texture.
	{
		int w = 0;
		int h = 0;

		unsigned char* data = stbi_load("res/textures/Image.jpg", &w, &h, NULL, 4);

		if (data) DE_LOG(LogGL, Info, "Loaded image")
		else DE_LOG(LogGL, Error, "Failed to load image")

		// VBO creation
		glGenTextures(1, &VboTex);
		glBindTexture(GL_TEXTURE_2D, VboTex);

		// Upload the image data to the bound texture unit in the GPU
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA,
			GL_UNSIGNED_BYTE, data);

		// Free the loaded data because we now have a copy on the GPU
		free(data);

		// Generate Mipmap so the texture can be mapped correctly
		glGenerateMipmap(GL_TEXTURE_2D);

		// Unbind the texture because we are done operating on it
		glBindTexture(GL_TEXTURE_2D, 0);
	}
	

	// Create a new VAO on the GPU and bind it
	Vao = std::make_unique<VertexArray>();
	Vao->GenVertexArray();
	Vao->Bind();

	// Bind the vbos & attribs
	VboPosColor->Bind(GL_ARRAY_BUFFER);
	Vao->AttribPtr(3, GL_FLOAT);

	glBindBuffer(GL_TEXTURE_2D, VboTex);
	Vao->AttribPtr(2, GL_FLOAT);

	// Reset the state
	VertexBuffer::Unbind(GL_ARRAY_BUFFER);
	VertexArray::Unbind();

	// Create ShaderProgram
	Program = std::make_unique<ShaderProgram>();
	Program->CreateProgram();
	Program->CompileShader(GL_VERTEX_SHADER, File::Read("res/shaders/TexVert.glsl"));
	Program->CompileShader(GL_FRAGMENT_SHADER, File::Read("res/shaders/TexFrag.glsl"));
	Program->AttachShaders();

	Program->BindAttrib(0, "a_Position");
	Program->BindAttrib(1, "a_TexCoord");


	if (!Program->LinkProgram()) DE_LOG(LogGL, Error, "Link program failed")
	else DE_LOG(LogGL, Info, "Link program success")

	// Check Uniforms
	{
		// Check Projection Uniform
		ProjectionUniformId = glGetUniformLocation(Program->GetID(), "u_Projection");

		if (ProjectionUniformId == -1) DE_LOG(LogGL, Error, "Uniform not found")
		else DE_LOG(LogGL, Info, "Uniform found")

		// Check Model Uniform
		ModelUniformId = glGetUniformLocation(Program->GetID(), "u_Model");

		if (ModelUniformId == -1) DE_LOG(LogGL, Error, "Uniform not found")
		else DE_LOG(LogGL, Info, "Uniform found")

		// Check Texture Uniform
		TextureUniformId = glGetUniformLocation(Program->GetID(), "u_Texture");

		if (TextureUniformId == -1) DE_LOG(LogGL, Error, "Uniform not found")
		else DE_LOG(LogGL, Info, "Uniform found")
	}
}

Lab4::~Lab4()
{
}

void Lab4::Update()
{
	// Update UI
	ImGui::Begin("Lab2 Settings");
	ImGui::SeparatorText("Camera");
	ImGui::DragFloat3("CamPos", &CamPos[0]);
	ImGui::Checkbox("Perspec Proj", &IsPerspective);
	ImGui::DragFloat("FOV", &FOV);
	ImGui::DragFloat("Near Plane", &NearPlane);
	ImGui::DragFloat("Far Plane", &FarPlane);

	ImGui::SeparatorText("Rotation");
	if (ImGui::Checkbox("Rotate", &IsRotating)) Angle = 0.f;
	ImGui::DragFloat("RotSpeed", &RotSpeed);
	ImGui::End();

	// Prepare the projection matrix
	if (!IsPerspective)
	{
		Projection = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, NearPlane, FarPlane);
	}
	else
	{
		Projection = glm::perspective(glm::radians(FOV), 800.f / 600.f, NearPlane, FarPlane);
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

void Lab4::Draw()
{

	// Move this to renderer in the future
	Program->Bind();
	Vao->Bind();

	// Upload the model matrix
	glUniformMatrix4fv(ModelUniformId, 1, GL_FALSE, glm::value_ptr(Model));

	// Upload the projection matrix
	glUniformMatrix4fv(ProjectionUniformId, 1, GL_FALSE, glm::value_ptr(Projection));

	// Bind the texture

	glBindTexture(GL_TEXTURE_2D, VboTex);
	glActiveTexture(GL_TEXTURE0);

	glUniform1d(TextureUniformId, 0);

	glBindTexture(GL_TEXTURE_2D, 0);

	VertexArray::Unbind();
	ShaderProgram::Unbind();
}