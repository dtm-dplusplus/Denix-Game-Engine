#pragma once

#include "Denix/Scene/Component.h"
#include "Denix/Video/GL/VertexArray.h"
#include "Denix/Video/GL/VertexBuffer.h"

constexpr float SqureData[] = {
			-0.5f, 0.5f, 0.0f, // top left point
			0.5f, 0.5f, 0.0f, // top right point
			0.5f, -0.5f, 0.0f, // bottom right point
			0.5f, -0.5f, 0.0f, // bottom right point
			-0.5f, -0.5f, 0.0f, // bottom left point
			-0.5f, 0.5f, 0.0f // top left point
};

namespace Denix
{
	class MeshComponent : public Denix::Component
	{
	public:
		MeshComponent() : Component(ObjectInitializer("Mesh Component")) 
		{
			m_VBO = MakeRef<VertexBuffer>();
			m_VAO = MakeRef<VertexArray>();
		}

		MeshComponent(const GLint _parentID) : Component(_parentID, ObjectInitializer("Mesh Component")) 
		{
			m_VBO = MakeRef<VertexBuffer>();
			m_VAO = MakeRef<VertexArray>();
		}

		MeshComponent(const GLint _parentID,
			const GLenum _target, const GLsizei _size, const void* _data, const GLuint _perPrimitive,const GLuint _count, const GLenum _type) :
			Component(_parentID, ObjectInitializer("Mesh Component"))
		{
			m_VBO = MakeRef<VertexBuffer>(_target, _size, _data, _perPrimitive, _count, _type);

			m_VAO = MakeRef<VertexArray>();
			m_VAO->Bind();

			// Bind the vbos & attribs
			m_VBO->Bind(m_VBO->GetTarget());
			m_VAO->AttribPtr(m_VBO->GetCount(), m_VBO->GetType());

			// Reset the state
			VertexBuffer::Unbind(GL_ARRAY_BUFFER);
			VertexArray::Unbind();
		}

		~MeshComponent() override = default;

		Ref<VertexArray> GetVertexArray() const { return m_VAO; }
		Ref<VertexBuffer> GetVertexBuffer() const { return m_VBO; }

	private:
		Ref<VertexArray> m_VAO;
		Ref<VertexBuffer> m_VBO;

		friend class RendererSubsystem;
	};
}
