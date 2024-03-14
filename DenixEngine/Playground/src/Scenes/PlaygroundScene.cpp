#include "PlaygroundScene.h"

#include "Denix/System/SceneSubsystem.h"
#include "Denix/Video/GL/VertexBuffer.h"
#include "Denix/Video/GL/VertexArray.h"
#include "Denix/Video/GL/GLShader.h"

namespace Denix
{
	// TestObject/////////////////////
	TestObject::TestObject() : GameObject(ObjectInitializer("Test Object"))
	{
		constexpr float squreData[] = {
			-0.5f, 0.5f, 0.0f, // top left point
			0.5f, 0.5f, 0.0f, // top right point
			0.5f, -0.5f, 0.0f, // bottom right point
			0.5f, -0.5f, 0.0f, // bottom right point
			-0.5f, -0.5f, 0.0f, // bottom left point
			-0.5f, 0.5f, 0.0f // top left point
		};

		// VBO creation
		Ref<VertexBuffer> vbo = m_MeshComponent->GetVertexBuffer();
		vbo->Bind(GL_ARRAY_BUFFER);
		vbo->BufferData(GL_ARRAY_BUFFER, sizeof(squreData), squreData, 3, 6, GL_FLOAT);
		
		Ref<VertexArray> vao = m_MeshComponent->GetVertexArray();

		vao->Bind();
		vbo->Bind(vbo->GetTarget());

		// Bind Attribute at Location 0
		vao->AttribPtr(vbo->GetPerPrimitive(), vbo->GetType());
		
		//// Reset the state
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
		//ShaderSubSystem* shaderSubSystem = ShaderSubSystem::Get();

		//const Ref<GLShader> program = m_RenderComponent->GetShader();//shaderSubSystem->GetShader("DebugShader");

		//// Move this to renderer in the future
		//program->Bind();
		//m_MeshComponent->GetVertexArray()->Bind();

		//// Upload the color
		//glUniform4fv(program->GetUniform("u_Color"), 1, &m_RenderComponent->GetDebugColor()[0]);

		//// Draw the triangle
		//glDrawArrays(GL_TRIANGLES, 0, 
		//	m_MeshComponent->GetVertexBuffer()->GetCount());

		//VertexArray::Unbind();
		//GLShader::Unbind();
	}

	////////////// PlaygroundScene ////////////////////////
	PlaygroundScene::PlaygroundScene(const ObjectInitializer& _objInit):
		Scene(_objInit)
	{ 
	}

	PlaygroundScene::~PlaygroundScene()
	{
	}

	bool PlaygroundScene::Load()
	{
		Scene::Load();

		m_SceneObjects.push_back(MakeRef<TestObject>());
		return true;
	}

	void PlaygroundScene::Unload()
	{
		Scene::Unload();
	}

	void PlaygroundScene::BeginScene()
	{
		Scene::BeginScene();

	}

	void PlaygroundScene::EndScene()
	{
		Scene::EndScene();
	}

	void PlaygroundScene::Update(float _deltaTime)
	{
		Scene::Update(_deltaTime);
	}
}