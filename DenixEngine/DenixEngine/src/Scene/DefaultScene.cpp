#include "DePch.h"
#include "DefaultScene.h"

#include "imgui.h"
#include "../Engine.h"
#include "Video/GL/VertexBuffer.h"
#include "Video/GL/VertexArray.h"
#include "Video/GL/Shader.h"

#include "Object.h"
#include "GameObject.h"


// TestObject/////////////////////

TestObject::TestObject() : GameObject(ObjectInitializer("Test Object"))
{
	constexpr GLfloat data[] = {
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

	glm::mat4 camProjection = Engine::Get().GetEngineScene()->GetCamera()->GetProjectionMatrix();

	// Increase the float angle so next frame the triangle rotates further
	Angle += RotSpeed;

	// Move this to renderer in the future
	Program->Bind();
	Vao->Bind();

	// Upload the model matrix
	glUniformMatrix4fv(ModelUniformId, 1, GL_FALSE, glm::value_ptr(m_TransformComponent->GetModel()));

	// Upload the projection matrix
	glUniformMatrix4fv(ProjectionUniformId, 1, GL_FALSE, glm::value_ptr(camProjection));


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
	m_Camera = std::make_shared<Camera>();
	m_SceneObjects.push_back(m_Camera);

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
	// Update Camera
	ImGui::Begin(m_Name.c_str());
	
	for (const auto& obj : m_SceneObjects)
	{
		ImGui::PushID(obj->GetID());
		if (ImGui::CollapsingHeader(obj->GetName().c_str(), ImGuiTreeNodeFlags_DefaultOpen))
		{
			ImGui::SeparatorText("Transform");
			ImGui::DragFloat3("Position", &obj->GetTransformComponent()->GetPosition()[0]);
			ImGui::DragFloat3("Rotation", &obj->GetTransformComponent()->GetRotation()[0]);
			ImGui::DragFloat3("Scale", &obj->GetTransformComponent()->GetScale()[0]);

			if(typeid(Camera) == typeid(*obj))
			{
				ImGui::SeparatorText("Camera Properties");
				ImGui::Checkbox("Perspective Projection", &m_Camera->IsPerspective);
				ImGui::DragFloat("Fov", &m_Camera->Fov);
				ImGui::DragFloat("Near Plane", &m_Camera->NearPlane);
				ImGui::DragFloat("Far Plane", &m_Camera->FarPlane);
			}
			else if (typeid(TestObject) == typeid(*obj))
			{
				if(auto* testObj = dynamic_cast<TestObject*>(obj.get()))
				{
					ImGui::SeparatorText("Test Object Properties");
					ImGui::ColorEdit4("Color", &testObj->Color[0]);
					if (ImGui::Checkbox("Is Rotating", &testObj->IsRotating)) testObj->Angle = 0.f;
					ImGui::DragFloat("Rotation Speed", &testObj->RotSpeed);
				}
			}
		}
		ImGui::PopID();
	}

	ImGui::End();
}