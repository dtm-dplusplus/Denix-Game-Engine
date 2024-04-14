#pragma once

#include "Denix/Core.h"
#include "Denix/Scene/Object.h"
#include "Denix/Video/GL/VertexArray.h"
#include "Denix/Video/GL/VertexBuffer.h"

namespace Denix
{
    class Mesh: public Object
    {
    public:
        Mesh(const ObjectInitializer& _objInit = { "Mesh" }) : Object(_objInit), 
			m_VAO{ MakeRef<VertexArray>() },
			m_VBO{ MakeRef<VertexBuffer>() },
			m_IBO{ MakeRef<IndexBuffer>() } {}

		Mesh(const float* _vertices, const unsigned int* _indices, const unsigned int _verticesCount,
		     const unsigned int _numOfIndices, const ObjectInitializer& _objInit = {"Mesh"}) : Object(_objInit),
			m_VAO{ MakeRef<VertexArray>() },
			m_VBO{ MakeRef<VertexBuffer>() },
			m_IBO{ MakeRef<IndexBuffer>() } 
		{
			CreateMesh(_vertices, _indices, _verticesCount, _numOfIndices);
		}

        ~Mesh() override {}

		void CreateMesh(const float* _vertices, const unsigned int* _indices, const unsigned int _verticesCount, const unsigned int _numOfIndices)
		{
			m_VAO->Bind();

			m_IBO->Bind();
			m_IBO->SetIndexCount(_numOfIndices);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(_indices[0]) * _numOfIndices, _indices, GL_STATIC_DRAW);

			m_VBO->Bind();
			glBufferData(GL_ARRAY_BUFFER, sizeof(_vertices[0]) * _verticesCount, _vertices, GL_STATIC_DRAW);

			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(_vertices[0]) * 8, 0);
			glEnableVertexAttribArray(0);

			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(_vertices[0]) * 8, (void*)(sizeof(_vertices[0]) * 3));
			glEnableVertexAttribArray(1);

			glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(_vertices[0]) * 8, (void*)(sizeof(_vertices[0]) * 5));
			glEnableVertexAttribArray(2);

			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
			glBindVertexArray(0);
		}

		Ref<VertexArray> GetVertexArray() const { return m_VAO; }
		Ref<VertexBuffer> GetVertexBuffer() const { return m_VBO; }
		Ref<IndexBuffer> GetIndexBuffer() const { return m_IBO; }
		GLuint GetIndexCount() const 
		{
			if(m_IBO) return m_IBO->GetIndexCount();
			return 0;
		}
    protected:
        Ref<VertexArray> m_VAO;
        Ref<VertexBuffer> m_VBO;
        Ref<IndexBuffer> m_IBO;

        friend class RendererSubsystem;
    };
}


