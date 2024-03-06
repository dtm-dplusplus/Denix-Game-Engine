#include "DePch.h"
#include "DefaultScene.h"

#include "imgui.h"


#include "Video/GL/VertexBuffer.h"
#include "Video/GL/VertexArray.h"
#include "Video/GL/Shader.h"

#include "Object.h"
#include "GameObject.h"
#include "../System/SceneSubsystem.h"


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
	// Move this to renderer in the future
	Program->Bind();
	Vao->Bind();

	// Upload the model matrix
	glUniformMatrix4fv(ModelUniformId, 1, GL_FALSE, glm::value_ptr(m_TransformComponent->GetModel()));

	if(const Ref<Camera> camera = SceneSubSystem::Get()->GetActiveCamera())
	{
		// Upload the projection matrix
		glUniformMatrix4fv(ProjectionUniformId, 1, GL_FALSE, glm::value_ptr(camera->GetProjectionMatrix()));

		// Upload the view matrix
		glUniformMatrix4fv(ViewUniformId, 1, GL_FALSE, glm::value_ptr(camera->GetViewMatrix()));
	}
	

	// Upload the color
	glUniform4fv(ColorUniformId, 1, &m_RenderComponent->GetDebugColor()[0]);

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
	m_SceneObjects.push_back(std::make_shared<TestObject>());
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
	
}