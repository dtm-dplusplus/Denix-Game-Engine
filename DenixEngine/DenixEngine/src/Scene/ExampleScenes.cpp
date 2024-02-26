#include "DePch.h"
#include "ExampleScenes.h"

#include "imgui.h"
#include "imgui_internal.h"
#include "Video/GL/VertexBuffer.h"
#include "Video/GL/VertexArray.h"
#include "Video/GL/Shader.h"
#define STB_IMAGE_IMPLEMENTATION 
#include <stb_image.h>
#include "Object.h"
#include "GameObject.h"

////////////// ObjectScene ////////////////////////
ObjectScene::ObjectScene()
{
	const GLfloat data[] = {
		-0.5f, 0.5f, 0.0f, // top left point
		0.5f, 0.5f, 0.0f, // top right point
		0.5f, -0.5f, 0.0f, // bottom right point
		0.5f, -0.5f, 0.0f, // bottom right point
		-0.5f, -0.5f, 0.0f, // bottom left point
		-0.5f, 0.5f, 0.0f // top left point
	};

	// VBO creation
	Vbo = std::make_shared<VertexBuffer>
		(GL_ARRAY_BUFFER, sizeof(data), data, 3, GL_FLOAT);


	// Create a new VAO on the GPU and bind it
	Vao = std::make_shared<VertexArray>();
	Vao->GenVertexArray();
	Vao->Bind();

	// Bind the vbos & attribs
	Vbo->Bind(GL_ARRAY_BUFFER);
	Vao->AttribPtr(Vbo->GetCount(), GL_FLOAT);

	// Reset the state
	VertexBuffer::Unbind(GL_ARRAY_BUFFER);
	VertexArray::Unbind();

	// Create ShaderProgram
	Program = std::make_shared<ShaderProgram>();
	Program->CreateProgram();
	Program->CompileShader(GL_VERTEX_SHADER, File::Read("res/shaders/UniVert.glsl"));
	Program->CompileShader(GL_FRAGMENT_SHADER, File::Read("res/shaders/UniFrag.glsl"));
	Program->AttachShaders();

	Program->BindAttrib(0, "a_Position");
	Program->LinkProgram();

	// Check Uniforms
	ModelUniformId = Program->GetUniform("u_Model");
	ProjectionUniformId = Program->GetUniform("u_Projection");
	ColorUniformId = Program->GetUniform("u_Color");

	TestObject = std::make_shared<GameObject>(ObjectInitializer{ "Square" });
}

ObjectScene::~ObjectScene() = default;

