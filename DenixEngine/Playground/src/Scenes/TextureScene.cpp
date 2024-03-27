#include "TextureScene.h"
#include "Denix/System/ShaderSubSystem.h"
#include <stb_image.h>

using namespace Denix;

bool TextureScene::Load()
{
	Scene::Load();

	m_Camera->GetTransformComponent()->SetPosition({ 0.0, 0.0, 50.0 });
	m_Camera->GetFov() = 80.0f;

	TexPlane = MakeRef<TexturePlane>();
	TexPlane->GetRenderComponent()->SetIsVisible(false);
	m_SceneObjects.push_back(TexPlane);

	// Position Buffer
	glGenBuffers(1, &TexPlane->VBO);
	glBindBuffer(GL_ARRAY_BUFFER, TexPlane->VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(TexPlane->Vertices), TexPlane->Vertices, GL_STATIC_DRAW);
	
	// Index Buffer
	glGenBuffers(1, &TexPlane->IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, TexPlane->IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(TexPlane->Indices), TexPlane->Indices, GL_STATIC_DRAW);
	
	//// Texture Coords Buffer
	//glGenBuffers(1, &TexPlane->TCBO);
	//glBindBuffer(GL_ARRAY_BUFFER, TexPlane->TCBO);
	//// Texture
	//glGenTextures(1, &TexPlane->texture);
	//glBindTexture(GL_TEXTURE_2D, TexPlane->texture);
	//// set the texture wrapping/filtering options (on the currently bound texture object)
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//// load and generate the texture
	//int width, height, nrChannels;
	//unsigned char* data = stbi_load("container.jpg", &width, &height, &nrChannels, 0);
	//if (data)
	//{
	//	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	//	glGenerateMipmap(GL_TEXTURE_2D);
	//}
	//else
	//{
	//	DE_LOG(LogPlayground, Error, "Failed to load texture")
	//}
	//stbi_image_free(data);

	// Vertex Array Object
	glGenVertexArrays(1, &TexPlane->VAO);
	glBindVertexArray(TexPlane->VAO);

	glBindBuffer(GL_ARRAY_BUFFER, TexPlane->VBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, TexPlane->IBO);
	glVertexAttribPointer(1, 2, GL_UNSIGNED_INT, GL_FALSE, 2 * sizeof(float), nullptr);

	TexPlane->m_Shader = ShaderSubsystem::Get()->GetShader("DebugShader");
	return true;
}

void TextureScene::Unload()
{
	Scene::Unload();
}

void TextureScene::BeginScene()
{
	Scene::BeginScene();


}

void TextureScene::EndScene()
{
	Scene::EndScene();
}

void TextureScene::Update(float _deltaTime)
{
	Scene::Update(_deltaTime);

	TexPlane->m_Shader->Bind();
	glBindVertexArray(TexPlane->VAO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, TexPlane->IBO);

	// Upload the model matrix
	glUniformMatrix4fv(TexPlane->m_Shader->GetUniform("u_Model"), 1,
		GL_FALSE, glm::value_ptr(TexPlane->GetTransformComponent()->GetModel()));

	if (m_Camera)
	{
		// Upload the projection matrix
		glUniformMatrix4fv(TexPlane->m_Shader->GetUniform("u_Projection"), 1,
			GL_FALSE, glm::value_ptr(m_Camera->GetProjectionMatrix()));

		// Upload the view matrix
		glUniformMatrix4fv(TexPlane->m_Shader->GetUniform("u_View"), 1,
			GL_FALSE, glm::value_ptr(m_Camera->GetViewMatrix()));
	}


	// Upload the color
	glUniform4fv(TexPlane->m_Shader->GetUniform("u_Color"), 1, &TexPlane->GetRenderComponent()->GetDebugColor()[0]);


	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);
	TexPlane->m_Shader->Unbind();
}
