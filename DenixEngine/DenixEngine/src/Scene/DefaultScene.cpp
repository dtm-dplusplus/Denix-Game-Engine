#include "DePch.h"
#include "DefaultScene.h"

#include "imgui.h"
#include "Video/GL/VertexBuffer.h"
#include "Video/GL/VertexArray.h"
#include "Video/GL/Shader.h"

#include "Object/Object.h"
#include "Object/GameObject.h"


// TestObject/////////////////////

TestObject::TestObject() : GameObject(ObjectInitializer("Test Object"))
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

TestObject::~TestObject() = default;

void TestObject::Update()
{

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
	m_TransformComponent->Update();
	
	// Increase the float angle so next frame the triangle rotates further
	Angle += RotSpeed;

	// Move this to renderer in the future
	Program->Bind();
	Vao->Bind();

	// Upload the model matrix
	glUniformMatrix4fv(ModelUniformId, 1, GL_FALSE, glm::value_ptr(m_TransformComponent->GetModel()));

	// Upload the projection matrix
	glUniformMatrix4fv(ProjectionUniformId, 1, GL_FALSE, glm::value_ptr(Projection));


	glUniform4fv(ColorUniformId, 1, &Color[0]);
	glDrawArrays(GL_TRIANGLES, 0, 6);

	VertexArray::Unbind();
	ShaderProgram::Unbind();
}


////////////// DefaultScene ////////////////////////
DefaultScene::DefaultScene() : Scene(ObjectInitializer("Object Scene"))
{
}

DefaultScene::~DefaultScene() = default;

bool DefaultScene::Load()
{
	Obj = std::make_shared<TestObject>();
	m_SceneObjects.push_back(Obj);
	return true;
}

void DefaultScene::Unload()
{
	Scene::Unload();
}

void DefaultScene::Update()
{
	//m_SceneObjects[0]->Update();

	// Update Camera
	ImGui::Begin("Scene");
	ImGui::SeparatorText("Camera");
	//ImGui::DragFloat3("Position", &m_Camera.Position[0]);
	//ImGui::Checkbox("Perspective Projection", &m_Camera.IsPerspective);
	//ImGui::DragFloat("Fov", &m_Camera.Fov);
	//ImGui::DragFloat("Near Plane", &m_Camera.NearPlane);
	//ImGui::DragFloat("Far Plane", &m_Camera.FarPlane);

	for (const auto& obj : m_SceneObjects)
	{
		if (ImGui::CollapsingHeader(obj->GetName().c_str(), ImGuiTreeNodeFlags_DefaultOpen))
		{
			ImGui::SeparatorText("Transform");
			ImGui::DragFloat3("Position", &obj->GetTransformComponent()->GetPosition()[0]);
			ImGui::DragFloat3("Rotation", &obj->GetTransformComponent()->GetRotation()[0]);
			ImGui::DragFloat3("Scale", &obj->GetTransformComponent()->GetScale()[0]);

			// Show TestObject UI
			if (typeid(*obj) == typeid(TestObject))
			{
				TestObject* testObj = dynamic_cast<TestObject*>(obj.get());
				ImGui::ColorEdit4("Color", &testObj->Color[0]);
				if (ImGui::Checkbox("Is Rotating", &testObj->IsRotating)) testObj->Angle = 0.f;
				ImGui::DragFloat("Rotation Speed", &testObj->RotSpeed);
			}
		}
	}

	ImGui::End();
}