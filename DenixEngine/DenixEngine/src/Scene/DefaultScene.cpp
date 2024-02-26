#include "DePch.h"
#include "DefaultScene.h"

#include "imgui.h"
#include "imgui_internal.h"
#include "Video/GL/VertexBuffer.h"
#include "Video/GL/VertexArray.h"
#include "Video/GL/Shader.h"

#include "Object/Object.h"
#include "Object/GameObject.h"


// TestObject/////////////////////

TestObject::TestObject() : GameObject(ObjectInitializer("Test Object"))
{
	
}

TestObject::~TestObject() = default;

void TestObject::Draw()
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

void TestObject::Update()
{
	// Update UI
	{
		ImGui::Begin(GetName().c_str());
		ImGui::Columns(2);

		ImGui::Text("Color");
		ImGui::Text("Is Rotating");
		ImGui::Text("Rotation Speed");
		ImGui::NextColumn();

		ImGui::ColorEdit4("##", &Color[0]);
		if (ImGui::Checkbox("##", &IsRotating)) Angle = 0.f;
		ImGui::DragFloat("##", &RotSpeed);
		ImGui::Columns();

		ImGui::SeparatorText("Camera");
		ImGui::Columns(2);
		ImGui::Text("Position");
		ImGui::Text("Perspective Projection");
		ImGui::Text("Fov");
		ImGui::Text("Near Plane");
		ImGui::Text("Far Plane");
		ImGui::NextColumn();


		ImGui::DragFloat3("##", &CamPos[0]);
		ImGui::Checkbox("##", &IsPerspective);
		ImGui::DragFloat("##", &Fov);
		ImGui::DragFloat("##", &NearPlane);
		ImGui::DragFloat("##", &FarPlane);
		ImGui::Columns();

		ImGui::SeparatorText("Transform");
		ImGui::Columns(2);
		ImGui::Text("Position");
		ImGui::Text("Rotation");
		ImGui::Text("Scale");
		ImGui::NextColumn();

		ImGui::DragFloat3("##", &m_TransformComponent->GetPosition()[0]);
		ImGui::DragFloat3("##", &m_TransformComponent->GetRotation()[0]);
		ImGui::DragFloat3("##", &m_TransformComponent->GetScale()[0]);
		ImGui::Columns();
		ImGui::End();
	}
	

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
	Model = glm::translate(glm::mat4(1.0f), CamPos);
	Model = glm::rotate(Model, glm::radians(Angle), glm::vec3(0, 1, 0));
	Model = glm::scale(Model, m_TransformComponent->GetScale());

	// Increase the float angle so next frame the triangle rotates further
	Angle += RotSpeed;
}

void TestObject::Setup()
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
}

////////////// DefaultScene ////////////////////////
DefaultScene::DefaultScene() : Scene(ObjectInitializer("Object Scene"))
{
}

DefaultScene::~DefaultScene() = default;

bool DefaultScene::Load()
{
	//const GLfloat data[] = {
	//	-0.5f, 0.5f, 0.0f, // top left point
	//	0.5f, 0.5f, 0.0f, // top right point
	//	0.5f, -0.5f, 0.0f, // bottom right point
	//	0.5f, -0.5f, 0.0f, // bottom right point
	//	-0.5f, -0.5f, 0.0f, // bottom left point
	//	-0.5f, 0.5f, 0.0f // top left point
	//};
	//
	//// VBO creation
	//Vbo = std::make_shared<VertexBuffer>
	//	(GL_ARRAY_BUFFER, sizeof(data), data, 3, GL_FLOAT);
	//
	//
	//// Create a new VAO on the GPU and bind it
	//Vao = std::make_shared<VertexArray>();
	//Vao->GenVertexArray();
	//Vao->Bind();
	//
	//// Bind the vbos & attribs
	//Vbo->Bind(GL_ARRAY_BUFFER);
	//Vao->AttribPtr(Vbo->GetCount(), GL_FLOAT);
	//
	//// Reset the state
	//VertexBuffer::Unbind(GL_ARRAY_BUFFER);
	//VertexArray::Unbind();
	//
	//// Create ShaderProgram
	//Program = std::make_shared<ShaderProgram>();
	//Program->CreateProgram();
	//Program->CompileShader(GL_VERTEX_SHADER, File::Read("res/shaders/UniVert.glsl"));
	//Program->CompileShader(GL_FRAGMENT_SHADER, File::Read("res/shaders/UniFrag.glsl"));
	//Program->AttachShaders();
	//
	//Program->BindAttrib(0, "a_Position");
	//Program->LinkProgram();
	//
	//// Check Uniforms
	//ModelUniformId = Program->GetUniform("u_Model");
	//ProjectionUniformId = Program->GetUniform("u_Projection");
	//ColorUniformId = Program->GetUniform("u_Color");

	Obj = std::make_shared<TestObject>();
	Obj->Setup();

	return true;
}

void DefaultScene::Unload()
{
	Scene::Unload();
}

void DefaultScene::Update()
{
	Obj->Update();
}

void DefaultScene::Draw()
{
	Obj->Draw();

}