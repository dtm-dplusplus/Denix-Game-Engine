#include "DePch.h"
#include "ExampleScenes.h"

#include "imgui.h"
#include "imgui_internal.h"
#include "Video/GL/VertexBuffer.h"
#include "Video/GL/VertexArray.h"
#include "Video/GL/Shader.h"
#define STB_IMAGE_IMPLEMENTATION 
#include <stb_image.h>
#include "Object/Object.h"
#include "Object/GameObject.h"

////////////// ObjectScene ////////////////////////
ObjectScene::ObjectScene() : Scene(ObjectInitializer("Object Scene"))
{
}

ObjectScene::~ObjectScene() = default;

bool ObjectScene::Load()
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

	return true;
}

void ObjectScene::Unload()
{
	Scene::Unload();
}

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