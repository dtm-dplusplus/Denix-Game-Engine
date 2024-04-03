#pragma once

#include "Denix/Scene/Component.h"
#include "Denix/Video/GL/VertexArray.h"
#include "Denix/Video/GL/VertexBuffer.h"

namespace Denix
{
	class MeshComponent final : public Component
	{
	public:
		MeshComponent() : Component(ObjectInitializer("Mesh Component")) 
		{
			m_VBO = MakeRef<VertexBuffer>();
			m_IBO = MakeRef<IndexBuffer>();
			m_VAO = MakeRef<VertexArray>();
		}

		MeshComponent(const GLint _parentID) : Component(_parentID, ObjectInitializer("Mesh Component")) 
		{
			m_VBO = MakeRef<VertexBuffer>();
			m_IBO = MakeRef<IndexBuffer>();
			m_VAO = MakeRef<VertexArray>();
		}

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

		Ref<VertexArray> GetVertexArray() const { return m_VAO; }
		Ref<VertexBuffer> GetVertexBuffer() const { return m_VBO; }
		Ref<IndexBuffer> GetIndexBuffer() const { return m_IBO; }

	private:
		Ref<VertexArray> m_VAO;
		Ref<VertexBuffer> m_VBO;
		Ref<IndexBuffer> m_IBO;

		friend class RendererSubsystem;
	};
}
