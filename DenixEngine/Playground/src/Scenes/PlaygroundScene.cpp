#include "PlaygroundScene.h"
#include "imgui.h"

#include <filesystem>
#include "Denix/System/ShaderSubSystem.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

namespace Denix
{
	TexPlane::TexPlane() : GameObject({ "TexPlane" })
	{
		// Positions Vertex Buffer
		const Ref<VertexBuffer> vbo = m_MeshComponent->GetVertexBuffer();
		vbo->Bind();
		glBufferData(GL_ARRAY_BUFFER, sizeof(PlaneData) + sizeof(TexCoords), NULL, GL_STATIC_DRAW);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(PlaneData), PlaneData);
		glBufferSubData(GL_ARRAY_BUFFER, sizeof(PlaneData), sizeof(TexCoords), TexCoords);

		// Indices Vertex Buffer
		const Ref<IndexBuffer> ibo = m_MeshComponent->GetIndexBuffer();
		ibo->Bind();
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(PlaneIndices), PlaneIndices, GL_STATIC_DRAW);


		// Setup Vertex Array
		const Ref<VertexArray> vao = m_MeshComponent->GetVertexArray();
		vao->Bind();

		// Bind Vertex Buffer at Location 0
		vbo->Bind();
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(0);
	
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)sizeof(PlaneData));
		glEnableVertexAttribArray(1);

		ibo->Bind();
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(2);

		//// Reset the state
		VertexBuffer::Unbind();
		VertexArray::Unbind();
		IndexBuffer::Unbind();

		// Setup Texture
		// load and generate the texture
		int width, height;
		std::string fullPath = std::filesystem::current_path().parent_path().string() + "\\Playground\\Content\\Texture.jpg";
		unsigned char* data = stbi_load(fullPath.c_str(), &width, &height, NULL, 4);

		
		glGenTextures(1, &Texture);

		if (!Texture)
		{
			throw std::exception();
		}

		glBindTexture(GL_TEXTURE_2D, Texture);

		// Upload the image data to the bound texture unit in the GPU
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA,
			GL_UNSIGNED_BYTE, data);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		// Free the loaded data because we now have a copy on the GPU
		free(data);

		// Generate Mipmap so the texture can be mapped correctly
		glGenerateMipmap(GL_TEXTURE_2D);

		// Unbind the texture because we are done operating on it
		glBindTexture(GL_TEXTURE_2D, 0);

		// Set the shader
		m_RenderComponent->GetShader() = ShaderSubsystem::Get()->GetShader("TextureShader");
		m_RenderComponent->IsVisible() = false;
	}

	bool PlaygroundScene::Load()
	{
		Scene::Load();

		m_Camera->GetTransformComponent()->SetPosition({ -20.0, 15.0, 55.0 });
		m_Camera->GetTransformComponent()->SetRotation({ -5.0, -70.0, 0.0 });
		m_Camera->GetFov() = 80.0f;
		
		Ref<Cube> cube = MakeRef<Cube>();
		cube->SetMoveability(Moveability::Dynamic);
		Ref<TransformComponent> transform = cube->GetTransformComponent();
		transform->SetPosition({ 15.0, 50,0.0 });
		transform->SetRotation({ 0.0, 0.0, 0.0 });
		cube->GetRenderComponent()->SetDebugColor({ 1.0, 80.0/255.0, 50.0/255.0, 1.0 });
		m_SceneObjects.push_back(cube);
		
		// Walls
		Ref<Plane> wall1 = MakeRef<Plane>();
		Ref<TransformComponent> wall1Transform = wall1->GetTransformComponent();
		wall1Transform->SetPosition({ 0.0, 15.0, 0.0 });
		wall1Transform->SetRotation({ 0.0, 90.0, 0.0 });
		wall1Transform->SetScale({ 30.0, 30.0, 1.0 });
		wall1->GetRenderComponent()->SetDebugColor({ 0.0, 1.0, 1.0, 1.0 });
		wall1->GetPhysicsComponent()->IsSimulated() = false;
		m_SceneObjects.push_back(wall1);
		
		Ref<Plane> wall2 = MakeRef<Plane>();
		Ref<TransformComponent> wall2Transform = wall2->GetTransformComponent();
		wall2Transform->SetPosition({ 15.0, 0.0, 0.0 });
		wall2Transform->SetRotation({ 90.0,0.0, 0.0 });
		wall2Transform->SetScale({ 30.0, 30.0, 1.0 });
		wall2->GetRenderComponent()->SetDebugColor({ 1.0, 0.0, 1.0, 1.0 });
		wall2->GetPhysicsComponent()->IsSimulated() = false;
		m_SceneObjects.push_back(wall2);
		
		Ref<Plane> wall3 = MakeRef<Plane>();
		Ref<TransformComponent> wall3Transform = wall3->GetTransformComponent();
		wall3Transform->SetPosition({ 30.0, 15.0, 0.0 });
		wall3Transform->SetRotation({ 0.0, 90.0, 0.0 });
		wall3Transform->SetScale({ 30.0, 30.0, 1.0 });
		wall3->GetRenderComponent()->SetDebugColor({ 45.0 / 255.0, 1.0, 0.0, 1.0 });
		wall3->GetPhysicsComponent()->IsSimulated() = false;
		m_SceneObjects.push_back(wall3);
		
		Ref<Plane> wall4 = MakeRef<Plane>();
		Ref<TransformComponent> wall4Transform = wall4->GetTransformComponent();
		wall4Transform->SetPosition({ 15.0, 15.0, -15.0 });
		wall4Transform->SetRotation({ 0.0, 0.0, 0.0 });
		wall4Transform->SetScale({ 30.0, 30.0, 1.0 });
		wall4->GetRenderComponent()->SetDebugColor({ 140.0 / 255.0, 140.0 / 255.0, 140.0 / 255.0, 1.0 });
		wall4->GetPhysicsComponent()->IsSimulated() = false;
		m_SceneObjects.push_back(wall4);

		TestPlane = MakeRef<TexPlane>();
		TestPlane->GetTransformComponent()->SetScale({ 30.0, 30.0, 1.0 });
		m_SceneObjects.push_back(TestPlane);
		return true;
	}

	void PlaygroundScene::Update(float _deltaTime)
	{
		Scene::Update(_deltaTime);

		ImGui::Begin("Playground Scene");
		
		ImGui::End();

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, TestPlane->Texture);
		glUniform1i(TestPlane->GetRenderComponent()->GetShader()->GetUniform("u_Texture"), TestPlane->Texture);
		TestPlane->GetRenderComponent()->GetShader()->Bind();
		TestPlane->GetMeshComponent()->GetVertexArray()->Bind();
		
		// Upload the model matrix
		glUniformMatrix4fv(TestPlane->GetRenderComponent()->GetShader()->GetUniform("u_Model"), 1,
			GL_FALSE, glm::value_ptr(TestPlane->GetTransformComponent()->GetModel()));
		
		// Upload the projection matrix
		glUniformMatrix4fv(TestPlane->GetRenderComponent()->GetShader()->GetUniform("u_Projection"), 1,
			GL_FALSE, glm::value_ptr(m_Camera->GetProjectionMatrix()));
		
		// Upload the view matrix
		glUniformMatrix4fv(TestPlane->GetRenderComponent()->GetShader()->GetUniform("u_View"), 1,
			GL_FALSE, glm::value_ptr(m_Camera->GetViewMatrix()));
		
		
		// Draw the indexed mesh
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		
		VertexArray::Unbind();
		GLShader::Unbind();
		glBindTexture(GL_TEXTURE_2D, 0);

	}
}
