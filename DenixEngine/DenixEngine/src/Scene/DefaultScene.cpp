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
	ViewUniformId = Program->GetUniform("u_View");

}

TestObject::~TestObject() = default;

void TestObject::Update(float _deltaTime)
{
	Ref<Camera> camera = Engine::Get().GetEngineScene()->GetCamera();
	

	// Rotate the object
	if (IsRotating) m_TransformComponent->GetRotation() += glm::vec3(0.f, RotSpeed, 0.f);

	// Move this to renderer in the future
	Program->Bind();
	Vao->Bind();

	// Upload the model matrix
	glUniformMatrix4fv(ModelUniformId, 1, GL_FALSE, glm::value_ptr(m_TransformComponent->GetModel()));

	// Upload the projection matrix
	glUniformMatrix4fv(ProjectionUniformId, 1, GL_FALSE, glm::value_ptr(camera->GetProjectionMatrix()));

	// Upload the view matrix
	glUniformMatrix4fv(ViewUniformId, 1, GL_FALSE, glm::value_ptr(camera->GetViewMatrix()));

	// Upload the color
	glUniform4fv(ColorUniformId, 1, &Color[0]);

	// Draw the triangle
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

void DefaultScene::Update(float _deltaTime)
{
	static float dragSpeed = 1.0f;
	static bool showDemoWindow = false;

	const float dragSpeedDelta = dragSpeed * _deltaTime;

	//// Update Camera
	//ImGui::Begin(m_Name.c_str());
	//ImGui::SeparatorText("Scene Properties");
	//ImGui::Checkbox("Show Demo Window", &showDemoWindow);
	//if (showDemoWindow) ImGui::ShowDemoWindow(&showDemoWindow);
	//ImGui::DragFloat("UI Drag Speed", &dragSpeed, 0.1f, 0.1f, 10.0f);
	//

	//// Show Scen Object Properties
	//for (const auto& obj : m_SceneObjects)
	//{
	//	ImGui::PushID(obj->GetID());
	//	if (ImGui::CollapsingHeader(obj->GetName().c_str(), ImGuiTreeNodeFlags_DefaultOpen))
	//	{
	//		ImGui::SeparatorText("Transform");
	//		ImGui::DragFloat3("Position", &obj->GetTransformComponent()->GetPosition()[0], dragSpeedDelta); ImGui::SameLine();
	//		if (ImGui::ArrowButton("##ResetPosition", ImGuiDir_Left)) obj->GetTransformComponent()->SetPosition(glm::vec3(0.f));
	//		ImGui::SetItemTooltip("Reset");

	//		ImGui::DragFloat3("Rotation", &obj->GetTransformComponent()->GetRotation()[0], dragSpeedDelta); ImGui::SameLine();
	//		if (ImGui::ArrowButton("##ResetRotation", ImGuiDir_Left)) obj->GetTransformComponent()->SetRotation(glm::vec3(0.f));
	//		ImGui::SetItemTooltip("Reset");

	//		ImGui::DragFloat3("Scale", &obj->GetTransformComponent()->GetScale()[0], dragSpeedDelta); ImGui::SameLine();
	//		if (ImGui::ArrowButton("##ResetScale", ImGuiDir_Left)) obj->GetTransformComponent()->SetScale(glm::vec3(1.f));
	//		ImGui::SetItemTooltip("Reset");

	//		if(typeid(Camera) == typeid(*obj))
	//		{
	//			ImGui::SeparatorText("Camera Properties");
	//			ImGui::DragFloat("MoveSpeed", &m_Camera->MoveSpeed, dragSpeedDelta);
	//			

	//			
	//			ImGui::Checkbox("Perspective Projection", &m_Camera->IsPerspective);
	//			ImGui::DragFloat("Fov", &m_Camera->Fov, dragSpeedDelta);
	//			ImGui::DragFloat("Near Plane", &m_Camera->NearPlane, dragSpeedDelta);
	//			ImGui::DragFloat("Far Plane", &m_Camera->FarPlane, dragSpeedDelta);
	//		}
	//		else if (typeid(TestObject) == typeid(*obj))
	//		{
	//			if(auto* testObj = dynamic_cast<TestObject*>(obj.get()))
	//			{
	//				ImGui::SeparatorText("Test Object Properties");
	//				ImGui::ColorEdit4("Color", &testObj->Color[0], dragSpeedDelta);
	//				if (ImGui::Checkbox("Is Rotating", &testObj->IsRotating)) testObj->GetTransformComponent()->SetRotation(glm::vec3(0.f));
	//				ImGui::DragFloat("Rotation Speed", &testObj->RotSpeed, dragSpeedDelta);
	//			}
	//		}
	//	}
	//	ImGui::PopID();
	//}

	//ImGui::End();

	static bool p_open = true;
	const glm::vec2 winSize = Engine::Get().GetEngineWindow()->GetWindowSize();

	ImGui::SetNextWindowSize(ImVec2(winSize.x/4, winSize.y), ImGuiCond_Once);
	ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Once);

	ImGui::Begin("Scene Panel", &p_open, ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoCollapse);
	{
		// Left
		static int selected = 0;
		{
			ImGui::BeginChild("left pane", ImVec2(150, 0), ImGuiChildFlags_Border | ImGuiChildFlags_ResizeX );
			for (int i = 0; i < m_SceneObjects.size(); i++)
			{
				// FIXME: Good candidate to use ImGuiSelectableFlags_SelectOnNav
				if (ImGui::Selectable(m_SceneObjects[i]->GetName().c_str(), selected == i))
					selected = i;
			}
			ImGui::EndChild();
		}
		ImGui::SameLine();

		// Right
		{
			ImGui::BeginGroup();
			ImGui::BeginChild("Object Properties", ImVec2(0, -ImGui::GetFrameHeightWithSpacing())); // Leave room for 1 line below us
			ImGui::Text("Properties");
			ImGui::Separator();

			ImGui::SeparatorText("Transform");
			ImGui::DragFloat3("Position", &m_SceneObjects[selected]->GetTransformComponent()->GetPosition()[0], dragSpeedDelta); ImGui::SameLine();
			if (ImGui::ArrowButton("##ResetPosition", ImGuiDir_Left)) m_SceneObjects[selected]->GetTransformComponent()->SetPosition(glm::vec3(0.f));
			ImGui::SetItemTooltip("Reset");

			ImGui::DragFloat3("Rotation", &m_SceneObjects[selected]->GetTransformComponent()->GetRotation()[0], dragSpeedDelta); ImGui::SameLine();
			if (ImGui::ArrowButton("##ResetRotation", ImGuiDir_Left)) m_SceneObjects[selected]->GetTransformComponent()->SetRotation(glm::vec3(0.f));
			ImGui::SetItemTooltip("Reset");

			ImGui::DragFloat3("Scale", &m_SceneObjects[selected]->GetTransformComponent()->GetScale()[0], dragSpeedDelta); ImGui::SameLine();
			if (ImGui::ArrowButton("##ResetScale", ImGuiDir_Left)) m_SceneObjects[selected]->GetTransformComponent()->SetScale(glm::vec3(1.f));
			ImGui::SetItemTooltip("Reset");

			if (typeid(Camera) == typeid(*m_SceneObjects[selected]))
			{
				if (Camera* camera = dynamic_cast<Camera*>(m_SceneObjects[selected].get()))

				ImGui::SeparatorText("Camera Properties");
				ImGui::DragFloat("MoveSpeed", &m_Camera->MoveSpeed, dragSpeedDelta);
				if (ImGui::ArrowButton("##ResetMoveSpeed", ImGuiDir_Left)) m_Camera->MoveSpeed = 0.1f;
				ImGui::SetItemTooltip("Reset");


				ImGui::Checkbox("Perspective Projection", &m_Camera->IsPerspective);

				ImGui::DragFloat("Fov", &m_Camera->Fov, dragSpeedDelta); ImGui::SameLine();
				if (ImGui::ArrowButton("##ResetFov", ImGuiDir_Left)) m_Camera->Fov = 45.f;
				ImGui::SetItemTooltip("Reset");

				ImGui::DragFloat("Near Plane", &m_Camera->NearPlane, dragSpeedDelta); ImGui::SameLine();
				if (ImGui::ArrowButton("##ResetNear Plane", ImGuiDir_Left)) m_Camera->NearPlane = 0.1f;
				ImGui::SetItemTooltip("Reset");

				ImGui::DragFloat("Far Plane", &m_Camera->FarPlane, dragSpeedDelta); ImGui::SameLine();
				if (ImGui::ArrowButton("##ResetFar Plane", ImGuiDir_Left)) m_Camera->FarPlane = 100.f;
				ImGui::SetItemTooltip("Reset");
			}
			else if (typeid(TestObject) == typeid(*m_SceneObjects[selected]))
			{
				if (TestObject* testObj = dynamic_cast<TestObject*>(m_SceneObjects[selected].get()))
				{
					ImGui::SeparatorText("Test Object Properties");
					ImGui::ColorEdit4("Color", &testObj->Color[0]);
					if (ImGui::Checkbox("Is Rotating", &testObj->IsRotating)) testObj->GetTransformComponent()->SetRotation(glm::vec3(0.f));

					ImGui::DragFloat("Rotation Speed", &testObj->RotSpeed, dragSpeedDelta); ImGui::SameLine();
					if (ImGui::ArrowButton("##Rotation Speed", ImGuiDir_Left)) testObj->RotSpeed = .1f;
					ImGui::SetItemTooltip("Reset");
				}
			}

			ImGui::EndChild();
			ImGui::EndGroup();
		}
	}
	ImGui::End();

	// Move Camera
	if (const Uint8* keyboard = SDL_GetKeyboardState(NULL))
	{
		if(keyboard[SDL_SCANCODE_W])
		{
			DE_LOG(LogScene, Info, "W Pressed")
			m_Camera->GetTransformComponent()->GetPosition().z += m_Camera->MoveSpeed * _deltaTime;
		}
		if (keyboard[SDL_SCANCODE_S])
		{
			DE_LOG(LogScene, Info, "S Pressed")
			m_Camera->GetTransformComponent()->GetPosition().z -= m_Camera->MoveSpeed * _deltaTime;
		}
		if (keyboard[SDL_SCANCODE_A])
		{
			DE_LOG(LogScene, Info, "A Pressed")
			m_Camera->GetTransformComponent()->GetPosition().x += m_Camera->MoveSpeed * _deltaTime;

		}
		if (keyboard[SDL_SCANCODE_D])
		{
			DE_LOG(LogScene, Info, "D Pressed")
			m_Camera->GetTransformComponent()->GetPosition().x -= m_Camera->MoveSpeed * _deltaTime;
		}
		if (keyboard[SDL_SCANCODE_E])
		{
			DE_LOG(LogScene, Info, "E Pressed")
			m_Camera->GetTransformComponent()->GetPosition().y -= m_Camera->MoveSpeed * _deltaTime;
		}
		if (keyboard[SDL_SCANCODE_Q])
		{
			DE_LOG(LogScene, Info, "Q Pressed")
			m_Camera->GetTransformComponent()->GetPosition().y += m_Camera->MoveSpeed * _deltaTime;
		}
	}
}