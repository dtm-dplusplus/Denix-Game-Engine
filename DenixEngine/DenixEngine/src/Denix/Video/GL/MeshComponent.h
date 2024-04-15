#pragma once

#include "Denix/Scene/Component.h"
#include "Denix/Video/GL/VertexArray.h"
#include "Denix/Video/GL/VertexBuffer.h"
#include "Denix/Video/GL/Mesh.h"

namespace Denix
{

	class
	MeshComponent final : public Component
	{
	public:
		MeshComponent() : Component(ObjectInitializer("Mesh Component")),
			m_Mesh{nullptr},
			m_VAO{MakeRef<VertexArray>()},
			m_VBO{MakeRef<VertexBuffer>()},
			m_IBO{MakeRef<IndexBuffer>()} {}

		MeshComponent(const std::string& _parentName) : Component(_parentName, ObjectInitializer("Mesh Component")),
			m_Mesh{nullptr},
			m_VAO{MakeRef<VertexArray>()},
			m_VBO{MakeRef<VertexBuffer>()},
			m_IBO{MakeRef<IndexBuffer>()} {}

		MeshComponent(const std::string& _parentName, const Ref<Mesh>& _mesh) : Component(_parentName, ObjectInitializer("Mesh Component")),
			m_Mesh{ _mesh },
			m_VAO{ MakeRef<VertexArray>() },
			m_VBO{ MakeRef<VertexBuffer>() },
			m_IBO{ MakeRef<IndexBuffer>() } {}

		void CreateMesh(GLfloat* _vertices, unsigned int* _indices, const unsigned int _verticesCount, const unsigned int _numOfIndices)
		{
			m_VAO = MakeRef<VertexArray>();
			m_VAO->Bind();

			m_IBO = MakeRef<IndexBuffer>();
			m_IBO->Bind();
			m_IBO->SetIndexCount(_numOfIndices);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(_indices[0]) * _numOfIndices, _indices, GL_STATIC_DRAW);

			m_VBO = MakeRef<VertexBuffer>();
			m_VBO->Bind();
			glBufferData(GL_ARRAY_BUFFER, sizeof(_vertices[0]) * _verticesCount, _vertices, GL_STATIC_DRAW);

			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(_vertices[0]) * 5, 0);
			glEnableVertexAttribArray(0);

			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(_vertices[0]) * 5, (void*)(sizeof(_vertices[0]) * 3));
			glEnableVertexAttribArray(1);

			glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(_vertices[0]) * 5, (void*)(sizeof(_vertices[0]) * 8));
			glEnableVertexAttribArray(2);

			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
			glBindVertexArray(0);
		}

		Ref<Mesh> GetMesh() const { return m_Mesh; }
		Ref<Mesh>& GetMesh() { return m_Mesh; }
		void SetMesh(const Ref<Mesh>& _mesh) { m_Mesh = _mesh; }

		Ref<VertexArray> GetVertexArray() const { return m_VAO; }
		Ref<VertexBuffer> GetVertexBuffer() const { return m_VBO; }
		Ref<IndexBuffer> GetIndexBuffer() const { return m_IBO; }

	private:

		Ref<Mesh> m_Mesh;

		Ref<VertexArray> m_VAO;
		Ref<VertexBuffer> m_VBO;
		Ref<IndexBuffer> m_IBO;

		friend class RendererSubsystem;
	};
}
