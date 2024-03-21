#include "LightingScene.h"
#include "Denix/Core.h"
#include "Denix/System/ShaderSubSystem.h"

using namespace Denix;

LightingScene::LightingScene(const Denix::ObjectInitializer& _objInit)
{
}

LightingScene::~LightingScene()
{
}

bool LightingScene::Load()
{
	return Scene::Load();
}

void LightingScene::Unload()
{
	Scene::Unload();
}

void LightingScene::EndScene()
{
	Scene::EndScene();
}

void LightingScene::Update(float _deltaTime)
{
	Scene::Update(_deltaTime);
}

void LightingScene::BeginScene()
{
	Scene::BeginScene();

	ShaderSubsystem* shaderSubsystem = ShaderSubsystem::Get();
	// Create Debug Shader
	std::vector<std::pair<GLenum, std::string>> shaders;
	shaders.emplace_back(GL_VERTEX_SHADER, File::Read("res\\shaders\\LightVertex.glsl"));
	shaders.emplace_back(GL_FRAGMENT_SHADER, File::Read("res\\shaders\\LightFragment.glsl"));
	shaderSubsystem->LoadShader(shaders, "LightShader");

	const Ref<GLShader> lightProgram = shaderSubsystem->GetShader("LightShader");

	// Check Uniforms
	lightProgram->GetUniform("u_Model");
	lightProgram->GetUniform("u_Projection");
	lightProgram->GetUniform("u_Color");
	lightProgram->GetUniform("u_View");


	const Ref<LightCube> lightCube = MakeRef<LightCube>();

	// Posiiton Buffer
	const Ref<VertexBuffer> positionVbo = lightCube->GetMeshComponent()->GetVertexBuffer();
	positionVbo->Bind(GL_ARRAY_BUFFER);
	positionVbo->BufferData(GL_ARRAY_BUFFER, sizeof(CubeData), CubeData, 3, 36, GL_FLOAT);


	// Normal Buffer
	lightCube->NormalBuffer = MakeRef<VertexBuffer>();
	lightCube->NormalBuffer->Bind(GL_ARRAY_BUFFER);
	lightCube->NormalBuffer->BufferData(GL_ARRAY_BUFFER, sizeof(CubeData), CubeData, 3, 36, GL_FLOAT);

	// Bind the VAO attributes
	const Ref<VertexArray> vao = lightCube->GetMeshComponent()->GetVertexArray();
	vao->Bind();

	positionVbo->Bind(GL_ARRAY_BUFFER);
	vao->AttribPtr(positionVbo->GetPerPrimitive(), positionVbo->GetType());

	lightCube->NormalBuffer->Bind(GL_ARRAY_BUFFER);
	vao->AttribPtr(lightCube->NormalBuffer->GetPerPrimitive(), lightCube->NormalBuffer->GetType());

	//// Reset the state
	VertexBuffer::Unbind(GL_ARRAY_BUFFER);
	VertexArray::Unbind();
}