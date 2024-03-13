#include "DePch.h"
#include "ExampleScenes.h"

#include "imgui.h"


#include "Denix/Video/GL/VertexBuffer.h"
#include "Denix/Video/GL/VertexArray.h"
#include "Denix/Video/GL/GLShader.h"

#include "Object.h"
#include "GameObject.h"
#include "../System/SceneSubsystem.h"
#include "../System/ShaderSubSystem.h"

namespace Denix
{
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
		Vbo = MakeRef<VertexBuffer>
			(GL_ARRAY_BUFFER, sizeof(data), data, 3, GL_FLOAT);


		// Create a new VAO on the GPU and bind it
		Vao = MakeRef<VertexArray>();
		Vao->GenVertexArray();
		Vao->Bind();

		// Bind the vbos & attribs
		Vbo->Bind(GL_ARRAY_BUFFER);
		Vao->AttribPtr(Vbo->GetCount(), GL_FLOAT);

		// Reset the state
		VertexBuffer::Unbind(GL_ARRAY_BUFFER);
		VertexArray::Unbind();
	}

	TestObject::~TestObject() = default;

	void TestObject::BeginScene()
	{
		GameObject::BeginScene();
	}

	void TestObject::EndScene()
	{
		GameObject::EndScene();
	}

	void TestObject::Update(float _deltaTime)
	{
		ShaderSubSystem* shaderSubSystem = ShaderSubSystem::Get();

		const Ref<GLShader> shader = shaderSubSystem->GetShader("DebugShader");

		// Move this to renderer in the future
		shader->Bind();
		Vao->Bind();

		// Upload the model matrix
		glUniformMatrix4fv(shader->GetUniform("u_Model"), 1, GL_FALSE, glm::value_ptr(m_TransformComponent->GetModel()));

		if (const Ref<Camera> camera = SceneSubSystem::Get()->GetActiveCamera())
		{
			// Upload the projection matrix
			glUniformMatrix4fv(shader->GetUniform("u_Projection"), 1, GL_FALSE, glm::value_ptr(camera->GetProjectionMatrix()));

			// Upload the view matrix
			glUniformMatrix4fv(shader->GetUniform("u_View"), 1, GL_FALSE, glm::value_ptr(camera->GetViewMatrix()));
		}


		// Upload the color
		glUniform4fv(shader->GetUniform("u_Color"), 1, &m_RenderComponent->GetDebugColor()[0]);

		// Draw the triangle
		glDrawArrays(GL_TRIANGLES, 0, 6);

		VertexArray::Unbind();
		GLShader::Unbind();
	}

	////////////// DefaultScene ////////////////////////
	DefaultScene::DefaultScene() : Scene(ObjectInitializer("DefaultScene"))
	{
	}

	DefaultScene::~DefaultScene()
	{
	}

	bool DefaultScene::Load()
	{
		Scene::Load();

		m_SceneObjects.push_back(MakeRef<TestObject>());
		return true;
	}

	void DefaultScene::Unload()
	{
		Scene::Unload();
	}

	void DefaultScene::BeginScene()
	{
		Scene::BeginScene();

	}

	void DefaultScene::EndScene()
	{
		Scene::EndScene();
	}

	void DefaultScene::Update(float _deltaTime)
	{
		Scene::Update(_deltaTime);
	}
}