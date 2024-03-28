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

		~MeshComponent() override = default;

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