void ObjectScene::Update()
{
	// Update UI
	ImGui::Begin("ObjectScene Settings");
	ImGui::SeparatorText("Color");
	ImGui::ColorEdit4("Color", &Color[0]);

	ImGui::SeparatorText("Camera");
	ImGui::DragFloat3("CamPos", &CamPos[0]);
	ImGui::Checkbox("Perspec Proj", &IsPerspective);
	ImGui::DragFloat("Fov", &Fov);
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
		Projection = glm::perspective(glm::radians(Fov), 800.f / 600.f, NearPlane, FarPlane);
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

void ObjectScene::Draw()
{

	// Move this to renderer in the future
	Program->Bind();
	Vao->Bind();

	// Upload the model matrix
	glUniformMatrix4fv(ModelUniformId, 1, GL_FALSE, glm::value_ptr(Model));

	// Upload the projection matrix
	glUniformMatrix4fv(ProjectionUniformId, 1, GL_FALSE, glm::value_ptr(Projection));


	glUniform4fv(ColorUniformId, 1, &Color[0]);
	glDrawArrays(GL_TRIANGLES, 0, 6);

	VertexArray::Unbind();
	ShaderProgram::Unbind();
}


////////////// TextureScene  ////////////////////////
TextureScene::TextureScene()
{
	const GLfloat positions[] = {
		-0.5f, 0.5f, 0.0f, // top left point
		0.5f, 0.5f, 0.0f, // top right point
		0.5f, -0.5f, 0.0f, // bottom right point
		0.5f, -0.5f, 0.0f, // bottom right point
		-0.5f, -0.5f, 0.0f, // bottom left point
		-0.5f, 0.5f, 0.0f // top left point
	};

	float coords[] = {
		0.0f, 1.0f, // Top-left
		1.0f, 1.0f, // Top-right
		1.0f, 0.0f, // Bottom-right
		0.0f, 0.0f  // Bottom-left
	};

	// Create VBOs
	VboPos = std::make_shared<VertexBuffer>
		(GL_ARRAY_BUFFER, sizeof(positions), positions, 3, GL_FLOAT);

	VboTexCoord = std::make_shared<VertexBuffer>
		(GL_ARRAY_BUFFER, sizeof(coords), coords, 2, GL_FLOAT);

	// Create a new VAO on the GPU and bind it
	Vao = std::make_shared<VertexArray>();
	Vao->GenVertexArray();
	Vao->Bind();

	// Bind the vbos & attribs
	VboPos->Bind(GL_ARRAY_BUFFER);
	Vao->AttribPtr(VboPos->GetCount(), GL_FLOAT);

	VboTexCoord->Bind(GL_ARRAY_BUFFER);
	Vao->AttribPtr(VboTexCoord->GetCount(), GL_FLOAT);

	// Reset the state
	VertexBuffer::Unbind(GL_ARRAY_BUFFER);
	VertexArray::Unbind();

	// Create ShaderProgram
	Program = std::make_shared<ShaderProgram>();
	Program->CreateProgram();
	Program->CompileShader(GL_VERTEX_SHADER, File::Read("res/shaders/TexVert.glsl"));
	Program->CompileShader(GL_FRAGMENT_SHADER, File::Read("res/shaders/TexFrag.glsl"));
	Program->AttachShaders();

	Program->BindAttrib(0, "in_Position");
	Program->BindAttrib(1, "in_TexCoord");
	Program->LinkProgram();

	// Check Uniforms
	ProjectionUniformId = Program->GetUniform("in_Projection");
	ModelUniformId = Program->GetUniform("in_Model");
	TextureUniformId = Program->GetUniform("in_Texture");

	// Create and bind a texture.
	{
		int w = 0;
		int h = 0;

		unsigned char* data = stbi_load("res/textures/Image.jpg", &w, &h, NULL, 4);

		glGenTextures(1, &TextureID);

		if (!TextureID)
		{
			throw std::exception();
		}

		glBindTexture(GL_TEXTURE_2D, TextureID);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		free(data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glBindTexture(GL_TEXTURE_2D, 0);

		glActiveTexture(GL_TEXTURE0 + 1);
		glBindTexture(GL_TEXTURE_2D, TextureID);
	}
}

TextureScene::~TextureScene() = default;

void TextureScene::Update()
{
	// Update UI
	ImGui::Begin("Lab2 Settings");
	ImGui::SeparatorText("Rendering");
	//if (ImGui::Checkbox("Cull Face", &IsCulled)) Angle = 0.f;


	ImGui::SeparatorText("Camera");
	ImGui::DragFloat3("CamPos", &CamPos[0]);
	ImGui::Checkbox("Perspec Proj", &IsPerspective);
	ImGui::DragFloat("Fov", &Fov);
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
		Projection = glm::perspective(glm::radians(Fov), 800.f / 600.f, NearPlane, FarPlane);
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

void TextureScene::Draw()
{

	// Move this to renderer in the future
	Program->Bind();
	Vao->Bind();

	// Upload the model matrix
	glUniformMatrix4fv(ModelUniformId, 1, GL_FALSE, glm::value_ptr(Model));

	// Upload the projection matrix
	glUniformMatrix4fv(ProjectionUniformId, 1, GL_FALSE, glm::value_ptr(Projection));

	// Bind the texture
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, TextureID);

	// U[load the texture uniform
	glUniform1d(TextureUniformId, 1);
	glDrawArrays(GL_TRIANGLES, 0, 6);

	glBindTexture(GL_TEXTURE_2D, 0);

	VertexArray::Unbind();
	ShaderProgram::Unbind();
}