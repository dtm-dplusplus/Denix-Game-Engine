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
		MeshComponent() : Component(ObjectInitializer("Mesh Component")) {}

		MeshComponent(const GLint _parentID) : Component(_parentID, ObjectInitializer("Mesh Component")) {}

		MeshComponent(const GLint _parentID,
			const GLenum _target, const GLsizei _size, const void* _data, const GLuint _count, const GLenum _type) :
			Component(_parentID, ObjectInitializer("Mesh Component"))
		{
			Vbo = MakeRef<VertexBuffer>(_target, sizeof(SqureData), SqureData, _count, _type);

			Vao = MakeRef<VertexArray>();
			Vao->GenVertexArray();
			Vao->Bind();

			// Bind the vbos & attribs
			Vbo->Bind(Vbo->GetTarget());
			Vao->AttribPtr(Vbo->GetCount(), Vbo->GetType());

			// Reset the state
			VertexBuffer::Unbind(GL_ARRAY_BUFFER);
			VertexArray::Unbind();
		}

		~MeshComponent() override = default;

	private:
		Ref<VertexArray> Vao;
		Ref<VertexBuffer> Vbo;
	};
}
