#pragma once

#include "Denix/Scene/Component.h"
#include "Denix/Video/GL/VertexArray.h"
#include "Denix/Video/GL/VertexBuffer.h"

namespace Denix
{
	class MeshComponent : public Denix::Component
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

		~MeshComponent() override
		{
			//if (IBO != 0)
		//{
		//	//glDeleteBuffers(1, &IBO);
		//	IBO = 0;
		//}

		//if (VBO != 0)
		//{
		//	//glDeleteBuffers(1, &VBO);
		//	VBO = 0;
		//}

		//if (VAO != 0)
		//{
		//	//glDeleteVertexArrays(1, &VAO);
		//	VAO = 0;
		//}

		//indexCount = 0;
		}

		void CreateMesh(GLfloat* vertices, unsigned int* indices, unsigned int numOfVertices, unsigned int numOfIndices)
		{
			indexCount = numOfIndices;

			m_VAO = MakeRef<VertexArray>();
			m_VAO->Bind();

			m_IBO = MakeRef<IndexBuffer>();
			m_IBO->Bind();
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * numOfIndices, indices, GL_STATIC_DRAW);

			m_VBO = MakeRef<VertexBuffer>();
			m_VBO->Bind();
			glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0]) * numOfVertices, vertices, GL_STATIC_DRAW);

			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertices[0]) * 5, 0);
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(vertices[0]) * 5, (void*)(sizeof(vertices[0]) * 3));
			glEnableVertexAttribArray(1);

			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

			glBindVertexArray(0);
		}

		void RenderMesh()
		{
			m_VAO->Bind();
			m_IBO->Bind();
			glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
			IndexBuffer::Unbind();
			VertexArray::Unbind();
		}

		Ref<VertexArray> GetVertexArray() const { return m_VAO; }
		Ref<VertexBuffer> GetVertexBuffer() const { return m_VBO; }
		Ref<IndexBuffer> GetIndexBuffer() const { return m_IBO; }


	private:
		Ref<VertexArray> m_VAO;
		Ref<VertexBuffer> m_VBO;
		Ref<IndexBuffer> m_IBO;

		GLsizei indexCount;

		friend class RendererSubsystem;
	};
}
